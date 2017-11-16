#include "stm32f10x.h"
#include "Array.h"
#include "ccdebug.h"
#include "stdio.h"
#include "deque.h"
#include "iic_bus.h"
#include "24cxx.h"
#include "lib_mem.h"

//__align(4) char membase[MEM_MAX_SIZE];

IIC_phy iic_port;

void register_iic_port(IIC_phy* port)
{
    port->port_init     = iic_port_init;
    port->sda_in        = iic_sda_in;
    port->sda_out       = iic_sda_out;
    port->sda_high      = iic_sda_high;
    port->sda_low       = iic_sda_low;
    port->scl_high      = iic_scl_high;
    port->scl_low       = iic_scl_low;
    port->read_sda      = iic_read_sda;
    port->delay_us      = iic_delay_us;
}

void bsp_init()
{
    ccdebug_port_init(9600);
}

void eeprom_test()
{
    register_iic_port(&iic_port);
    iic_port.port_init();
    AT24CXX_init(&iic_port);
    printf("eeprom test begin\r");
    while(1)
    {

        if(!AT24CXX_Check())
            printf("eeprom ok\r");
        else
            printf("eeprom error\r");

        iic_port.delay_us(1000000);

    }
}

void mem_test()
{
    u8 *palloc1 = NULL, *palloc2 = NULL, *palloc3 = NULL;
    size_t heap_sz = 0;

    palloc1 = pvPortMalloc(128);
    palloc2 = pvPortMalloc(128);
    palloc3 = pvPortMalloc(128);
    heap_sz = xPortGetFreeHeapSize();
    printf("heap remain size : %d\r", heap_sz);
    vPortFree(palloc1);
    vPortFree(palloc2);
    vPortFree(palloc3);
    heap_sz = xPortGetFreeHeapSize();
    printf("heap remain size : %d\r", heap_sz);
}

void array_test()
{
    Array *ar = NULL;
    array_new(&ar);
    array_destroy(ar);

}

int main(void)
{
    bsp_init();
    //eeprom_test();
    //mem_test();
    array_test();
    while(1)
    {
        ;
    }
}
