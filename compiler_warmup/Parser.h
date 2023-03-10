#pragma once
#include <iostream>
#include <string>

#include "BasicBlock.h"
#include "IR.h"
#include "Tokenizer.h"

using std::string;
using std::cout;
using std::endl;
using std::make_shared;

struct Parser
{
    Parser(const string& inputStr);

    int Designator();

    shared_ptr<IR> Factor();

    shared_ptr<IR> Term();

    shared_ptr<IR> Expression();

    shared_ptr<IR> BuildIR(shared_ptr<IR> A, shared_ptr<IR> B, OpType op);

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

    shared_ptr<BasicBlock> m_ConstantBlock{};

    vector<shared_ptr<BasicBlock>> m_BasicBlocks{};

    // indicate the current block during the parsing
    shared_ptr<BasicBlock> m_CurrentBlock{};


};
