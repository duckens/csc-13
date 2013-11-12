#include "selectorStructParser.h"

selectorStruct pasreSelector(uint32_t selector){
	
	selectorStruct res;

	res.index = selector >> selectorStruct_index_shift;
	res.rpl = selector & selectorStruct_rpl_bm;
	res.global = selector & selectorStruct_global_bm;

	return res;

}