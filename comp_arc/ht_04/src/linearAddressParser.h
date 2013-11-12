#ifndef LINAER_ADDRESS_PARSER_H
#define LINAER_ADDRESS_PARSER_H

#include "linearAddressStruct.h"

#define linearAddress_directory_bm			0xFFC00000
#define linearAddress_directory_shift		22

#define linearAddress_table_bm				0x003FF000
#define linearAddress_table_shift			12

#define linearAddress_offset_bm				0x00000FFF
#define linearAddress_offset_shift			0

#define linearAddress_longOffset_bm			0x003FFFFF
#define linearAddress_longOffset_shift		0




linearAddressStruct parseLinearAddress(uint32_t linearAddress);


#endif
