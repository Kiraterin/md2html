#include "Markdown.hpp"
#include "public.hpp"
#include "Tokenizer.hpp"
#include <fstream>
#include <sstream>

int main() {
    std::ifstream ifs("test1.md");
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string str(buffer.str());
    std::cout << str << std::endl;
    Token::Tokenize(str);
    return 0;
}