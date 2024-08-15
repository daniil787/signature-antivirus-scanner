#include "avrecord.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
#include "isFileExist.h"
#include "sha256hash.h"
#include "AVBFile.h"

using namespace std;

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
        cout << "> Invalid arguments. Stop." << endl;
        return 0;
    }

    const char* srcFile = argv[1];
    const char* dstFile = argv[2];
    DWORD offset = static_cast<DWORD>(atoi(argv[3]));
    DWORD length = static_cast<DWORD>(atoi(argv[4]));
    const char* name = argv[5];

    cout << "srcFile: " << srcFile << endl;
    cout << "dstFile: " << dstFile << endl;
    cout << "offset: " << offset << endl;
    cout << "length: " << length << endl;
    cout << "name: " << name << endl;

    // check for file presence
    if (!isFileExist(srcFile)) {
        cout << "> Source file does not exist. Stop." << endl;
        return 0;
    }

    // create record of base signatures(avbase)
    SAVRecord Record;
    Record.Signature.Offset = offset;
    Record.Signature.Lenght = length;
    Record.NameLen = static_cast<BYTE>(strlen(name));
    Record.allocName(Record.NameLen);
    strncpy_s(Record.Name, Record.NameLen + 1, name, _TRUNCATE);

    // open file in binary 
    ifstream hSrcFile(srcFile, ios::in | ios::binary);
    if (!hSrcFile.is_open()) {
        cout << "> Can't open source file. Stop." << endl;
        return 0;
    }

    // Reading data to calculate checksum
    unique_ptr<BYTE[]> Buffer(new BYTE[Record.Signature.Lenght]);
    if (!Buffer) {
        cout << "> Can't alloc memory for sign data. Stop." << endl;
        hSrcFile.close();
        return 0;
    }
    hSrcFile.seekg(Record.Signature.Offset, ios::beg);
    hSrcFile.read(reinterpret_cast<char*>(Buffer.get()), Record.Signature.Lenght);
    hSrcFile.close();

    // Calculating the signature hash
    getSHA256(Buffer.get(), Record.Signature.Lenght, reinterpret_cast<BYTE*>(Record.Signature.Hash));

    // Adding a record to the database
    cout << "Record info:" << endl;
    cout << " Name: " << Record.Name << endl;
    cout << " Offset: 0x" << hex << Record.Signature.Offset << " (" << dec << Record.Signature.Offset << ")" << endl;
    cout << " Length: 0x" << hex << Record.Signature.Lenght << " (" << dec << Record.Signature.Lenght << ")" << endl;
    printf("CheckSum: 0x%08x%08x%08x%08x%08x%08x%08x%08x\n",
        Record.Signature.Hash[0], Record.Signature.Hash[1], Record.Signature.Hash[2],
        Record.Signature.Hash[3], Record.Signature.Hash[4], Record.Signature.Hash[5],
        Record.Signature.Hash[6], Record.Signature.Hash[7]);

    CAVBFileWriter hAVBFile;
    if (!hAVBFile.open(dstFile)) {
        cout << "> Can't open database file. Stop." << endl;
        return 0;
    }
    hAVBFile.addRecord(&Record);
    hAVBFile.close();

    cout << "Record added." << endl;

    return 0;
}

