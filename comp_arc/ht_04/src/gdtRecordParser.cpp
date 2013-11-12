#include "gdtRecordParser.h"


gdtRecordStruct parseGDTRecord(uint64_t record){

	gdtRecordStruct res;

	res.dpl = (record & gdtRecord_dpl_bm) >> gdtRecord_dpl_shift;

	
	res.a = record & gdtRecord_a_bm;
	res.rw = record & gdtRecord_rw_bm;
	res.c = record & gdtRecord_c_bm;
	res.x = record & gdtRecord_x_bm;
	res.s = record & gdtRecord_s_bm;
	res.p = record & gdtRecord_p_bm;
	res.avl = record & gdtRecord_avl_bm;
	res.d = record & gdtRecord_d_bm;
	res.g = record & gdtRecord_g_bm;

	res.segmentLimit = 
		((record & gdtRecord_segmentLimit_19_16_bm) >> gdtRecord_segmentLimit_19_16_shift)
		|
		((record & gdtRecord_segmentLimit_15_0_bm) >> gdtRecord_segmentLimit_15_0_shift);
	
	res.baseAddress =
		((record & gdtRecord_baseAdress_31_24_bm) >> gdtRecord_baseAdress_31_24_shift )
		|
		((record & gdtRecord_baseAdress_23_0_bm) >> gdtRecord_baseAdress_23_0_shift );

	return res;

};
