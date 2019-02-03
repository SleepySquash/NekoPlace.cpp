//
//  Base.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 25/01/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef Base_hpp
#define Base_hpp

#include <iostream>
#include "../Engine/NovelSomeScript.hpp"

//FileExists
#include <codecvt>
#include <sys/stat.h>

namespace ns
{
    namespace base
    {
        std::string utf8(const std::wstring& str);
        std::wstring utf16(const std::string& str);
        
        std::wstring GetCurrentWorkingDir();
        bool FileExists(const std::wstring& path);
        bool DoesFileExistWithResolutionClass(const std::wstring& currentPath);
        bool CreateDirectory(const std::wstring& path);
        
        std::wstring GetFolderPath(const std::wstring& path);
        std::wstring GetExtentionFromString(std::wstring filename);
        std::wstring GetStringWithNoExtention(const std::wstring& filename, const std::wstring& extention);
        
        size_t GetLengthWONewLinesAndSpaces(const std::wstring& string);
        
        int atoi(const std::wstring& stringValue);
        float atof(const std::wstring& stringValue);
        bool atob(const std::wstring& stringValue);
    }
}

using ns::base::utf8;
using ns::base::utf16;

#endif /* Base_hpp */
