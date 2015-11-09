#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "cmovcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "cmovcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cmovcc-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(cmove_rm2r)

make_helper_v(cmovge_rm2r)

make_helper_v(cmovs_rm2r)

make_helper_v(cmovns_rm2r)

