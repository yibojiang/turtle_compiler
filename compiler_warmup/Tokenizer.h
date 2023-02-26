#pragma once
#include <string>
#include <unordered_map>

#include "FileReader.h"

using std::string;
using std::unordered_map;

enum class Token
{
    END,
    VAR,
    COMPUTATION,
    IDENTIFIER,
    NUMBER,
    PLUS,
    MINUS,
    MUL,
    DIV,
    LEFTPAREN,
    RIGHTPAREN,
    LEFTBRACKET,
    RIGHTBRACKET,
    ASSIGN,
    SIMICOLON,
    DOT,
    RELOP,
    LET,
    CALL,
    IF,
    THEN,
    ELSE,
    FI,
    WHILE,
    DO,
    OD,
    RETURN,
    ARRAY,
    VOID,
    FUNCTION,
    MAIN,
};

enum ReservedWord
{
    VAR = 0,
    COMPUTATION = 1,
    LET = 2,
    CALL = 3,
    IF = 4,
    THEN = 5,
    ELSE = 6,
    FI = 7,
    WHILE = 8,
    DO = 9,
    OD = 10,
    RETURN = 11,
    ARRAY = 12,
    VOID = 13,
    FUNCTION = 14,
    MAIN = 15,
};

enum class RelOp
{
    EQUAL,
    NOTEQUAL,
    LESS,
    LESSEQUAL,
    GREATER,
    GREATEREQUAL
};

struct Tokenizer
{
public:
    Tokenizer(const string& inputString) : reader{ inputString }
    {
        m_IdentifierTable["var"] = ReservedWord::VAR;
        m_IdentifierTable["computation"] = ReservedWord::COMPUTATION;
        m_IdentifierTable["let"] = ReservedWord::LET;
        m_IdentifierTable["call"] = ReservedWord::CALL;
        m_IdentifierTable["if"] = ReservedWord::IF;
        m_IdentifierTable["then"] = ReservedWord::THEN;
        m_IdentifierTable["else"] = ReservedWord::ELSE;
        m_IdentifierTable["fi"] = ReservedWord::FI;
        m_IdentifierTable["while"] = ReservedWord::WHILE;
        m_IdentifierTable["do"] = ReservedWord::DO;
        m_IdentifierTable["od"] = ReservedWord::OD;
        m_IdentifierTable["return"] = ReservedWord::RETURN;
        m_IdentifierTable["array"] = ReservedWord::ARRAY;
        m_IdentifierTable["void"] = ReservedWord::VOID;
        m_IdentifierTable["function"] = ReservedWord::FUNCTION;
        m_IdentifierTable["main"] = ReservedWord::MAIN;
    }

    bool IsDigit(char ch) const
    {
        return ch >= '0' && ch <= '9';
    }

    int Digit() const
    {
        char ch = reader.Symbol();
        return  ch - '0';
    }

    bool IsLetter(char ch) const
    {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
    }

    char Letter() const
    {
        return reader.Symbol();
    }

    int Number()
    {
        int result = Digit();
        reader.GetNext();
        while (IsDigit(reader.Symbol()))
        {
            result = result * 10 + Digit();
            reader.GetNext();
        }

        return result;
    }

    string Identifier()
    {
        string result{ "" };
        result = Letter();
        reader.GetNext();
        while (true)
        {
            char ch = reader.Symbol();
            if (IsLetter(ch))
            {
                result += Letter();
                reader.GetNext();
            }
            else if (IsDigit(ch))
            {
                result += ch;
                reader.GetNext();
            }
            else
            {
                break;
            }
        }
        return result;
    }

    Token GetToken()
    {
        return m_Token;
    }

    void GetNext()
    {
        while (true)
        {
            if (IsLetter(reader.Symbol()))
            {
                string identifier = Identifier();
                auto itr = m_IdentifierTable.find(identifier);
                if (itr != m_IdentifierTable.end())
                {
                    m_Identifier = itr->second;
                    if (m_Identifier == ReservedWord::VAR)
                    {
                        m_Token = Token::VAR;
                        return;
                    }

                    if (m_Identifier == ReservedWord::COMPUTATION)
                    {
                        m_Token = Token::COMPUTATION;
                        return;
                    }
                }
                else
                {
                    m_IdentifierTable[identifier] = m_IdentifierTable.size();
                    m_Identifier = m_IdentifierTable[identifier];
                }

                m_Token = Token::IDENTIFIER;
                return;
            }
            else if (IsDigit(reader.Symbol()))
            {
                m_Number = Number();
                m_Token = Token::NUMBER;
                return;
            }
            else if (reader.Symbol() == ';')
            {
                reader.GetNext();
                m_Token = Token::SIMICOLON;
                return;
            }
            else if (reader.Symbol() == '(')
            {
                reader.GetNext();
                m_Token = Token::LEFTPAREN;
                return;
            }
            else if (reader.Symbol() == ')')
            {
                reader.GetNext();
                m_Token = Token::RIGHTPAREN;
                return;
            }
            else if (reader.Symbol() == '+')
            {
                reader.GetNext();
                m_Token = Token::PLUS;
                return;
            }
            else if (reader.Symbol() == '-')
            {
                reader.GetNext();
                m_Token = Token::MINUS;
                return;
            }
            else if (reader.Symbol() == '*')
            {
                reader.GetNext();
                m_Token = Token::MUL;
                return;
            }
            else if (reader.Symbol() == '/')
            {
                reader.GetNext();
                m_Token = Token::DIV;
                return;
            }
            else if (reader.Symbol() == '.')
            {
                reader.GetNext();
                m_Token = Token::DOT;
                return;
            }
            else if (reader.Symbol() == '<')
            {
                reader.GetNext();
                if (reader.Symbol() == '-')
                {
                    reader.GetNext();
                    m_Token = Token::ASSIGN;
                    return;
                }
                else if (reader.Symbol() == '=')
                {
                    reader.GetNext();
                    m_Token = Token::RELOP;
                    m_RelOps = RelOp::LESSEQUAL;
                    return;
                }
                else
                {
                    m_Token = Token::RELOP;
                    m_RelOps = RelOp::LESS;
                    return;
                }
            }
            else if (reader.Symbol() == '>')
            {
                reader.GetNext();
                if (reader.Symbol() == '=')
                {
                    reader.GetNext();
                    m_Token = Token::RELOP;
                    m_RelOps = RelOp::GREATEREQUAL;
                    return;
                }
                else
                {
                    m_Token = Token::RELOP;
                    m_RelOps = RelOp::GREATER;
                    return;
                }
            }
            else if (reader.Symbol() == '!')
            {
                reader.GetNext();
                if (reader.Symbol() == '=')
                {
                    m_Token = Token::RELOP;
                    m_RelOps = RelOp::NOTEQUAL;
                    return;
                }
            }
            else if (reader.Symbol() == '=')
            {
                reader.GetNext();
                if (reader.Symbol() == '=')
                {
                    m_Token = Token::RELOP;
                    m_RelOps = RelOp::EQUAL;
                    return;
                }
            }
            else if (reader.Symbol() == '\0')
            {
                m_Token = Token::END;
                return;
            }
            else
            {
                reader.GetNext();
            }
        }
    }

    int GetNumber() const
    {
        return m_Number;
    }

    int GetIdentifier() const
    {
        return m_Identifier;
    }

    RelOp GetRelOp()
    {
        return m_RelOps;
    }

private:
    unordered_map<string, int> m_IdentifierTable{};

    int m_Number;

    int m_Identifier;

    RelOp m_RelOps;

    Token m_Token;

    FileReader reader{};
};
