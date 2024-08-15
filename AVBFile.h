#ifndef AVBFILE_H
#define AVBFILE_H

#include <fstream>
#include "avrecord.h"

class CAVBFile {
public:
    virtual ~CAVBFile() = default;
};

class CAVBFileReader : public CAVBFile {
public:
    CAVBFileReader();
    ~CAVBFileReader();
    bool open(const char* path);
    bool readRecord(SAVRecord* record);
    void close();
private:
    std::ifstream file;
};

class CAVBFileWriter : public CAVBFile {
public:
    CAVBFileWriter();
    ~CAVBFileWriter();
    bool open(const char* path);
    bool addRecord(const SAVRecord* record);
    void close();
private:
    std::ofstream file;
};

#endif



