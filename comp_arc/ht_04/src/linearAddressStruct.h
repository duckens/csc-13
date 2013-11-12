#ifndef LINEAR_ADDRESS_STRUCT_H
#define LINEAR_ADDRESS_STRUCT_H

#include "types.h"

struct linearAddressStruct{
	uint32_t directory;
	uint32_t table;
	uint32_t offset;
	uint32_t longOffset;
};

#endif
