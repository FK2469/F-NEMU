#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

bool flag;

extern void new_wp(char *args);
extern void free_wp(int n);
extern void print_wp();

void cpu_exec(uint32_t);

/* We use the ``readline'' library to provide more flexibility to read from stdin. */

char* rl_gets() {
	static char *line_read = NULL;
	static char last_command[30];
	int i;
	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");

	if (line_read && *line_read) {
		add_history(line_read);
	}
	if (strlen(line_read) == 0) {
		line_read = (char *)realloc(line_read, sizeof(last_command));
		for(i = 0; i < strlen(last_command); ++ i) {
			line_read[i] = last_command[i];
		}
		line_read[i] = '\0';
	}else {
		for(i = 0; i < strlen(line_read); ++ i) {
		//	if (i > 13) break;
			last_command[i] = line_read[i];
		}
		last_command[i] = '\0';
	}
	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args) {
//	printf("%s\n", strtab);
	int x=0,i;
	if (args == NULL) x = 1; else
	for (i = 0; i < strlen(args); ++ i ) {
		if (args[i] > '9' || args[i] < '0') flag = false;
		if (! flag) break;
		x = x * 10 + args[i] - 48;
	}
	if (! flag) return 0;
//	printf("%d\n",x);
	if (x == 0) x = 1;
	cpu_exec(x);
	return 0;
}


static int cmd_info(char *args) {
	int i;
	if (args == NULL) {
		flag = false; return 0;
	}
	if (strlen(args) != 1) {
		flag = false; return 0;
	}
	if (args[0] == 'r') {
		for(i = R_EAX; i <= R_EDI; ++ i) {
			printf("%-6s0x%-10x\n", regsl[i], reg_l(i));
		}
		printf("%-6s0x%-10x\n", "eip", cpu.eip);
		printf("%-6s0x%-10x\n", "cs", cpu.cs);
		printf("%-6s0x%-10x\n", "EFLAGS", cpu.EFLAGS);
	}else 
	if (args[0] == 'w') {
		print_wp(0);
	}
	else {
		flag = false; return 0;
	}
	return 0;
}

static int cmd_x(char *args) {
 	if (args == NULL) {
		flag = false; return 0;
 	}
	char *pos = strchr(args, ' ');
 	if (pos == NULL) {
		flag = false; return 0;
	}
	int n = 0;
	char *i;
 	for(i = args; i != pos; ++ i) {
 		if (*(i)>'9' || *(i)<'0') {
			flag = false; return 0;
		}
		n = n * 10 + *(i) - '0';
	}
	swaddr_t addr = expr(pos+1, &flag);
	if (!flag) return 0;
	int j;
	int len = 1;
	printf("0x%08x: ",addr);
	for(j = 0; j < n; ++ j) {
		int value = swaddr_read(addr + j, len, R_DS);
		printf("0x%02x ", value);
	}
	printf("\n");
	return 0;
}

static int cmd_p(char *args) {
	if (args == NULL) {
		flag = false;
		return 0;
	}
	int value;
	if (args[0] == '/' && args[1] == 'x') {
		value = expr(args + 3, &flag);
	}else value = expr(args, &flag);
	if (!flag) return 0;
	printf(args[0] == '/' ? "0x%x\n" : "%d\n", value);
	return 0;
}


static int cmd_w(char *args) {
	if(args == NULL) {
		flag = false;
		return 0;
	}
	expr(args, &flag);
	if (!flag) return 0;
	new_wp(args);
	print_wp(0);
	return 0;
}

static int cmd_d(char *args) {
	if(args == NULL) {
		flag = false;
		return 0;
	}
	int value = expr(args, &flag) - 1;
	if (!flag) return 0;
	free_wp(value);
	print_wp(1);
	return 0;
}

extern bool find_func(int addr, char *str);

static int cmd_bt(char *args) {
	int ebp = cpu.ebp;
	int addr = cpu.eip;
	char str[32];
	if(!find_func(addr, str)) {
		printf("No stack.\n");
		return 0;
	}
	int cnt = 0;
	while(find_func(addr, str)) {
		printf("#%d 0x%x in %s(%d, %d, %d, %d)\n", cnt ++, addr, str, 
				swaddr_read(ebp + 0x8, 4, R_SS), swaddr_read(ebp + 0xc, 4, R_SS), 
				swaddr_read(ebp + 0x10, 4, R_SS), swaddr_read(ebp + 0x14, 4, R_SS));
		addr = swaddr_read(ebp + 4, 4, R_SS);
		ebp = swaddr_read(ebp, 4, R_SS);
	}
	return 0;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help          ", "Display informations about all supported commands", cmd_help },
	{ "c             ", "Continue the execution of the program", cmd_c },
	{ "q             ", "Exit NEMU", cmd_q },
	{ "si [N]        ", "Run the program by N command,default by one", cmd_si},
	{ "info SUBCMD   ", "SUBCMD = r print the value of register\n                        = w print the status of watch point", cmd_info },
	{ "x N EXPR      ", "Calculate the value of EXPR, let the answer be the beginning of the memory Address and print the value in the following 4N byte with sixteen decimal", cmd_x },
	{ "p [/x] EXPR   ", "Show the value of the EXPR in decimal, if \"/x\" print in sixteen decimal", cmd_p },
	{ "w EXPR        ", "When the EXPR's value changes, the program will stop", cmd_w },
	{ "d N           ", "Delete the Nth watchpoint", cmd_d},
	{ "bt            ", "Print the stack", cmd_bt},
	/* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strstr(cmd_table[i].name,args) == cmd_table[i].name) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);
		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		flag = true;
		for(i = 0; i < NR_CMD; i ++) {
			if(strstr(cmd_table[i].name, cmd) == cmd_table[i].name) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}
		if(!flag) printf("Wrong usage of '%s'\n",cmd);
		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
