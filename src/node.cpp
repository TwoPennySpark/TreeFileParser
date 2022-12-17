#include "node.h"

#define LIST_START '{'
#define LIST_END   '}'
#define VALUE_START '"'
#define VALUE_END VALUE_START

const std::string NODE_NAME_FIRST_CHARS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                          "abcdefghijklmnopqrstuvwxyz_";
const std::string NODE_NAME_CHARS = NODE_NAME_FIRST_CHARS + "0123456789";

uint32_t Node::m_lastID = 0;

pNode Node::create_node(std::string_view& view, uint32_t parentID, uint32_t paddM)
{
    size_t pos = 0;

    // look for the symbol after '=' in order to determine node's type
    pos = check_first_not_of(view, NODE_NAME_CHARS + WHITESPACE_CHARS, "=");
    pos = check_first_not_of(view, WHITESPACE_CHARS, "\"{", pos+1);

    if (view[pos] == VALUE_START)
        return std::make_shared<ValueNode>(parentID, ++m_lastID, view, paddM+1);
    else
        return std::make_shared<ListNode> (parentID, ++m_lastID, view, paddM+1);
}

void Node::parse_name()
{
    const std::string NAME_END_CHARS = WHITESPACE_CHARS + "=";

    auto nameStart = check_first_not_of(m_view, WHITESPACE_CHARS, NODE_NAME_FIRST_CHARS);
    auto nameEnd   = check_first_not_of(m_view, NODE_NAME_CHARS, NAME_END_CHARS, nameStart);
    m_name = { m_view.begin() + nameStart, m_view.begin() + nameEnd };

    shift_view(m_view, nameEnd);
}

void ValueNode::parse_value()
{
    const std::string VALUE_END_CHARS = "\n\"";

    skip_until(m_view, VALUE_START);
    auto pos = check_first_of(m_view, VALUE_END_CHARS, {VALUE_END});
    m_value = { m_view.begin(), m_view.begin() + pos };

    shift_view(m_view, pos+1);
}

void ValueNode::parse()
{
    parse_name();
    skip_until(m_view, '=');
    parse_value();

    if (!m_parentID)
        check_any_left(m_view);
}

void ListNode::parse_list()
{   
    skip_until(m_view, LIST_START);

    size_t pos = 0;
    do
    {
        // create new child node and parse it
        m_vChildren.emplace_back(create_node(m_view, m_ID, m_paddM));
        m_vChildren.back()->parse();

        // check for list end ('}'), if none found - there is another child node
        pos = find_first_of(m_view, NODE_NAME_CHARS + LIST_END);
    } while (m_view[pos] != LIST_END);

    skip_until(m_view, LIST_END);
}

void ListNode::parse()
{
    parse_name();
    skip_until(m_view, '=');
    parse_list();

    if (!m_parentID)
        check_any_left(m_view);
}

std::string Node::format() const
{
    std::string ret(m_paddM*2, ' ');

    ret += "(" + std::to_string(m_ID) + ", " +
           std::to_string(m_parentID) + ", " + m_name + ",";

    return ret;
}

std::string ValueNode::format() const
{
    return Node::format() + " " + m_value + ")\r\n";
}

std::string ListNode::format() const
{
    std::string res = Node::format(), childFormat;

    for (auto& child: m_vChildren)
    {
        res += " " + std::to_string(child->get_id());
        childFormat += child->format();
    }
    res += ")\r\n" + childFormat;

    return res;
}
