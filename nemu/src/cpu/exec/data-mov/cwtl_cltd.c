#include "cpu/exec/helper.h"

#define DATA_BYTE 2
#include "cwtl_cltd-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "cwtl_cltd-template.h"
#undef DATA_BYTE

make_helper_v(cwtl_cltd_r)
