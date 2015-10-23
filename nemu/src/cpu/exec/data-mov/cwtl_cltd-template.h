#include "cpu/exec/template-start.h"

make_helper(concat(cwtl_cltd_r_,SUFFIX))
{
    DATA_TYPE sign0 = 0x0;
    DATA_TYPE_S val = REG(cpu.eax);
    if(val < 0){
	REG(cpu.edx) =~sign0;
    }
    else {
	REG(cpu.edx) = sign0;
    }
    return 1;

#include "cpu/exec/template-end.h"
}
