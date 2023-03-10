#pragma once
#include <memory>
#include <vector>

using std::weak_ptr;
using std::shared_ptr;
using std::vector;

enum class OpType
{
    Constant,
    Plus,
    Minus,
    Mul,
    Div,
    Assignment,
    Phi
};


class IR
{
public:
    OpType GetOp() const { return m_Op; }

    IR(int constant);

    IR(OpType op, shared_ptr<IR> arg1, shared_ptr<IR> arg2);

protected:
    int m_Number;

private:
    OpType m_Op;
    weak_ptr<IR> m_Arg1{};
    weak_ptr<IR> m_Arg2{};
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