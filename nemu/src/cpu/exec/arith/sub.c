#include "cpu/exec/helper.h"

//#define DATA_BYTE 1
//#include "sub-template.h"
//#undef DATA_BYTE

#define DATA_BYTE 2
#include "sub-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "sub-template.h"
#undef DATA_BYTE

/* for instruction encoding overloading */

//make_helper(sub_i2rm_v);
//make_helper(sub_i2rm_w);
//make_helper(sub_i2rm_l);
//make_helper(sub_i2rm_b);

//make_helper_v(mov_i2r)
make_helper_v(sub_si2rm)
//make_helper_v(mov_r2rm)
//make_helper_v(mov_rm2r)
//make_helper_v(mov_a2moffs)
//make_helper_v(mov_moffs2a)

//make_helper_v(sub_i2rm)
