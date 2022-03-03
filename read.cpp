#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

int main() {
    string distro;
    distro = system("grep -m 1 NAME /etc/os-release | tr -d 'NAME=\"' ");
    /* std::cout << "\n" << distro << std::endl; */
    return 0;
}
