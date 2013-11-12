#ifndef SELECTOR_STRUCT_PARSER_H
#define SELECTOR_STRUCT_PARSER_H


#include "selectorStruct.h"


#define selectorStruct_index_shift		3
#define selectorStruct_rpl_bm			0x3
#define selectorStruct_global_bm		0x4
#define selectorStruct_globalTable		0
#define selectorStruct_localTable		1

selectorStruct pasreSelector(uint32_t selector);

#endif
