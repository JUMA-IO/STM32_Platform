/*
 *
 *  JUMA.IO - JUMA SDK for STM families
 *
 *  Copyright (C) 2013-2016  JUMA Technology
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the Apache V2 License as published by
 *  the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 */
#ifndef _DISPATCH_H_
#define _DISPATCH_H_

typedef struct _operation_t operation_t;
typedef struct _dispatch_queue_t dispatch_queue_t;

struct _operation_t {
    operation_t* next;
    function_t func;
    void* args;
    uint32_t timestamp;
};

struct _dispatch_queue_t {
    operation_t *head, *tail;
};

void dispatch_init( void );
void dispatch( void );

#endif // _DISPATCH_H_



