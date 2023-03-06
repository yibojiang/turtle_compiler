#pragma once
#include <memory>

using std::weak_ptr;

struct IR;

enum class ArgumentType
{
    IR,
    Constant
};

struct Argument
{
public:
    ArgumentType m_Type;
    int m_ConstantVal;
    weak_ptr<IR> m_ResultPtr;
};