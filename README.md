# MP3 Tag Reader & Editor

A command-line application developed in C to read and modify metadata stored in MP3 files using the ID3v2 tag format.

## Overview

**MP3 Tag Reader and Editor** is a powerful tool that allows users to view and edit metadata information in MP3 files without affecting the audio content. The application extracts and modifies information such as Title, Artist, Album, Year, Genre, and Comment from MP3 files, displaying it in a user-friendly format with color-coded output.

## Features

- **View Metadata**: Extract and display ID3v2 tag information from MP3 files
  - Title
  - Artist
  - Album
  - Year
  - Genre
  - Comment

- **Edit Metadata**: Modify MP3 file tags with proper validation
  - Edit individual tag fields
  - Year validation (accepts 4-digit years between 1000-2026)
  - Preserves original audio content during editing

- **Robust File Handling**: 
  - Binary file operations with proper endian conversion
  - Input validation for user data
  - Error handling and informative error messages

- **Command-Line Interface**: Simple and intuitive CLI with help documentation

## Project Structure

```
.
├── main.c              # Main program entry point and command processing
├── view.c/h            # Functions to read and display MP3 tag metadata
├── edit.c/h            # Functions to edit MP3 tag metadata
├── operations.h        # Enum definitions and data structures
├── common.h            # Common header definitions
├── colors.h            # Color codes for terminal output
└── README.md           # This file
```

## Usage

### Viewing MP3 Tags

```bash
./mp3tag -v <filename.mp3>
```

Displays all metadata information from the specified MP3 file in a formatted output with color-coding.

### Editing MP3 Tags

```bash
./mp3tag -e -<tag> <value> <filename.mp3>
```

Supported tags:
- `-t` : Edit Title
- `-a` : Edit Artist
- `-A` : Edit Album
- `-y` : Edit Year (must be a 4-digit number between 1000-2026)
- `-g` : Edit Genre
- `-c` : Edit Comment

Example:
```bash
./mp3tag -e -t "My Song" song.mp3
./mp3tag -e -y 2024 song.mp3
./mp3tag -e -a "Artist Name" song.mp3
```

### Getting Help

```bash
./mp3tag -h
```

Displays usage instructions and available commands.

## Technical Details

### Key Concepts Implemented

- **File Handling**: Reading and writing binary MP3 files
- **ID3v2 Tags**: Understanding and manipulating MP3 metadata structure
- **Endian Conversion**: Converting between little-endian and big-endian formats
- **Binary File Manipulation**: Precise byte-level operations on audio files
- **Command-Line Argument Processing**: Parsing and validating user input
- **Structured Programming**: Modular design with clear separation of concerns
- **Data Structures**: Use of structs for organizing file and edit operations

### ID3v2 Tag Format

The application works with the ID3v2 tag format, which stores metadata at the beginning of MP3 files. The reader:
1. Identifies ID3v2 headers
2. Extracts tag information with proper size decoding
3. Maintains file integrity during edits

## Data Structures

### tagreader
```c
typedef struct {
    FILE *fptr;
} tagreader;
```
Used for viewing operations.

### edit_op
```c
typedef struct {
    FILE *src_fptr;
    FILE *dest_ptr;
    char tag[5];
    unsigned int size;
    char *file_name;
    char new_data[100];
} edit_op;
```
Used for editing operations.

## Compilation

To compile the project:

```bash
gcc -o mp3tag main.c view.c edit.c -Wall -Wextra
```

## Requirements

- GCC compiler (or compatible C compiler)
- Standard C library
- Linux/Unix/Windows with terminal support

## Author

**Jupalli Karthik**

## License

This project is provided as-is for educational and personal use.

## Notes

- Always backup your MP3 files before editing
- Ensure MP3 files are not corrupted before attempting to read/edit
- The application validates year input to ensure data integrity
- Original audio content is never modified during metadata editing
