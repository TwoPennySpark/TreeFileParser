#include "node.h"

#define PADDING "  "
#define LIST_START '{'
#define LIST_END   '}'
#define VALUE_START '\"'
#define VALUE_END VALUE_START

//#define NON_NODE_VALUE_CHARS "\n\""
//#define NODE_NAME_FIRST_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_"
//#define NODE_NAME_CHARS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_0123456789"

uint32_t Node::m_lastID = 0;

const std::string SKIP_THROUGH_CHARS = " \r\n";
const std::string NON_NODE_VALUE_CHARS = "\r\n\"";
const std::string NODE_NAME_FIRST_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                          "abcdefghijklmnopqrstuvwxyz_";
const std::string NODE_NAME_CHARS = NODE_NAME_FIRST_CHARS + "0123456789";

pNode Node::create_node(std::string_view &start, uint32_t parentID, uint32_t paddM)
{
    size_t pos = 0;
    pos = check_first_not_of(start, NODE_NAME_CHARS + SKIP_THROUGH_CHARS, "=");
    pos = check_first_not_of(start, SKIP_THROUGH_CHARS, "\"{", pos+1);

    if (start[pos] == VALUE_START)
        return std::make_shared<ValueNode>(parentID, ++m_lastID, start, paddM+1);
    else
        return std::make_shared<ListNode>(parentID, ++m_lastID, start, paddM+1);
}

void Node::parse_name()
{
    auto nameStart = check_first_not_of(m_start, SKIP_THROUGH_CHARS, NODE_NAME_FIRST_CHARS);
    auto nameEnd   = check_first_not_of(m_start, NODE_NAME_CHARS, SKIP_THROUGH_CHARS + "=", nameStart);
    m_name = {m_start.begin()+nameStart, m_start.begin()+nameEnd};

    m_start = m_start.substr(nameEnd);
}

void Node::skip_until(char symbol)
{
    auto pos = check_first_not_of(m_start, SKIP_THROUGH_CHARS, {symbol});

    m_start = m_start.substr(pos+1);
}

void ValueNode::parse_value()
{
    skip_until(VALUE_START);
    auto pos = check_first_of(m_start, NON_NODE_VALUE_CHARS, {VALUE_END});
    m_value = VALUE_START + std::string(m_start.begin(), m_start.begin()+pos+1);

    m_start = m_start.substr(pos+1);
}

void ValueNode::parse()
{
    parse_name();
    skip_until('=');
    parse_value();
}

void ListNode::parse_list()
{
    m_vChildren.emplace_back(create_node(m_start, m_ID, m_paddM));
    m_vChildren.back()->parse();
}

void ListNode::parse()
{
    parse_name();
    skip_until('=');
    skip_until(LIST_START);

    size_t pos = 0;
    do
    {
        parse_list();
        pos = find_first_of(m_start, NODE_NAME_CHARS + LIST_END);
    } while (m_start[pos] != LIST_END);

    skip_until(LIST_END);

    if (!m_parentID)
        check_end(m_start, SKIP_THROUGH_CHARS);
}

std::string Node::display() const
{
    std::string ret;
    for (uint32_t i = 0; i < m_paddM; i++)
        ret += PADDING;
    ret += "(" + std::to_string(m_ID) + ", " +
           std::to_string(m_parentID) + ", " + m_name + ", ";
    return ret;
}

std::string ValueNode::display() const
{
    return Node::display() + m_value + ")\r\n";
}

std::string ListNode::display() const
{
    std::string res;

    res += Node::display();
    for (auto& child: m_vChildren)
        res += std::to_string(child->get_id());
    res += ")\r\n";

    for (auto& child: m_vChildren)
        res += child->display();

    return res;
}
