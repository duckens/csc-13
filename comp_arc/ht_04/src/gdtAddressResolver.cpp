#include "gdtAddressResolver.h"

bool expandingDown(gdtRecordStruct* struc){
	// here is written: non-system (i.e. code or data), not executable (i.e. data, not code), expanding down.
	return (struc->s == 1) && (struc->x == 0) && (struc->c == 1);
}

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

uint64_t gdtResolveAddress(bool* valid, gdtRecordStruct* struc, selectorStruct* ss, uint32_t logicalAddress){
	
	if(checkLimitOk(struc, logicalAddress) && checkPrivellegeLevelOk(struc, ss) && checkPresentOk(struc) ){
		*valid = true;
		return (struc->baseAddress) + logicalAddress;
	} else {
		*valid = false;
		return 0;
	}

}
