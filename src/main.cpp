#include "Markdown.hpp"
#include "Tokenizer.hpp"
#include "debug.hpp"
#include "public.hpp"

int main() {
    std::ifstream ifs("test1.md");
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    out_tokens(Token::Tokenize(buffer.str()), "tokens.txt");
    return 0;
}