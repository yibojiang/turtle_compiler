#include "Parser.h"

Parser::Parser(const string& inputStr): tokenizer(inputStr)
{}

int Parser::GetIdentifierValue(int id) const
{
    auto itr = m_VariableTable.find(id);
    if (itr == m_VariableTable.end())
    {
        SyntaxError(0);
    }
    return itr->second;
}

void Parser::Designator()
{
    if (tokenizer.GetToken() == Token::IDENTIFIER)
    {
        int identifier = tokenizer.GetIdentifier();
        tokenizer.GetNext();

            
        if (tokenizer.GetToken() == Token::LEFTBRACKET)
        {
            tokenizer.GetNext();
            Expression();
            if (tokenizer.GetToken() == Token::RIGHTBRACKET)
            {
                tokenizer.GetNext();
            }
        }
    }
}

int Parser::Factor()
{
    if (tokenizer.GetToken() == Token::NUMBER)
    {
        int number = tokenizer.GetNumber();
        tokenizer.GetNext();
        return number;
    }
    else if (tokenizer.GetToken() == Token::IDENTIFIER)
    {
        int identifier = tokenizer.GetIdentifier();
        int val = GetIdentifierValue(identifier);
        tokenizer.GetNext();
        return val;
    }
    else if (tokenizer.GetToken() == Token::LEFTPAREN)
    {
        tokenizer.GetNext();
        int val = Expression();

        if (tokenizer.GetToken() == Token::RIGHTPAREN)
        {
            tokenizer.GetNext();
            return val;
        }

        SyntaxError(0);
        return 0;
    }
    return 0;
}

int Parser::Term()
{
    int result = Factor();
    while (true)
    {
        if (tokenizer.GetToken() == Token::MUL)
        {
            tokenizer.GetNext();
            result *= Factor();
        }
        else if (tokenizer.GetToken() == Token::DIV)
        {
            tokenizer.GetNext();
            result /= Factor();
        }
        else
        {
            break;
        }
    }
    return result;
}

int Parser::Expression()
{
    int result = Term();
    while (true)
    {
        if (tokenizer.GetToken() == Token::PLUS)
        {
            tokenizer.GetNext();
            result += Term();
        }
        else if (tokenizer.GetToken() == Token::MINUS)
        {
            tokenizer.GetNext();
            result -= Term();
        }
        else
        {
            break;
        }
    }
    return result;
}

bool Parser::Relation()
{
    int val1 = Expression();
    if (tokenizer.GetToken() == Token::RELOP)
    {
        RelOp relOp = tokenizer.GetRelOp();
        tokenizer.GetNext();
        int val2 = Expression();
        if (relOp == RelOp::EQUAL)
        {
            return val1 == val2;
        }
        else if (relOp == RelOp::NOTEQUAL)
        {
            return val1 != val2;
        }
        else if (relOp == RelOp::LESS)
        {
            return val1 < val2;
        }
        else if (relOp == RelOp::LESSEQUAL)
        {
            return val1 <= val2;
        }
        else if (relOp == RelOp::GREATER)
        {
            return val1 > val2;
        }
        else if (relOp == RelOp::GREATEREQUAL)
        {
            return val1 >= val2;
        }
    }
    else
    {
        SyntaxError(0);
    }
    return false;
}

void Parser::Assignment()
{
    if (tokenizer.GetToken() == Token::LET)
    {
        Designator();
        if (tokenizer.GetToken() == Token::ASSIGN)
        {
            Expression();
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
        if (tokenizer.GetToken() == Token::RIGHTCURLBRACKET)
        {
            StatSequence();
        }
    }
}

void Parser::Computation()
{
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
