#include "parser.h"

uint32_t Node::lastID = 0;

Node Parser::parse()
{
    m_lookahead = m_tokenizer.next_token();

    auto root = parse_node(0);

    if (m_tokenizer.has_more_tokens() || m_lookahead)
        throw std::runtime_error("Unexpected data at the end");

    return root;
}

TokenBody Parser::consume(TOKEN_TYPE nextExpectedType)
{
    if (!m_lookahead)
        throw std::runtime_error("Unexpected end of input");

    auto [nextTokenType, nextTokenBody] = *m_lookahead;

    if (nextTokenType != nextExpectedType)
        throw std::runtime_error("Unexpected token");

    m_lookahead = m_tokenizer.next_token();

    return nextTokenBody;
}

NodeValue Parser::parse_node_value()
{
    auto value = consume(TOKEN_VALUE);

    // trim string from ""
    return value.substr(1, value.size()-2);
}

NodeList Parser::parse_node_list(uint32_t parentID)
{
    NodeList nodes;

    consume(TOKEN_LIST_START);
    do
    { nodes.emplace_back(parse_node(parentID)); }
    while (m_lookahead && m_lookahead->first != TOKEN_LIST_END);
    consume(TOKEN_LIST_END);

    return nodes;
}

Node Parser::parse_node(uint32_t parentID)
{
    Node node(++Node::lastID, parentID);

    node.name = consume(TOKEN_NAME);
    consume(TOKEN_EQUAL_SIGN);

    if (!m_lookahead)
        throw std::runtime_error("Unexpected end of input");

    switch (m_lookahead->first)
    {
    case TOKEN_VALUE:
        node.value = parse_node_value();
        break;
    case TOKEN_LIST_START:
        node.value = parse_node_list(node.ID);
        break;
    default:
        throw std::runtime_error("Unexpected node type");
    }

    return node;
}

std::string Node::format(uint32_t nPadd)
{
    std::string ret(nPadd*2, ' ');
    ret += "(" + std::to_string(ID) + ", " + std::to_string(parentID) + ", " + name + ",";

    if (auto str = std::get_if<NodeValue>(&value); str)
        ret += " " + *str + ")\n";
    else
    {
        std::string childFormat;
        auto& children = std::get<NodeList>(value);

        for (auto& child: children)
        {
            ret += " " + std::to_string(child.ID);
            childFormat += child.format(nPadd+1);
        }
        ret += ")\n" + childFormat;
    }

    return ret;
}
