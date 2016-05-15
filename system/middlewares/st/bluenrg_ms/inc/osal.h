#ifndef __OSAL_H__
#define __OSAL_H__

extern void* Osal_MemCpy(void *dest,const void *src, unsigned int size);
extern void* Osal_MemSet(void *ptr, int value, unsigned int size);
uint32_t Osal_Get_Cur_Time(void);


#endif /* __OSAL_H__ */
