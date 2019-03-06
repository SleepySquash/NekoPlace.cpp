//
//  NovelSomeScript.hpp
//  NovelSome
//
//  Created by Никита Исаенко on 31/08/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#ifndef NovelSomeScript_hpp
#define NovelSomeScript_hpp

#include <iostream>
#include <string>
#include <vector>

//nss::SetStringWithLineBreaks
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>

#include "../Essentials/Base.hpp"

//for mathparser, stack functionality
#include "List.hpp"

using std::cout;
using std::cin;
using std::endl;
using namespace ns;

namespace nss
{
    bool Command(const std::wstring& line, const std::wstring& command);
    bool Command(const std::string& line, const std::string& command);
    
    std::wstring ParseUntil(const std::wstring& line, const wchar_t until, unsigned int pos = 0);
    std::string ParseUntil(const std::string& line, const char until, unsigned int pos = 0);
    
    bool ContainsUsefulInformation(const std::wstring& wstr);
    
    float MathParser(const std::wstring& finalLine);
    
    
    
    struct CommandSettings
    {
        std::wstring line{ L"" };
        unsigned int lastPos{ 0 };
        bool keepTheLastPos{ true };
        bool lowercaseCommand{ true };
        
        void Command(std::wstring line);
        void Step();
    };
    
    bool Command(CommandSettings& results, const std::wstring& command);
    
    void SkipSpaces(CommandSettings& results);
    std::wstring ParseUntil(CommandSettings& results, const wchar_t until);
    std::wstring ParseWhile(CommandSettings& results, const wchar_t until);
    
    std::wstring ParseAsQuoteString(CommandSettings& results);
    std::wstring ParseAsMaybeQuoteString(CommandSettings& results);
    
    std::wstring ParseArgument(CommandSettings& results);
    void ParseArguments(CommandSettings& results, std::vector<std::wstring>& vec);
    std::wstring ArgumentAsString(CommandSettings& results);
    std::wstring ArgumentAsStringWOLowerCase(CommandSettings& results);
    bool ArgumentAsBool(CommandSettings& results);
    float ArgumentAsFloat(CommandSettings& results);
    int ArgumentAsInt(CommandSettings& results);
    
    std::wstring GetStringWithLineBreaks(sf::Text& text, const std::wstring& line, unsigned int width, int shift = 0);
}

#endif /* NovelSomeScript_hpp */
