#include <stdio.h>
#include <unistd.h>
#include <string.h>

void
error()
{
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}