#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <fstream>
#include <string>
#include <vector>
#include "CResult.h" 
#include "CTree.h"  


class FileState {
public:
    static bool is_opened;
};

bool FileState::is_opened = false;

template <typename T>
class Serializer {
public:
    static void saveResult(CResult<T, CError>& result) {
        std::ofstream outFile("zapis.txt", FileState::is_opened ? std::ios::app : std::ios::trunc);
        FileState::is_opened = true;

        for (CError* e : result.vGetErrors())
        {
            outFile << e->sGetError()<<endl;
        }
        outFile.close();
    }
private:
};

// dla CTree
template <>
class Serializer<CTree*> {
public:
    static void saveResult(CResult<CTree*, CError>& result) {

        std::ofstream outFile("zapis.txt", FileState::is_opened ? std::ios::app : std::ios::trunc);
        FileState::is_opened = true;

        if (result.bIsSuccess())
        {
            outFile << result.cGetValue()->inOrder()<<endl;
        }
        for (CError* e : result.vGetErrors())
        {
            outFile << e->sGetError()<<endl;
        }
        outFile.close();
        
    }
};

#endif // SERIALIZER_H
