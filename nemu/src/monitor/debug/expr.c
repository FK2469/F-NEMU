#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>

enum {
	NOTYPE = 256, EQ, NB, MS, DR, NEQ, AND_L, OR_L ,NOT_L, MOD, AND, OR, NOT, LE, BE, L, B, VAR,

	/* TODO: Add more token types */

};

extern char *strtab;

static struct rule {
	char *regex;
	int token_type;
	int level;
	bool singel;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +",	NOTYPE, 10, false},							// spaces
	{"\\+", '+', 4, false},								// plus
	{"-", '-', 4, false},								// minus
	{"\\*", '*', 5, false},								// multiply
	{"/", '/', 5, false},								// devide
	{"%", MOD, 5, false},								// mod
	{"0x[0-9a-fA-F]+|[0-9]+|\\$[a-z]+", NB, 10, false},	// number
	{"!=", NEQ, 0, true},								// not equal
	{"==", EQ, 0, false},								// equal
	{"<=", LE, 0, false},								// less or equal
	{">=", BE, 0, false},								// bigger or equal
	{"<", L, 0, false},									// less
	{">", B, 0, false},									// biger
	{"&&", AND_L, -1, false},							// logic and
	{"\\|\\|", OR_L, -1, false},						// logic or
	{"\\!", NOT_L, 0, true},							// logic not
	{"&", AND, 2, false},								// bit and
	{"\\|", OR, 1, false},								// bit or
	{"~", NOT, 3, false},								// bit not
	{"\\(", '(', 10, false},							// left par
	{"\\)", ')', 10, false},							// right par
	{"-", MS, 9, true},									// minus sign
	{"\\*", DR, 9, true},								// dereference
	{"[a-zA-Z_]+[0-9a-zA-Z_]*", VAR, 10, false},					// var
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
	int level;
	bool singel;
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
			//	printf("*****\n");
				char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;
//				Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				position += substr_len;
				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
				tokens[nr_token].type = rules[i].token_type;
				tokens[nr_token].level = rules[i].level;
				tokens[nr_token].singel = rules[i].singel;
				switch(rules[i].token_type) {
					case '+': case '(': case ')': case '/':	
					case EQ: case MS: case DR: 
					case AND_L: case OR_L: case NOT_L:
					case AND: case OR: case NOT: case MOD: 
					case LE: case BE: case L: case B: break;
					case '-': 
						if (nr_token == 0 || tokens[nr_token-1].type != NB) {
							tokens[nr_token].type = MS;
							tokens[nr_token].level = 9;
							tokens[nr_token].singel = true;
						}
						break;
					case '*':
						if (nr_token == 0 || tokens[nr_token-1].type != NB) {
							tokens[nr_token].type = DR;
							tokens[nr_token].level = 9;
							tokens[nr_token].singel = true;
						}
						break;
					case NB: case VAR:
						strncpy(tokens[nr_token].str, substr_start, substr_len);
						tokens[nr_token].str[substr_len] = '\0';
						break;
					case NOTYPE: nr_token --; break;
					default: panic("please implement me");
				}
				//printf("***%d\n",tokens[nr_token].type);
				nr_token ++;
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

extern bool flag;

bool check_parentheses(int p, int q) {
	if (tokens[p].type != '(' || tokens[q].type != ')') return false;
	int i,num=0;
	for(i = p + 1; i < q; ++ i) {
		if (tokens[i].type == '(') num ++;
		if (tokens[i].type == ')') num --;
		if (num < 0) return false;
	}
	return num == 0;
}

uint32_t select_op(int p, int q) {
	int i, min_level = 10;
	int in_par = 0;
	for(i = p; i <= q; ++ i) {
		if (tokens[i].type == '(') in_par ++;
		if (tokens[i].type == ')') in_par --;		
		if (in_par) continue;
		if (tokens[i].level < min_level) min_level = tokens[i].level;
	}
	for(i = q; i >= p; -- i) {
		if (tokens[i].type == '(') in_par ++;
		if (tokens[i].type == ')') in_par --;		
		if (in_par) continue;
		if (tokens[i].level == min_level) {
			if (tokens[i].level == 9) {
				while(i > p && tokens[i-1].level == 9) {
					i --;
				}
			}
//			printf("%d\n",i);
			return i;
		}
	}
//	panic("Can't find op!");
	flag = false;
	return 0;

}

extern int find_var(char *str);

uint32_t eval(p, q) {
	if (!flag) return 0;
	if (p > q) {
//		panic("No number!");
		flag = false;
		return 0;
		/* Bad expression */
	}
	else if (p == q) {
		/* Single token.
		 * For now this token should be a number. 
		 * Return the value of the number.
		 */
		if (tokens[p].type != NB && tokens[p].type != VAR) {
			flag = false;
			return 0;
		}
		int value = 0,i = 0;
//		printf("str=%s\n,value=%d\n",tokens[p].str,value);
		if (tokens[p].type == VAR) {
//			printf("%s\n", tokens[p].str);
			value = find_var(tokens[p].str);
			if(value == -1) {
				flag = false;
				return 0;
			}
		} else
		if (tokens[p].str[0] == '$') {
			char *reg = tokens[p].str + 1;
//			printf("%s\n",reg);
			if (strcmp(reg, "eip") == 0) {
				value = cpu.eip;
			} else
			for(i = 0; i < 8; ++ i) {
				if (strcmp(regsl[i], reg) == 0) {
					value = cpu.gpr[i]._32;
					break;
				}
			}
			if (i == 8) {
				flag = false;
				return 0;
			}
		} else 
		if (strlen(tokens[p].str) < 2 || tokens[p].str[1] != 'x') {
			for(i = 0; i < strlen(tokens[p].str); ++ i) {
				value = value * 10 + tokens[p].str[i] - '0';
			}
		} else {
			for(i = 2; i < strlen(tokens[p].str); ++ i) {
				if(tokens[p].str[i] >= '0' && tokens[p].str[i] <= '9')
					value = value * 16 + tokens[p].str[i] - '0'; 
				if(tokens[p].str[i] >= 'A' && tokens[p].str[i] <= 'F')
					value = value * 16 + tokens[p].str[i] - 'A' + 10; 
				if(tokens[p].str[i] >= 'a' && tokens[p].str[i] <= 'f')
					value = value * 16 + tokens[p].str[i] - 'a' + 10;
			}
		}
		return value;
	}
	else if(check_parentheses(p, q) == true) {
		/* The expression is surrounded by a matched pair of parentheses. 
		 * If that is the case, just throw away the parentheses.
		 */
		return eval(p + 1, q - 1);
	}
	else {
		int op = select_op(p, q); //the position of dominant operator in the token expression;
		int val1 = 0, val2 = 0; 
		if (!flag) return 0;
		if (tokens[op].singel) {
			val1 = eval(op + 1, q);
		} else {
			val1 = eval(p, op - 1); 
			val2 = eval(op + 1, q);
		}
		switch(tokens[op].type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			case EQ: return val1 == val2;
			case MS: return - val1;
			case DR: return swaddr_read(val1, 4, R_DS);
			case AND_L:	return val1 && val2;
			case OR_L: return val1 || val2;
			case NOT_L:	return !val1;
			case AND: return val1 & val2;
			case OR: return val1 | val2;
			case NOT: return ~val1;
			case MOD: return val1 % val2;
			case LE: return val1 <= val2;
			case BE: return val1 >= val2;
			case L: return val1 < val2;
			case B: return val1 > val2;
			case NB: case VAR: flag = false; return 0;
		default: assert(0);
		}
	//	panic("error");
		return 0;
	}
	return 0;
}

extern void print();

uint32_t expr(char *e, bool *success) {
	if(!make_token(e)) {
		*success = false;
		return 0;
	}
//	print();
	return eval(0,nr_token-1);
	/* TODO: Insert codes to evaluate the expression. */
	panic("please implement me");
	return 0;
}

