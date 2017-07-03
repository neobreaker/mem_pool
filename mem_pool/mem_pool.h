#ifndef __MEM_POOL_H__
#define __MEM_POOL_H__

typedef struct 
{
    char* mem_base;
	char* mem_tbl;
	unsigned int mem_base_size;
	unsigned int mem_tbl_size;
	unsigned int mem_block_size;
    
}mem_dev_t;

typedef struct
{
	char *p;
	unsigned int size;
}memp_t;

typedef enum
{
	mem_err_OK,
	mem_err_EMPTY,
	mem_err_NULL,
	mem_err_OUT_OF_RANGE,
}mem_err_t;

mem_err_t mem_pool_init(mem_dev_t *mem_dev, char* mem_base, unsigned int mem_base_size,
                        char *mem_tbl, unsigned int mem_tbl_size);
memp_t mem_pool_malloc(mem_dev_t *mem_dev, unsigned int num_byte);
mem_err_t mem_pool_free(mem_dev_t *mem_dev, memp_t *mp);

void mem_pool_print_tbl(mem_dev_t *mem_dev);

#endif

