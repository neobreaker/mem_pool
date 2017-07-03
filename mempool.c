#include "mempool.h"
#include "ccdebug.h"

mem_err_t mem_pool_init(mem_dev_t* mem_dev, char* mem_base, u32 mem_base_size, u32 block_size)
{
    u32 tbl_block_num = 0;
    u32 valid_block_num = 0;

    CC_ASSERT("[MEM_POOL]:  mem_base_size % block_size == 0 ", mem_base_size % block_size == 0);

    mem_dev->mem_base       = (u8*)mem_base;
    mem_dev->mem_base_size  = mem_base_size;
    mem_dev->mem_block_size = block_size;
    mem_dev->mem_block_num  = mem_base_size / block_size;
    mem_dev->mem_tbl_size   = mem_dev->mem_block_num;

    tbl_block_num = mem_dev->mem_tbl_size / block_size;
    if(mem_dev->mem_tbl_size % block_size)
        tbl_block_num++;

    valid_block_num = mem_dev->mem_block_num - tbl_block_num;

    mem_dev->mem_tbl         = (u8*)(mem_base + valid_block_num*block_size);
    mem_dev->mem_valid_size = valid_block_num * block_size;

    mem_pool_memset((char*)mem_dev->mem_base, 0x00, mem_dev->mem_base_size);
    mem_pool_memset((char*)mem_dev->mem_tbl+mem_dev->mem_tbl_size-tbl_block_num,  0xff, tbl_block_num);
    return mem_err_OK; 
}

void * mem_pool_alloc(mem_dev_t* mem_dev, u32 num_byte)
{
    u32 i = 0, n = 0;
    u32 num_block = 0;
    signed long long offset = 0;

    if(num_byte == 0 || num_byte > mem_dev->mem_valid_size)
        return NULL;
	
    num_block = num_byte/mem_dev->mem_block_size;
    if(num_byte%mem_dev->mem_block_size)
        num_block++;
    for(offset = mem_dev->mem_tbl_size-1; offset >=0; offset--)
    {
        if(!mem_dev->mem_tbl[offset])
            n++;
        else
            n = 0;
        if(n == num_block)
        {
            for(i = 0; i < num_block; i++)
            {
                mem_dev->mem_tbl[offset+i] = num_block;
            }
			mem_dev->mem_valid_size -= num_block*mem_dev->mem_block_size;
            return mem_dev->mem_base + offset*mem_dev->mem_block_size;
        }
    }
    return NULL;
}

void * mem_pool_malloc(mem_dev_t* mem_dev, u32 num_byte)
{
    return mem_pool_alloc(mem_dev, num_byte);
}

void * mem_pool_calloc(mem_dev_t* mem_dev, u32 n, u32 size)
{
    return mem_pool_alloc(mem_dev, n*size);
}

void mem_pool_memset(char *p, char c, u32 num_byte)
{
    u32 i = 0;
    for(i = 0; i < num_byte; ++i)
    {
        *(p+i) = c;
    }
}

void mem_pool_free(mem_dev_t *mem_dev, char *p)
{
    u32 offset = 0;
    u32 i = 0;
    u32 tbl_offset = 0;
    u32 num_block = 0;
	
    if(p == NULL)
        return;

    offset =  (u32)p - (u32)(mem_dev->mem_base);
    if(offset < mem_dev->mem_valid_size)
    {
        tbl_offset = offset/mem_dev->mem_block_size;
        num_block = mem_dev->mem_tbl[tbl_offset];
        for(i =0; i < num_block; ++i)
        {
            mem_dev->mem_tbl[tbl_offset+i] = 0;
        }
		mem_dev->mem_valid_size -= num_block*mem_dev->mem_block_size;
    }

}

void mem_pool_print_tbl(mem_dev_t *mem_dev)
{
    u32 i = 0;

    for(i = 0; i < mem_dev->mem_tbl_size; i++)
    {
//      CC_DEBUGF(CC_DBG_LEVEL_WARNING, ("%"U32_F" ", mem_dev->mem_tbl[i]));
        CCDBG_PLATFORM_DIAG(("%d:0x%x ", i, mem_dev->mem_tbl[i]));
    }
}

float mem_pool_utilization(mem_dev_t *mem_dev)
{
	return (1.0)*(mem_dev->mem_valid_size)/(mem_dev->mem_base_size);
}



