#include <iostream>
#include <cstring>
#include "scanner.h" 
#include "avrecord.h" 



int main(int argc, char* argv[]) {
    SetConsoleOutputCP(1251);
    if (argc < 2) {
        std::cout << "Enter: cd <path to dir with project.exe>" << std::endl;
        std::cout << "Usage: project.exe <command> [<args>]" << std::endl;
        std::cout << "\nCommands:" << std::endl;
        std::cout << "\n  scan <path_to_scan>                                      - Scan the directory for viruses" << std::endl;
        std::cout << "\n  addrecord <src_file> <dst_file> <offset> <length> <name> - Add a record to the AV base" << std::endl;
        return 0;
    }

    std::string command = argv[1];

    if (command == "scan") {
        if (argc != 3) {
            std::cout << "Usage: main.exe scan <path_to_scan>" << std::endl;
            return 0;
        }
        return scanMain(argc - 1, argv + 1); // scan
    }
    else if (command == "addrecord") {
        if (argc != 7) {
            std::cout << "Usage: main.exe addrecord <src_file> <dst_file> <offset> <length> <name>" << std::endl;
            return 0;
        }
        return addRecordMain(argc - 1, argv + 1); // add record
    }
    else {
        std::cout << "Unknown command: " << command << std::endl;
        return 0;
    }
}

