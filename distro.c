#include <stdio.h> /* FILE, fprintf, fopen, fread, fclose, printf */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* strtok, strncmp, strcpy, strlen */
#include <sys/stat.h> /* struct stat, stat */

/*
 * @param distro, a pointer to a char array.
 * @return 0 if successful.
 * Unsafe, if the passed char array is smaller than the distro name.
 */
int get_distro1(char* distro)
{
    struct stat s; /* /etc/os-release file info, contains its size */
    FILE* fp; /* File Pointer */
    char* buff; /* temporary buffer */
    char* token;

    /* get /etc/os-release info, check man page for stat (man 2 stat),
     * -1 error return value */
    if (stat("/tmp/fetch/none"/*"/etc/os-release"*/, &s) == -1) {
        fprintf(stderr, "[Error] Failed to stat /etc/os-release!\n");
        return -1;
    }
    /* allocate the temporary buffer on the heap,
     * the size of the file + 1(for the null terminator) */
    buff = malloc(s.st_size + 1);
    buff[s.st_size] = '\0'; /* null terminate the buffer */

    /* open /etc/os-release in read only mode */
    fp = fopen("/tmp/fetch/none"/*"/etc/os-release"*/, "r");
    if (fp == NULL) {
        fprintf(stderr, "[Error] Failed to open /etc/os-release!\n");
        return -1;
    }

    /* read the contents of the file into the temporary buffer */
    fread(buff, sizeof(char), s.st_size, fp);
    fclose(fp);

    /* (man strtok), cuts the string into tokens (each line) */
    token = strtok(buff, "\n");
    while (token != NULL) {
        /* compare the first 5 chars of each line to NAME= */
        if (strncmp(token, "NAME=", 5) == 0) {
            token += 5; /* skip NAME= (5 chars) */
            strcpy(distro, token); /* copy the distro name into the distro arg */
            distro[strlen(token)] = '\0'; /* null terminate it */
            free(buff); /* free the temporary buffer */
            return 0; /* return successfully */
        }
        token = strtok(NULL, "\n"); /* get the next token in the buffer */
    }

    free(buff);
    fprintf(stderr, "[Error] Failed to find NAME in /etc/os-release!\n");
    return -1;
}

/*
 * same as get_distro1, but takes the array length
 * and checks before copying to distro.
 */
int get_distro2(char* distro, int array_length)
{
    struct stat s;
    FILE* fp;
    char* buff;
    char* token;
    int distro_size;

    if (stat("/etc/os-release", &s) == -1) {
        fprintf(stderr, "[Error] Failed to stat /etc/os-release!\n");
        return -1;
    }

    buff = malloc(s.st_size + 1);
    buff[s.st_size] = '\0';

    fp = fopen("/etc/os-release", "r");
    if (fp == NULL) {
        fprintf(stderr, "[Error] Failed to open /etc/os-release!\n");
        return -1;
    }

    fread(buff, sizeof(char), s.st_size, fp);
    fclose(fp);

    token = strtok(buff, "\n");
    while (token != NULL) {
        if (strncmp(token, "NAME=", 5) == 0) {
            token += 5;
            distro_size = strlen(token);
            /* added this check */
            if (distro_size >= array_length) {
                fprintf(stderr, "[Error] Distro name size is bigger than the passed array length!\n");
                return -1;
            }
            strcpy(distro, token);
            distro[distro_size] = '\0';
            free(buff);
            return 0;
        }
        token = strtok(NULL, "\n");
    }

    free(buff);
    fprintf(stderr, "[Error] Failed to find NAME in /etc/os-release!\n");
    return -1;
}

/*
 * @return NULL in a case of an error,
 * else return a pointer to the distro name.
 * The returned pointer should be freed. (was allocated on the heap)
 */
char* get_distro3()
{
    struct stat s;
    FILE* fp;
    char* buff;
    char* token;
    char* distro; /* distro name */
    int distro_size; /* distro name size/length */

    if (stat("/tmp/fetch/none"/*"/etc/os-release"*/, &s) == -1) {
        fprintf(stderr, "[Error] Failed to stat /etc/os-release!\n");
        return NULL;
    }
    buff = malloc(s.st_size + 1);
    buff[s.st_size] = '\0'; /* null terminate the buffer */

    fp = fopen("/tmp/fetch/none"/*"/etc/os-release"*/, "r");
    if (fp == NULL) {
        fprintf(stderr, "[Error] Failed to open /etc/os-release!\n");
        return NULL;
    }

    fread(buff, sizeof(char), s.st_size, fp);
    fclose(fp);

    token = strtok(buff, "\n");
    while (token != NULL) {
        if (strncmp(token, "NAME=", 5) == 0) {
            token += 5;
            distro_size = strlen(token);
            distro = malloc(distro_size + 1);
            strcpy(distro, token);
            distro[distro_size] = '\0';
            free(buff);
            return distro;
        }
        token = strtok(NULL, "\n");
    }

    free(buff);
    fprintf(stderr, "[Error] Failed to find NAME in /etc/os-release!\n");
    return NULL;
}

char* get_distro()
{
    return get_distro3();
}

/* example of using these functions

#define ARR_LEN 255

int main()
{
    char distro1[ARR_LEN];
    char distro2[ARR_LEN];
    char* distro3;
    
    if (get_distro1(distro1)) {
        return 1;
    }
    printf("distro1 name: %s\n", distro1);
    
    if (get_distro2(distro2, ARR_LEN)) {
        return 1;
    }
    printf("distro2 name: %s\n", distro2);
    
    if ((distro3 = get_distro3()) == NULL) {
        return 1;
    }
    printf("distro3 name: %s\n", distro3);

    free(distro3);
    return 0;
}
*/

/* NOTE
 * 
 * ~ You can skip some error checks
 * ~ Choose the best for you
 * ~ There are tons of other ways to parse the NAME
 *
 * Hope this helped!
 * */
