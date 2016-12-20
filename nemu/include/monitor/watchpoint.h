#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next;
	int last_value;
	char str[128];

	/* TODO: Add more members if necessary */


} WP;

#endif
