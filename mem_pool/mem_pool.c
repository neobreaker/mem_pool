#include "mem_pool.h"
#include <stdlib.h>
#include "ccdebug.h"

unsigned int mem_pool_calc_tbl_size(unsigned int mem_base_size, unsigned int block_size)
{
	CC_ASSERT("[mem_pool] : block_size", mem_base_size%(block_size*8) == 0);
	return mem_base_size/block_size/8;
}


mem_err_t mem_pool_init(mem_dev_t *mem_dev, char* mem_base, unsigned int mem_base_size,
                        char *mem_tbl, unsigned int mem_tbl_size)
{

    if(!mem_base_size || !mem_tbl_size)
        return mem_err_EMPTY;
	
	CC_ASSERT("[mem_pool] : block_size", (mem_base_size%(mem_tbl_size*8) == 0));
	
    mem_dev->mem_base = mem_base;
    mem_dev->mem_base_size = mem_base_size;
    mem_dev->mem_tbl = mem_tbl;
    mem_dev->mem_tbl_size = mem_tbl_size;
    mem_dev->mem_block_size = mem_base_size/(mem_tbl_size*8);

    return mem_err_OK;
}

memp_t mem_pool_malloc(mem_dev_t *mem_dev, unsigned int num_byte)
{
    memp_t mp;
    unsigned int num_block = 0;
    signed long long offset = 0;
    char tbl;
    unsigned int i = 0, j = 0, n = 0;
    mp.p = NULL;
    mp.size = num_byte;

    if(!num_byte)
        return mp;

    num_block = num_byte / mem_dev->mem_block_size;
    if(num_byte % mem_dev->mem_block_size)
        num_block++;

    for(offset = mem_dev->mem_tbl_size-1; offset >= 0; offset--)
    {
        tbl = mem_dev->mem_tbl[offset];

        for(i = 0; i < 8 && n < num_block; ++i)
        {
            if(tbl | 0)
                n = 0;
            else
                n++;
            tbl >>= 1;
        }
        i--;
        if(n == num_block)
        {
            while(j < num_block)
            {
                
                if(i == 0x7 && num_block-j >= 8)
                {
                    mem_dev->mem_tbl[offset] |= 0xff;
                    j += 8;
                }
                else
                {
                    mem_dev->mem_tbl[offset] |= 1<<(i--);
                    j++;
                }

                i = i & 0x7;

                if(i == 0x7)
                {
                    offset++;
                }

            }
			mp.p = mem_dev->mem_base + (offset*8 + (8-i-1))*mem_dev->mem_block_size;
            mp.size = num_block * mem_dev->mem_block_size;
            break;
        }

    }

    return mp;
}

mem_err_t mem_pool_free(mem_dev_t *mem_dev, memp_t *mp)
{
    unsigned int addr_offset = 0;
    unsigned int num_block = 0;
    unsigned int tbl_idx = 0 ;
    unsigned int tbl_offset =0 ;
    unsigned int i = 0;

    if(mp->p == NULL)
    {
        return mem_err_NULL;
    }

    addr_offset = mp->p - mem_dev->mem_base;
    if( mp->p < mem_dev->mem_base || addr_offset > mem_dev->mem_base_size)
    {
        return mem_err_OUT_OF_RANGE;
    }

    tbl_idx = addr_offset / mem_dev->mem_block_size / 8;
    tbl_offset = addr_offset / mem_dev->mem_block_size % 8;

    num_block = mp->size / mem_dev->mem_block_size;

    while(i < num_block)
    {
        if(tbl_offset == 0 && (num_block-i) >= 8)
        {
            mem_dev->mem_tbl[tbl_idx] &= 0x0;
            i += 8;
        }
        else
        {
            mem_dev->mem_tbl[tbl_idx] &= ~(1<<(7-tbl_offset));
            i++;
            tbl_offset++;
        }


        tbl_offset &= 0x7;

        if(tbl_offset == 0)
        {
            tbl_idx++;
        }
    }

    mp->p = NULL;
    mp->size = 0;

    return mem_err_OK;

}

void mem_pool_print_tbl(mem_dev_t *mem_dev)
{
    unsigned int i = 0;

    for(i = 0; i < mem_dev->mem_tbl_size; i++)
    {
//      CC_DEBUGF(CC_DBG_LEVEL_WARNING, ("%"U32_F" ", mem_dev->mem_tbl[i]));
        CCDBG_PLATFORM_DIAG(("%d:0x%x ", i, mem_dev->mem_tbl[i]));
    }
}




