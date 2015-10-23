#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	char* what;
	uint32_t value;
	struct watchpoint *next;

	/* TODO: Add more members if necessary */


} WP;

WP* new_wp();
void free_wp(int N);
void print_wp();
bool check_watchpoint();
#endif
