#include "AVBFile.h"
#include <iostream>

// CAVBFileReader
CAVBFileReader::CAVBFileReader() {}
CAVBFileReader::~CAVBFileReader() {}

bool ifFileExist(const char* fileName) {

    return GetFileAttributesA(fileName) != DWORD(-1);
}

bool CAVBFileReader::open(const char* FilePath) {

    file.open(FilePath, std::ios::binary, std::ios::in);
    return file.is_open();

}
bool CAVBFileReader::readRecord(SAVRecord* record) {

    if (!file.read(reinterpret_cast<char*>(record), sizeof(SAVRecord))) {
        return false;
    }
    return true;
}

void CAVBFileReader::close() {
    if (file.is_open()) {
        file.close();
    }
}

// CAVBFileWriter
CAVBFileWriter::CAVBFileWriter() {}
CAVBFileWriter::~CAVBFileWriter() {}

bool CAVBFileWriter::open(const char* path) {
    file.open(path, std::ios::binary | std::ios::out | std::ios::app);
    return file.is_open();
}

bool CAVBFileWriter::addRecord(const SAVRecord* record) {
    if (!file.write(reinterpret_cast<const char*>(record), sizeof(SAVRecord))) {
        return false;
    }
    return true;
}

void CAVBFileWriter::close() {
    if (file.is_open()) {
        file.close();
    }
}

