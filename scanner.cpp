
#include "scanner.h"
#include <fstream>
#include <vector>
#include <string>

// records of signatures of malware
PSAVRecordCollection AVRCollection = NULL;


void processPath(PCSTR Path) {
    std::string SrcPath = Path;
    std::string File = Path;

    File += "\\*.*"; // - marking for checking all files
    WIN32_FIND_DATAA FindData; // struct of data of file
    HANDLE hFind = FindFirstFileA(File.c_str(), &FindData);

    if (hFind == INVALID_HANDLE_VALUE) {
             std::cout << "Invalid path: " << Path << std::endl;
        return;
    }
    do {
        // Skipping folders named "." and ".."
        if (!strcmp(FindData.cFileName, ".") || !strcmp(FindData.cFileName, "..")) continue;

        File = Path;
        File += "\\";
        File += FindData.cFileName;

        // If folder - scan by recursing
        if (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            processPath(File.c_str());
        }
        else {
            // check file
            checkFile(File.c_str());
        }
    } while (FindNextFileA(hFind, &FindData));

    FindClose(hFind);
}

void checkFile(PCSTR FileName) {

    std::cout << FileName << "\t";

    // open file to check
    HANDLE hFile = CreateFileA(FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cout << "Error opening file" << std::endl;
        return;
    }

    // Giving size of file
    DWORD FileSize = GetFileSize(hFile, NULL);
    if (FileSize == INVALID_FILE_SIZE) {
        std::cout << "Error getting file size" << std::endl;
        CloseHandle(hFile);
        return;
    }

    // Introduce file in the memory
    HANDLE hMap = CreateFileMappingA(hFile, NULL, PAGE_READONLY, 0, FileSize, NULL);
    if (hMap == NULL) {
        std::cout << "Error mapping file" << std::endl;
        CloseHandle(hFile);
        return;
    }
    // 
    LPVOID File = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, FileSize);
    if (File == NULL) {
        std::cout << "Error viewing file" << std::endl;
        CloseHandle(hMap);
        CloseHandle(hFile);
        return;
    }

    // Seaching by signatures
    bool Detected = false;
    for (DWORD RecID = 0; RecID < AVRCollection->RecordCount; RecID++) {
        PSAVRecord Record = &AVRCollection->Record[RecID];

        // If file is too smal -> skip file
        if (FileSize < (Record->Signature.Offset + Record->Signature.Lenght)) continue;

        // Calculate total sum for signatures
        BYTE Hash[32];
        getSHA256((PBYTE)File + Record->Signature.Offset, Record->Signature.Lenght, Hash);//!! need to correct 

        // Detect
        if (!memcmp(Hash, Record->Signature.Hash, 32)) {
            std::cout << " DETECTED\t" << Record->Name << std::endl;
            Detected = true;
            break;
        }
    }
    UnmapViewOfFile(File);
    CloseHandle(hMap);
    CloseHandle(hFile);

    if (!Detected) std::cout << "OK" << std::endl;
}

int scanMain(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage: scanner.exe <path_to_scan>" << std::endl;
        return 0;
    }

    char* SrcPath = argv[1];

    // loading base of signatures
    std::ifstream hAVBFile("avbase.avb", std::ios::in | std::ios::binary);
    if (!hAVBFile.is_open()) {
        std::cout << "> Can't open AV base file. Stop." << std::endl;
        return 0;
    }

    hAVBFile.seekg(0, std::ios::end);
    DWORD FileSize = hAVBFile.tellg();
    hAVBFile.seekg(0, std::ios::beg);

    if (FileSize > 0) {
        DWORD RecordCount = FileSize / sizeof(SAVRecord);
        AVRCollection = new SAVRecordCollection(RecordCount);
        for (DWORD RecID = 0; RecID < RecordCount; RecID++) {
            hAVBFile.read(reinterpret_cast<char*>(&AVRCollection->Record[RecID]), sizeof(SAVRecord));
            if (!hAVBFile) {
                std::cout << "> Error loading record #" << RecID << std::endl;
            }
        }
        hAVBFile.close();
    }
    else {
        hAVBFile.close();
        std::cout << "> Empty AV Base. Stop." << std::endl;

        return 0;
    }
    std::cout << "\t" << AVRCollection->RecordCount << " records loaded." << std::endl;

    std::cout << std::endl;
    std::cout << "Starting scan for viruses" << std::endl;
    std::cout << std::endl;

    processPath(SrcPath);

    delete AVRCollection;

    return 0;
}
