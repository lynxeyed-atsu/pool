//
//  pool.h
//  
//
//  Created by atsu on 13/08/20.
//
//

#ifndef _pool_h
#define _pool_h


#define memory_available    1
#define memory_invalid      0
#define pool_size           128
#define magic_value         0xDEADDEAD
//#define USE_MAGIC

typedef struct {
    int *first_pointer;
    int *next_pointer;
    int pool_size_all;
    int available;
}pool_all;

typedef struct {
    int **this_pointer;
    int my_size;
    int available;
#if defined(USE_MAGIC)
    int magic_num;
#endif
}pool_each;

#endif
