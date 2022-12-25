#ifndef PARSER_H
#define PARSER_H

#include "tokenizer.h"

struct Node;
using NodeValue = std::string;
using NodeList = std::vector<Node>;

struct Node
{
    Node() = default;
    Node(uint32_t _ID, uint32_t _parentID): ID(_ID), parentID(_parentID) {}

    // display node as formatted string
    std::string format(uint32_t nPadd = 0);

    uint32_t ID = 0, parentID = 0;

    /*
     * each node can either have a simple value:
     * key = "var"
     * or a list filled with other nodes:
     * key = { x = "1" y = "0" z = "0" }
     */
    std::variant<NodeValue, NodeList> value;
    std::string name;

    static uint32_t lastID;
};

class Parser
{
public:
    Parser(const std::string& in): m_tokenizer(in) {}
    Node parse();

private:
    TokenBody consume(TOKEN_TYPE type);

    Node parse_node(uint32_t parentID);
    NodeList parse_node_list(uint32_t parentID);
    NodeValue parse_node_value();

    Tokenizer m_tokenizer;

    // contains next token that is getting consumed
    std::optional<Token> m_lookahead;
};

#endif // PARSER_H
