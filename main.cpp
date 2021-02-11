#include <iostream>
#include <string>
using namespace std;

#include "./src/parser.cpp"

int main(int argc, char const* argv[]) {
    if (argc == 1) {
        printf("Please input a source file.\n");
        return 1;
    }
    if (argc > 2) {
        printf("Invalid inputs being provided.\n");
        return 1;
    }

    // Check filename
    // Send filename to parser
    string filename = string(argv[1]);

    bool success = startParser(filename);

    return 0;
}