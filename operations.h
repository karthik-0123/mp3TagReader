#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <stdio.h>

typedef enum
{
    view,
    edit,
    help,
    unsupported
} OperationType;

typedef enum
{
    success,
    failure
} Status;

typedef struct
{
    FILE *fptr;
} tagreader;

typedef struct
{
    FILE *src_fptr;
    FILE *dest_ptr;
    char tag[5];
    unsigned int size;
    char *file_name;
    char new_data[100];
} edit_op;

#endif