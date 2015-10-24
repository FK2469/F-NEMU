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

make_helper_v(ja_si)
make_helper_v(jae_si)
make_helper_v(jb_si)
make_helper_v(jbe_si)
make_helper_v(jc_si)
make_helper_v(je_si)
make_helper_v(jz_si)
make_helper_v(jg_si)
make_helper_v(jge_si)
make_helper_v(jl_si)
make_helper_v(jle_si)
make_helper_v(jna_si)
make_helper_v(jnae_si)
make_helper_v(jnb_si)
make_helper_v(jnbe_si)
make_helper_v(jnc_si)
make_helper_v(jne_si)
make_helper_v(jng_si)
make_helper_v(jnge_si)
make_helper_v(jnl_si)
make_helper_v(jnle_si)
make_helper_v(jno_si)
make_helper_v(jnp_si)
make_helper_v(jns_si)
make_helper_v(jnz_si)
make_helper_v(jo_si)
make_helper_v(jp_si)
make_helper_v(jpe_si)
make_helper_v(jpo_si)
make_helper_v(js_si)
