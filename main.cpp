#include "node.h"

pNode make_tree(std::string_view &s)
{
    auto root = Node::create_node(s);
    root->parse();

    return root;
}

int main()
{
    {
        std::vector<std::string> in = {
            "key = \"var\" ",
            "_key123_     =\"123asd !@#$%^&*()\"",
            "_key123_9=    \"{78{}(['\"",
            "_=\"\"",
        };
        std::vector<std::string> out = {
            "(1, 0, key, \"var\")\r\n",
            "(1, 0, _key123_, \"123asd !@#$%^&*()\")\r\n",
            "(1, 0, _key123_9, \"{78{}(['\")\r\n",
            "(1, 0, _, \"\")\r\n",
        };

        for (size_t i = 0; i < in.size(); i++)
        {
            std::string_view v(in[i]);
            auto tree = make_tree(v);
            auto res = tree->display();

            std::cout << res;
            assert(res == out[i]);
            Node::m_lastID = 0;
        }
    }

    {
        std::cout << "=================================\n";
        std::vector<std::string> in = {
            "point = { x = \"1\" } ",
            "point\r\n =\r\n \r\n \r\n{\r\n x\r\n  \r\n \r\n=\r\n \r\n\"1\"\r\n \r\n}\r\n \r\n"
        };
        std::vector<std::string> out = {
            "(1, 0, point, 2)\r\n"
            "  (2, 1, x, \"1\")\r\n",
            "(1, 0, point, 2)\r\n"
            "  (2, 1, x, \"1\")\r\n",
        };

        for (size_t i = 0; i < in.size(); i++)
        {
            std::string_view v(in[i]);
            auto tree = make_tree(v);
            auto res = tree->display();

            std::cout << res;
            assert(res == out[i]);
            Node::m_lastID = 0;
        }
    }

    {
        std::cout << "=================================\n";
        std::vector<std::string> in = {
            "point = { x = \"1\" y = \"0\" z = \"0\" } "
        };
        std::vector<std::string> out = {
            "(1, 0, point, 234)\r\n"
            "  (2, 1, x, \"1\")\r\n"
            "  (3, 1, y, \"0\")\r\n"
            "  (4, 1, z, \"0\")\r\n"
        };

        for (size_t i = 0; i < in.size(); i++)
        {
            std::string_view v(in[i]);
            auto tree = make_tree(v);
            auto res = tree->display();

            std::cout << res;
            assert(res == out[i]);
            Node::m_lastID = 0;
        }
    }

    {
        std::cout << "=================================\n";
        std::string in =
        {
            "shape = { \r\n"
            "type = \"tetrahedron\" \r\n"
            "vertices = { \r\n"
            "point = { x = \"1\" y = \"0\" z = \"0\" } \r\n"
            "point = { x = \"0\" y = \"1\" z = \"0\" } \r\n"
            "point = { x = \"0\" y = \"0\" z = \"1\" } \r\n"
            "point = { x = \"1\" y = \"1\" z = \"1\" } \r\n"
            "} \r\n"
            "color = { r = \"0xFF\" g = \"0x00\" b = \"0x80\" alpha = \"0x80\" } \r\n"
            "} "
        };

        std::string out =
        {
            "(1, 0, shape, 2320)\r\n"
            "  (2, 1, type, \"tetrahedron\")\r\n"
            "  (3, 1, vertices, 481216)\r\n"
            "    (4, 3, point, 567)\r\n"
            "      (5, 4, x, \"1\")\r\n"
            "      (6, 4, y, \"0\")\r\n"
            "      (7, 4, z, \"0\")\r\n"
            "    (8, 3, point, 91011)\r\n"
            "      (9, 8, x, \"0\")\r\n"
            "      (10, 8, y, \"1\")\r\n"
            "      (11, 8, z, \"0\")\r\n"
            "    (12, 3, point, 131415)\r\n"
            "      (13, 12, x, \"0\")\r\n"
            "      (14, 12, y, \"0\")\r\n"
            "      (15, 12, z, \"1\")\r\n"
            "    (16, 3, point, 171819)\r\n"
            "      (17, 16, x, \"1\")\r\n"
            "      (18, 16, y, \"1\")\r\n"
            "      (19, 16, z, \"1\")\r\n"
            "  (20, 1, color, 21222324)\r\n"
            "    (21, 20, r, \"0xFF\")\r\n"
            "    (22, 20, g, \"0x00\")\r\n"
            "    (23, 20, b, \"0x80\")\r\n"
            "    (24, 20, alpha, \"0x80\")\r\n"
        };

        std::string_view v(in);
        auto tree = make_tree(v);
        auto res = tree->display();

        std::cout << res;
        assert(res == out);
    }

    std::cout << "END\n";

    return 0;
}
