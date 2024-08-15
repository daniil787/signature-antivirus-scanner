#ifndef _AVRECORD_H_INCLUDE_
#define _AVRECORD_H_INCLUDE_

#include  <windows.h>

int scanMain(int argc, char* argv[]);


typedef struct SAVSignature {
	SAVSignature() {
		this->Offset = 0;
		this->Lenght = 0;
		memset(this->Hash, 0, sizeof(this->Hash));
	}

	DWORD Hash[8];
	DWORD Lenght;
	DWORD Offset;
} *PSAVSignature;

typedef struct SAVRecord {

	SAVRecord() {
		this->Name = NULL;
		this->NameLen = 0;

	}
	~SAVRecord() { if (this->Name != NULL) delete[]  this->Name; }

	void allocName(BYTE NameLen) {
		if (this->Name == NULL) {
			this->NameLen = NameLen;
			this->Name = new CHAR[this->NameLen + 1];
			memset(this->Name, 0, this->NameLen + 1);
		}
	}
	PSTR Name;
	BYTE NameLen;
	SAVSignature Signature;
} *PSAVRecord;
#endif
