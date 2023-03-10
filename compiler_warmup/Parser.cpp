#include "Parser.h"

Parser::Parser(const string& inputStr): tokenizer(inputStr)
{}

// shared_ptr<IR> Parser::GetIdentifierValue(int id) const
// {
//     auto itr = m_SymbolTable.find(id);
//     if (itr == m_SymbolTable.end())
//     {
//         SyntaxError(0);
//     }
//     return itr->second;
// }

int Parser::Designator()
{
    if (tokenizer.GetToken() == Token::IDENTIFIER)
    {
        int identifier = tokenizer.GetIdentifier();
        tokenizer.GetNext();
            
        if (tokenizer.GetToken() == Token::LEFTBRACKET)
        {
            tokenizer.GetNext();
            shared_ptr<IR> resB = Expression();
            // TODO: Evaluate array identifier
            if (tokenizer.GetToken() == Token::RIGHTBRACKET)
            {
                tokenizer.GetNext();
            }
        }
        return identifier;
    }
}

shared_ptr<IR> Parser::Factor()
{
    if (tokenizer.GetToken() == Token::NUMBER)
    {
        int number = tokenizer.GetNumber();
        tokenizer.GetNext();
        shared_ptr<IR> res = make_shared<IR>(number);
        m_ConstantBlock->AddIR(res);
        return res;
    }
    else if (tokenizer.GetToken() == Token::IDENTIFIER)
    {
        int identifier = tokenizer.GetIdentifier();
        tokenizer.GetNext();
        shared_ptr<IR> res = m_ConstantBlock->GetIdentifierValue(identifier);
        return res;
    }
    else if (tokenizer.GetToken() == Token::LEFTPAREN)
    {
        tokenizer.GetNext();
        shared_ptr<IR> res = Expression();

        if (tokenizer.GetToken() == Token::RIGHTPAREN)
        {
            tokenizer.GetNext();
            return res;
        }
        else
        {
            SyntaxError(0);
        }
    }

    return nullptr;
}

shared_ptr<IR> Parser::Term()
{
    shared_ptr<IR> resA = Factor();
    while (true)
    {
        if (tokenizer.GetToken() == Token::MUL)
        {
            tokenizer.GetNext();
            shared_ptr<IR> resB = Factor();
            resA = BuildIR(resA, resB, OpType::Mul);
        }
        else if (tokenizer.GetToken() == Token::DIV)
        {
            tokenizer.GetNext();
            shared_ptr<IR> resB = Factor();
            resA = BuildIR(resA, resB, OpType::Div);
        }
        else
        {
            break;
        }
    }

    return resA;
}

shared_ptr<IR> Parser::Expression()
{
    shared_ptr<IR> resA = Term();
    while (true)
    {
        if (tokenizer.GetToken() == Token::PLUS)
        {
            tokenizer.GetNext();
            shared_ptr<IR> resB = Term();
            resA = BuildIR(resA, resB, OpType::Plus);
        }
        else if (tokenizer.GetToken() == Token::MINUS)
        {
            tokenizer.GetNext();
            shared_ptr<IR> resB = Term();
            resA = BuildIR(resA, resB, OpType::Minus);
        }
        else
        {
            break;
        }
    }

    return resA;
}

shared_ptr<IR> Parser::BuildIR(shared_ptr<IR> resA, shared_ptr<IR> resB, OpType op)
{
    shared_ptr<IR> ir = make_shared<IR>(op, resA, resB);
    m_CurrentBlock->AddIR(ir);
    return resA;
}

void Parser::Relation()
{
    Expression();
    if (tokenizer.GetToken() == Token::RELOP)
    {
        RelOp relOp = tokenizer.GetRelOp();
        tokenizer.GetNext();
        Expression();
        if (relOp == RelOp::EQUAL)
        {
            // TODO: Evaluate expression
            return;
        }
        else if (relOp == RelOp::NOTEQUAL)
        {
            // TODO: Evaluate expression
            return;
        }
        else if (relOp == RelOp::LESS)
        {
            // TODO: Evaluate expression
            return;
        }
        else if (relOp == RelOp::LESSEQUAL)
        {
            // TODO: Evaluate expression
            return;
        }
        else if (relOp == RelOp::GREATER)
        {
            // TODO: Evaluate expression
            return;
        }
        else if (relOp == RelOp::GREATEREQUAL)
        {
            // TODO: Evaluate expression
            return;
        }
    }
    else
    {
        SyntaxError(0);
    }
}

void Parser::Assignment()
{
    if (tokenizer.GetToken() == Token::LET)
    {
        int identifier = Designator();
        if (tokenizer.GetToken() == Token::ASSIGN)
        {
            shared_ptr<IR> resB = Expression();
            // TODO: SSA
            // m_SymbolTable[identifier] = resB;
        }
    }
}

void Parser::FuncCall()
{
    if (tokenizer.GetToken() == Token::CALL)
    {
        tokenizer.GetNext();
        if (tokenizer.GetToken() == Token::IDENTIFIER)
        {
            int ident = tokenizer.GetIdentifier();
            tokenizer.GetNext();
            if (tokenizer.GetToken() == Token::LEFTPAREN)
            {
                tokenizer.GetNext();
                Expression();

                while (tokenizer.GetToken() == Token::COMMA)
                {
                    tokenizer.GetNext();
                    Expression();
                }

                if (tokenizer.GetToken() == Token::RIGHTPAREN)
                {
                    tokenizer.GetNext();

                    // TODO evaluate function call
                }
                else
                {
                    SyntaxError(0);
                }
            }
        }
    }
}

void Parser::IfStatement()
{
    shared_ptr<BasicBlock> prevBasicBlock = m_CurrentBlock;
    if (tokenizer.GetToken() == Token::IF)
    {
        tokenizer.GetNext();
        Relation();
        if (tokenizer.GetToken() == Token::THEN)
        {
            tokenizer.GetNext();

            const shared_ptr<BasicBlock> bb = make_shared<BasicBlock>();
            m_BasicBlocks.push_back(bb);
            m_CurrentBlock = bb;
            //TODO: Copy the symbol table to basic block from the prev basic block

            StatSequence();

            if (tokenizer.GetToken() == Token::ELSE)
            {
                tokenizer.GetNext();

                const shared_ptr<BasicBlock> bbElse = make_shared<BasicBlock>();
                m_BasicBlocks.push_back(bbElse);
                m_CurrentBlock = bbElse;
                //TODO: Copy the symbol table to this basic block from the prev basic block

                StatSequence();

                if (tokenizer.GetToken() == Token::FI)
                {
                    tokenizer.GetNext();
                }
                else
                {
                    SyntaxError(0);
                }
            }
        }

        const shared_ptr<BasicBlock> joinBlock = make_shared<BasicBlock>();
        m_BasicBlocks.push_back(joinBlock);
        m_CurrentBlock = joinBlock;
        //TODO: Copy the symbol table to basic block from the prev basic block
    }
}

void Parser::WhileStatement()
{
    if (tokenizer.GetToken() == Token::WHILE)
    {
        tokenizer.GetNext();
        Relation();
        if (tokenizer.GetToken() == Token::DO)
        {
            tokenizer.GetNext();
            StatSequence();
            if (tokenizer.GetToken() == Token::OD)
            {
                tokenizer.GetNext();
            }
            else
            {
                SyntaxError(0);
            }
        }
    }
}

void Parser::ReturnStatement()
{
    if (tokenizer.GetToken() == Token::RETURN)
    {
        tokenizer.GetNext();
        Expression();
    }
}

void Parser::Statement()
{
    Assignment();
    FuncCall();
    IfStatement();
    WhileStatement();
    ReturnStatement();
}

void Parser::StatSequence()
{
    Statement();
    while (tokenizer.GetToken() == Token::SIMICOLON)
    {
        tokenizer.GetNext();
        Statement();
    }

    if (tokenizer.GetToken() == Token::SIMICOLON)
    {
        tokenizer.GetNext();
    }
    else
    {
        SyntaxError(0);
    }
}

void Parser::TypeDecl()
{
    if (tokenizer.GetToken() == Token::VAR)
    {
        tokenizer.GetNext();
    }
    else if (tokenizer.GetToken() == Token::ARRAY)
    {
        if (tokenizer.GetToken() == Token::LEFTBRACKET)
        {
            tokenizer.GetNext();
            if (tokenizer.GetToken() == Token::NUMBER)
            {
                int number = tokenizer.GetNumber();
                tokenizer.GetNext();

                if (tokenizer.GetToken() == Token::RIGHTBRACKET)
                {
                    tokenizer.GetNext();
                }
                else
                {
                    SyntaxError(0);
                }
            }
        }
    }
}

void Parser::VarDecl()
{
    TypeDecl();
    if (tokenizer.GetToken() == Token::IDENTIFIER)
    {
        int ident = tokenizer.GetIdentifier();
        tokenizer.GetNext();
    }

    while (tokenizer.GetToken() == Token::COMMA)
    {
        tokenizer.GetNext();
        if (tokenizer.GetToken() == Token::IDENTIFIER)
        {
            int ident = tokenizer.GetIdentifier();
            tokenizer.GetNext();
        }
    }

    if (tokenizer.GetToken() == Token::SIMICOLON)
    {
        tokenizer.GetNext();
    }
    else
    {
        SyntaxError(0);
    }
}

void Parser::FuncDecl()
{
    if (tokenizer.GetToken() == Token::VOID)
    {
        tokenizer.GetNext();
    }

    if (tokenizer.GetToken() == Token::FUNCTION)
    {
        tokenizer.GetNext();
        if (tokenizer.GetToken() == Token::IDENTIFIER)
        {
            int ident = tokenizer.GetIdentifier();
            tokenizer.GetNext();
        }

        FormalParam();
        if (tokenizer.GetToken() == Token::SIMICOLON)
        {
            tokenizer.GetNext();
        }

        FuncBody();
        if (tokenizer.GetToken() == Token::SIMICOLON)
        {
            tokenizer.GetNext();
        }
    }
}

void Parser::FormalParam()
{
    if (tokenizer.GetToken() == Token::LEFTPAREN)
    {
        tokenizer.GetNext();
        if (tokenizer.GetToken() == Token::IDENTIFIER)
        {
            int ident = tokenizer.GetIdentifier();
            tokenizer.GetNext();

            while (tokenizer.GetToken() == Token::COMMA)
            {
                tokenizer.GetNext();
                if (tokenizer.GetToken() == Token::IDENTIFIER)
                {
                    int ident = tokenizer.GetIdentifier();
                    tokenizer.GetNext();
                }
            }
        }

        if (tokenizer.GetToken() == Token::RIGHTPAREN)
        {
            tokenizer.GetNext();
        }
        else
        {
            SyntaxError(0);
        }
    }
}

void Parser::FuncBody()
{
    VarDecl();
    if (tokenizer.GetToken() == Token::LEFTCURLBRACKET)
    {
        tokenizer.GetNext();
        StatSequence();
        if (tokenizer.GetToken() == Token::RIGHTCURLBRACKET)
        {
            tokenizer.GetNext();
        }
    }
}

void Parser::Computation()
{
    // Create the basic block for all constant SSA value
    m_ConstantBlock = make_shared<BasicBlock>();
    m_BasicBlocks.push_back(m_ConstantBlock);

    tokenizer.GetNext();
    if (tokenizer.GetToken() == Token::MAIN)
    {
        // Create the basic block for the main
        const shared_ptr<BasicBlock> bb1 = make_shared<BasicBlock>();
        m_BasicBlocks.push_back(bb1);
        m_CurrentBlock = bb1;

        tokenizer.GetNext();
        VarDecl();
        FuncDecl();
        if (tokenizer.GetToken() == Token::LEFTCURLBRACKET)
        {
            tokenizer.GetNext();
            StatSequence();
            if (tokenizer.GetToken() == Token::RIGHTCURLBRACKET)
            {
                tokenizer.GetNext();
                if (tokenizer.GetToken() == Token::DOT)
                {
                    tokenizer.GetNext();
                }
            }
        }
    }
}

void Parser::SyntaxError(int errorCode) const
{
    __debugbreak();
    cout << "syntax error" << errorCode << endl;
}

void Parser::CheckFor(Token token)
{
    if (tokenizer.GetToken() == token)
    {
        tokenizer.GetNext();
    }
    else
    {
        SyntaxError(0);
    }
}
