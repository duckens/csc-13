#ifndef PD_ADDRESS_RESOLVER_H
#define PD_ADDRESS_RESOLVER_H

#include <vector>
#include "linearAddressStruct.h"
#include "pdRecordStruct.h"




uint32_t pdResolveAddress(bool* physicalAddress_valid, bool* pageInMemory, std::vector<uint32_t>* pd, std::vector<uint32_t>* pt, linearAddressStruct* las);
bool canFindPDPTRecord(std::vector<uint32_t>* pd, uint32_t directory);
bool inMemory(pdRecordStruct* pageDirectoryStruc);
bool largePageInPD(pdRecordStruct* pageDirectoryStruc);
uint32_t processLargePageBaseAddress(uint32_t baseAddress);


#endif
