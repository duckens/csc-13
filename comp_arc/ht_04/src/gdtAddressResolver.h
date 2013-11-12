#ifndef GDT_ADDRESS_RESOLVER
#define GDT_ADDRESS_RESOLVER

#include "selectorStruct.h"
#include "gdtRecordStruct.h"

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

bool expandingDown(gdtRecordStruct* struc);
uint32_t getMaxAddress(gdtRecordStruct* struc);
uint32_t getMinAddress(gdtRecordStruct* struc);
uint64_t gdtResolveAddress(bool* valid, gdtRecordStruct* struc, selectorStruct* ss, uint32_t logicalAddress);
bool checkLimitOk(gdtRecordStruct* struc, uint32_t logicalAddress);
bool checkPrivellegeLevelOk(gdtRecordStruct* struc, selectorStruct* ss);
bool checkPresentOk(gdtRecordStruct* struc);
uint32_t effectiveSegmentLimitGranularityPage(uint32_t segmentLimit);


#endif
