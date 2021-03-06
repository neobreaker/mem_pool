#include "stm32f10x.h"
#include "ccdebug.h"
#include "stdio.h"
#include "iic_bus.h"
#include "24cxx.h"
#include "lib_mem.h"
#include "array.h"
#include "deque.h"
#include "queue.h"
#include "stack.h"
#include "slist.h"
#include "list.h"
#include "hashtable.h"
#include "hashset.h"
#include "treetable.h"
#include "treeset.h"

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
    size_t i = 0;
    int *pv = NULL;
    int val[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Array *ar = NULL;

    array_new(&ar);

    for(i = 0 ; i < sizeof(val)/ sizeof(int); i++)
    {
        array_add(ar, &val[i]);
    }

    for(i = 0 ; i < sizeof(val)/ sizeof(int); i++)
    {
        array_get_at(ar, i, (void**)&pv);
        printf("index %d is %d\r", i, *pv);
    }

    array_destroy(ar);

}

void deque_test()
{
    size_t i = 0;
    int *pv = NULL;
    int val[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Deque *deque = NULL;
    DequeIter iter;

    deque_new(&deque);


    for(i = 0 ; i < sizeof(val)/ sizeof(int); i++)
    {
        deque_add(deque, &val[i]);
    }

    deque_iter_init(&iter, deque);
    while(deque_iter_next(&iter, (void **)&pv) != CC_ITER_END)
    {
        printf("index %d is %d\r", i, *pv);
    }

    deque_destroy(deque);

}

void queue_test()
{
    size_t i = 0;
    int *pv = NULL;
    int val[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Queue *queue = NULL;
    QueueIter iter;

    queue_new(&queue);

    for(i = 0 ; i < sizeof(val)/ sizeof(int); i++)
    {
        queue_enqueue(queue, &val[i]);
    }

    queue_iter_init(&iter, queue);
    while(queue_iter_next(&iter, (void **)&pv) != CC_ITER_END)
    {
        printf("index %d is %d\r", i, *pv);
    }

    while(queue_size(queue) > 0)
    {
        queue_poll(queue, (void **)&pv);
        printf("poll %d\r", *pv);
    }

    CC_ASSERT("queue poll error", queue_size(queue) == 0);

    queue_destroy(queue);

}

void stack_test()
{
    size_t i = 0;
    int *pv = NULL;
    int val[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Stack *stack = NULL;
    StackIter iter;

    stack_new(&stack);

    for(i = 0 ; i < sizeof(val)/ sizeof(int); i++)
    {
        stack_push(stack, &val[i]);
    }

    stack_iter_init(&iter, stack);
    while(stack_iter_next(&iter, (void **)&pv) != CC_ITER_END)
    {
        printf("index %d is %d\r", i, *pv);
    }

    while(stack_size(stack) > 0)
    {
        stack_pop(stack, (void **)&pv);
        printf("poll %d\r", *pv);
    }

    stack_destroy(stack);
}

void slist_test()
{
    size_t i = 0;
    int *pv = NULL;
    int val[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    SList *slist = NULL;
    SListIter iter;

    slist_new(&slist);

    for(i = 0 ; i < sizeof(val)/ sizeof(int); i++)
    {
        slist_add(slist, &val[i]);
    }

    slist_iter_init(&iter, slist);

    while (slist_iter_next(&iter, (void*) &pv) != CC_ITER_END)
    {
        if (*pv == 3)
        {
            slist_iter_remove(&iter, NULL);
        }
    }

    CC_ASSERT("slist_iter_remove error ", slist_size(slist) == 9);

    slist_destroy(slist);
}

void list_test()
{
    size_t i = 0;
    int *pv = NULL;
    int val[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    List *list = NULL;
    ListIter iter;

    list_new(&list);

    for(i = 0 ; i < sizeof(val)/ sizeof(int); i++)
    {
        list_add(list, &val[i]);
    }

    list_iter_init(&iter, list);
    while(list_iter_next(&iter, (void*) &pv) != CC_ITER_END)
    {
        if (*pv == 3)
        {
            list_iter_remove(&iter, NULL);
        }
    }

    CC_ASSERT("list_iter_remove error ", list_size(list) == 9);

    list_destroy(list);

}

void hashtable_test()
{
    size_t i = 0;
    int *pv = NULL;
    char *key[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
    int val[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    HashTable *hashtable;
    HashTableIter iter;

    hashtable_new(&hashtable);

    for(i = 0; i < sizeof(key)/ sizeof(char*); i++)
    {
        hashtable_add(hashtable, key[i], (void*)&val[i]);
    }

    for(i = 0; i < sizeof(key)/ sizeof(char*); i++)
    {
        hashtable_get(hashtable, key[i], (void **)&pv);
    }

    hashtable_iter_init(&iter, hashtable);


    hashtable_destroy(hashtable);

}

void hashset_test()
{
    size_t i = 0;
    char* pk = NULL;        //dummy
    char* key[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};

    HashSet* hashset;

    hashset_new(&hashset);

    for(i = 0; i < sizeof(key)/ sizeof(char*); i++)
    {
        hashset_add(hashset, (void *) key[i]);
    }

    if(hashset_contains(hashset, (void *)key[5]))
    {
        hashset_remove(hashset, (void *)key[5], (void *)&pk);
    }

    hashset_destroy(hashset);
}

void treetable_test()
{
    size_t i = 0;
    int *pv = NULL;
    char* key[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
    int val[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    TreeTable *treetbl = NULL;

    treetable_new(cc_common_cmp_ref, &treetbl);

    for(i = 0; i < sizeof(key)/ sizeof(char*); i++)
    {
        treetable_add(treetbl, (void *)key[i], (void *)&val[i]);
    }

    for(i = 0; i < sizeof(key)/ sizeof(char*); i++)
    {
        treetable_get(treetbl, (void *)key[i], (void*)&pv);
    }

    treetable_destroy(treetbl);
}

void treeset_test()
{
    size_t i = 0;
    int *pv = NULL;     //dummy
    char* key[] = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"};
    TreeSet *treeset = NULL;

    treeset_new(cc_common_cmp_ref, &treeset);

    for(i = 0; i < sizeof(key)/ sizeof(char*); i++)
    {
        treeset_add(treeset, (void*)key[i]);
    }

    if(treeset_contains(treeset, (void*)key[0]))
    {
        treeset_remove(treeset, (void*)key[0], (void*)&pv);
    }

    treeset_destroy(treeset);
}

int main(void)
{
    bsp_init();
    //eeprom_test();
    //mem_test();
    //array_test();
    //deque_test();
    //queue_test();
    //stack_test();
    //slist_test();
    //list_test();
    //hashtable_test();
    //hashset_test();
    //treetable_test();
    treeset_test();
    while(1)
    {
        ;
    }
}

