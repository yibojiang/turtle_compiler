#include "IR.h"

int IR::s_Number{ 0 };

IR::IR(int constant): m_Op(OpType::Const), m_ConstantVal(constant)
{
    m_Number = s_Number;
    ++s_Number;
}

IR::IR(OpType op, shared_ptr<IR> arg1, shared_ptr<IR> arg2): m_Op(op), m_X(arg1), m_Y(arg2)
{
    m_Number = s_Number;
    ++s_Number;
}
