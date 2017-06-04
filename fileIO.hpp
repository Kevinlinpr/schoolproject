#ifndef FILEIO
#define FILEIO

#define READ_BINARY "rb"
#define READ_TEXT "r"

/* reads a file. name - file name, read - READ_BINARY = "rb" or READ_TEXT = "r" */
char *readFile( const char *name, const char *read );

#endif
