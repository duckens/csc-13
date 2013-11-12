#ifndef PD_RECORD_STRUCT_H
#define PD_RECORD_STRUCT_H

#include "types.h"

struct pdRecordStruct{
	
	bool present;
	bool readWrite;
	bool userSupervisor;
	bool pageWriteThrough;
	bool pageCacheDisable;
	bool accessed;
	bool dirty;
	bool pageSize_pageAttributeTableIndex;
	bool global;

	uint32_t userAlailable;

	uint32_t baseAddress;
	uint32_t baseAddressLargePage;
};

#endif
