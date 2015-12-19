#undef COL_WIDTH
#undef LINE_WIDTH
#undef SET_WIDTH
#undef FLAG_WIDTH
#undef LEVEL

#undef NR_COL
#undef NR_LINE
#undef NR_SET
#undef NR_FLAG
#undef HW_MEM_SIZE
#undef LINE_MASK
#undef COL_MASK


#undef cache
#undef cache_addr
#undef cache_t
#undef cache_read
#undef cache_write
#undef cache_set_read
#undef cache_set_write
#undef write_cache
#undef write_back

#ifdef WRITE_THOUGH
	#undef WRITE_THOUGH
#else 
	#undef WRITE_BACK
#endif
