typedef union {
	struct {
		uint32_t col	: COL_WIDTH;
		uint32_t flag	: FLAG_WIDTH;
	};
	uint32_t addr;
} tlb_addr;

#define NR_LINE (1 << LINE_WIDTH)
#define NR_FLAG (1 << FLAG_WIDTH)

#define LINE_MASK (NR_LINE - 1)
#define COL_MASK (NR_COL - 1)

typedef	struct {
	PTE data[NR_LINE];
	bool valid[NR_LINE];
	uint32_t flag[NR_LINE];
} tlb_t;

