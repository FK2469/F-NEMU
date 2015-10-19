#include "cpu/exec/helper.h"
#define DATA_BYTE 1
#include "cpu/exec/template-start.h"

int seta_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if((cpu.CF == 0) && (cpu.ZF == 0)){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setae_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.CF == 0){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setb_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.CF == 1){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setbe_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if((cpu.ZF == 1) || (cpu.CF == 1)){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setc_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.CF == 1){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int sete_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.ZF == 1){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setg_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if((cpu.ZF == 0) && (cpu.SF == cpu.OF)){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setge_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.SF == cpu.OF){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setl_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.SF != cpu.OF){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setle_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if((cpu.ZF = 1) || ( cpu.SF != cpu.OF)){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setna_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if((cpu.CF == 1) || (cpu.ZF == 1)){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setnae_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.CF == 1){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setnb_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.CF == 0){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setnbe_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if((cpu.CF == 0) && (cpu.ZF == 0)){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setnc_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.CF == 0){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setne_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.ZF == 0){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setng_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if((cpu.SF != cpu.OF) || (cpu.ZF == 1)){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setnge_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.SF != cpu.OF){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setnl_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.SF == cpu.OF){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setnle_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if((cpu.SF == cpu.OF) && (cpu.ZF == 0)){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setno_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.OF == 0){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setnp_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.PF == 0){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setns_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.SF == 0){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setnz_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.ZF == 0){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int seto_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.OF == 1){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setp_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.PF == 1){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setpe_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.PF == 1){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setpo_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.PF == 0){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int sets_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.SF == 1){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

int setz_rm_b(swaddr_t eip){
    int len = decode_rm_b(eip + 1);
    if(cpu.ZF == 1){
        OPERAND_W(op_src, 1);
    }
    else { OPERAND_W(op_src, 0); }
    return len + 1;
}

#undef DATA_BYTE
#include "cpu/exec/template-end.h"
