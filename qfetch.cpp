#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <sys/sysinfo.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <pwd.h>
#include <libgen.h>

// functions //
// hostname
std::string getHostname() {
    char name[256];
    if (gethostname(name, sizeof(name)) == 0)
        return std::string(name);
    return "unknown";
}

// distro
std::string getOSName() {
    std::ifstream file("/etc/os-release");
    std::string line;
    while (std::getline(file, line)) {
        if (line.rfind("PRETTY_NAME=", 0) == 0) {
            // remove surrounding quotes if present
            std::string value = line.substr(13, line.size() - 14);
            return value;
        }
    }
    return "unknown linux";
}

// shell and user
std::tuple<std::string, std::string> getUserShell() {
    struct passwd* pw = getpwuid(getuid());
    std::string user = "unknown";
    std::string shell = "unknown";

    if (pw) {
        if (pw->pw_name) user = pw->pw_name;
        if (pw->pw_shell) shell = basename (pw->pw_shell);
    }

    return {user, shell};
}

// cpu info
std::string getCPUInfo() {
    std::ifstream file("/proc/cpuinfo");
    std::string line, model;
    int cores = 0;

    while (std::getline(file, line)) {
        if (line.rfind("model name", 0) == 0) {
            if (model.empty()) {
                //extract after ": "
                size_t pos = line.find(':');
                if (pos != std::string::npos)
                    model = line.substr(pos + 2);
            }
            ++cores;
        }
    }

    if (model.empty()) return "unknown CPU";
    std::ostringstream out;
    out << model << " (" << cores << " cores)";
    return out.str();
}

// kernel
std::string getKernelVersion() {
    struct utsname buf;
    if (uname(&buf) == 0) {
        std::ostringstream out;
        out << buf.sysname << " " << buf.release;
        return out.str();
    }
    return "unknown kernel";
}

// uptime
std::string getUptime() {
    std::ifstream file("/proc/uptime");
    double seconds;
    if (!(file >> seconds)) return "unknown";
    int days = seconds / 86400;
    int hours = ((int)seconds % 86400) / 3600;
    int minutes = ((int)seconds % 3600) / 60;

    std::ostringstream out;
    if (days) out << days << "d ";
    if (hours) out << hours << "h ";
    out << minutes << "m";
    return out.str();
}

// process count
std::string getProcs() {
    struct sysinfo buf;
    if (sysinfo(&buf) == 0) {
        std::ostringstream out;
        out << buf.procs;
        return out.str();
    }
    return "unknown procs";
}

// display //
void printStat(const std::string& label, const std::string& value, const std::string& color = "\033[1;36m") {
    std::cout << color << std::left << std::setw(12) << label << "\033[0m" << value << '\n';
}

// main //
int main() {
    std::ios::sync_with_stdio(false);

    auto [user, shell] = getUserShell();
    std::string host = getHostname();
    std::string os = getOSName();
    std::string cpu = getCPUInfo();
    std::string kernel = getKernelVersion();
    std::string uptime = getUptime();
    std::string procs = getProcs();

    std::cout << "\n"
              << "\033[1m\033[37mwelcome, " << user << "\033[1;31m ♥\033[0m \n\n"
              << "      " << "\033[1;37m•\033[0m \033[1m\033[37m host\033[0m        " << host << "\n"
              << "      " << "\033[1;33m•\033[0m \033[1m\033[37m distro\033[0m      " << os << "\n"
              << "      " << "\033[1;32m•\033[0m \033[1m\033[37m shell\033[0m       " << shell << "\n" 
              << "      " << "\033[1;36m•\033[0m \033[1m\033[37m cpu\033[0m         " << cpu << "\n"
              << "      " << "\033[1;34m•\033[0m \033[1m\033[37m kernel\033[0m      " << kernel << "\n"
              << "      " << "\033[1;35m•\033[0m \033[1m\033[37m uptime\033[0m      " << uptime << "\n"
              << "      " << "\033[1;31m•\033[0m \033[1m\033[37m procs\033[0m       " << procs << "\n" << std::endl;

    return 0;
}

