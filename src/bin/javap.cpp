//
// Created by lzx on 5/22/20.
//
#include <LiVM/classfile/classfileParser.h>
#include <iostream>
using namespace LiVM;
using namespace  std;
int main(int argc, const char **argv) {
    if (argc != 2) {
        printf("Usage: javap <class file>\n");
        return 1;
    }
    cout << argv[1] << endl;
    ClassfileParser parser(argv[1]);
    std::unique_ptr<ClassFile> file = parser.parse();
    if (file != nullptr) {
        file->printConstantPool();
    }
    return 0;
}
