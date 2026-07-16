/*
Name : Jupalli Karthik
Date : 
Project Title : MP3 TAG READER
Description : MP3 Tag Reader and Editor is a command-line application developed in C to read and modify
              the metadata stored in MP3 files using the ID3v2 tag format.The project extracts information such as Title,
              Artist, Album, Year, Genre, and Comment from an MP3 file and displays it in a user-friendly format. It also 
              allows users to edit these metadata fields without affecting the audio content of the file. The application 
              performs proper validation of user inputs and file operations to ensure reliable execution. This project 
              demonstrates concepts such as file handling,command-line argument processing, binary file manipulation, 
              endian conversion, and structured programming in C.
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "operations.h"
#include "view.h"
#include "colors.h"
#include "edit.h"

OperationType check_operation_type(char *symbol);//to check the operation type
void help_command(void);//help function
int validate_year(char *year);//to validate the year when user pass the data at the time of edit

int main(int argc, char *argv[])
{

    tagreader t1;//struct variable for view
    edit_op e1;//struct variable for edit
    if (argc == 3)/*For View*/
    {
        OperationType n = check_operation_type(argv[1]);

        if (n == view)
        {
            if (view_file(argv[2], &t1) == failure)
            {
                printf("Error occurred while viewing the metadata.\n");
                return 1;
            }
        }
        else
        {
            printf("Please enter a valid command to view the metadata.\n");
            printf("Use: mp3tag -h\n");
        }
    }
    /*For help*/
    else if (argc == 2)
    {
        OperationType n = check_operation_type(argv[1]);

        if (n == help)
        {
            help_command();
        }
        else
        {
            printf("Invalid argument.\n");
            printf("Use: mp3tag -h\n");
        }
    }
    /*For edit*/
    else if (argc == 5)
    {
        OperationType n = check_operation_type(argv[1]);

        if (n == edit)
        {
            /* Validating the year if editing Year tag */
            if (strcmp(argv[2], "-y") == 0)
            {
                if (!validate_year(argv[3]))
                {
                    printf(RED "[ERROR] Invalid year entered.\n" RESET);
                    printf(YELLOW "[INFO] Enter a valid 4-digit year between 1000 and 2026.\n" RESET);
                    return 1;
                }
            }

            if (Edit_Data(argv, &e1) == failure)
            {
                printf(RED "[ERROR] Failed to edit MP3 metadata.\n" RESET);
                return 1;
            }
        }
        else
        {
            printf(RED "[ERROR] Invalid edit command.\n" RESET);
            printf(YELLOW "Use: mp3tag -h\n" RESET);
        }
    }
    else
    {
        printf("Invalid number of arguments.\n");
        printf("Use: mp3tag -h\n");
    }

    return 0;
}

OperationType check_operation_type(char *symbol)
{
    if (strcmp(symbol, "-v") == 0)
    {
        return view;
    }
    else if (strcmp(symbol, "-e") == 0)
    {
        return edit;
    }
    else if (strcmp(symbol, "-h") == 0)
    {
        return help;
    }

    return unsupported;
}
void help_command(void)
{
    printf(BOLD_CYAN);
    printf("-------------------------------------------------\n");
    printf("               MP3 TAG READER HELP\n");
    printf("-------------------------------------------------\n");
    printf(RESET);

    printf(BOLD_YELLOW "\nUsage:\n" RESET);
    printf("  mp3tag -v <file.mp3>\n");
    printf("  mp3tag -e -[t/a/A/y/c/g] \"value\" <file.mp3>\n\n");

    printf(BOLD_GREEN "Options:\n" RESET);

    printf(BOLD_BLUE "  -t" RESET "    Modify the Title tag\n");
    printf(BOLD_BLUE "  -a" RESET "    Modify the Artist tag\n");
    printf(BOLD_BLUE "  -A" RESET "    Modify the Album tag\n");
    printf(BOLD_BLUE "  -y" RESET "    Modify the Year tag\n");
    printf(BOLD_BLUE "  -g" RESET "    Modify the Genre tag\n");
    printf(BOLD_BLUE "  -c" RESET "    Modify the Comment tag\n");
    printf(BOLD_BLUE "  -h" RESET "    Display help information\n");
    printf(BOLD_BLUE "  -v" RESET "    View MP3 metadata\n");

    printf(BOLD_CYAN);
    printf("\n-------------------------------------------------\n");
    printf(RESET);
}
/*Validations doing at the time of editing the year*/
int validate_year(char *year)
{
    /*The len should be = 4*/
    if (strlen(year) != 4)
    {
        return 0;
    }
    /*If all are not digits*/
    for (int i = 0; year[i] != '\0'; i++)
    {
        if (!isdigit((unsigned char)year[i]))
        {
            return 0;
        }
    }
    /*Converting ascii to integer for checking range*/
    int yr = atoi(year);

    if (yr > 2026 || yr < 1000)
    {
        return 0;
    }

    return 1;
}