#include <iostream>
// #include <iomanip>

#include "debugFunctions.h"

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
	std::cout << "global=" << glob[ss.global] << std::endl;
	std::cout << std::endl;
}

void printLinearAddressStruct(linearAddressStruct las){
	std::cout << "[linearAddressStruct]" << std::endl;
	std::cout << "directory=" << las.directory << std::endl;
	std::cout << "table=" << las.table << std::endl;
	std::cout << "offset=" << las.offset << std::endl;
	std::cout << "longOffset=" << las.longOffset << std::endl;
	std::cout << std::endl;
}

void printPDRecordStruct(pdRecordStruct pdrs){
	std::cout << "[pdRecordStruct]" << std::endl;
	
	std::cout << "baseAddress=" << pdrs.baseAddress << std::endl;
	std::cout << "baseAddressLargePage=" << pdrs.baseAddressLargePage << std::endl;
	std::cout << "userAlailable=" << pdrs.userAlailable << std::endl;
	
	std::cout << "present=" << pdrs.present << std::endl;
	std::cout << "readWrite=" << pdrs.readWrite << std::endl;
	std::cout << "userSupervisor=" << pdrs.userSupervisor << std::endl;
	std::cout << "pageWriteThrough=" << pdrs.pageWriteThrough << std::endl;
	std::cout << "pageCacheDisable=" << pdrs.pageCacheDisable << std::endl;
	std::cout << "accessed=" << pdrs.accessed << std::endl;
	std::cout << "dirty=" << pdrs.dirty << std::endl;
	std::cout << "pageSize_pageAttributeTableIndex=" << pdrs.pageSize_pageAttributeTableIndex << std::endl;
	std::cout << "global=" << pdrs.global << std::endl;
	std::cout << std::endl;

}