/**
  */
  
#include "bsp_common.h"
#include <osal.h>
 
void* Osal_MemCpy(void *dest, const void *src, unsigned int size)
{
    return(memcpy(dest,src,size)); 
}

void* Osal_MemSet(void *ptr, int value, unsigned int size)
{
    return(memset(ptr,value,size));
}

