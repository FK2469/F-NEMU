typedef union {
	struct {
		uint32_t col	: COL_WIDTH;
		uint32_t set	: SET_WIDTH;
		uint32_t flag	: FLAG_WIDTH;
	};
	uint32_t addr;
} concat(cache_addr_, LEVEL);

#define NR_COL (1 << COL_WIDTH)
#define NR_LINE (1 << LINE_WIDTH)
#define NR_SET (1 << SET_WIDTH)
#define NR_FLAG (1 << FLAG_WIDTH)

#define HW_MEM_SIZE (1 << 27)

#define LINE_MASK (NR_LINE - 1)
#define COL_MASK (NR_COL - 1)

typedef struct {
	struct {
		uint8_t data[NR_LINE][NR_COL];
		bool valid[NR_LINE];
		bool dirty[NR_LINE];
		uint32_t flag[NR_LINE];
	} set[NR_SET];
} concat(cache_t_, LEVEL);

#define cache concat(cache_, LEVEL)
#define cache_addr concat(cache_addr_, LEVEL)
#define cache_t concat(cache_t_, LEVEL)
#define cache_read concat(cache_read_, LEVEL)
#define cache_write concat(cache_write_, LEVEL)
#define cache_set_read concat(cache_set_read_, LEVEL)
#define cache_set_write concat(cache_set_write_, LEVEL)
#define write_cache concat(write_cache_, LEVEL)
#define write_back concat(write_back_, LEVEL)

