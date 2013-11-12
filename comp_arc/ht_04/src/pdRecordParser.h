#ifndef PD_RECORD_STRUCT_PARSER_H
#define PD_RECORD_STRUCT_PARSER_H


#include "pdRecordStruct.h"	


#define pdRecord_present_bp								0
#define pdRecord_readWrite_bp							1
#define pdRecord_userSupervisor_bp						2
#define pdRecord_pageWriteThrough_bp					3
#define pdRecord_pageCacheDisable_bp					4
#define pdRecord_accessed_bp							5
#define pdRecord_dirty_bp								6
#define pdRecord_pageSize_pageAttributeTableIndex_bp	7
#define pdRecord_global_bp								8

#define pdRecord_present_bm								(1 << pdRecord_present_bp)
#define pdRecord_readWrite_bm							(1 << pdRecord_readWrite_bp)
#define pdRecord_userSupervisor_bm						(1 << pdRecord_userSupervisor_bp)
#define pdRecord_pageWriteThrough_bm					(1 << pdRecord_pageWriteThrough_bp)
#define pdRecord_pageCacheDisable_bm					(1 << pdRecord_pageCacheDisable_bp)
#define pdRecord_accessed_bm							(1 << pdRecord_accessed_bp)
#define pdRecord_dirty_bm								(1 << pdRecord_dirty_bp)
#define pdRecord_pageSize_pageAttributeTableIndex_bm	(1 << pdRecord_pageSize_pageAttributeTableIndex_bp)
#define pdRecord_global_bm								(1 << pdRecord_global_bp)

#define pdRecord_userAlailable_bm						0x00000E00
#define pdRecord_userAlailable_shift					9

#define pdRecord_baseAddress_bm							0xFFFFF000
#define pdRecord_baseAddressLargePage_bm				0xFFC00000


pdRecordStruct pasrePDRecord(uint32_t record);


#endif
