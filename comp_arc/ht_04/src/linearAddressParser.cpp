#include "linearAddressParser.h"

linearAddressStruct parseLinearAddress(uint32_t linearAddress){

	linearAddressStruct res;

	res.directory = (linearAddress & linearAddress_directory_bm) >> linearAddress_directory_shift;
	res.table = (linearAddress & linearAddress_table_bm) >> linearAddress_table_shift;
	res.offset = (linearAddress & linearAddress_offset_bm) >> linearAddress_offset_shift;
	res.longOffset = (linearAddress & linearAddress_longOffset_bm) >> linearAddress_longOffset_shift;

	return res;
}
