#include "pdRecordParser.h"	

pdRecordStruct pasrePDRecord(uint32_t record){
	
	pdRecordStruct res;

	res.present = 							record & pdRecord_present_bm;
	res.readWrite = 						record & pdRecord_readWrite_bm;
	res.userSupervisor = 					record & pdRecord_userSupervisor_bm;
	res.pageWriteThrough = 					record & pdRecord_pageWriteThrough_bm;
	res.pageCacheDisable = 					record & pdRecord_pageCacheDisable_bm;
	res.accessed = 							record & pdRecord_accessed_bm;
	res.dirty = 							record & pdRecord_dirty_bm;
	res.pageSize_pageAttributeTableIndex = 	record & pdRecord_pageSize_pageAttributeTableIndex_bm;
	res.global = 							record & pdRecord_global_bm;

	res.userAlailable = (record & pdRecord_userAlailable_bm) >> pdRecord_userAlailable_shift;

	res.baseAddress = record & pdRecord_baseAddress_bm;
	res.baseAddressLargePage = record & pdRecord_baseAddressLargePage_bm;

	return res;
}
