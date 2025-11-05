#include "Markdown.hpp"
#include "public.hpp"

int main() {
    Markdown md{"# h1\n## h2\n##atst4\n### h3\n"};
    for (auto c : md.content) {
        std::cout << std::endl;
    }
    return 0;
}