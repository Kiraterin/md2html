#include "Tokenizer.hpp"

Token::Token(TokenType token_t) { this->token_type = token_t; }

Token::Token(TokenType token_t, std::string content) {
    this->token_type = token_t;
    this->content = content;
}

static bool isblank(char c, bool include_newline = true,
                    bool include_tab = true) {
    bool is_space = c == ' ';
    bool is_tab = c == '\t' && include_tab;
    bool is_newline = c == '\n' && include_newline;
    return is_space || is_tab || is_newline;
}

void text_flush(std::string &buf, std::vector<Token> &ret) {
    if (!buf.empty()) {
        ret.push_back({TokenType::TextPart, buf});
        buf.clear();
    }
}

std::vector<Token> Token::Tokenize(const std::string &src) {
    std::vector<Token> ret;
    auto iter = src.begin();
    auto forward = src.begin();

    std::string buf;

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

            if (cur == '#') {
                size_t cnt = 0;
                forward = iter;
                while (forward != src.end() && *forward == '#') {
                    ++forward;
                    ++cnt;
                }
                if (forward != src.end() && *forward == ' ' && cnt <= 6) {
                    ret.push_back({TokenType::Title, std::to_string(cnt)});
                    iter = forward + 1;
                    continue;
                }
            } else if (cur == '*') {
                forward = iter;
                size_t cnt = 0;
                while (forward != src.end() && (*forward == '*' || isblank(*forward, false))) {
                    ++forward;
                    ++cnt;
                }
                if (forward != src.end() && *forward == '\n' && cnt >= 3) {
                    ret.push_back({TokenType::HorizonTalRule});
                    iter = forward;
                    continue;
                }
            } else {
                forward = iter + 1;
                if (forward != src.end()) {
                    if (cur == '-' && *forward == ' ') {
                        ret.push_back({TokenType::UnorderedList});
                        while (forward != src.end() &&
                               isblank(*forward, false)) {
                            ++forward;
                        }
                        iter = forward;
                        continue;
                    } else if (std::isdigit(cur)) {
                        std::string number;
                        number.push_back(cur);
                        while (forward != src.end() && std::isdigit(*forward)) {
                            number.push_back(*forward);
                            ++forward;
                        }
                        if (forward != src.end() && *forward == '.') {
                            forward += 1;
                            if (forward != src.end() && *forward == ' ') {
                                ret.push_back({TokenType::OrderedList, number});
                                while (forward != src.end() &&
                                       isblank(*forward, false)) {
                                    ++forward;
                                }
                                iter = forward;
                                continue;
                            }
                        }
                    }
                }
            }
        }
        switch (cur) {
        case '*': {
            text_flush(buf, ret);
            ret.push_back({TokenType::Star});
            break;
        }
        case '~': {
            forward = iter + 1;
            if (forward != src.end() && *forward == '~') {
                text_flush(buf, ret);
                ret.push_back({TokenType::Delete});
                iter = forward;
            } else {
                buf.push_back(cur);
            }
            break;
        }
        case '\n':
            is_newline = true;
            text_flush(buf, ret);
            break;
        default:
            buf.push_back(cur);
            break;
        }
        ++iter;
    }

    text_flush(buf, ret);

    return ret;
}