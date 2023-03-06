#pragma once
#include <iostream>
#include <string>

#include "IR.h"
#include "Tokenizer.h"

using std::string;
using std::cout;
using std::endl;
using std::make_shared;

struct Parser
{
    Parser(const string& inputStr);

    shared_ptr<IR> GetIdentifierValue(int id) const;

    int Designator();

    Result Factor();

    Result Term();

    Result Expression();

    Result CombinResult(Result A, Result B, Token token);

    void Relation();

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

    void CheckFor(Token token);

    Tokenizer tokenizer;

    unordered_map<int, shared_ptr<IR>> m_SymbolTable;

    vector<shared_ptr<IR>> m_IRInstructions;
};
