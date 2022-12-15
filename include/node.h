#ifndef _NODE_H_
#define _NODE_H_

#include "util.h"

/*
 * N ::= I = ( V | L )
 * L ::= {[\n] N [N...] [\n]}
 * I ::= [0-9][a-zA-Z][_]
 * V ::= "..." -[\"\n]
 */

class Node; using pNode = std::shared_ptr<Node>;

class Node
{
public:
    Node(uint32_t _parentID, uint32_t _ID, std::string_view& _start, uint32_t _paddM):
        m_parentID(_parentID), m_ID(_ID), m_paddM(_paddM), m_start(_start) {}

    virtual ~Node() {}
    virtual void parse() = 0;
    virtual std::string display() const;

    uint32_t get_id() { return m_ID; }

    static pNode create_node(std::string_view& start, uint32_t parentID = 0, uint32_t paddM = uint32_t(-1));

    static uint32_t m_lastID;

protected:
    void parse_name();
    void skip_until(char symbol);

    uint32_t m_parentID, m_ID, m_paddM;
    std::string m_name;

    std::string_view& m_start;
};

class ValueNode: public Node
{
public:
    ValueNode(uint32_t _parentID, uint32_t _ID, std::string_view& _start, uint32_t _paddM):
        Node(_parentID, _ID, _start, _paddM) {}

    void parse() override;
    std::string display() const override;

private:
    void parse_value();

    std::string m_value;
};

class ListNode: public Node
{
public:
    ListNode(uint32_t _parentID, uint32_t _ID, std::string_view& _start, uint32_t _paddM):
        Node(_parentID, _ID, _start, _paddM) {}

    void parse() override;
    std::string display() const override;

private:
    void parse_list();

    std::vector<pNode> m_vChildren;
};

#endif
