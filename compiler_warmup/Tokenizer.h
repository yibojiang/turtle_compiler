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
    COMMA,
    IDENT,
    LEFTCURLBRACKET,
    RIGHTCURLBRACKET
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
    Tokenizer(const string& inputString);

    bool IsDigit(char ch) const;

    int Digit() const;

    bool IsLetter(char ch) const;

    char Letter() const;

    int Number();

    string Identifier();

    Token GetToken();

    void GetNext();

    int GetNumber() const;

    int GetIdentifier() const;

    RelOp GetRelOp();

    string Id2String(int id);

    int String2Id(const string& str);

private:
    unordered_map<string, int> m_IdentifierTable{};

    int m_Number;

    int m_Identifier;

    RelOp m_RelOps;

    Token m_Token;

    FileReader reader{};
};
