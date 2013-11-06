#include <iostream>

typedef unsigned long long int uint64_t;
typedef unsigned int uint32_t;

#define gdtRecord_p_bp		15
#define gdtRecord_avl_bp	20
#define gdtRecord_g_bp		23
#define gdtRecord_s_bp		12
#define gdtRecord_db_bp		22

#define gdtRecord_p_bm		(1 << gdtRecord_p_bp)
#define gdtRecord_avl_bm	(1 << gdtRecord_a_bp)
#define gdtRecord_g_bm		(1 << gdtRecord_g_bp)
#define gdtRecord_s_bm		(1 << gdtRecord_s_bp)
#define gdtRecord_db_bm		(1 << gdtRecord_db_bp)



#define gdtRecord_dpl_bm					0x00006000
#define gdtRecord_dpl_shift					

#define gdtRecord_segmentLimit_19_16_bm		0x000000FF00000000LL
#define gdtRecord_segmentLimit_19_16_shift	
#define gdtRecord_segmentLimit_15_0_bm		0x000000000000FFFFLL
#define gdtRecord_segmentLimit_15_0_shift	

#define gdtRecord_baseAdress_31_24_bm		0xFF00000000000000LL
#define gdtRecord_baseAdress_31_24_shift	32
#define gdtRecord_baseAdress_23_0_bm		0x000000FFFFFF0000LL
#define gdtRecord_baseAdress_23_0_shift		16




struct gdtRecordStruct{
	uint32_t segmentLimit;
	uint64_t baseAddress;
	uint32_t type;
	uint32_t dpl;
	bool g;
	bool db;
	bool a;
	bool p;
	bool s;
};

gdtRecordStruct parseGDTRecord(uint64_t record);


gdtRecordStruct parseGDTRecord(uint64_t record){

	gdtRecordStruct res;

	res.dpl = (record & gdtRecord_dpl_bm) >> gdtRecord_dpl_shift;
	res.type = (record & gdtRecord_type_bm) >> gdtRecord_type_shift;

	res.g = record & gdtRecord_p_bm;
	res.db = record & gdtRecord_a_bm;
	res.a = record & gdtRecord_g_bm;
	res.p = record & gdtRecord_s_bm;
	res.s = record & gdtRecord_db_bm;
	
	res.segmentLimit = ((record & gdtRecord_segmentLimit_0_15_bm) >> gdtRecord_segmentLimit_0_15_shift) || ((record & gdtRecord_segmentLimit_16_19_bm) >> gdtRecord_segmentLimit_16_19_shift);
	
	res.baseAddress = ( (record & gdtRecord_baseAdress_0_15_bm) >> gdtRecord_baseAdress_0_15_shift )
		|| ( (record & gdtRecord_baseAdress_16_23_bm) >> gdtRecord_baseAdress_16_23_shift )
		|| ( (record & gdtRecord_baseAdress_24_31_bm) >> gdtRecord_baseAdress_24_31_shift );

	return res;

};

int main(){

	uint32_t logicalAddress = 0;
	uint32_t selector = 0;
	int gdtRecordsAmount = 0;
	uint64_t gdtRecord = 0;
	int ldtRecordsAmount = 0;
	uint64_t ldtRecord = 0;

	while(1){
		std::cin >> std::hex >> logicalAddress >> selector >> gdtRecordsAmount;
		for(int i = 0; i < gdtRecordsAmount; i++){
			std::cin >> std::hex >> gdtRecord;
			gdtRecordStruct gdtRS = parseGDTRecord(gdtRecord);
		}
		std::cin >> std::hex >> ldtRecordsAmount;
		for(int i = 0; i < ldtRecordsAmount; i++){
			std::cin >> std::hex >> ldtRecordsAmount;
			// gdtRecordStruct ldtRS = parseGDTRecord(ldtRecordsAmount);
		}
		
	}
	


	return 0;
}
