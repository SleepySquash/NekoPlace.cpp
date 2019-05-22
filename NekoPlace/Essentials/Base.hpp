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
#ifdef SFML_SYSTEM_ANDROID
    #include <stdio.h>
    #include "ResourcePath.hpp"

    #include <jni.h>
    #include <android/native_activity.h>
    #include <android/asset_manager.h>
    #include <android/asset_manager_jni.h>
    #include <android/log.h>
    #define LOGE(...) ((void)__android_log_print(ANDROID_LOG_INFO, "sfml-activity", __VA_ARGS__))
    #include <SFML/System/NativeActivity.hpp>
#endif

namespace ns
{
    namespace base
    {
        std::string utf8(const std::wstring& str);
        std::wstring utf16(const std::string& str);
        std::string utf8(const wchar_t& wchar);
        std::wstring utf16(const char& wchar);
        
        std::wstring GetCurrentWorkingDir();
        bool FileExists(const std::wstring& path);
        bool CreateDirectory(const std::wstring& path);
        
        bool DoesFileExistWithResolutionClass(const std::wstring& currentPath);
        
        std::wstring GetFolderPath(const std::wstring& path);
        std::wstring GetExtentionFromString(const std::wstring& filename);
        std::wstring GetStringWithNoExtention(const std::wstring& filename);
        
        size_t GetLengthWONewLinesAndSpaces(const std::wstring& string);
        
        int atoi(const std::wstring& stringValue);
        float atof(const std::wstring& stringValue);
        bool atob(const std::wstring& stringValue);
    }
}

using ns::base::utf8;
using ns::base::utf16;

#endif /* Base_hpp */
