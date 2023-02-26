#pragma once
#include <string>

#include "Tokenizer.h"

using std::string;
using std::cout;
using std::endl;

struct Parser
{
    Parser(const string& inputStr) : tokenizer(inputStr) {}

    int GetIdentifierValue(int id) const
    {
        auto itr = m_VariableTable.find(id);
        if (itr == m_VariableTable.end())
        {
            SyntaxError(0);
        }
        return itr->second;
    }

    void Designator()
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

    int Factor()
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

    int Term()
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

    int Expression()
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

    bool Relation()
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

    void Assignment()
    {
        tokenizer.GetNext();
        if (tokenizer.GetToken() == Token::LET)
        {
            Designator();
            if (tokenizer.GetToken() == Token::ASSIGN)
            {
                Expression();
            }
        }
    }

    void Computation()
    {
        tokenizer.GetNext();
        if (tokenizer.GetToken() == Token::COMPUTATION)
        {
            tokenizer.GetNext();

            // Assignment expression
            while (tokenizer.GetToken() == Token::VAR)
            {
                tokenizer.GetNext();
                if (tokenizer.GetToken() == Token::IDENTIFIER)
                {
                    int id = tokenizer.GetIdentifier();
                    tokenizer.GetNext();
                    if (tokenizer.GetToken() == Token::ASSIGN)
                    {
                        tokenizer.GetNext();
                        int val = Expression();

                        // Update the variable table
                        m_VariableTable[id] = val;

                        if (tokenizer.GetToken() == Token::SIMICOLON)
                        {
                            tokenizer.GetNext();
                        }
                        else
                        {
                            SyntaxError(0);
                            return;
                        }
                    }
                    else
                    {
                        SyntaxError(0);
                        return;
                    }
                }
                else
                {
                    SyntaxError(0);
                    return;
                }
            }

            cout << Expression() << endl;
            while (tokenizer.GetToken() == Token::SIMICOLON)
            {
                tokenizer.GetNext();
                cout << Expression() << endl;
            }

            if (tokenizer.GetToken() == Token::DOT)
            {
                return;
            }
            else
            {
                SyntaxError(0);
            }
        }
        else
        {
            SyntaxError(0);
        }
    }

    void SyntaxError(int errorCode) const
    {
        __debugbreak();
        cout << "syntax error" << errorCode << endl;
    }

    Tokenizer tokenizer;

    unordered_map<int, int> m_VariableTable;
};
