#include "Markdown.hpp"
#include "public.hpp"
#include "Tokenizer.hpp"

int main() {
    // Markdown md{"# h1\n## h2\n##atst4\n### h3\n"};
    Token::Tokenize("\n\naaa\n\n\t   \n   \taaa\n* * ** *\t**\n\t *  *   \n* *** a\n");
    return 0;
}