#pragma once
#include <memory>
#include <vector>
#include <string>
#include <sstream>

using std::weak_ptr;
using std::shared_ptr;
using std::vector;
using std::string;
using std::stringstream;

enum class OpType
{
    Const,
    Add,
    Minus,
    Mul,
    Div,
    Cmp,
    Adda,
    Load,
    Store,
    Phi,
    End,
    Bra,
    Bne,
    Beq,
    Ble,
    Blt,
    Bge,
    Bgt,
    Read,
    Write,
    WriteNL
};

class IR
{
public:
    OpType GetOp() const { return m_Op; }
    
    string GetString() const
    {
        stringstream ss;
        switch (m_Op)
        {
        case OpType::Const:
            ss << "const #" << m_ConstantVal;
            break;
        case OpType::Add:
            ss << "add (" << m_X.lock()->m_Number << ") (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::Minus:
            ss << "sub (" << m_X.lock()->m_Number << ") (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::Mul:
            ss << "mul (" << m_X.lock()->m_Number << ") (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::Div:
            ss << "div (" << m_X.lock()->m_Number << ") (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::Cmp:
            ss << "cmp (" << m_X.lock()->m_Number << ") (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::Adda:
            ss << "adda (" << m_X.lock()->m_Number << ") (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::Load:
            ss << "load (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::Store:
            ss << "store (" << m_Y.lock()->m_Number << ") (" << m_X.lock()->m_Number << ")";
            break;
        case OpType::Phi:
            ss << "phi (" << m_X.lock()->m_Number << ") (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::End:
            ss << "end";
            break;
        case OpType::Bra:
            ss << "bra (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::Bne:
            ss << "bne (" << m_X.lock()->m_Number << ") (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::Beq:
            ss << "beq (" << m_X.lock()->m_Number << ") (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::Ble:
            ss << "ble (" << m_X.lock()->m_Number << ") (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::Blt:
            ss << "blt (" << m_X.lock()->m_Number << ") (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::Bge:
            ss << "bge (" << m_X.lock()->m_Number << ") (" << m_Y.lock()->m_Number << ")";
            break;
        case OpType::Bgt:
            ss << "bgt (" << m_X.lock()->m_Number << ") (" << m_Y.lock()->m_Number << ")";
            break;
        }
        return ss.str();
    }

    IR(int constant);

    IR(OpType op, shared_ptr<IR> arg1, shared_ptr<IR> arg2);

    weak_ptr<IR> GetX() const { return m_X; }

    weak_ptr<IR> GetY() const { return m_Y; }

protected:
    int m_Number;

private:
    OpType m_Op;
    weak_ptr<IR> m_X{};
    weak_ptr<IR> m_Y{};
    int m_ConstantVal;

    static int s_Number;
};

enum class ResultType
{
    Constant,
    IR,
};

struct Result
{
    ResultType m_Type;
    int m_ConstantVal;
    weak_ptr<IR> m_Result;
};