#include "Markdown.hpp"

Markdown::Markdown(std::string src) { *this = Deserialize(src); }

Markdown Markdown::Deserialize(std::string src) {
    using TextParagraphType = TextParagraph::Type;

    Markdown object{};
    std::map<char, uint8_t> char_counter;
    char cur, fore;

    bool heading_start = false;
    Heading heading_buffer{};

    TextParagraphType tp_status{};
    TextParagraph tp_buffer{};
    std::string tp_string_buffer{};

    bool is_linebegin = true;
    enum class StatusType {
        NONE,
        HEADING,
        TEXT_PARAGRAPH,
        CODE_BLOCK
    } status = StatusType::NONE;

    for (auto it = src.begin(); it != src.end(); ++it) {
        cur = *it;
        fore = (it + 1) != src.end() ? *(it + 1) : '\0';
        if (cur == '\n') {
            is_linebegin = true;
            continue;
        }
        is_linebegin = false;
    }

    return object;
}