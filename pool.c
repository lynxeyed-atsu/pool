//
//  pool.c
//  
//
//  Created by atsu on 13/08/20.
//
//

#include <stdio.h>
#include "pool.h"


int pl_buf[pool_size];
//int pl_pointer[pool_size / 4];
pool_all poal;


int *pool_memmove(int size){
    int *pointr;
    pool_each poeh;
    size = (size + (sizeof(int) - 1)) / sizeof(int); //round int type alignment
    poal.first_pointer = &pl_buf[0];
    
    //seek pool_each header
    pointr = poal.first_pointer + (int)((sizeof(pool_all) + (sizeof(int) - 1)) / sizeof(int));
    
    //todo: add magic number
    while(1){
        memcpy(&poeh, pointr, sizeof(pool_each));
        if((poeh.my_size == 0)||(pointr >= poal.next_pointer)){return NULL;}
        
        if(poeh.available == memory_invalid){
            if(size <= poeh.my_size){
                // seek data_header
                poeh.available = memory_available;
                memcpy(pointr, &poeh, sizeof(pool_each));
                
                pointr = pointr + (int)((sizeof(pool_each) + (sizeof(int) - 1)) / sizeof(int));
                return pointr;
            }
        }
        pointr = pointr + (int)((sizeof(pool_each) + (sizeof(int) - 1)) / sizeof(int)) + poeh.my_size;
    }
    
    
}
void pool_init(void){
    
    poal.first_pointer = &pl_buf[0];
    poal.this_pointer = &poal.first_pointer;
    
    // *poal.this_pointer = &pl_buf2[0];
    
    poal.pool_size_all = pool_size;
    
    poal.available = memory_available;
    poal.next_pointer = &poal.first_pointer[(int)((sizeof(pool_all) + (sizeof(int) - 1)) / sizeof(int))];
    memcpy(poal.first_pointer, &poal, sizeof(pool_all)); //todo: change 'pl_buf' pointer

}

int *pool_alloc(int size){

    int *pointr;
    
    if(NULL != (pointr = pool_memmove(size)))return pointr;
    size = (size + (sizeof(int) - 1)) / sizeof(int); //round int type alignment
    
    pool_each poeh;
    
    poal.pool_size_all = poal.pool_size_all - ((sizeof(pool_each) + (sizeof(int) - 1)) / sizeof(int)) - size; // int alignment
    
    if(poal.pool_size_all <= 0){
        printf("malloc error!\r\n");
        return NULL;
    }
    
    poeh.my_size = size;
#if defined(USE_MAGIC)
    poeh.magic_num = magic_value;
#endif
    poeh.available  = memory_available;
    pointr = poal.next_pointer + (int)((sizeof(pool_each) + (sizeof(int) - 1)) / sizeof(int));

    poeh.this_pointer = &pointr;
    
    memcpy(poal.next_pointer, &poeh, sizeof(pool_each)); //todo: change 'pl_buf' pointer
    poal.next_pointer = pointr + size;

    return pointr;
}

void pool_free(int *pointr){
    if (pointr == NULL)return;
    pool_each poeh;
    
    pointr = pointr - (int)((sizeof(pool_each) + (sizeof(int) - 1)) / sizeof(int)); // align header
    memcpy(&poeh,pointr,sizeof(pool_each));
#if defined(USE_MAGIC)
    if (poeh.magic_num != magic_value){
        printf("pointer error\r\n");
        return;
    }
#endif
    poeh.available = memory_invalid;
    memcpy(pointr, &poeh, sizeof(pool_each));
    return;
    
}

int *pool_realloc(int *pointr, int size){

    int *new_pointr;
    
    if(pointr == NULL)return NULL;
    pool_each poeh;
    size = (size + (sizeof(int) - 1)) / sizeof(int); //round int type alignment
    
    pointr = pointr - (int)((sizeof(pool_each) + (sizeof(int) - 1)) / sizeof(int)); // align header
    
    memcpy(&poeh,pointr,sizeof(pool_each));
#if defined(USE_MAGIC)
    if (poeh.magic_num != magic_value){
        printf("pointer error\r\n");
        return NULL;
    }
#endif
    if (poeh.my_size >= size){
        
        //poeh.my_size = size;
        memcpy(pointr, &poeh, sizeof(pool_each));
        pointr = pointr + (int)((sizeof(pool_each) + (sizeof(int) - 1)) / sizeof(int)); // align pointer
        return pointr;
        
    }
    poeh.available = memory_invalid;
    memcpy(pointr, &poeh, sizeof(pool_each));

    new_pointr = (int *)pool_alloc(sizeof(int) * size);
    pointr = pointr + (int)((sizeof(pool_each) + (sizeof(int) - 1)) / sizeof(int)); // align pointer

    memcpy(new_pointr, pointr, sizeof(int) * poeh.my_size);
    //pointr = new_pointr;
    return new_pointr;
}


typedef struct{
    int reg1;
    int reg2;
    int *reg3;
}st_reg;


int main(void){
    int i;
    
    pool_init();
    int *p = (int *)pool_alloc(sizeof(int) * 2);
    p[0] = 0xAAAA;
    p[1] = 0xBBBB;
    
    int *q = (int *)pool_alloc(sizeof(int) * 5);
    q[0] = 0xEEEE;
    q[1] = 0xFFFF;
  
    for(i = 0 ; i < 25 ; i++ ){
        printf("pl_buf[%d]=0x%x\r\n",i,pl_buf[i]);
    }
    
    pool_free(q);
    
    
    p = (int *)pool_realloc(p, sizeof(int) * 3);
    p[2] = 0xCCCC;
    
    int *r = (int *)pool_alloc(sizeof(int) * 3);
    r[0] = 0xABAB;
    
    for(i = 0 ; i < 30 ; i++ ){
        printf("pl_buf[%d]=0x%x\r\n",i,pl_buf[i]);
    }

    
    
    pool_free(p);
    
    
}


