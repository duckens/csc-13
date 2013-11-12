#ifndef DEBUG_FUNCTIONS_H
#define DEBUG_FUNCTIONS_H

#include "selectorStruct.h"
#include "gdtRecordStruct.h"
#include "linearAddressStruct.h"
#include "pdRecordStruct.h"

void printGdtRecordStruct(gdtRecordStruct struc);
void printSelectorStruct(selectorStruct ss);
void printLinearAddressStruct(linearAddressStruct las);
void printPDRecordStruct(pdRecordStruct pdrs);

#endif
