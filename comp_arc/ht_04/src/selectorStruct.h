#ifndef SELECTOR_STRUCT_H
#define SELECTOR_STRUCT_H

#include "types.h"

struct selectorStruct{

	uint32_t index;
	uint32_t rpl; 	// requested privellege level
	bool global; 	// 0 - gdt, 1 - ldt

};

#endif
