#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "setcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "setcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "setcc-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

//make_helper_v(setle_si)

