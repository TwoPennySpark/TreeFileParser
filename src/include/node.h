#ifndef NODE_H
#define NODE_H

#include <memory>
#include <vector>
#include "util.h"

class Node; using pNode = std::shared_ptr<Node>;

class Node
{
public:
    Node(uint32_t _parentID, uint32_t _ID, std::string_view& _view, uint32_t _paddM):
        m_parentID(_parentID), m_ID(_ID), m_paddM(_paddM), m_view(_view) {}

    virtual ~Node() {}
    virtual void parse() = 0;
    virtual std::string format() const;

    uint32_t get_id() { return m_ID; }

    static pNode create_node(std::string_view& view, uint32_t parentID = 0, uint32_t paddM = uint32_t(-1));

protected:
    void parse_name();

    static uint32_t m_lastID;

    uint32_t m_parentID, m_ID, m_paddM;
    std::string m_name;
    std::string_view& m_view;
};

class ValueNode: public Node
{
public:
    ValueNode(uint32_t _parentID, uint32_t _ID, std::string_view& _view, uint32_t _paddM):
        Node(_parentID, _ID, _view, _paddM) {}

    void parse() override;
    std::string format() const override;

private:
    void parse_value();

    std::string m_value;
};

class ListNode: public Node
{
public:
    ListNode(uint32_t _parentID, uint32_t _ID, std::string_view& _view, uint32_t _paddM):
        Node(_parentID, _ID, _view, _paddM) {}

    void parse() override;
    std::string format() const override;

private:
    void parse_list();

    std::vector<pNode> m_vChildren;
};

#endif
