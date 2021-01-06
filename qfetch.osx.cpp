#include <iostream>
#include <chrono>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <sys/utsname.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
using namespace std ;

#define CPU 128

int main(){

    struct utsname uinfo;
    struct passwd *pw;
    struct timeval ts;
    uname(&uinfo);
    pw = getpwuid(geteuid());
    string name;
    name = pw -> pw_name;
    std::cout << "\n welcome, " << name << std::endl;
    
    string host;
    host = uinfo.nodename;
    std::cout << "      " << "host      " << host << std::endl;

    string kernel;
    kernel = uinfo.sysname;
    string version;
    version = uinfo.release;
    std::cout << "      " << "kernel        " << kernel << " " << version << std::endl;

    char cpu[CPU];
    size_t cpulen = CPU;
    sysctlbyname("machdep.cpu.brand_string",&cpu,&cpulen,NULL,0);
    std::cout << "      " << "cpu       " << cpu << std::endl;

    std::cout << "\n" << std::endl;

    return 0;
}
