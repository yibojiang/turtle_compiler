#include <iostream>
#include <unordered_map>
#include <string>

enum Token
{
    End,
    Var,
    Computation,
    Identifier,
    Number,
    Plus,
    Minus,
    Mul,
    Div,
    LeftParen,
    RightParen,
    Assign,
    Comma,
    Period,
};

enum ReservedWord
{
    VAR = 0,
    COMPUTATION = 1
};

struct FileReader
{
public:
    FileReader() {}

    FileReader(std::string inputString) : m_InputStr(inputString), m_Pos(0) {}

    char Symbol() const
    {
        return m_InputStr[m_Pos];
    }

    void GetNext()
    {
        m_Pos++;
    }

private:
    std::string m_InputStr{ "" };

    int m_Pos{ 0 };
};

struct Tokenizer
{
public:
    Tokenizer(std::string inputString) : reader { inputString }
    {
        m_IdentifierTable["var"] = ReservedWord::VAR;
        m_IdentifierTable["computation"] = ReservedWord::COMPUTATION;
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

    std::string Identifier()
    {
        std::string result{""};
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
                std::string identifier = Identifier();
                auto itr = m_IdentifierTable.find(identifier);
                if (itr != m_IdentifierTable.end())
                {
                    m_Identifier = itr->second;
                    if (m_Identifier == ReservedWord::VAR)
                    {
                        m_Token = Token::Var;
                        return;
                    }

                    if (m_Identifier == ReservedWord::COMPUTATION)
                    {
                        m_Token = Token::Computation;
                        return;
                    }
                }
                else
                {
                    m_IdentifierTable[identifier] = m_IdentifierTable.size();
                    m_Identifier = m_IdentifierTable[identifier];
                }

                m_Token = Token::Identifier;
                return;
            }
            else if (IsDigit(reader.Symbol()))
            {
                m_Number = Number();
                m_Token = Token::Number;
                return;
            }
            else if (reader.Symbol() == ';')
            {
                reader.GetNext();
                m_Token = Token::Comma;
                return;
            }
            else if (reader.Symbol() == '(')
            {
                reader.GetNext();
                m_Token = Token::LeftParen;
                return;
            }
            else if (reader.Symbol() == ')')
            {
                reader.GetNext();
                m_Token = Token::RightParen;
                return;
            }
            else if (reader.Symbol() == '+')
            {
                reader.GetNext();
                m_Token = Token::Plus;
                return;
            }
            else if (reader.Symbol() == '-')
            {
                reader.GetNext();
                m_Token = Token::Minus;
                return;
            }
            else if (reader.Symbol() == '*')
            {
                reader.GetNext();
                m_Token = Token::Mul;
                return;
            }
            else if (reader.Symbol() == '/')
            {
                reader.GetNext();
                m_Token = Token::Div;
                return;
            }
            else if (reader.Symbol() == '.')
            {
                reader.GetNext();
                m_Token = Token::Period;
                return;
            }
            else if (reader.Symbol() == '<')
            {
                reader.GetNext();
                if (reader.Symbol() == '-')
                {
                    reader.GetNext();
                    m_Token = Token::Assign;
                    return;
                }
            }
            else if (reader.Symbol() == '\0')
            {
                m_Token = Token::End;
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

private:
    std::unordered_map<std::string, int> m_IdentifierTable {};

    int m_Number;

    int m_Identifier;

    Token m_Token;

    FileReader reader{};
};


struct Parser
{
    Parser(std::string inputStr) : tokenizer(inputStr) {}

    int Term()
    {
        int result = Factor();
        while (true)
        {
            if (tokenizer.GetToken() == Token::Mul)
            {
                tokenizer.GetNext();
                result *= Factor();
            }
            else if (tokenizer.GetToken() == Token::Div)
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

    int GetIdentifierValue(int id) const
    {
        auto itr = m_VariableTable.find(id);
        if (itr == m_VariableTable.end())
        {
            SyntaxError(3);
        }
        return itr->second;
    }

    int Expression()
    {
        int result = Term();
        while (true)
        {
            if (tokenizer.GetToken() == Token::Plus)
            {
                tokenizer.GetNext();
                result += Term();
            }
            else if (tokenizer.GetToken() == Token::Minus)
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

    int Factor()
    {
        if (tokenizer.GetToken() == Token::Number)
        {
            int number = tokenizer.GetNumber();
            tokenizer.GetNext();
            return number;
        }
        else if (tokenizer.GetToken() == Token::Identifier)
        {
            int identifier = tokenizer.GetIdentifier();
            int val = GetIdentifierValue(identifier);
            tokenizer.GetNext();
            return val;
        }
        else if (tokenizer.GetToken() == Token::LeftParen)
        {
            tokenizer.GetNext();
            int val = Expression();

            if (tokenizer.GetToken() == Token::RightParen)
            {
                tokenizer.GetNext();
                return val;
            }
            
            SyntaxError(0);
            return 0;
        }
        return 0;
    }

    void Computation()
    {
        tokenizer.GetNext();
        if (tokenizer.GetToken() == Token::Computation)
        {
            tokenizer.GetNext();
            
            // Assignment expression
            while (tokenizer.GetToken() == Token::Var)
            {
                tokenizer.GetNext();
                if (tokenizer.GetToken() == Token::Identifier)
                {
                    int id = tokenizer.GetIdentifier();
                    tokenizer.GetNext();
                    if (tokenizer.GetToken() == Token::Assign)
                    {
                        tokenizer.GetNext();
                        int val = Expression();

                        // Update the variable table
                        m_VariableTable[id] = val;

                        if (tokenizer.GetToken() == Token::Comma)
                        {
                            tokenizer.GetNext();
                        }
                        else
                        {
                            SyntaxError(1);
                            return;
                        }
                    }
                    else
                    {
                        SyntaxError(1);
                        return;
                    }
                }
                else
                {
                    SyntaxError(1);
                    return;
                }
            }

            std::cout << Expression() << std::endl;
            while (tokenizer.GetToken() == Token::Comma)
            {
                tokenizer.GetNext();
                std::cout << Expression() << std::endl;
            }

            if (tokenizer.GetToken() == Token::Period)
            {
                return;
            }
            else
            {
                SyntaxError(2);
            }
        }
        else
        {
            SyntaxError(3);
        }
    }

    void SyntaxError(int errorCode) const
    {
        std::cout << "syntax error" << errorCode << std::endl;
    }

    Tokenizer tokenizer;

    std::unordered_map<int, int> m_VariableTable;
};

int main()
{
    // std::string inputStr = "computation var i <- 2 * 3; var abracadabra <- 7;(((abracadabra * i)));i - 5 - 1.";
    std::string inputStr = "computation var i <- 2 * (-3) + 1; var abracadabra <- 7;(((abracadabra * i))) + 10;i - 5 - 1.";
    Parser parser(inputStr);
    parser.Computation();
    Parser pasrser(inputStr);

    return 1;
}
