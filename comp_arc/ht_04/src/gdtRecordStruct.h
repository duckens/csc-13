#ifndef GDT_RECORD_STRUCT_H
#define GDT_RECORD_STRUCT_H

#include "types.h"

struct gdtRecordStruct{

	uint32_t segmentLimit;
	uint64_t baseAddress;

	uint32_t dpl;
	
	bool a;
	bool rw;
	bool c;
	bool x;
	bool s;
	bool p;
	bool avl;
	bool d;
	bool g;

};

#endif
