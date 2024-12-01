#include <fstream>
#include <type_traits>
#include "CTree.h" // Zak³adam, ¿e masz definicjê klasy CTree
#include "CError.h"

template <typename T, typename E>
class Serializer {
public:
    static void saveToFile(const CResult<T, E>& result, const std::string& filename) {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            throw std::runtime_error("Cannot open file for writing: " + filename);
        }

        if constexpr (std::is_same<T, CTree*>::value) {
            // Specjalne zachowanie dla CTree*
            saveCTreeResult(result, outFile);
        }
        else {
            // Ogólne zachowanie dla innych typów
            saveGeneralResult(result, outFile);
        }

        outFile.close();
    }

private:
    static void saveGeneralResult(const CResult<T, E>& result, std::ofstream& outFile) {
        if (!result.bIsSuccess()) {
            outFile << "Errors occurred:\n";
            for (const auto& error : result.vGetErrors()) {
                outFile << error->getDescription() << "\n";
            }
        }
        else {
            outFile << "Success: No errors to save.\n";
        }
    }

    static void saveCTreeResult(const CResult<CTree*, E>& result, std::ofstream& outFile) {
        if (!result.bIsSuccess()) {
            outFile << "Errors occurred:\n";
            for (const auto& error : result.vGetErrors()) {
                outFile << error->getDescription() << "\n";
            }
        }
        else if (result.cGetValue() != nullptr) {
            outFile << "Tree (prefix notation):\n";
            saveTreePrefix(result.cGetValue(), outFile);
        }
        else {
            outFile << "No tree or errors to save.\n";
        }
    }

    static void saveTreePrefix(CTree* tree, std::ofstream& outFile) {
        if (tree == nullptr) return;
        outFile << tree->inOrder(); 
    }
};
