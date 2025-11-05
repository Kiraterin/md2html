#ifndef __DOCUMENT_HPP__
#define __DOCUMENT_HPP__

#include "public.hpp"

class Markdown {
  private:
    class Section {};
    class TextParagraph : public Section {

      public:
        enum class Type {
            PLAIN,
            BOLD,
            ITALIC,
            BOLD_ITALIC,
            STRIKE_THROUGH,
            CODE,
            NEW_LINE
        };
        class Text {
          public:
            Type type = Type::PLAIN;
            std::string content;
        };
        std::vector<Text> texts;
    };
    class Heading : public Section {
      public:
        unsigned short level = 0;
        std::string text;
    };
    class HorizontalRule : public Section {};
    class List : public Section {};
    class CheckList : public Section {};
    class HyperLink : public Section {};
    class Image : public Section {};
    class CodeBlock : public Section {};
    class BlockQuote : public Section {};

  public:
    std::vector<Section> content{};

    Markdown Deserialize(std::string);
    Markdown(){};
    Markdown(std::string);
};

#endif