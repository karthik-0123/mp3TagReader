#include "edit.h"
#include <string.h>
#include "colors.h"
#include "view.h"
OperationType Tag_Type(char *str)//Checking the tag type whether user entered correct command to edit tag or not
{
    const char *options[] = {"-t", "-a", "-A", "-y", "-g", "-c"};
    for (int i = 0; i < 6; i++)
    {
        if (strcmp(str, options[i]) == 0)//if tag command is present
        {
            return success;
        }
    }
    return failure;
}

OperationType open_files(char *src_file, edit_op *ed)//To open the files
{
    ed->src_fptr = fopen(src_file, "r");//opening the  file in read mode
    if (ed->src_fptr == NULL)
    {
        printf(RED "[ERROR] Cannot open file : %s\n" RESET, src_file);
        return failure;
    }
    ed->dest_ptr = fopen("temp.mp3", "w");//creating our own temporary file
    return success;
}
/* To convert the data to little endian to big endian because the size of the editing data is in little endian but in the file we have
to store in big endian format*/
void little_to_big(unsigned int value, char buffer[4])
{
    buffer[0] = (value >> 24) & 0xFF;
    buffer[1] = (value >> 16) & 0xFF;
    buffer[2] = (value >> 8) & 0xFF;
    buffer[3] = value & 0xFF;
}
/*To edit the Tag Data*/
OperationType tag_edit(char *str, edit_op *ed)
{
    char buffer[10];//Creating a character buffer of 10 bytes
    fread(buffer, 10, 1, ed->src_fptr);//reading 10 bytes from src file
    fwrite(buffer, 10, 1, ed->dest_ptr);//writing 10 bytes to dest file

    char *tags[] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};//Creating Tags array 
    const char *options[] = {"-t", "-a", "-A", "-y", "-g", "-c"};//Creating Tag Commands Array The Order should be same for tags and options array

    for (int i = 0; i < 6; i++)//As we are dealing with 6 Tags So,loop is running for the 6 times
    {
        /* To Read Tag ID */
        char buffer[5];
        fread(buffer, 4, 1, ed->src_fptr);
        buffer[4] = '\0';

        /* To Read Frame Size */
        char sizbuf[4];
        fread(sizbuf, 4, 1, ed->src_fptr);

        /*Converting Data Big endian to Little endian to copy That much no of bytes to dest file*/
        int srcdatasize = big_to_little(sizbuf);

        /* To Read Flags */
        char flag[2];
        fread(flag, 2, 1, ed->src_fptr);

        if (strcmp(str, options[i]) == 0 && strcmp(buffer, tags[i]) == 0)//Checking If Tag is matched with the command enetered
        {
            /* To Write Tag ID */
            fwrite(buffer, 4, 1, ed->dest_ptr);

            /* To Write New Size */
            unsigned int len = strlen(ed->new_data) + 1;//Finding the lenght of new data which is user entered
            /*Creating a Buffer*/
            char newsize[4];
            /*Converting size from little endian to big endian to store in the file as the size follows big endian format*/
            little_to_big(len, newsize);
            /*To Write Converted Big endian size to dest file*/
            fwrite(newsize, 4, 1, ed->dest_ptr);
            /*Copying the flag data to dest file*/
            fwrite(flag, 2, 1, ed->dest_ptr);

            /* Skip old frame data */
            fseek(ed->src_fptr, srcdatasize, SEEK_CUR);

            /* Encoding byte -> if we dont do this ,the data entered by user in that first character wont copy to the file then the 1st character becomes encoding byte*/
            char encoding = 0x00;
            fwrite(&encoding, 1, 1, ed->dest_ptr);

            /* Writing the Actual text */
            fwrite(ed->new_data, strlen(ed->new_data), 1, ed->dest_ptr);
        }
        else//if tag or command is not mathces directly copying the same data to the dest file 
        {
            /* Copy Tag ID */
            fwrite(buffer, 4, 1, ed->dest_ptr);

            /* Copy Size */
            fwrite(sizbuf, 4, 1, ed->dest_ptr);

            /* Copy Flags */
            fwrite(flag, 2, 1, ed->dest_ptr);

            /* Copy Data */
            char datbuf[srcdatasize];
            fread(datbuf, srcdatasize, 1, ed->src_fptr);
            fwrite(datbuf, srcdatasize, 1, ed->dest_ptr);
        }
    }

    /* Copy the remaining bytes of data to look as it is like src file */
    char ch;
    while (fread(&ch, 1, 1, ed->src_fptr) == 1)
    {
        fwrite(&ch, 1, 1, ed->dest_ptr);
    }

    return success;
}

OperationType Edit_Data(char **argv, edit_op *ed)
{
    /*Copying the user entered data to the structure*/
    strcpy(ed->new_data, argv[3]);
    /*Checking the user entered argument is matching or not*/
    if (Tag_Type(argv[2]) == failure)
    {
        printf(RED "[ERROR] Invalid editing option.\n" RESET);
        printf(YELLOW "Usage : ./a.out -e <-t|-a|-A|-y|-g|-c> \"new_data\" sample.mp3\n" RESET);
        return failure;
    }
    printf(CYAN "------------------------------------------------\n" RESET);
    printf(BLUE "                 MP3 TAG EDITOR\n" RESET);
    printf(CYAN "------------------------------------------------\n\n" RESET);
    /*Opening The files src and temporary*/
    if (open_files(argv[4], ed) == failure)
    {
        printf(RED "[ERROR] Unable to open MP3 file.\n" RESET);
        return failure;
    }
    else
    {
        printf(YELLOW "Opening source file...............\n" RESET);
        printf(GREEN "[SUCCESS] file opened successfully.\n" RESET);
    }
    /*Editing the tag data*/
    if (tag_edit(argv[2], ed) == failure)
    {
        printf(RED "[ERROR] Failed to edit MP3 metadata.\n" RESET);
        return failure;
    }
    else
    {
        printf(GREEN "[SUCCESS] MP3 metadata updated successfully.\n" RESET);
    }
    /*Storing the file name in structure*/
    ed->file_name = argv[4];
    /*Closing the openened src and temporary file*/
    fclose(ed->src_fptr);
    fclose(ed->dest_ptr);
    /*Removing the src file*/
    remove(ed->file_name);
    /*Renaming the temporary file as src file name to meet user requirement like we have modified the data and returned same file*/
    rename("temp.mp3", ed->file_name);
    printf(CYAN "\n------------------------------------------------\n" RESET);
    printf(CYAN "        THANK YOU FOR USING MP3 TAG EDITOR\n" RESET);
    printf(CYAN "------------------------------------------------\n" RESET);
}