#ifndef __TOKENIZER_HPP__
#define __TOKENIZER_HPP__

#include "public.hpp"

enum class TokenType {
    TextPart,
    Title,
    Delete,
    Star,
    HorizontalRule,
    BlankLine,
    UnorderedList,
    OrderedList,
    QuotationBegin,
    QuotationEnd,
    CodeLang,
    CodeBlock,
    Highlight,
    CheckBoxList,
    LinkTextBegin,
    LinkTextEnd,
    Link,
    Image
};

class Token {
  public:
    TokenType token_type;
    std::string content;

    static std::vector<Token> Tokenize(std::string);
    std::string ToString();

    Token(TokenType);
    Token(TokenType, std::string);
};

#endif