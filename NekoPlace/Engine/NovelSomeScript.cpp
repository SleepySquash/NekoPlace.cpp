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
    
    
    
    
    
    
    void CommandSettings::Command(std::wstring line)
    {
        //this->line = ns::base::LowercaseTheString(line);
        this->line = line;
        lastPos = 0;
    }
    void CommandSettings::Step() { lastPos++; }
    
    bool Command(CommandSettings& results, const std::wstring& command)
    {
        bool ReallyFound{ false };
        
        SkipSpaces(results);
        std::wstring commandLine = results.line;//results.lowercaseCommand ? ns::base::LowercaseTheString(results.line) : results.line;
        if (command.length() <= commandLine.length())
        {
            bool Found{ true };
            int i = results.keepTheLastPos ? results.lastPos : 0;
            for (int j = 0; Found && j < command.length(); i++, j++)
                Found = (commandLine[i] == command[j]);
            
            if (Found)
                results.lastPos = i;
            
            ReallyFound = Found;
        }
        
        return ReallyFound;
    }
    
    
    
    void SkipSpaces(CommandSettings& results)
    {
        while (results.lastPos < results.line.length() && (results.line[results.lastPos] == L' ' || results.line[results.lastPos] == L'\t'))
            results.lastPos++;
    }
    std::wstring ParseUntil(CommandSettings& results, const wchar_t until)
    {
        unsigned int pos{ results.lastPos };
        std::wstring text = L"";
        bool Found{ false };
        
        while (!Found && pos < results.line.length())
        {
            if (!(Found = (results.line[pos] == until)))
            {
                if (results.line[pos] != 13)
                    text += results.line[pos];
                pos++;
            }
        }
        results.lastPos = pos + 1;
        
        return text;
    }
    std::wstring ParseWhile(CommandSettings& results, const wchar_t until)
    {
        unsigned int pos{ results.lastPos };
        std::wstring text = L""; bool Found{ true };
        while (Found && pos < results.line.length())
            if ((Found = (results.line[pos] == until))) {
                if (results.line[pos] != 13) text += results.line[pos]; pos++; }
        return text;
    }
    
    
    
    std::wstring ParseAsQuoteString(CommandSettings& results)
    {
        SkipSpaces(results);
        
        unsigned int pos{ results.lastPos };
        std::wstring text = L"";
        bool QuotesInline{ false }, Found{ false };
        
        while (!Found && pos < results.line.length())
        {
            Found = (results.line[pos] == '"');
            
            if (!Found)
                text += results.line[pos];
            
            if (!QuotesInline && Found)
            {
                QuotesInline = true;
                Found = false;
            }
            
            pos++;
        }
        if (Found)
        {
            results.lastPos = pos;
            return text;
        }
        else
            return L"";
    }
    std::wstring ParseAsMaybeQuoteString(CommandSettings& results)
    {
        if (results.line[results.lastPos] == L'"') return nss::ParseAsQuoteString(results);
        else return nss::ParseUntil(results, L' ');
    }
    
    
    
    std::wstring ParseArgument(CommandSettings& results)
    {
        unsigned int pos{ results.lastPos };
        std::wstring text = L"";
        bool Found{ false };
        bool insideQuotes{ false };
        
        while (!Found && pos < results.line.length())
        {
            if (results.line[pos] == '"')
                insideQuotes = !insideQuotes;
            if (!(Found = (results.line[pos] == ' ' && !insideQuotes)))
            {
                if (results.line[pos] != 13 && results.line[pos] != ',')
                    text += results.line[pos];
                pos++;
            }
        }
        results.lastPos = pos + 1;
        
        return text;
    }
    void ParseArguments(CommandSettings& results, std::vector<std::wstring>& vec)
    {
        while (results.lastPos+1 < results.line.length())
        {
            SkipSpaces(results);
            std::wstring argument = nss::ParseArgument(results);
            if (argument != L"with" && argument != L"as")
                vec.push_back(argument);
        }
    }
    std::wstring ArgumentAsString(CommandSettings& results)
    {
        return nss::ParseUntil(results, '\0');
        /*std::wstring parsedUntil = nss::ParseUntil(results, '\0');
        std::wstring stringValue = ns::base::LowercaseTheString(parsedUntil);
        return stringValue;*/
    }
    std::wstring ArgumentAsStringWOLowerCase(CommandSettings& results)
    {
        std::wstring stringValue = nss::ParseUntil(results, '\0');
        return stringValue;
    }
    bool ArgumentAsBool(CommandSettings& results)
    {
        std::wstring stringValue = nss::ArgumentAsString(results);
        if (stringValue == L"true" || stringValue == L"1")
            return true;
        else
            return false;
    }
    float ArgumentAsFloat(CommandSettings& results)
    {
        std::wstring stringValue = nss::ParseUntil(results, '\0');
        std::string parsingString = "";
        for (int i = 0; stringValue[i] != '\0'; i++)
            if (stringValue[i] == 46 || (stringValue[i] >= 48 && stringValue[i] <= 57))
                parsingString += (char)stringValue[i];
        
        return std::atof(parsingString.c_str());
    }
    int ArgumentAsInt(CommandSettings& results)
    {
        std::wstring stringValue = nss::ParseUntil(results, '\0');
        std::string parsingString = "";
        for (int i = 0; stringValue[i] != '\0'; i++)
            if (stringValue[i] >= 48 && stringValue[i] <= 57)
                parsingString += (char)stringValue[i];
        
        return std::atoi(parsingString.c_str());
    }
    
    
    
    
    std::wstring GetStringWithLineBreaks(sf::Text& text, const std::wstring& line, unsigned int width, int shift)
    {
        sf::Text tempText = text;
        tempText.setString(line);
        if (tempText.getLocalBounds().width >= width)
        {
            std::wstring finalLine = L"";
            std::wstring currentLine = L"";
            bool shiftShifted{ false };
            for (int i = 0; i < line.length(); i++)
            {
                currentLine += line[i];
                tempText.setString(currentLine);
                if (tempText.getLocalBounds().width >= width)
                {
                    bool spaceFound{ false };
                    for (int j = currentLine.length() - 1; j >= 0 && !spaceFound; --j)
                    {
                        if ((spaceFound = (currentLine[j] == L' ')))
                        {
                            spaceFound = false;
                            for (int k = j - 1; k >= 0 && !spaceFound; --k)
                                if (currentLine[k] != L' ')
                                    spaceFound = true;
                            
                            if (spaceFound)
                            {
                                std::wstring toFinalLine;
                                for (int k = 0; k <= j; k++)
                                    toFinalLine += currentLine[k];
                                toFinalLine += L'\n';
                                finalLine += toFinalLine;
                                
                                if (shift != 0 && !shiftShifted)
                                    width -= shiftShifted, shiftShifted = true;
                                std::wstring newCurrentLine = L"";
                                if (shift != 0 && shift < width/4)
                                {
                                    tempText.setString(L"");
                                    while (tempText.getLocalBounds().width < shift)
                                        newCurrentLine += L' ', tempText.setString(newCurrentLine);
                                }
                                for (int k = j + 1; k < currentLine.length(); k++)
                                    newCurrentLine += currentLine[k];
                                
                                currentLine = newCurrentLine;
                            }
                        }
                    }
                    if (!spaceFound)
                    {
                        currentLine[currentLine.length() - 1] = L'\n';
                        finalLine += currentLine;
                        
                        if (shift != 0 && !shiftShifted)
                            width -= shiftShifted, shiftShifted = true;
                        if (shift != 0 && shift < width/10)
                        {
                            tempText.setString(L"");
                            while (tempText.getLocalBounds().width < shift)
                                currentLine += L' ', tempText.setString(currentLine);
                        }
                        currentLine = L"";
                        
                        currentLine += line[i];
                    }
                }
            }
            finalLine += currentLine;
            return finalLine;
        }
        return std::wstring(line);
    }
}
