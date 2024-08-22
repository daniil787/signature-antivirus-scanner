#include "avrecord.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
#include "isFileExist.h"
#include "sha256hash.h"
#include "AVBFile.h"


// argument copying
bool copyArg(const char* Arg, DWORD Offset, char* Buffer, DWORD Size) {
    size_t ArgLen = strlen(Arg);
    if (Offset > ArgLen) {
        return false;
    }
    size_t CopyLen = ArgLen - Offset;
    if (CopyLen > Size - 1) {
        return false;
    }
    strncpy_s(Buffer, Size, Arg + Offset, _TRUNCATE);

    Buffer[Size - 1] = '\0';
    return true;
}


void getSHA256(const BYTE* data, DWORD size, BYTE* hash);

int addRecordMain(int argc, char* argv[]) {
    if (argc != 6) {
        std::cout << "> Invalid arguments. Stop." << std::endl;
        return 0;
    }


    const char* srcFile = argv[1];
    
    // check for file presence
    if (!isFileExist(srcFile)) {
        std::cout << "> Source file does not exist. Stop." << std::endl;
        return 0;
    }
    
    const char* dstFile = argv[2];
    DWORD offset = static_cast<DWORD>(atoi(argv[3]));
    DWORD length = static_cast<DWORD>(atoi(argv[4]));
    const char* name = argv[5];

    std::cout << "srcFile: " << srcFile << std::endl;
    std::cout << "dstFile: " << dstFile << std::endl;
    std::cout << "offset: " << offset << std::endl;
    std::cout << "length: " << length << std::endl;
    std::cout << "name: " << name << std::endl;

    


    // create record of base signatures(avbase)
    SAVRecord Record;
    Record.Signature.Offset = offset;
    Record.Signature.Lenght = length;
    Record.NameLen = static_cast<BYTE>(strlen(name));
    Record.allocName(Record.NameLen);
    strncpy_s(Record.Name, Record.NameLen + 1, name, _TRUNCATE);





    // open file in binary 
    std::ifstream hSrcFile(srcFile, std::ios::in | std::ios::binary);
    if (!hSrcFile.is_open()) {
        std::cout << "> Can't open source file. Stop." << std::endl;
        return 0;
    }





    // Reading data to calculate checksum
    std::unique_ptr<BYTE[]> Buffer(new BYTE[Record.Signature.Lenght]);
    if (!Buffer) {
        std::cout << "> Can't alloc memory for sign data. Stop." << std::endl;
        hSrcFile.close();
        return 0;
    }

    hSrcFile.seekg(Record.Signature.Offset, std::ios::beg);
    hSrcFile.read(reinterpret_cast<char*>(Buffer.get()), Record.Signature.Lenght);
    hSrcFile.close();




    // Calculating the signature hash
    getSHA256(Buffer.get(), Record.Signature.Lenght, reinterpret_cast<BYTE*>(Record.Signature.Hash));
    



    // print info
    std::cout << "Record info:" << std::endl;
    std::cout << " Name: " << Record.Name << std::endl;
    std::cout << " Offset: 0x" << std::hex << Record.Signature.Offset << " (" << std::dec << Record.Signature.Offset << ")" << std::endl;
    std::cout << " Length: 0x" << std::hex << Record.Signature.Lenght << " (" << std::dec << Record.Signature.Lenght << ")" << std::endl;
    printf("CheckSum: 0x%08x%08x%08x%08x%08x%08x%08x%08x\n",
        Record.Signature.Hash[0], Record.Signature.Hash[1], Record.Signature.Hash[2],
        Record.Signature.Hash[3], Record.Signature.Hash[4], Record.Signature.Hash[5],
        Record.Signature.Hash[6], Record.Signature.Hash[7]);





    // Adding a record to the database
    CAVBFileWriter hAVBFile;
    if (!hAVBFile.open(dstFile)) {
        std::cout << "> Can't open database file. Stop." << std::endl;
        return 0;
    }
    hAVBFile.addRecord(&Record);
    hAVBFile.close();

    std::cout << "Record added." << std::endl;

    return 0;
}

