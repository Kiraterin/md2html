#include "debug.hpp"

void out_tokens(std::vector<Token> tokens, std::string filename) {
    std::ofstream ofs(filename);
    for (auto c : tokens) {
        ofs << c.ToString() << "\n";
    }
}