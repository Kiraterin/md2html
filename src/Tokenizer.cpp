#include "Tokenizer.hpp"

Token::Token(TokenType token_t) { this->token_type = token_t; }

Token::Token(TokenType token_t, std::string content) {
    this->token_type = token_t;
    this->content = content;
}

static bool isblank(char c, bool include_newline = true) {
    if (include_newline) {
        return c == ' ' || c == '\t' || c == '\n';
    } else {
        return c == ' ' || c == '\t';
    }
}

std::vector<Token> Token::Tokenize(const std::string &src) {
    std::vector<Token> ret;
    auto iter = src.begin();

    std::string buf;
    bool should_flush = false;

    bool is_newline = true;
    bool is_multi_newline = false;

    size_t star_cnt = 0;

    while (iter != src.end()) {
        auto &cur = *iter;

        if (is_newline) {
            if (isblank(cur)) {
                if (cur == '\n') {
                    is_multi_newline = true;
                }
                ++iter;
                continue;
            } else {
                if (is_multi_newline) {
                    is_multi_newline = false;
                    ret.push_back(Token(TokenType::BlankLine));
                }
                is_newline = false;
            }
        }

        switch (cur) {
        case '#': {
            if (is_newline) {
                ret.push_back(Token(TokenType::HashTag));
            } else {
                buf.push_back(cur);
            }
            break;
        }
        case '*': {
            auto forward = iter;
            while (isblank(*forward, false) || *forward == '*') {
                if (*forward == '*') {
                    ++star_cnt;
                }
                ++forward;
            }
            if (*forward == '\n' && star_cnt >= 3) {
                ret.push_back(Token(TokenType::HorizonTalRule));
            } else {
                while (star_cnt > 0) {
                    ret.push_back(Token(TokenType::Star));
                    --star_cnt;
                }
            }
            iter = forward - 1;
            star_cnt = 0;
            break;
        }
        case '\n':
            is_newline = true;
            should_flush = true;
            break;
        default:
            buf.push_back(cur);
            break;
        }

        if (should_flush) {
            if (!buf.empty()) {
                ret.push_back(Token(TokenType::TextPart, buf));
                buf.clear();
            }
            should_flush = false;
        }
        ++iter;
    }
    return ret;
}