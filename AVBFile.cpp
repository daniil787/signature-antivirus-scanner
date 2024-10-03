#include "AVBFile.h"
#include <iostream>

// CAVBFileReader

CAVBFileReader::CAVBFileReader() : CAVBFile() {}

CAVBFileReader::~CAVBFileReader() {
    
}

bool CAVBFileReader::open(const char* file_path) {

    file.open(file_path, std::ios::binary | std::ios::in);

    if (!file.is_open()) {
        return false;   
    }

    file.read(reinterpret_cast<char*>(&record_count), sizeof(DWORD));

    return true;
}


bool CAVBFileReader::readRecord(SAVRecord* record) {

    if (!file.read(reinterpret_cast<char*>(record), sizeof(SAVRecord))) {

        return false;
    }

    return true;
}

// CAVBFileWriter

CAVBFileWriter::CAVBFileWriter() : CAVBFile() {}


CAVBFileWriter::~CAVBFileWriter() {}

bool CAVBFileWriter::open(const char* path) {

    file.open(path, std::ios::binary | std::ios::out | std::ios::app);

    return file.is_open();
}

bool CAVBFileWriter::addRecord(const SAVRecord* record) {

    if (file.write(reinterpret_cast<const char*>(record), sizeof(SAVRecord))) {
    
        record_count++;
        return true;
    }

    return false;
}



