#ifndef EDIT_H
#define EDIT_H
#include "operations.h"
/*To check The tag like command which type*/
OperationType Tag_Type(char *str);

/*This is the main calling all the functions from here*/
OperationType Edit_Data(char **argv,edit_op *ed);

/*To open the src file and temp file*/
OperationType open_files(char *str,edit_op *ed);

/*To Copy the header 10 bytes , edit the tag data and copying remaining data as it is*/
OperationType tag_edit(char *str,edit_op *ed);

/*To convert the size to little endian to big endian*/
void little_to_big(unsigned int value,char buffer[4]);


#endif