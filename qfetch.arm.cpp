#include <iostream>
#include <cstdio> /*printf*/
#include <cstdlib> /*system, NULL*/
#include <pwd.h> /*password*/
#include <unistd.h> /*setsid*/
#include <sys/utsname.h> /*host info*/
#include <sys/sysinfo.h> /*system info -- critical*/
#include <cstring> /*standard string*/

using namespace std ;

int main ( int argc, char *argv[] )
{
    /*insert help argument*/
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
    std::cout << "\n \033[1m\033[37mwelcome, " << name << "\033[1;31m ♥\033[0m \n\n" << std::endl;

    string host ;
    host = uinfo.nodename ;
    std::cout << "      "   "\033[1;33m•\033[0m \033[1m\033[37m host\033[0m        "  << host << std::endl;

    string shell ;
    shell = basename(pw -> pw_shell) ;
    std::cout << "      "   "\033[1;32m•\033[0m \033[1m\033[37m shell\033[0m       "  << shell << std::endl;

    string kernel ;
    kernel = uinfo.sysname ; 
    string version ;
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
