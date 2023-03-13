#include "Tokenizer.h"

Tokenizer::Tokenizer(const string& inputString): reader{ inputString }
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

bool Tokenizer::IsDigit(char ch) const
{
    return ch >= '0' && ch <= '9';
}

int Tokenizer::Digit() const
{
    char ch = reader.Symbol();
    return  ch - '0';
}

bool Tokenizer::IsLetter(char ch) const
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

char Tokenizer::Letter() const
{
    return reader.Symbol();
}

int Tokenizer::Number()
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

string Tokenizer::Identifier()
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

Token Tokenizer::GetToken()
{
    return m_Token;
}

void Tokenizer::GetNext()
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

                if (m_Identifier == ReservedWord::LET)
                {
                    m_Token = Token::LET;
                    return;
                }

                if (m_Identifier == ReservedWord::CALL)
                {
                    m_Token = Token::CALL;
                    return;
                }

                if (m_Identifier == ReservedWord::IF)
                {
                    m_Token = Token::IF;
                    return;
                }
                
                if (m_Identifier == ReservedWord::THEN)
                {
                    m_Token = Token::THEN;
                    return;
                }
                if (m_Identifier == ReservedWord::ELSE)
                {
                    m_Token = Token::ELSE;
                    return;
                }
                if (m_Identifier == ReservedWord::FI)
                {
                    m_Token = Token::FI;
                    return;
                }
                if (m_Identifier == ReservedWord::WHILE)
                {
                    m_Token = Token::WHILE;
                    return;
                }
                if (m_Identifier == ReservedWord::DO)
                {
                    m_Token = Token::DO;
                    return;
                }
                if (m_Identifier == ReservedWord::OD)
                {
                    m_Token = Token::OD;
                    return;
                }
                if (m_Identifier == ReservedWord::RETURN)
                {
                    m_Token = Token::RETURN;
                    return;
                }
                if (m_Identifier == ReservedWord::ARRAY)
                {
                    m_Token = Token::ARRAY;
                    return;
                }
                if (m_Identifier == ReservedWord::VOID)
                {
                    m_Token = Token::VOID;
                    return;
                }
                if (m_Identifier == ReservedWord::FUNCTION)
                {
                    m_Token = Token::FUNCTION;
                    return;
                }
                if (m_Identifier == ReservedWord::MAIN)
                {
                    m_Token = Token::MAIN;
                    return;
                }
            }
            else
            {
                // TODO: Variable would be evaluated as an IR
                m_IdentifierTable[identifier] = static_cast<int>(m_IdentifierTable.size());
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
        else if (reader.Symbol() == '[')
        {
            reader.GetNext();
            m_Token = Token::LEFTBRACKET;
            return;
        }
        else if (reader.Symbol() == ']')
        {
            reader.GetNext();
            m_Token = Token::RIGHTBRACKET;
            return;
        }
        else if (reader.Symbol() == '{')
        {
            reader.GetNext();
            m_Token = Token::LEFTCURLBRACKET;
            return;
        }
        else if (reader.Symbol() == '}')
        {
            reader.GetNext();
            m_Token = Token::RIGHTCURLBRACKET;
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
        else if (reader.Symbol() == ',')
        {
            reader.GetNext();
            m_Token = Token::COMMA;
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

int Tokenizer::GetNumber() const
{
    return m_Number;
}

int Tokenizer::GetIdentifier() const
{
    return m_Identifier;
}

RelOp Tokenizer::GetRelOp()
{
    return m_RelOps;
}

string Tokenizer::Id2String(int id)
{
    // TODO
    return "";
}

int Tokenizer::String2Id(const string& str)
{
    // TODO
    return 0;
}
