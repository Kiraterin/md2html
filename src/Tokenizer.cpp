#include "Tokenizer.hpp"

Token::Token(TokenType token_t) { this->token_type = token_t; }

Token::Token(TokenType token_t, std::string content) {
    this->token_type = token_t;
    this->content = content;
}

std::string Token::ToString() {
    std::string enum_str = [this] {
        switch (token_type) {
        case TokenType::TextPart: return "TextPart";
        case TokenType::Title: return "Title";
        case TokenType::Delete: return "Delete";
        case TokenType::Star: return "Star";
        case TokenType::HorizontalRule: return "HorizontalRule";
        case TokenType::BlankLine: return "BlankLine";
        case TokenType::UnorderedList: return "UnorderedList";
        case TokenType::OrderedList: return "OrderedList";
        case TokenType::QuotationBegin: return "QuotationBegin";
        case TokenType::QuotationEnd: return "QuotationEnd";
        case TokenType::CodeLang: return "CodeLang";
        case TokenType::CodeBlock: return "CodeBlock";
        default: return "UnknownToken";
        }
    }();
    std::string content_tmp;
    for (auto c : content) {
        if (c == '\n') {
            content_tmp += "\\n";
        } else {
            content_tmp += c;
        }
    }
    return std::format("<{}{}>", enum_str,
                       content_tmp.empty() ? "" : ", \"" + content_tmp + "\"");
}

static bool isblank(char c, bool include_newline = false,
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

void base_tokenize(std::string src, std::vector<Token> &ret) {
    src.push_back('\n');

    auto iter = src.begin();
    auto forward = src.begin();

    std::string buf;

    bool is_newline = true;
    bool is_multi_newline = false;

    while (iter != src.end()) {
        if (is_newline) {
            if (*iter == '\n') {
                is_multi_newline = true;
            }
            if (isblank(*iter, true)) {
                ++iter;
                continue;
            } else {
                if (is_multi_newline) {
                    is_multi_newline = false;
                    ret.push_back(Token(TokenType::BlankLine));
                }
                is_newline = false;
            }

            forward = iter + 1;
            switch (forward = iter + 1; *iter) {
            case '#': {
                size_t cnt = 0;
                forward = iter;
                while (*forward == '#') {
                    ++forward;
                    ++cnt;
                }
                if (*forward == ' ' && cnt <= 6) {
                    ret.push_back({TokenType::Title, std::to_string(cnt)});
                    iter = forward + 1;
                    continue;
                }
                break;
            }
            case '*': {
                auto gorward = forward;
                size_t cnt = 0;
                while (*gorward == '*' || isblank(*gorward)) {
                    ++gorward;
                    ++cnt;
                }
                if (*forward == ' ' && *gorward != '\n') {
                    ret.push_back({TokenType::UnorderedList});
                    while (isblank(*forward)) {
                        ++forward;
                    }
                    iter = forward;
                    continue;
                } else if (cnt >= 3) {
                    ret.push_back({TokenType::HorizontalRule});
                    iter = gorward;
                    continue;
                }
                break;
            }
            case '>': {
                ret.push_back({TokenType::QuotationBegin});
                std::string section;
                forward = iter;
                while (forward != src.end() && *forward == '>') {
                    forward += 1;
                    while (isblank(*forward)) {
                        ++forward;
                    }
                    while (*forward != '\n') {
                        section.push_back(*forward);
                        ++forward;
                    }
                    section.push_back('\n');
                    forward += 1;
                }
                base_tokenize(section, ret);
                ret.push_back({TokenType::QuotationEnd});
                continue;
                break;
            }
            case '-': {
                if (isblank(*forward)) {
                    ret.push_back({TokenType::UnorderedList});
                    while (isblank(*forward)) {
                        ++forward;
                    }
                    iter = forward;
                    continue;
                }
                break;
            }
            case '`': {
                size_t sign_amount = 1;
                std::string lang;
                std::string code;
                while (*forward == '`') {
                    ++forward;
                    ++sign_amount;
                }
                if (sign_amount == 3) {
                    while (isblank(*forward)) {
                        ++forward;
                    }
                    if (*forward != '\n') {
                        while (*forward != ' ' && *forward != '\n') {
                            lang.push_back(*forward);
                            ++forward;
                        }
                        ret.push_back({TokenType::CodeLang, lang});
                        while (*forward != '\n') {
                            ++forward;
                        }
                    } else {
                        ret.push_back({TokenType::CodeLang});
                    }
                    forward += 1;
                    while (forward != src.end()) {
                        if (*forward == '`') {
                            auto before = forward;
                            sign_amount = 0;
                            while (*forward == '`') {
                                ++sign_amount;
                                ++forward;
                            }
                            if (sign_amount == 3) {
                                break;
                            }
                        }
                        code.push_back(*forward);
                        ++forward;
                    }
                    ret.push_back({TokenType::CodeBlock, code});
                    iter = forward;
                    continue;
                }
                break;
            }
            case '0' ... '9': {
                std::string number;
                number.push_back(*iter);
                while (std::isdigit(*forward)) {
                    number.push_back(*forward);
                    ++forward;
                }
                if (*forward == '.') {
                    forward += 1;
                    if (*forward == ' ') {
                        ret.push_back({TokenType::OrderedList, number});
                        while (isblank(*forward)) {
                            ++forward;
                        }
                        iter = forward;
                        continue;
                    }
                }
                break;
            }
            default: {
                break;
            }
            }
        }

        switch (*iter) {
        case '*': {
            text_flush(buf, ret);
            ret.push_back({TokenType::Star});
            break;
        }
        case '~': {
            forward = iter + 1;
            if (*forward == '~') {
                text_flush(buf, ret);
                ret.push_back({TokenType::Delete});
                iter = forward;
            } else {
                buf.push_back(*iter);
            }
            break;
        }
        case '`': {
            std::string code;
            iter += 1;
            while (iter != src.end() && *iter != '`') {
                code.push_back(*iter);
                ++iter;
            }
            if (iter == src.end()) {
                iter -= 1;
            }
            ret.push_back({TokenType::CodeBlock, code});
            break;
        }
        case '\n': {
            is_newline = true;
            text_flush(buf, ret);
            break;
        }
        default: {
            buf.push_back(*iter);
            break;
        }
        }
        ++iter;
    }
}

std::vector<Token> Token::Tokenize(std::string src) {
    std::vector<Token> ret;
    base_tokenize(src, ret);
    return ret;
}