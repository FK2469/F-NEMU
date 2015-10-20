#include "cpu/exec/template-start.h"

make_helper(concat(movzx_b_rm2r_, SUFFIX)){
    int len = decode_rm2r_b(eip+1);
    DATA_TYPE val = (DATA_TYPE)(op_src->val & 0xff);
    REG(op_dest->reg) = val;
    return len + 1;
}

#if DATA_BYTE == 4
make_helper(movzx_w_rm2r_l){
    int len = decode_rm2r_w(eip + 1);
    unsigned val = (unsigned)(op_src->val & 0xffff);
    reg_l(op_dest->reg) = val;
    return len + 1;
}
#endif

#include "cpu/exec/template-end.h"
