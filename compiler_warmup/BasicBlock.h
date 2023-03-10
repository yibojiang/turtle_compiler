#pragma once
#include <list>
#include <unordered_map>

#include "IR.h"

using std::list;
using std::vector;
using std::unordered_map;
using std::unique_ptr;

class BasicBlock
{
public:
    void AddIR(shared_ptr<IR> ir)
    {
        // TODO: Check if the ir already in m_Oplist to optimize for copy propagation
        m_SSAInstruction.push_back(ir);
    }

    shared_ptr<IR> GetIdentifierValue(int id) const;

private:
    list<shared_ptr<IR>> m_SSAInstruction{};

    unordered_map<int, shared_ptr<IR>> m_SymbolTable{};

    // Current list for copy propagation
    unordered_map<OpType, shared_ptr<IR>> m_Oplist{};

    vector<weak_ptr<BasicBlock>> m_ForwardEdgeSets{};

    vector<weak_ptr<BasicBlock>> m_BackEdgeSets{};
};
