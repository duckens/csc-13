	
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <vector>

#include "selectorStructParser.h"
#include "gdtRecordParser.h"
#include "pdRecordParser.h"
#include "linearAddressParser.h"
#include "gdtAddressResolver.h"
#include "pdAddressResolver.h"
#include "debugFunctions.h"


// this programm supposes that
// 1) MAXPHYADDR is 32
// 2) CR4.PAE = 0 cause output should always be 32-bit long (in case CR4.PAE = 1 it's 36-bit)
// 3) CR4.PSE = 1 cause otherwise there's no possibility for PD to cantain large (4Mb) page entries => boring


// #define DEBUG
// #define LOCAL


bool canAccessTableRecord(std::vector<uint64_t>* table, bool global, uint32_t index);
void giveAnswer(bool valid, uint32_t physicalAddress, bool pageInMemory);



void giveAnswer(bool valid, uint32_t physicalAddress, bool pageInMemory){
	if(!valid){
		std::cout << "INVALID" << std::endl;
	} else {
		std::cout << std::hex << std::setfill('0') << std::setw(8) << physicalAddress << ":" << pageInMemory << std::endl;
	}
}

bool canAccessTableRecord(std::vector<uint64_t>* table, bool global, uint32_t index){
	return (index >= 0)
		&& (index < table->size())
		&& ( (global != selectorStruct_globalTable) || (index != 0) );
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
	std::vector<uint32_t>* pd = new std::vector<uint32_t>;
	std::vector<uint32_t>* pt = new std::vector<uint32_t>;

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
	

	std::cin >> std::dec >> gdtRecordsAmount;
	gdt->resize(gdtRecordsAmount);
	for(uint32_t i = 0; i < gdtRecordsAmount; i++){
		std::cin >> std::hex >> (*gdt)[i];
	}
	#ifdef DEBUG
	std::cout << "gdtRecordsAmount=" << gdtRecordsAmount << std::endl;
	#endif

	std::cin >> std::dec  >> ldtRecordsAmount;
	ldt->resize(ldtRecordsAmount);
	for(uint32_t i = 0; i < ldtRecordsAmount; i++){
		std::cin >> std::hex >> (*ldt)[i];	
	}
	#ifdef DEBUG
	std::cout << "ldtRecordsAmount=" << ldtRecordsAmount << std::endl;
	#endif
	
	std::cin >> std::dec  >> pdRecordsAmount;
	pd->resize(pdRecordsAmount);
	for(uint32_t i = 0; i < pdRecordsAmount; i++){
		std::cin >> std::hex >> (*pd)[i];	
	}
	#ifdef DEBUG
	std::cout << "pdRecordsAmount=" << pdRecordsAmount << std::endl;
	#endif

	std::cin >> std::dec  >> ptRecordsAmount;
	pt->resize(ptRecordsAmount);
	for(uint32_t i = 0; i < ptRecordsAmount; i++){
		std::cin >> std::hex >> (*pt)[i];
	}
	#ifdef DEBUG
	std::cout << "ptRecordsAmount=" << ptRecordsAmount << std::endl;
	#endif



	selectorStruct ss = pasreSelector(selector);

	#ifdef DEBUG
	std::cout << "logicalAddress=" << logicalAddress << std::endl;
	printSelectorStruct(ss);
	#endif

	std::vector<uint64_t>* table = (ss.global == selectorStruct_globalTable) ? gdt : ldt;

	if( canAccessTableRecord(table, ss.global, ss.index) ){
		
		uint64_t record = (*table)[ss.index];

		gdtRecordStruct tableRS = parseGDTRecord(record);

		bool linearAddress_valid;
		uint64_t linearAddress = gdtResolveAddress(&linearAddress_valid, &tableRS, &ss, logicalAddress);

		#ifdef DEBUG
		printGdtRecordStruct(tableRS);
		#endif

		if(linearAddress_valid){

			linearAddressStruct las = parseLinearAddress(linearAddress);

			#ifdef DEBUG
			printLinearAddressStruct(las);
			#endif

			bool physicalAddress_valid;
			bool pageInMemory;
			uint32_t physicalAddress = pdResolveAddress(&physicalAddress_valid, &pageInMemory, pd, pt, &las);
			
			giveAnswer(physicalAddress_valid, physicalAddress, pageInMemory);

		} else {
			#ifdef DEBUG
			std::cout << "cannot resolve linear address" << std::endl;
			#endif
			giveAnswer(false, 0, 0);	
		}

	} else {
		#ifdef DEBUG
		std::cout << "not found table entry" << std::endl;
		#endif
		giveAnswer(false, 0, 0);
	}

	delete gdt;
	delete ldt;

	return 0;
}
