#include "node.h"
#include <fstream>

int main(int argc, char **argv)
{
    if (argc < 3)
        return 1;

    std::ifstream fIn;
    std::ofstream fOut;
    fIn.open (argv[1], std::ios::in);
    fOut.open(argv[2], std::ios::out | std::ios::trunc);
    if (!fIn.is_open() || !fOut.is_open())
        return 1;

    std::string sIn((std::istreambuf_iterator<char>(fIn)),
                     std::istreambuf_iterator<char>());
    std::string_view svIn(sIn);

    pNode root;
    try {
        root = Node::create_node(svIn);
        root->parse();
    } catch (...) {
        std::cout << "Неверный формат данных\n";
        return 1;
    }
    fOut << root->format();

    return 0;
}
