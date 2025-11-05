#include "Markdown.hpp"

Markdown::Markdown(std::string src) { *this = Deserialize(src); }

Markdown Markdown::Deserialize(std::string src) {
    using TextParagraphType = TextParagraph::Type;

    Markdown object{};
    TextParagraphType tp_status{};

    unsigned short heading_level = 1;
    bool heading_start = false;
    Heading heading_buffer{};

    std::string tp_buffer{};
    bool flush = false;
    bool at_line_begin = false;
    enum class StatusType {
        NONE,
        HEADING,
        TEXT_PARAGRAPH
    } status = StatusType::NONE;

    for (auto it = src.begin(); it != src.end(); ++it) {
        switch (status) {
        case StatusType::HEADING:
            if (*it == '#') {
                ++heading_level;
                continue;
            } else if (*it == ' ') {
                heading_start = true;
                continue;
            } else if (heading_start) {
                if (*it != '\n') {
                    if (heading_buffer.level == 0) {
                        heading_buffer.level = heading_level;
                    }
                    heading_buffer.text.push_back(*it);
                } else {
                    heading_level = 1;
                    heading_start = false;
                    flush = true;
                }
            } else {
                status = StatusType::TEXT_PARAGRAPH;
                tp_buffer = std::string(heading_level, '#') + *it;
            }
            break;
        case StatusType::TEXT_PARAGRAPH:
            break;
        default:
            switch (*it) {
            case '*':
                status = StatusType::TEXT_PARAGRAPH;
                tp_status = TextParagraphType::BOLD;
                break;
            case '#':
                status = StatusType::HEADING;
                break;
            case '~':
                break;
            default:
                status = StatusType::TEXT_PARAGRAPH;
                break;
            }
            break;
        }

        if (flush) {
            switch (status) {
            case StatusType::HEADING:
                object.content.push_back(heading_buffer);
                heading_buffer.level = 0;
                heading_buffer.text.clear();
                break;
            default:
                break;
            }
            status = StatusType::NONE;
            flush = false;
        }
    }
    return object;
}