#pragma once
#include <list>
#include <unordered_map>

#include "IR.h"

using std::list;
using std::vector;
using std::unordered_map;
using std::unique_ptr;
using std::string;

class BasicBlock
{
public:
    shared_ptr<IR> AddIR(shared_ptr<IR> ir);

    void AddForwardEdgeSets(const shared_ptr<BasicBlock>& basicBlock)
    {
        m_ForwardEdgeSets.push_back(basicBlock);
    }

    void AddBackwardEdgeSets(const shared_ptr<BasicBlock>& basicBlock)
    {
        m_BackwardEdgeSets.push_back(basicBlock);
    }

    shared_ptr<IR> GetIdentifierValue(int id) const;

    void SetIdentifierValue(int id, const shared_ptr<IR>& ir);

    string GetGraphString() const;

private:
    list<shared_ptr<IR>> m_SSAInstruction{};

    unordered_map<int, shared_ptr<IR>> m_SymbolTable{};

    // Current list for copy propagation
    unordered_map<OpType, list<weak_ptr<IR>>> m_Oplist{};

    vector<weak_ptr<BasicBlock>> m_ForwardEdgeSets{};

    vector<weak_ptr<BasicBlock>> m_BackwardEdgeSets{};
};
