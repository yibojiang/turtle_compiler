#pragma once
#include <memory>
#include <vector>
#include "Argument.h"

using std::weak_ptr;
using std::shared_ptr;
using std::vector;

enum class OpType
{
    Plus,
    Minus,
    Mul,
    Div,
    Assignment,
};

struct IR
{
public:
    IR()
    {

    }

public:
    OpType m_Op;
    Argument m_Arg1;
    Argument m_Arg2;
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