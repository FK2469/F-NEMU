#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, DINT, HINT, REG, EQ, UEQ, AND, OR, NOT, DEREF,VAR

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE},				// spaces
	{"0x[0-9|a-f|A-F]*", HINT},			// hex number
	{"0|[1-9][0-9]*", DINT},			// decimalism number
	{"\\$[a-z|A-Z]+", REG},				// register
	{"\\+", '+'},					// plus
	{"-", '-'},					// minus
	{"\\*", '*'},					// multiple
	{"/", '/'},					// besides
	{"\\(", '('},					// left bracket
	{"\\)", ')'},					// right bracket
	{"==", EQ},					// equal
	{"!=", UEQ},					// unequal
	{"&&", AND},					// and
	{"\\|\\|", OR},					// or
	{"!", NOT},					// not
	{"[a-zA-Z_]+[0-9a-zA-Z_]*", VAR}		// var
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[32];
} Token;

Token tokens[32];
int nr_token;

static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	
	nr_token = 0;

	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
				if(substr_len >= 32) {
					printf("Too long token!\n");
					return false;
				}

				//Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */

				if(nr_token >= 32) {
					printf("Too long expression!\n");
					return false;
				}
				Token t;
				t.type = rules[i].token_type;
				switch(rules[i].token_type) {
					case NOTYPE:
						break;
					case DINT:
					case HINT:
					case REG:
						memcpy(t.str, substr_start, substr_len);
						t.str[substr_len] = '\0';
					   	tokens[nr_token] = t; 
						nr_token ++;
						break;
					case '+':
					case '-':
					case '*':
					case '/':
					case '(':
					case ')':
					case EQ:
					case UEQ:
					case AND:
					case OR:
					case NOT:
						tokens[nr_token] = t;
						nr_token ++;
						break;
					case VAR:
						tokens[nr_token] = t;
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len] = '\0';
						nr_token ++;
						break;
					default: panic("please implement me");
						break;
				}

				break;
			}
		}

		if(i == NR_REGEX) {
			printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
			return false;
		}
	}

	return true; 
}

bool count_parentheses(int p, int q) {
	int i;
	int count = 0;
	for(i = p; i <= q; i ++) {
		if(tokens[i].type == '(') count ++;
		if(tokens[i].type == ')') count --;
		if(count < 0) return false;
	}
	if(count == 0) return true;
	else return false;
}
bool check_parentheses(int p, int q, bool* success) {
	if(tokens[p].type == '(' && tokens[q].type == ')') {
		if(count_parentheses(p, q) == false) {
			printf("Bad expression! Unmatched parentheses\n");
			*success = false;
			return false;
		} else {
			return count_parentheses(p+1, q-1);
		}
	}
	return false;
}

int get_priority(int type) {
	switch (type) {
		case OR:
			return 12;
			break;
		case AND:
			return 11;
			break;
		case EQ:
		case UEQ:
			return 7;
			break;
		case '+':
		case '-':
			return 4;
			break;
		case '*':
		case '/':
			return 3;
			break;
		case NOT:
		case DEREF:
			return 2;
			break;
		default:
			printf("Cannot get priority!\n");
			return -1;
			break;
	}
}

bool is_op(int type) {
	switch (type) {
		case DINT:
		case HINT:
		case REG:
		case '(':
		case ')':
			return false;
	}
	return true;
}

int find_dominant_operator(int p, int q) {
	int op = -1;
	int priority = -1;
	int i;
	int in_parenthes = 0;
	for(i = p; i <= q; i ++) {
		if(tokens[i].type == '(') {
			in_parenthes ++;
			continue;	
		}
		if(tokens[i].type == ')') {
			in_parenthes --;
			continue;
		}
		if(!is_op(tokens[i].type)) continue;
		if(in_parenthes > 0) continue;

		if(priority <= get_priority(tokens[i].type)) {
			priority = get_priority(tokens[i].type);
			op = i;
		}
		
	}
	return op;
}

uint32_t get_register(char* reg) {
	if(strcmp(reg, "$eax") == 0) return cpu.eax;
	if(strcmp(reg, "$ecx") == 0) return cpu.ecx;
	if(strcmp(reg, "$edx") == 0) return cpu.edx;
	if(strcmp(reg, "$ebx") == 0) return cpu.ebx;
	if(strcmp(reg, "$esp") == 0) return cpu.esp;
	if(strcmp(reg, "$ebp") == 0) return cpu.ebp;
	if(strcmp(reg, "$esi") == 0) return cpu.esi;
	if(strcmp(reg, "$edi") == 0) return cpu.edi;
	if(strcmp(reg, "$eip") == 0) return cpu.eip;
	return 0;
}

extern int find_var(char *str);

int Exp_flag = 0;

uint32_t eval(int p, int q, bool *success) {
	if(p > q) {
		printf("Bad expression!\n");
		*success = false;
		return 0;
	} else if(p == q) {
		uint32_t val;
		if(tokens[p].type == DINT) sscanf(tokens[p].str, "%d", &val);
		else if(tokens[p].type == HINT) sscanf(tokens[p].str, "%x", &val);
		else if(tokens[p].type == REG) val = get_register(tokens[p].str);
		else if(tokens[p].type == VAR) {
			Exp_flag = 1;
			val = find_var(tokens[p].str);
			if(val == -1) {
			printf("No var matched!\n");
			*success = false;
			return 0;
			}
		} 
		else {
			printf("No number matched!\n");
			*success = false;
			return 0;
		}
		return val;
	} else if(check_parentheses(p, q, success) == true) {
		return eval(p + 1, q - 1, success);
	} else if(*success == true) {
		int op = find_dominant_operator(p, q);
		//printf("%d\n", op);
		int op_type = tokens[op].type;
		uint32_t val1, val2;
		if(op_type == NOT || op_type == DEREF) {
			val1 = eval(op+1, q, success);
			val2 = eval(op+1, q, success);
		} else {
			val1 = eval(p, op-1, success);
			val2 = eval(op+1, q, success);
		}

		switch(op_type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			case EQ: return val1 == val2;
			case UEQ: return val1 != val2;
			case AND: return val1 && val2;
			case OR: return val1 || val2;
			case NOT: return !val2;
			case DEREF: return swaddr_read(val2, 4,R_DS);
			default: 
				*success = false;
				printf("Cannot evaluate expression!\n");
				return 0;
		}
	} else {
		return 0;
	}
}

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}

	/* TODO: Insert codes to evaluate the expression. */
	int i;
	for(i = 0; i < nr_token; i ++) {
		if(tokens[i].type == '*' && (i == 0 || is_op(tokens[i-1].type))) {
				tokens[i].type = DEREF;
				}
	}
	/* TEST: printf("Test func:expr(char*,bool*)\n and the nr_token is %d\n",nr_token); */
	uint32_t result = eval(0, nr_token-1, success);
	return result;
}
