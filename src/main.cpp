#include "parser.h"
#include <fstream>

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "Russian");

    if (argc < 3)
    {
        std::cout << "Пример использования: parser.exe <входной файл> <выходной файл>\n";
        return 1;
    }

    std::ifstream fIn;
    fIn.open (argv[1], std::ios::in);
    if (!fIn.is_open())
    {
        std::cout << "Ошибка при открытии файла: " << argv[1] << "\n";
        return 1;
    }

    std::ofstream fOut;
    fOut.open(argv[2], std::ios::out | std::ios::trunc);
    if (!fOut.is_open())
    {
        std::cout << "Ошибка при открытии файла: " << argv[2] << "\n";
        return 1;
    }

    std::string sIn((std::istreambuf_iterator<char>(fIn)),
                     std::istreambuf_iterator<char>());

    Node root;
    Parser parser(sIn);
    try {
        root = parser.parse();
    } catch (...) {
        std::cout << "Неверный формат данных\n";
        return 1;
    }

    fOut << root.format();

    return 0;
}
