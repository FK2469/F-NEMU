#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "movsr-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "movsr-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "movsr-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

