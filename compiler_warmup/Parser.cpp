#include "Parser.h"

Parser::Parser(const string& inputStr): tokenizer(inputStr)
{}

shared_ptr<IR> Parser::GetIdentifierValue(int id) const
{
    auto itr = m_SymbolTable.find(id);
    if (itr == m_SymbolTable.end())
    {
        SyntaxError(0);
    }
    return itr->second;
}

int Parser::Designator()
{
    if (tokenizer.GetToken() == Token::IDENTIFIER)
    {
        int identifier = tokenizer.GetIdentifier();
        tokenizer.GetNext();
            
        if (tokenizer.GetToken() == Token::LEFTBRACKET)
        {
            tokenizer.GetNext();
            Result resB = Expression();
            // TODO: Evaluate array identifier
            if (tokenizer.GetToken() == Token::RIGHTBRACKET)
            {
                tokenizer.GetNext();
            }
        }
        return identifier;
    }
}

Result Parser::Factor()
{
    Result res;
    if (tokenizer.GetToken() == Token::NUMBER)
    {
        int number = tokenizer.GetNumber();
        res.m_Type = ResultType::Constant;
        res.m_ConstantVal = number;
        tokenizer.GetNext();
    }
    else if (tokenizer.GetToken() == Token::IDENTIFIER)
    {
        int identifier = tokenizer.GetIdentifier();
        res.m_Type = ResultType::IR;
        res.m_Result = GetIdentifierValue(identifier);
        tokenizer.GetNext();
    }
    else if (tokenizer.GetToken() == Token::LEFTPAREN)
    {
        tokenizer.GetNext();
        res = Expression();

        if (tokenizer.GetToken() == Token::RIGHTPAREN)
        {
            tokenizer.GetNext();
        }
        else
        {
            SyntaxError(0);
        }
    }

    return res;
}

Result Parser::Term()
{
    Result resA = Factor();
    while (true)
    {
        if (tokenizer.GetToken() == Token::MUL)
        {
            tokenizer.GetNext();
            Result resB = Factor();
            resA = CombinResult(resA, resB, Token::MUL);
        }
        else if (tokenizer.GetToken() == Token::DIV)
        {
            tokenizer.GetNext();
            Result resB = Factor();
            resA = CombinResult(resA, resB, Token::DIV);
        }
        else
        {
            break;
        }
    }

    return resA;
}

Result Parser::Expression()
{
    Result resA = Term();
    while (true)
    {
        if (tokenizer.GetToken() == Token::PLUS)
        {
            tokenizer.GetNext();
            Result resB = Term();
            resA = CombinResult(resA, resB, Token::PLUS);
        }
        else if (tokenizer.GetToken() == Token::MINUS)
        {
            tokenizer.GetNext();
            Result resB = Term();
            resA = CombinResult(resA, resB, Token::MINUS);
        }
        else
        {
            break;
        }
    }

    return resA;
}

Result Parser::CombinResult(Result resA, Result resB, Token token)
{
    Result res;
    if (resA.m_Type == ResultType::Constant && resB.m_Type == ResultType::Constant)
    {
        if (token == Token::PLUS)
        {
            res.m_Type = ResultType::Constant;
            res.m_ConstantVal = resA.m_ConstantVal + resB.m_ConstantVal;
        }
        else if (token == Token::MINUS)
        {
            res.m_Type = ResultType::Constant;
            res.m_ConstantVal = resA.m_ConstantVal - resB.m_ConstantVal;
        }
        else if (token == Token::MUL)
        {
            res.m_Type = ResultType::Constant;
            res.m_ConstantVal = resA.m_ConstantVal * resB.m_ConstantVal;
        }
        else if (token == Token::DIV)
        {
            res.m_Type = ResultType::Constant;
            res.m_ConstantVal = resA.m_ConstantVal / resB.m_ConstantVal;
        }
    }
    else if (resA.m_Type == ResultType::IR && resB.m_Type == ResultType::Constant)
    {
        if (token == Token::PLUS)
        {
            res.m_Type = ResultType::IR;
            shared_ptr<IR> ir = make_shared<IR>();
            ir->m_Op = OpType::Plus;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg1.m_ResultPtr = resA.m_Result;
            ir->m_Arg1.m_Type = ArgumentType::Constant;
            ir->m_Arg2.m_ConstantVal = resB.m_ConstantVal;
            m_IRInstructions.push_back(ir);
        }
        else if (token == Token::MINUS)
        {
            res.m_Type = ResultType::IR;
            shared_ptr<IR> ir = make_shared<IR>();
            ir->m_Op = OpType::Minus;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg1.m_ResultPtr = resA.m_Result;
            ir->m_Arg1.m_Type = ArgumentType::Constant;
            ir->m_Arg2.m_ConstantVal = resB.m_ConstantVal;
            m_IRInstructions.push_back(ir);
        }
        else if (token == Token::MUL)
        {
            res.m_Type = ResultType::IR;
            shared_ptr<IR> ir = make_shared<IR>();
            ir->m_Op = OpType::Mul;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg1.m_ResultPtr = resA.m_Result;
            ir->m_Arg1.m_Type = ArgumentType::Constant;
            ir->m_Arg2.m_ConstantVal = resB.m_ConstantVal;
            m_IRInstructions.push_back(ir);
        }
        else if (token == Token::DIV)
        {
            res.m_Type = ResultType::IR;
            shared_ptr<IR> ir = make_shared<IR>();
            ir->m_Op = OpType::Div;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg1.m_ResultPtr = resA.m_Result;
            ir->m_Arg1.m_Type = ArgumentType::Constant;
            ir->m_Arg2.m_ConstantVal = resB.m_ConstantVal;
            m_IRInstructions.push_back(ir);
        }
    }
    else if (resA.m_Type == ResultType::Constant && resB.m_Type == ResultType::IR)
    {
        if (token == Token::PLUS)
        {
            res.m_Type = ResultType::IR;
            shared_ptr<IR> ir = make_shared<IR>();
            ir->m_Op = OpType::Plus;
            ir->m_Arg1.m_Type = ArgumentType::Constant;
            ir->m_Arg1.m_ConstantVal = resA.m_ConstantVal;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg2.m_ResultPtr = resB.m_Result;
            m_IRInstructions.push_back(ir);
        }
        else if (token == Token::MINUS)
        {
            res.m_Type = ResultType::IR;
            shared_ptr<IR> ir = make_shared<IR>();
            ir->m_Op = OpType::Minus;
            ir->m_Arg1.m_Type = ArgumentType::Constant;
            ir->m_Arg1.m_ConstantVal = resA.m_ConstantVal;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg2.m_ResultPtr = resB.m_Result;
            m_IRInstructions.push_back(ir);
        }
        else if (token == Token::MUL)
        {
            res.m_Type = ResultType::IR;
            shared_ptr<IR> ir = make_shared<IR>();
            ir->m_Op = OpType::Mul;
            ir->m_Arg1.m_Type = ArgumentType::Constant;
            ir->m_Arg1.m_ConstantVal = resA.m_ConstantVal;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg2.m_ResultPtr = resB.m_Result;
            m_IRInstructions.push_back(ir);
        }
        else if (token == Token::DIV)
        {
            res.m_Type = ResultType::IR;
            shared_ptr<IR> ir = make_shared<IR>();
            ir->m_Op = OpType::Div;
            ir->m_Arg1.m_Type = ArgumentType::Constant;
            ir->m_Arg1.m_ConstantVal = resA.m_ConstantVal;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg2.m_ResultPtr = resB.m_Result;
            m_IRInstructions.push_back(ir);
        }
    }
    else if (resA.m_Type == ResultType::IR && resB.m_Type == ResultType::IR)
    {
        if (token == Token::PLUS)
        {
            res.m_Type = ResultType::IR;
            shared_ptr<IR> ir = make_shared<IR>();
            ir->m_Op = OpType::Plus;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg1.m_ResultPtr = resA.m_Result;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg2.m_ResultPtr = resB.m_Result;
            m_IRInstructions.push_back(ir);
        }
        else if (token == Token::MINUS)
        {
            res.m_Type = ResultType::IR;
            shared_ptr<IR> ir = make_shared<IR>();
            ir->m_Op = OpType::Minus;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg1.m_ResultPtr = resA.m_Result;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg2.m_ResultPtr = resB.m_Result;
            m_IRInstructions.push_back(ir);
        }
        else if (token == Token::MUL)
        {
            res.m_Type = ResultType::IR;
            shared_ptr<IR> ir = make_shared<IR>();
            ir->m_Op = OpType::Mul;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg1.m_ResultPtr = resA.m_Result;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg2.m_ResultPtr = resB.m_Result;
            m_IRInstructions.push_back(ir);
        }
        else if (token == Token::DIV)
        {
            res.m_Type = ResultType::IR;
            shared_ptr<IR> ir = make_shared<IR>();
            ir->m_Op = OpType::Div;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg1.m_ResultPtr = resA.m_Result;
            ir->m_Arg1.m_Type = ArgumentType::IR;
            ir->m_Arg2.m_ResultPtr = resB.m_Result;
            m_IRInstructions.push_back(ir);
        }
    }

    return res;
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
            Result resB = Expression();
            shared_ptr<IR> ir = make_shared<IR>();
            ir->m_Op = OpType::Assignment;
            if (resB.m_Type == ResultType::Constant)
            {
                ir->m_Arg1.m_Type = ArgumentType::Constant;
                ir->m_Arg1.m_ConstantVal = resB.m_ConstantVal;
            }
            else if (resB.m_Type == ResultType::IR)
            {
                ir->m_Arg1.m_Type = ArgumentType::IR;
                ir->m_Arg1.m_ResultPtr = resB.m_Result.lock();
            }
            m_IRInstructions.push_back(ir);
            m_SymbolTable[identifier] = ir;
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
    if (tokenizer.GetToken() == Token::IF)
    {
        tokenizer.GetNext();
        Relation();
        if (tokenizer.GetToken() == Token::THEN)
        {
            tokenizer.GetNext();
            StatSequence();

            if (tokenizer.GetToken() == Token::ELSE)
            {
                tokenizer.GetNext();
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
    if (tokenizer.GetToken() == Token::VAR || tokenizer.GetToken() == Token::ARRAY)
    {
        tokenizer.GetNext();
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
        while (tokenizer.GetToken() == Token::IDENTIFIER)
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
    tokenizer.GetNext();
    if (tokenizer.GetToken() == Token::MAIN)
    {
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
