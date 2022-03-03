#include <iostream>
#include <fstream>
#include <cstdio> /*printf*/
#include <cstdlib> /*system, NULL*/
#include <pwd.h> /*password*/
#include <unistd.h> /*setsid*/
#include <sys/utsname.h> /*host info*/
#include <sys/sysinfo.h> /*system info -- critical*/
#include <cstring> /*standard string*/
#include <cpuid.h>  /*GCC-provided*/

using namespace std ;

char* get_distro();

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
    std::cout << "      "   "\033[1;33m•\033[0m \033[1m\033[37m host\033[0m        "  << host << std::endl;

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

    char* distro = get_distro();
    printf("distro: %s\n", distro);
    free(distro);

    std::cout << "\n" << std::endl;

    return 0;

}
