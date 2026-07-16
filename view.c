#include <stdio.h>
#include <string.h>
#include "view.h"
#include "operations.h"
#include "colors.h"
/*Opening the file*/
Status open_file(char *ptr, tagreader *tag)
{
    tag->fptr = fopen(ptr, "r"); // opening the file in binary mode
    if (tag->fptr == NULL)
    {
        printf(RED "Error Ocurred in opening the fle : %s\n"RESET,ptr);
        return failure;
    }
    return success;
}
/*To skip the header of the file*/
Status skip_header(tagreader *tag)
{
    fseek(tag->fptr, 10, SEEK_SET); // the header is 10 bytes
    return success;
}

unsigned int big_to_little(char *ptr) // the size in the file is big endian to convert it into little endian using this function
{
    return ((unsigned char)ptr[0] << 24) | ((unsigned char)ptr[1] << 16) |
           ((unsigned char)ptr[2] << 8) | ((unsigned char)ptr[3]);
}

Status tag_read(tagreader *tag)
{
    char *tags[] = {"TPE1", "TIT2", "TALB", "TYER", "TCON", "COMM"};                   // Tags which we are checking
    char *tagex[] = {"Artist Name", "Song Name", "Album", "Year", "Genre", "Comment"}; // This is used to print the message
    printf(BOLD_CYAN);
    printf("--------------------------------------------------\n");
    printf("           MP3 FILE METADATA\n");
    printf("--------------------------------------------------\n\n");
    printf(RESET);
    for (int i = 0; i < 6; i++) // We are Dealing with 6 tags
    {
        char buffer[5];                 // Creating a buffer with size of 5->4 bytes to read the data tag type from file
        fread(buffer, 4, 1, tag->fptr); // Fetching 4 bytes of data from file
        buffer[4] = '\0';               // Adding null character at last because we are comparing with the tags array
        int ind = -1;
        for (int j = 0; j < 6; j++)
        {
            if (strcmp(tags[j], buffer) == 0)
            {
                ind = j;
                break;
            }
        }
        if (ind == -1)
        {
            return failure;
        }
        char sizbuf[4];//Creating Buffer for size
        fread(sizbuf, 4, 1, tag->fptr);//reading 4 bytes tag from file
        int size = big_to_little(sizbuf);//converting size from big endian to little endian beacuse to get to know how many bytes of data we have to fetch
        fseek(tag->fptr, 2, SEEK_CUR);//Skipping next bytes data because it is flag and we are not using
        if (size <= 0)
        {
            return failure;
        }
        char databuf[size + 1];//creating a buffer with size to fetch the data
        fread(databuf, size, 1, tag->fptr);//fetching size bytes from the file
        databuf[size] = '\0';//adding null character at last to print the data
        printf(BOLD_GREEN "%-15s" RESET " : " BOLD_YELLOW "%s\n" RESET, tagex[ind], databuf + 1);//printing the data
        /*+1 is doing because the first byte is encoding byte we have to skip that*/
    }
}
Status view_file(char *ptr, tagreader *tag)
{
    /*Checking whether .mp3 file is there are not*/
    char *ptr1 = strstr(ptr, ".mp3");
    if (ptr1 == NULL)
    {
        printf(BOLD_RED "[ERROR] " RESET "Please provide an MP3 file.\n");
        return failure;
    }
    /*Opening the file*/
    if (open_file(ptr, tag) != failure)
    {
        printf(BOLD_GREEN "[SUCCESS] " RESET "File opened successfully.\n");
        printf(BOLD_BLUE "            File Name : " RESET "%s\n", ptr);
    }

    /*To skip 10 bytes of header*/
    if (skip_header(tag) == failure)
    {
        printf(BOLD_RED "[ERROR] " RESET "Failed to skip the ID3 header.\n");
        return failure;
    }
    /*To read the tags and data and print same*/
    if (tag_read(tag) == failure)
    {
        printf(BOLD_RED "[ERROR] " RESET "Failed to read MP3 tags.\n");
        return failure;
    }

    fclose(tag->fptr);
    printf(CYAN "\n------------------------------------------------\n" RESET);
    printf(CYAN "        THANK YOU FOR USING MP3 TAG EDITOR\n" RESET);
    printf(CYAN "------------------------------------------------\n" RESET);
    return success;
}