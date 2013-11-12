#include "pdAddressResolver.h"
#include "pdRecordParser.h"

#include "debugFunctions.h"

uint32_t pdResolveAddress(bool* physicalAddress_valid, bool* pageInMemory, std::vector<uint32_t>* pd, std::vector<uint32_t>* pt, linearAddressStruct* las){

	*physicalAddress_valid = false;

	uint64_t physicallAddsess;

	if(canFindPDPTRecord(pd, las->directory)){

		pdRecordStruct pdrs = pasrePDRecord((*pd)[las->directory]);

		#ifdef DEBUG
		printPDRecordStruct(pdrs);
		#endif

		if(largePageInPD(&pdrs)){


			*physicalAddress_valid = true;
			*pageInMemory = inMemory(&pdrs);

			physicallAddsess = pdrs.baseAddressLargePage + las->longOffset;

		} else {

			if(inMemory(&pdrs) && canFindPDPTRecord(pt, las->table)){

				pdRecordStruct ptrs = pasrePDRecord((*pt)[las->directory]);

				#ifdef DEBUG
				printPDRecordStruct(ptrs);
				#endif

				*physicalAddress_valid = true;
				*pageInMemory = inMemory(&ptrs);

				physicallAddsess = ptrs.baseAddress + las->offset;
				
			}
		}
	}
}

bool canFindPDPTRecord(std::vector<uint32_t>* pd, uint32_t directory){
	return (directory >= 0)
		&& (directory < pd->size());
}

bool inMemory(pdRecordStruct* pageDirectoryStruc){
	return pageDirectoryStruc->present;
}

bool largePageInPD(pdRecordStruct* pageDirectoryStruc){
	return pageDirectoryStruc->pageSize_pageAttributeTableIndex;
}
