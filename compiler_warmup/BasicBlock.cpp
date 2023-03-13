#include "BasicBlock.h"

#include <iostream>
#include <sstream>

using std::stringstream;

shared_ptr<IR> BasicBlock::AddIR(shared_ptr<IR> ir)
{
    if (m_Oplist.find(ir->GetOp()) != m_Oplist.end())
    {
        auto& opList = m_Oplist[ir->GetOp()];
        for (auto it = opList.begin(); it != opList.end(); it++)
        {
            if (it->lock()->GetX().lock() == ir->GetX().lock() && it->lock()->GetY().lock() == ir->GetY().lock())
            {
                return it->lock();
            }
        }
    }
        
    // TODO: Check if the ir already in m_Oplist to optimize for copy propagation
    m_SSAInstruction.push_back(ir);
    m_Oplist[ir->GetOp()].push_back(ir);
    return ir;
}

shared_ptr<IR> BasicBlock::GetIdentifierValue(int id) const
{
    auto itr = m_SymbolTable.find(id);
    if (itr == m_SymbolTable.end())
    {
        return nullptr;
    }
    return itr->second;
}

void BasicBlock::SetIdentifierValue(int id, const shared_ptr<IR>& ir)
{
    m_SymbolTable[id] = ir;
}

string BasicBlock::GetGraphString() const
{
    if (m_SSAInstruction.empty())
    {
        return "";
    }
    stringstream ss;
    ss << "{";
    auto it = m_SSAInstruction.begin();
    it++;
    for (; it != m_SSAInstruction.end(); ++it)
    {
        ss << "|";
        ss << it->get()->GetString();
    }

    ss << "}";
    return ss.str();
}
