#include <cstdio>
#include <iostream>

typedef unsigned long long int uint64_t;
typedef unsigned int uint32_t;

#define selectorStruct_index_shift		3
#define selectorStruct_rpl_bm			0x3
#define selectorStruct_global_bm		0x4
#define selectorStruct_globalTable		0
#define selectorStruct_localTable		1


#define gdtRecord_a_bp		40 // accessed
#define gdtRecord_rw_bp		41 // readable/writable
#define gdtRecord_c_bp		42 // conforming
#define gdtRecord_x_bp		43 // executable
#define gdtRecord_s_bp		44 // system
#define gdtRecord_p_bp		47 // present
#define gdtRecord_avl_bp	52 // avaliable for user
#define gdtRecord_d_bp		54 // default size 0 - 16bit, 1 - 32bit
#define gdtRecord_g_bp		55 // granularity 0 - 1byte, 1 - 4Kb page


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

#define gdtRecord_segmentLimit_19_16_bm		0x000000FF00000000LL
#define gdtRecord_segmentLimit_19_16_shift	32
#define gdtRecord_segmentLimit_15_0_bm		0x000000000000FFFFLL
#define gdtRecord_segmentLimit_15_0_shift	0

#define gdtRecord_baseAdress_31_24_bm		0xFF00000000000000LL
#define gdtRecord_baseAdress_31_24_shift	32
#define gdtRecord_baseAdress_23_0_bm		0x000000FFFFFF0000LL
#define gdtRecord_baseAdress_23_0_shift		16

#define _4KBYTES 8192


struct selectorStruct{
	uint32_t index;
	uint32_t rpl; // requested privellege level
	bool global; // 0 - gdt, 1 - ldt
};

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

selectorStruct pasreSelector(uint32_t selector);
gdtRecordStruct parseGDTRecord(uint64_t record);
void printGdtRecordStruct(gdtRecordStruct struc);
uint32_t getLimit(gdtRecordStruct* struc);
uint64_t resolveAddress(gdtRecordStruct* struc, selectorStruct* ss, uint32_t logicalAddress);
void giveAnswer(bool valid, uint64_t physicalAddress);

selectorStruct pasreSelector(uint32_t selector){
	selectorStruct res;

	res.index = selector >> selectorStruct_index_shift;
	res.rpl = selector & selectorStruct_rpl_bm;
	res.global = selector & selectorStruct_global_bm;

	return res;

}

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

uint32_t getLimit(gdtRecordStruct* struc){
	if(struc->g){
		return (struc->segmentLimit) * _4KBYTES;
	} else {
		return (struc->segmentLimit);
	}
}

uint64_t resolveAddress(bool* valid, gdtRecordStruct* struc, selectorStruct* ss, uint32_t logicalAddress){
	
	if(ss->index > getLimit(struc) || (struc->dpl < ss->rpl)){
		*valid = false;
		return 0;
	} else {
		*valid = true;
		return (struc->baseAddress) + logicalAddress;
	}

}

void giveAnswer(bool valid, uint64_t physicalAddress){
	if(!valid){
		std::cout << "INVALID" << std::endl;
	} else {
		std::cout << std::hex << physicalAddress << std::endl;
	}
}


void printGdtRecordStruct(gdtRecordStruct struc){
	std::cout << "[struc]" << std::endl;
	std::cout << "accessed=" << struc.a << std::endl;
	std::cout << "readable/writable=" << struc.rw << std::endl;
	std::cout << "conforming=" << struc.c << std::endl;
	std::cout << "executable=" << struc.x << std::endl;
	std::cout << "system=" << struc.s << std::endl;
	std::cout << "present=" << struc.p << std::endl;
	std::cout << "avaliable for user=" << struc.avl << std::endl;
	std::cout << "default size 0 - 16bit, 1 - 32bit=" << struc.d << std::endl;
	std::cout << "granularity 0 - 1byte, 1 - 4Kb page=" << struc.g << std::endl;
	std::cout << "dpl=" << struc.dpl << std::endl;
	std::cout << "baseAddress=" << struc.baseAddress << std::endl;
	std::cout << "segmentLimit=" << struc.segmentLimit << std::endl;
	std::cout << std::endl;
}

int main(){

	uint32_t logicalAddress		= 0;
	uint32_t selector 			= 0;
	uint32_t gdtRecordsAmount 	= 0;
	uint64_t gdtRecord 			= 0;
	uint32_t ldtRecordsAmount 	= 0;
	uint64_t ldtRecord 			= 0;

	// freopen("exe.in", "rt", stdin);


	std::cin >> std::hex >> logicalAddress;
	std::cin >> std::hex >> selector;

	selectorStruct ss = pasreSelector(selector);
	bool found = false;

	std::cin >> std::hex >> gdtRecordsAmount;
	
	for(uint32_t i = 0; i < gdtRecordsAmount; i++){
		
		std::cin >> std::hex >> gdtRecord;
		
		if( (ss.global == selectorStruct_globalTable) && (ss.index == i) ){
			
			found = true;
			
			gdtRecordStruct gdtRS = parseGDTRecord(gdtRecord);

			bool valid;
			uint64_t physicalAddress = resolveAddress(&valid, &gdtRS, &ss, logicalAddress);
			
			giveAnswer(valid, physicalAddress);
			// printGdtRecordStruct(gdtRS);
		}
	}

	std::cin >> std::hex >> ldtRecordsAmount;
	
	for(uint32_t i = 0; i < ldtRecordsAmount; i++){

		std::cin >> std::hex >> ldtRecord;

		if( (ss.global == selectorStruct_localTable) && (ss.index == i) ){
			
			found = true;
			
			gdtRecordStruct ldtRS = parseGDTRecord(ldtRecord);
			
			bool valid;
			uint64_t physicalAddress = resolveAddress(&valid, &ldtRS, &ss, logicalAddress);
			
			giveAnswer(valid, physicalAddress);
			// printGdtRecordStruct(ldtRS);
		}
	}
	if(!found){
		giveAnswer(false, 0);
	}


	return 0;
}
