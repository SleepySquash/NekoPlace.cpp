//
//  NovelSomeScript.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 31/08/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#include "NovelSomeScript.hpp"

namespace nss
{
    bool Command(const std::wstring& line, const std::wstring& command)
    {
        bool ReallyFound{ false };
        
        int lastPos{ 0 };
        while (lastPos < line.length() && (line[lastPos] == L' ' || line[lastPos] == L'\t')) ++lastPos;
        std::wstring commandLine = command;//ns::base::LowercaseTheString(line);
        if (command.length() <= commandLine.length())
        {
            bool Found{ true };
            int i = lastPos;
            for (int j = 0; Found && j < command.length(); i++, j++)
                Found = (commandLine[i] == command[j]);
            ReallyFound = Found;
        }
        
        return ReallyFound;
    }
    bool Command(const std::string& line, const std::string& command)
    {
        bool ReallyFound{ false };
        
        if (command.length() <= line.length())
        {
            bool Found{ true };
            for (int i = 0; Found && i < command.length(); i++)
                Found = (line[i] == command[i]);
            
            ReallyFound = Found;
        }
        
        return ReallyFound;
    }
    
    
    
    std::wstring ParseUntil(const std::wstring& line, const wchar_t until, unsigned int pos)
    {
        std::wstring text = L"";
        bool Found{ false };
        
        while (!Found && pos < line.length())
        {
            if (!(Found = (line[pos] == until)))
            {
                if (line[pos] != 13) text += line[pos];
                ++pos;
            }
        }
        
        return text;
    }
    std::string ParseUntil(const std::string& line, const char until, unsigned int pos)
    {
        std::string text = "";
        bool Found{ false };
        
        while (!Found && pos < line.length())
        {
            if (!(Found = (line[pos] == until)))
            {
                if (line[pos] != 13) text += line[pos];
                ++pos;
            }
        }
        
        return text;
    }
    
    
    
    bool ContainsUsefulInformation(const std::wstring& wstr)
    {
        for (int i = 0; i < wstr.length(); i++)
            if (wstr[i] != L' ' && wstr[i] != L'\t' && wstr[i] != L'\0' && wstr[i] != L'\n' && wstr[i] != 13)
                return true;
        return false;
    }
    
    
    
    
    //TODO: ^ operation and support for sin, cos, max etc functions.
    float MathParser(const std::wstring& line)
    {
        bool operatorWasFound{ false };
        ns::Stack<std::wstring> output;
        ns::Stack<std::wstring> operators;
        
        size_t wordLength = 0;
        size_t wordBeg = 0;
        for (int i = 0; i <= line.length(); ++i)
        {
            if (line[i] == L'/' || line[i] == L'+' || (line[i] == L'-') || line[i] == L'*' ||
                line[i] == L'(' || line[i] == L')' || line[i] == '\0')
            {
                if (wordLength != 0)
                {
                    std::wstring wstr = L"";
                    bool notANumber{ false };
                    for (int j = wordBeg; j < i; ++j)
                    {
                        if (line[j] != '\t' && line[j] != ' ')
                        {
                            if (!((line[j] >= 48 && line[j] <= 57) || line[j] == 46))
                                notANumber = true;
                            wstr += line[j];
                        }
                    }
                    
                    if (wstr.length() != 0)
                    {
                        if (notANumber)
                        {
                            /*if (wstr == L"sin" || wstr == L"cos")
                             operators.Push(wstr);
                             else*/
                            cout << "Warning :: MathParser :: not an operator." << endl;
                        }
                        else
                        {
                            bool makeNegative = false;
                            if (operators.size == 1 && output.size == 0)
                                if (operators[0][0] == '-')
                                    makeNegative = true, operators.Pop();
                            
                            if (makeNegative)
                                wstr.insert(wstr.begin(), L'-'), makeNegative = false;
                            output.Push(wstr);
                        }
                    }
                    wordLength = 0;
                }
                wordBeg = i + 1;
                
                if (line[i] != '\0')
                {
                    std::wstring wstr = L"";
                    wstr += line[i];
                    
                    if (line[i] == L')')
                    {
                        while (!operators.IsEmpty() && operators[0] != L"(")
                            output.Push(operators.Pop());
                        
                        if (!operators.IsEmpty())
                            operators.Pop();
                    }
                    else if (line[i] != L'(')
                    {
                        operatorWasFound = true;
                        
                        bool lowerFound{ false };
                        while (!operators.IsEmpty() && !lowerFound)
                        {
                            std::wstring op = operators[0];
                            if ((line[i] == '+' && op == L"-") || (line[i] == '-' && op == L"-") ||
                                (line[i] == '+' && op == L"+") || (line[i] == '-' && op == L"+"))
                                output.Push(operators.Pop());
                            else if ((line[i] == '*' && op == L"/") || (line[i] == '/' && op == L"/") ||
                                     (line[i] == '*' && op == L"*") || (line[i] == '/' && op == L"*"))
                                output.Push(operators.Pop());
                            else if ((line[i] == '+' && op == L"*") || (line[i] == '+' && op == L"/") ||
                                     (line[i] == '-' && op == L"*") || (line[i] == '-' && op == L"/"))
                                output.Push(operators.Pop());
                            else
                                lowerFound = true;
                        }
                    }
                    
                    if (line[i] != L')')
                        operators.Push(wstr);
                }
            }
            else
                wordLength++;
        }
        
        if (!operatorWasFound)
        {
            if (output.IsEmpty())
                return 0;
            else
                return base::atof(output[0]);
        }
        
        while (!operators.IsEmpty())
            output.Push(operators.Pop());
        
        /*cout << "Shunting-yard Algorithm's output: ";
         for (int j = output.size - 1; j >= 0; --j)
         cout << ns::base::ConvertToUTF8(output[j]) << " ";
         cout << endl;*/
        
        ns::Stack<std::wstring> values;
        int pos = output.size;
        while (pos >= 0)
        {
            std::wstring token = output[pos];
            bool notANumber{ false };
            bool possibleNegative{ false };
            for (int i = 0; i < token.length(); ++i)
            {
                if (token[i] != ' ' && token[i] != '\t')
                {
                    if (token[i] == '-')
                        possibleNegative = true;
                    else if (!((token[i] >= 48 && token[i] <= 57) || token[i] == 46))
                        notANumber = true;
                    else
                        possibleNegative = false;
                }
            }
            if (possibleNegative)
                notANumber = true;
            
            if (notANumber)
            {
                if (token == L"+" || token == L"-" || token == L"*" || token == L"/")
                {
                    if (!values.IsEmpty())
                    {
                        std::wstring roperand = values.Pop();
                        if (!values.IsEmpty())
                        {
                            std::wstring loperand = values.Pop();
                            float rop = base::atof(roperand);
                            float lop = base::atof(loperand);
                            
                            float result{ 0 };
                            if (token == L"+")
                                result = lop + rop;
                            else if (token == L"-")
                                result = lop - rop;
                            else if (token == L"*")
                                result = lop * rop;
                            else if (token == L"/")
                                result = lop / rop;
                            
                            values.Push(std::to_wstring(result));
                        }
                        else
                        {
                            cout << "Error :: MathParser :: Operation can't be done: '" << base::utf8(token) << "'." << endl;
                            values.Push(roperand);
                        }
                    }
                    else
                        cout << "Error :: MathParser :: Operation can't be done: '" << base::utf8(token) << "'." << endl;
                }
                else
                    cout << "Error :: MathParser :: Operation is not supported: '" << base::utf8(token) << "'." << endl;
            }
            else
                values.Push(token);
            
            --pos;
        }
        
        if (!values.IsEmpty())
            return base::atof(values.Pop());
        
        return 0;
    }
}
