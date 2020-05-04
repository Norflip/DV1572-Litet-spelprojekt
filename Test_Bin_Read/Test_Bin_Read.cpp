#include <iostream>
#include <string>
#include "ImportLibTA.h"

int main()
{
    float testRead;

    testRead = ImportLibTA::Importer::ReadBinFile("testScene.bin");

    std::cout << std::endl << std::to_string(testRead);
    
}

//Guides
/*
https://www.youtube.com/watch?v=r0QR3HH3xDs
https://www.youtube.com/watch?v=a4mS6aKgxys
*/