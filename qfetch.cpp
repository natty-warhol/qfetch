#include <iostream>
#include <cstdio> /*printf*/
#include <cstdlib> /*system, NULL*/
#include <pwd.h> /*password*/
#include <unistd.h> /*setsid*/
#include <sys/utsname.h> /*host info*/
#include <sys/sysinfo.h> /*system info -- critical*/
#include <cstring> /*standard string*/
#include <cpuid.h>  /*GCC-provided*/

using namespace std ;

int main ( int argc, char *argv[] )
{
    uint32_t brand[12];

    if (!__get_cpuid_max(0x80000004, NULL)) {
        fprintf(stderr, "Feature not implemented.");
        return 2; }

    if (argc >= 2) {
        if (strcmp("-h", argv[1]) != 0 && strcmp("--help", argv[1]) != 0)
            cout << "error: unrecognized option \n", argv[1];
            cout << "qfetch, based on bitfetch, minimized moreso and rewritten to C++ \n      --by anihilis \n\n";
            return 1;
    }
    
    /* variable definitions */

    struct utsname uinfo;
    struct sysinfo sinfo;
    struct passwd *pw;

    /* gathering information */

    uname(&uinfo);
    sysinfo(&sinfo);
    pw = getpwuid(geteuid());

    #ifndef SI_LOAD_SHIFT
    #define SI_LOAD_SHIFT 16
    #endif

    #define LOADAVG_SHIFT (1.0 / (1 << SI_LOAD_SHIFT))

    /* print all information */

    std::cout << "  " << std::endl;
    string name ;
    name = pw -> pw_name ;
    std::cout << "\n welcome, " << name << "\033[1;31m ♥\033[0m \n\n" << std::endl;

    string host ;
    host = uinfo.nodename ;
    std::cout << "      "   "◽ host        "  << host << std::endl;

    string shell ;
    shell = basename(pw -> pw_shell) ;
    std::cout << "      "   "◽ shell       "  << shell << std::endl;

    __get_cpuid(0x80000002, brand+0x0, brand+0x1, brand+0x2, brand+0x3);
    __get_cpuid(0x80000003, brand+0x4, brand+0x5, brand+0x6, brand+0x7);
    __get_cpuid(0x80000004, brand+0x8, brand+0x9, brand+0xa, brand+0xb);
    printf("      ◽ cpu         " "%s\n", brand);

    string kernel ;
    kernel = uinfo.sysname ; 
    string version ;
    version = uinfo.release ;
    std::cout << "      "   "◽ kernel      "  << kernel << " " << version << std::endl;

    printf(
        "      "   "◽ uptime      "  "%lih %lim\n",
        sinfo.uptime / 3600, (sinfo.uptime /60) - (sinfo.uptime / 3600 * 60),
        sinfo.loads[0] * LOADAVG_SHIFT, sinfo.loads[1] * LOADAVG_SHIFT, sinfo.loads[2] * LOADAVG_SHIFT );

    printf(
        "      "   "◽ procs       "  "%lu\n",
        sinfo.procs );

    std::cout << "\n" << std::endl;

    return 0;

}
