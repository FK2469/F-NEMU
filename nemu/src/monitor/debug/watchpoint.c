#include "monitor/watchpoint.h"
#include "monitor/expr.h"

#define NR_WP 32

static WP wp_list[NR_WP];
static WP *head, *free_;

void init_wp_list() {
	int i;
	for(i = 0; i < NR_WP; i ++) {
		wp_list[i].NO = i;
		wp_list[i].what = (char*)malloc(64*sizeof(char));
		wp_list[i].next = &wp_list[i + 1];
	}
	wp_list[NR_WP - 1].next = NULL;

	head = NULL;
	free_ = wp_list;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp() {
	if(free_ == NULL) {
		printf("No more free watchpoints!\n");
		assert(0);
	}

	WP* wp = free_;
	free_ = free_->next;
	wp->next = NULL;

	if(head == NULL) {
		head = wp;
	} else {
		WP* p = head;
		while(p->next != NULL) p = p->next;
		p->next = wp;
	}

	return wp;
}

void free_wp(int N) {
	WP* p = head;
	while(p->NO != N && p != NULL) p = p->next;
	if(p == NULL) {
		printf("Free watchpint failed: No watchpoint%d\n", N);
		return;
	}

	WP* wp = p;
	if(wp == head) {
		head = head->next;
	} else {
		p = head;
		while(p->next != wp) p = p->next;
		p->next = wp->next;
	}
	wp->next = NULL;
	if(free_ == NULL) {
		free_ = wp;
	} else {
		p = free_;
		while(p->next != NULL) p = p->next;
		p->next = wp;
	}
}

void print_wp() {
	printf("Num\tExpression\tValue\tValue\n");
	WP* p = head;
	while(p != NULL) {
		printf("%d\t%s\t%d\t%x\n", p->NO, p->what, p->value, p->value);
		p = p->next;
	}
}

bool check_watchpoint() {
	WP* p = head;
	while(p != NULL) {
		bool* success = malloc(sizeof(bool));
		*success = true;
		uint32_t val = expr(p->what, success);
		if(val != p->value) {
			printf("Watchpint %d occurred!\n", p->NO);
			printf("The \"%s\"'s value is changed!\n",p->what);
			return true;
		}
		p = p->next;
	}
	return false;
}
