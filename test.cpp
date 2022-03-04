/* #include <string.h>
#include <iostream>
#include <fstream>
#include <cstdio>

#define BUFLEN 64

int main(int argc, char **argv) {
    char input [BUFLEN] = "Rey, Luke, Leia: Anakin, Padme";
    int i=0;

    char *token = strtok(input, ",");

    while(token != NULL) {
        token = strtok(NULL, ",:");
        printf("%d:[%p] --- %s\n",i,token,token); }
    
    return 0;
} */
