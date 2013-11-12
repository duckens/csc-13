#ifndef GDT_RECORD_STRUCT_PARSER_H
#define GDT_RECORD_STRUCT_PARSER_H


#include "gdtRecordStruct.h"


#define gdtRecord_a_bp		40 // accessed
#define gdtRecord_rw_bp		41 // readable/writable
#define gdtRecord_c_bp		42 // conforming
#define gdtRecord_x_bp		43 // executable
#define gdtRecord_s_bp		44 // system. 0 - system, 1 - code or data
#define gdtRecord_p_bp		47 // present
#define gdtRecord_avl_bp	52 // avaliable for user
#define gdtRecord_d_bp		54 // default size. 0 - 16bit, 1 - 32bit
#define gdtRecord_g_bp		55 // granularity. 0 - 1byte, 1 - 4Kb page


#define gdtRecord_a_bm		(1LL << gdtRecord_a_bp)
#define gdtRecord_rw_bm		(1LL << gdtRecord_rw_bp)
#define gdtRecord_c_bm		(1LL << gdtRecord_c_bp)
#define gdtRecord_x_bm		(1LL << gdtRecord_x_bp)
#define gdtRecord_s_bm		(1LL << gdtRecord_s_bp)
#define gdtRecord_p_bm		(1LL << gdtRecord_p_bp)
#define gdtRecord_avl_bm	(1LL << gdtRecord_avl_bp)
#define gdtRecord_d_bm		(1LL << gdtRecord_d_bp)
#define gdtRecord_g_bm		(1LL << gdtRecord_g_bp)



#define gdtRecord_dpl_bm					0x0000600000000000LL // descriptor privellege level
#define gdtRecord_dpl_shift					45

#define gdtRecord_segmentLimit_19_16_bm		0x000F000000000000LL
#define gdtRecord_segmentLimit_19_16_shift	32
#define gdtRecord_segmentLimit_15_0_bm		0x000000000000FFFFLL
#define gdtRecord_segmentLimit_15_0_shift	0

#define gdtRecord_baseAdress_31_24_bm		0xFF00000000000000LL
#define gdtRecord_baseAdress_31_24_shift	32
#define gdtRecord_baseAdress_23_0_bm		0x000000FFFFFF0000LL
#define gdtRecord_baseAdress_23_0_shift		16






gdtRecordStruct parseGDTRecord(uint64_t record);



#endif
