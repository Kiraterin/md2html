#ifndef __TOKENIZER_HPP__
#define __TOKENIZER_HPP__

#include "public.hpp"

enum class TokenType { TextPart, Title, Delete, Star, HorizonTalRule, BlankLine, UnorderedList, OrderedList };

class Token {
  public:
    TokenType token_type;
    std::string content;

    static std::vector<Token> Tokenize(const std::string &);

    Token(TokenType);
    Token(TokenType, std::string);
};

#endif