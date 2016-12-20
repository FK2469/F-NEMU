#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

make_helper_v(jb_si)
make_helper_v(ja_si)
make_helper_v(jae_si)
make_helper_v(je_si)
make_helper_v(jbe_si)
make_helper_v(jg_si)
make_helper_v(jge_si)
make_helper_v(jl_si)
make_helper_v(jle_si)
make_helper_v(js_si)
make_helper_v(jne_si)
make_helper_v(jns_si)

