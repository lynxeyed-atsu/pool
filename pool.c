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
pool_all poal;

void pool_init(void){    
    poal.first_pointer = &pl_buf[0];
    poal.pool_size_all = pool_size;
    
    poal.available = memory_available;
    poal.next_pointer = &pl_buf[(int)((sizeof(pool_all) + (sizeof(int) - 1)) / sizeof(int))];
    memcpy(poal.first_pointer, &poal, sizeof(pool_all)); //todo: change 'pl_buf' pointer

}

int *pool_alloc(int size){
    
    size = (size + (sizeof(int) - 1)) / sizeof(int); //round int type alignment
        
    int *pointr;
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
    if(pointr == NULL)return;
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


typedef struct{
    int reg1;
    int reg2;
    int *reg3;
    int reg4;
    int reg5;
    int reg6;
    int reg7;
}st_reg;

int main(void){
    int i;
    //st_reg *st;
    
    pool_init();
    int *p = (int *)pool_alloc(sizeof(int) * 2);
    p[0] = 0xAAAA;
    p[1] = 0xBBBB;

    // before free
    for(i = 0 ; i < 25 ; i++ ){
        printf("pl_buf[%d]=0x%x\r\n",i,pl_buf[i]);
    }
    
    pool_free(p);
    
    //after free
    for(i = 0 ; i < 25 ; i++ ){
        printf("pl_buf[%d]=0x%x\r\n",i,pl_buf[i]);
    }
    
    

}

