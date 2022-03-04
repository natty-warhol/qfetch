#include <iostream>
#include <fstream>
#include <cstdio> /*printf*/
#include <cstdlib> /*system, NULL*/
#include <pwd.h> /*password*/
#include <unistd.h> /*setsid*/
#include <sys/utsname.h> /*host info*/
#include <sys/sysinfo.h> /*system info -- critical*/
#include <sys/stat.h> /* struct stat, stat */
#include <cstring> /*standard string*/
#include <cpuid.h>  /*GCC-provided*/

using namespace std ;

extern "C" char* get_distro();

int get_distro(char* distro, int array_length) {
    struct stat s;
    FILE* fp;
    char* buff;
    char* token;
    int distro_size;


    if (stat("/etc/os-release", &s) == -1) {
        fprintf(stderr, "[Error] Failed to stat /etc/os-release!\n");
        return -1;
    }

    buff = (char*)malloc(s.st_size + 1);
    buff[s.st_size] = '\0';

    fp = fopen("/usr/lib/os-release", "r");
    if (fp == NULL) {
        fprintf(stderr, "[Error] Failed to open /etc/os-release!\n");
        return -1;
    }

    fread(buff, sizeof(char*), s.st_size, fp);
    fclose(fp);

    token = strtok(buff, "\n");
    while (token != NULL) {
        if (strncmp(token, "NAME=", 5) == 0) {
            token += 6;
            distro_size = (strlen(token) - 1);
            if (distro_size >= array_length) {
                fprintf(stderr, "[Error] Distro name size is bigger than the passed array length!\n");
                return -1;
            }
            strcpy(distro, token);
            distro[distro_size] = '\0';
            free(buff);
            return 0; }
        token = strtok(NULL, "\n"); }

    free(buff);
    fprintf(stderr, "[Error] Failed to find NAME in /etc/os-release!\n");
    return -1; }

#define ARR_LEN 255

int main ( int argc, char *argv[] )
{
    /*grab CPU info*/
    uint32_t brand[12];

    if (!__get_cpuid_max(0x80000004, NULL)) {
        fprintf(stderr, "Feature not implemented.");
        return 2; }

    /*insert help argument*/
    if (argc >= 2) {
        if (strcmp("-h", argv[1]) != 0 && strcmp("--help", argv[1]) != 0)
            std::cout << "error: unrecognized option \n" << argv[1] << std::endl;
            std::cout << "qfetch, based on bitfetch, minimized moreso and rewritten to C++ \n      --by anihilis \n\n" << std::endl;
        return 1;
    }

    /* variable definitions */

    struct utsname uinfo;
    struct sysinfo sinfo;
    struct passwd *pw;

    uname(&uinfo);
    sysinfo(&sinfo);
    pw = getpwuid(geteuid());

    #ifndef SI_LOAD_SHIFT
    #define SI_LOAD_SHIFT 16
    #endif

    #define LOADAVG_SHIFT (1.0 / (1 << SI_LOAD_SHIFT))

    /* print all information */

    std::cout << "  " << std::endl;
    std::string name ;
    name = pw -> pw_name ;
    std::cout << "\n \033[1m\033[37mwelcome, " << name << "\033[1;31m ♥\033[0m \n\n" << std::endl;

    std::string host ;
    host = uinfo.nodename ;
    std::cout << "      "   "\033[1;37m•\033[0m \033[1m\033[37m host\033[0m        "  << host << std::endl;
    
    char distro[ARR_LEN];
    if (get_distro(distro, ARR_LEN)) {
        return 1;
    }
    printf("      \033[1;33m•\033[0m \033[1m\033[37m distro\033[0m      " "%s\n", distro);
    

    std::string shell ;
    shell = basename(pw -> pw_shell) ;
    std::cout << "      "   "\033[1;32m•\033[0m \033[1m\033[37m shell\033[0m       "  << shell << std::endl;

    __get_cpuid(0x80000002, brand+0x0, brand+0x1, brand+0x2, brand+0x3);
    __get_cpuid(0x80000003, brand+0x4, brand+0x5, brand+0x6, brand+0x7);
    __get_cpuid(0x80000004, brand+0x8, brand+0x9, brand+0xa, brand+0xb);
    printf("      \033[1;36m•\033[0m \033[1m\033[37m cpu\033[0m         " "%s\n", brand);

    std::string kernel ;
    kernel = uinfo.sysname ; 
    std::string version ;
    version = uinfo.release ;
    std::cout << "      "   "\033[1;34m•\033[0m \033[1m\033[37m kernel\033[0m      "  << kernel << " " << version << std::endl;

    printf(
        "      "   "\033[1;35m•\033[0m \033[1m\033[37m uptime\033[0m      "  "%lih %lim\n",
        sinfo.uptime / 3600, (sinfo.uptime /60) - (sinfo.uptime / 3600 * 60),
        sinfo.loads[0] * LOADAVG_SHIFT, sinfo.loads[1] * LOADAVG_SHIFT, sinfo.loads[2] * LOADAVG_SHIFT );

    printf(
        "      "   "\033[1;31m•\033[0m \033[1m\033[37m procs\033[0m       "  "%lu\n",
        sinfo.procs );


    std::cout << "\n" << std::endl;

    return 0;

}
