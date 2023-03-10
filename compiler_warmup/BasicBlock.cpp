#include "BasicBlock.h"

shared_ptr<IR> BasicBlock::GetIdentifierValue(int id) const
{
    auto itr = m_SymbolTable.find(id);
    if (itr == m_SymbolTable.end())
    {
    }
    return itr->second;
}