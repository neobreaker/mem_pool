#ifndef __LWMEM_POOL__
#define __LWMEM_POOL__

#include "stm32f10x.h"

typedef enum
{
	mem_err_OK,
	mem_err_EMPTY,
	mem_err_NULL,
	mem_err_OUT_OF_RANGE,
}mem_err_t;


//内存管理控制器
typedef struct 
{
	u8  *mem_base;
	u32  mem_base_size;
	u8  *mem_tbl;
	u32  mem_tbl_size;
	u32  mem_block_size;
	u32  mem_block_num;
	u32  mem_valid_size;
} mem_dev_t;

mem_err_t mem_pool_init(mem_dev_t* mem_dev, char* mem_base, u32 mem_base_size, u32 block_size);
void * mem_pool_alloc(mem_dev_t* mem_dev, u32 num_byte);
void * mem_pool_malloc(mem_dev_t* mem_dev, u32 num_byte);
void * mem_pool_calloc(mem_dev_t* mem_dev, u32 n, u32 size);
void mem_pool_memset(char *p, char c, u32 num_byte);
void mem_pool_free(mem_dev_t *mem_dev, char *p);
void mem_pool_print_tbl(mem_dev_t *mem_dev);
float mem_pool_utilization(mem_dev_t *mem_dev);
#endif

