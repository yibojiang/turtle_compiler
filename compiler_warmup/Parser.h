#pragma once
#include <iostream>
#include <string>

#include "Tokenizer.h"

using std::string;
using std::cout;
using std::endl;

struct Parser
{
    Parser(const string& inputStr);

    int GetIdentifierValue(int id) const;

    void Designator();

    int Factor();

    int Term();

    int Expression();

    bool Relation();

    void Assignment();

    void FuncCall();

    void IfStatement();

    void WhileStatement();

    void ReturnStatement();

    void Statement();

    void StatSequence();

    void TypeDecl();

    void VarDecl();

    void FuncDecl();

    void FormalParam();

    void FuncBody();

    void Computation();

    void SyntaxError(int errorCode) const;

    Tokenizer tokenizer;

    unordered_map<int, int> m_VariableTable;
};
