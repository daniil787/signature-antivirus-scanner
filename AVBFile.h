    #ifndef AVBFILE_H
    #define AVBFILE_H

    #include <fstream>
    #include "avrecord.h"

    class CAVBFile {

    public:
    
        CAVBFile():record_count(0) {};

        virtual ~CAVBFile() { close(); }

        virtual bool open(const char* path) = 0;

       bool is_open() const {
           return file.is_open();
       }
   
       void close() {
           if (file.is_open()) {
               file.close();
           }
       }

    protected:
        std::fstream file;
        DWORD record_count;  

    };

    class CAVBFileReader : public CAVBFile {

    public:

        CAVBFileReader();
        ~CAVBFileReader() override;
        bool open(const char* path) override;
        bool readRecord(SAVRecord* record);

    };

    class CAVBFileWriter : public CAVBFile {

    public:

        CAVBFileWriter();
        ~CAVBFileWriter() override;
        bool open(const char* path) override;
        bool addRecord(const SAVRecord* record);

    };

    #endif



