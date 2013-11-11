	
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <vector>

// this programm supposes that CR4.PAE = 0, CR4.PSE = 1
// CR4.PAE = 0 cause output should always be 32-bit long (in case CR4.PAE = 1 it's 36-bit)
// CR4.PSE = 1 cause otherwise there's no possibility for PD to cantain page entries => boring

// #define DEBUG
// #define LOCAL

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


#define gdtRecord_present_no									0
#define gdtRecord_present_yes									1

#define gdtRecord_granularityByte								0
#define gdtRecord_granularityPage								1

#define gdtRecord_defaultSize16bit								0
#define gdtRecord_defaultSize32bit								1

#define gdtRecord_segmentLimit_granularityPage_shift 			12
#define gdtRecord_segmentLimit_granularityPage_lowestBits_bm 	0xFFFLL


#define gdtRecord_segmentLimit_maxVal_defaultSize16bit 			0xFFFFLL
#define gdtRecord_segmentLimit_maxVal_defaultSize32bit 			0xFFFFFFFFLL

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
};

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
#define pdRecord_baseAddress_shift						0


struct selectorStruct{

	uint32_t index;
	uint32_t rpl; 	// requested privellege level
	bool global; 	// 0 - gdt, 1 - ldt

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



pdRecordStruct pasrePDRecord(uint32_t record);
bool expandingDown(gdtRecordStruct* struc);
selectorStruct pasreSelector(uint32_t selector);
gdtRecordStruct parseGDTRecord(uint64_t record);
void printGdtRecordStruct(gdtRecordStruct struc);
uint32_t getMaxAddress(gdtRecordStruct* struc);
uint32_t getMinAddress(gdtRecordStruct* struc);
uint64_t resolveAddress(gdtRecordStruct* struc, selectorStruct* ss, uint32_t logicalAddress);
void giveAnswer(bool valid, uint64_t physicalAddress);
void printSelectorStruct(selectorStruct ss);
bool checkLimitOk(gdtRecordStruct* struc, uint32_t logicalAddress);
bool checkPrivellegeLevelOk(gdtRecordStruct* struc, selectorStruct* ss);
bool checkPresentOk(gdtRecordStruct* struc);
bool canAccessTableRecord(std::vector<uint64_t>* table, bool global, uint32_t index);
uint32_t effectiveSegmentLimitGranularityPage(uint32_t segmentLimit);

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

	res.baseAddress = (record & pdRecord_baseAddress_bm) >> pdRecord_baseAddress_shift;

	return res;
}

bool expandingDown(gdtRecordStruct* struc){
	// here is written: non-system (i.e. code or data), not executable (i.e. data, not code), expanding down.
	return (struc->s == 1) && (struc->x == 0) && (struc->c == 1);
}


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

uint32_t effectiveSegmentLimitGranularityPage(uint32_t segmentLimit){
	return (segmentLimit << gdtRecord_segmentLimit_granularityPage_shift)
		| gdtRecord_segmentLimit_granularityPage_lowestBits_bm;
}


uint32_t getMaxAddress(gdtRecordStruct* struc){
	if(expandingDown(struc)){
		if(struc->d == gdtRecord_defaultSize16bit){
			return gdtRecord_segmentLimit_maxVal_defaultSize16bit;
		} else {
			return gdtRecord_segmentLimit_maxVal_defaultSize32bit;
		}
	} else {
		if(struc->g == gdtRecord_granularityByte){
			return (struc->segmentLimit);
		} else {
			return effectiveSegmentLimitGranularityPage(struc->segmentLimit);
		}
	}
}

uint32_t getMinAddress(gdtRecordStruct* struc){
	if(expandingDown(struc)){
		// return struc->segmentLimit + 1;
		if(struc->g == gdtRecord_granularityByte){
			return struc->segmentLimit + 1;
		} else {
			return effectiveSegmentLimitGranularityPage(struc->segmentLimit) + 1;
		}
	} else {
		return 0;
	}
}

bool checkLimitOk(gdtRecordStruct* struc, uint32_t logicalAddress){
	return (getMinAddress(struc) <= logicalAddress) && (logicalAddress <= getMaxAddress(struc));
}

bool checkPrivellegeLevelOk(gdtRecordStruct* struc, selectorStruct* ss){
	return (struc->dpl >= ss->rpl);
}

bool checkPresentOk(gdtRecordStruct* struc){
	return struc->p == gdtRecord_present_yes;
}

uint64_t resolveAddress(bool* valid, gdtRecordStruct* struc, selectorStruct* ss, uint32_t logicalAddress){
	
	if(checkLimitOk(struc, logicalAddress) && checkPrivellegeLevelOk(struc, ss) && checkPresentOk(struc) ){
		*valid = true;
		return (struc->baseAddress) + logicalAddress;
	} else {
		*valid = false;
		return 0;
	}

}

void giveAnswer(bool valid, uint64_t physicalAddress){
	if(!valid){
		std::cout << "INVALID" << std::endl;
	} else {
		std::cout << std::hex << std::setfill('0') << std::setw(8) << physicalAddress << std::endl;
	}
}

bool canAccessTableRecord(std::vector<uint64_t>* table, bool global, uint32_t index){
	return (index >= 0)
		&& (index < table->size())
		&& ( (global != selectorStruct_globalTable) || (index != 0) );
}

void printGdtRecordStruct(gdtRecordStruct struc){
	std::cout << "[gdtRecordStruct]" << std::endl;
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

void printSelectorStruct(selectorStruct ss){
	std::cout << "[selectorStruct]" << std::endl;
	std::cout << "index=" << ss.index << std::endl;
	std::cout << "rpl=" << ss.rpl << std::endl;
	const char* glob[2] = {"gdt", "ldt"};
	std::cout << "global=" << glob[ss.global] << std::endl << std::endl;
}



int main(int argc, char** argv){

	uint32_t logicalAddress		= 0;
	uint32_t selector 			= 0;
	uint32_t gdtRecordsAmount 	= 0;
	uint32_t ldtRecordsAmount 	= 0;
	uint32_t pdRecordsAmount	= 0;
	uint32_t ptRecordsAmount	= 0;

	std::vector<uint64_t>* gdt = new std::vector<uint64_t>;
	std::vector<uint64_t>* ldt = new std::vector<uint64_t>;
	std::vector<uint64_t>* pd = new std::vector<uint64_t>;
	std::vector<uint64_t>* pt = new std::vector<uint64_t>;

	#ifdef LOCAL
	if(argc > 1){
		std::cout << "LOCAL MODE." << std::endl;		
		const char* inputFile = argv[1];
		std::cout << "freopen(" << inputFile << ")" << std::endl;
		freopen(inputFile, "rt", stdin);
	}
	#endif

	std::cin >> std::hex >> logicalAddress;
	std::cin >> std::hex >> selector;
	

	std::cin >> std::hex >> gdtRecordsAmount;
	gdt->resize(gdtRecordsAmount);
	for(uint32_t i = 0; i < gdtRecordsAmount; i++){
		std::cin >> std::hex >> (*gdt)[i];
	}

	std::cin >> std::hex >> ldtRecordsAmount;
	ldt->resize(ldtRecordsAmount);
	for(uint32_t i = 0; i < ldtRecordsAmount; i++){
		std::cin >> std::hex >> (*ldt)[i];	
	}
	
	std::cin >> std::hex >> pdRecordsAmount;
	pd->resize(pdRecordsAmount);
	for(uint32_t i = 0; i < pdRecordsAmount; i++){
		std::cin >> std::hex >> (*pd)[i];	
	}

	std::cin >> std::hex >> ptRecordsAmount;
	pt->resize(ptRecordsAmount);
	for(uint32_t i = 0; i < ptRecordsAmount; i++){
		std::cin >> std::hex >> (*pt)[i];	
	}


	#ifdef DEBUG
	std::cout << "logicalAddress=" << logicalAddress << std::endl;
	#endif

	selectorStruct ss = pasreSelector(selector);
	#ifdef DEBUG
	printSelectorStruct(ss);
	#endif

	std::vector<uint64_t>* table = (ss.global == selectorStruct_globalTable) ? gdt : ldt;

	if( canAccessTableRecord(table, ss.global, ss.index) ){
		
		uint64_t record = (*table)[ss.index];

		bool valid;
		uint64_t physicalAddress;

		gdtRecordStruct tableRS = parseGDTRecord(record);
		physicalAddress = resolveAddress(&valid, &tableRS, &ss, logicalAddress);

		#ifdef DEBUG
		printGdtRecordStruct(tableRS);
		#endif

		giveAnswer(valid, physicalAddress);

	} else {
		giveAnswer(false, 0);
	}

	delete gdt;
	delete ldt;

	return 0;
}
