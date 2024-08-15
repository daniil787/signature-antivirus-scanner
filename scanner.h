
#ifndef SCANNER_H
#define SCANNER_H

#include <windows.h>
#include <iostream>
#include "avrecord.h"



typedef struct SAVRecordCollection {
    SAVRecordCollection(DWORD RecordCount) {
        this->RecordCount = RecordCount;
        this->Record = new SAVRecord[this->RecordCount];
    }
    ~SAVRecordCollection() {
        delete[] this->Record;
    }
    DWORD RecordCount;
    PSAVRecord Record;
} *PSAVRecordCollection;

extern PSAVRecordCollection AVRCollection;


int addRecordMain(int argc, char* argv[]);

void processPath(PCSTR Path);
void checkFile(PCSTR FileName);
void getSHA256(const BYTE* data, DWORD size, BYTE* hash);

#endif // SCANNER_H
