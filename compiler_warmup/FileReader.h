#pragma once
#include <string>

using std::string;

struct FileReader
{
public:
    FileReader() {}

    FileReader(const string& inputString) : m_InputStr(inputString), m_Pos(0) {}

    char Symbol() const
    {
        return m_InputStr[m_Pos];
    }

    void GetNext()
    {
        m_Pos++;
    }

private:
    string m_InputStr{ "" };

    int m_Pos{ 0 };
};