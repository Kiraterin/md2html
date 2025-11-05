#ifndef __CONVERTER_HPP__
#define __CONVERTER_HPP__

#include "public.hpp"
#include "Markdown.hpp"
#include "Html.hpp"

class Converter {
  public:
    Html Convert(Markdown);
};

#endif