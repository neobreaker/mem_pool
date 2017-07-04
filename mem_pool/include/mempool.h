#ifndef __LWMEM_POOL__
#define __LWMEM_POOL__

#include "stm32f10x.h"


typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */

typedef enum
{
    mem_err_OK,
    mem_err_EMPTY,
    mem_err_NULL,
    mem_err_OUT_OF_RANGE,
} mem_err_t;

typedef enum
{
	TBL_W8  = 1,
	TBL_W16 = 2,
	TBL_W32 = 4,
}tbl_width_t;

typedef struct
{
    u8  *tbl_w8;
	u16 *tbl_w16;
	u32 *tbl_w32;
} mem_tbl_t;

//内存管理控制器
typedef struct
{
    u8  *mem_base;
    u32  mem_base_bytes;
    //u8  *mem_tbl;
    mem_tbl_t    mem_tbl;
	tbl_width_t  tbl_width;  
    u32  tbl_bytes;
	u32  tbl_block_num;
    u32  block_bytes;
    u32  mem_block_num;
    u32  mem_valid_size;
	u32  mem_free_size;
} mem_dev_t;

mem_err_t mem_pool_init(mem_dev_t* mem_dev, char* mem_base, u32 mem_base_bytes, u32 block_size, tbl_width_t tbl_width);
void * mem_pool_malloc(mem_dev_t* mem_dev, u32 num_byte);
void * mem_pool_calloc(mem_dev_t* mem_dev, u32 n, u32 size);
void mem_pool_memset(char *p, char c, u32 num_byte);
void mem_pool_free(mem_dev_t *mem_dev, char *p);
void mem_pool_print_tbl(mem_dev_t *mem_dev);
float mem_pool_utilization(mem_dev_t *mem_dev);

static void* mem_pool_alloc(mem_dev_t* mem_dev, u32 num_byte);
static void* mem_set_tbl_8(mem_dev_t *mem_dev, u32 num_block);
static void* mem_set_tbl_16(mem_dev_t *mem_dev, u32 num_block);
static void* mem_set_tbl_32(mem_dev_t *mem_dev, u32 num_block);
static void  mem_reset_tbl_8(mem_dev_t *mem_dev, char *p);
static void  mem_reset_tbl_16(mem_dev_t *mem_dev, char *p);
static void  mem_reset_tbl_32(mem_dev_t *mem_dev, char *p);

#endif

