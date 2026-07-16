#ifndef VIEW_H
#define VIEW_H
#include "operations.h"

/*To call the file*/
Status view_file(char *ptr,tagreader *tag);

/*Open the file*/
Status open_file(char *ptr,tagreader *tag);

/*Skip Header*/
Status skip_header(tagreader *tag);

/*To read the tags*/
Status tag_read(tagreader *tag);

/*Convert Big endian to Little endian*/
unsigned int big_to_little(char *);

#endif