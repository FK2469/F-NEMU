#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "iret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "iret-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "iret-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

