//
//  Base.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 25/01/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "Base.hpp"

namespace ns
{
    namespace base
    {
        std::string utf8(const std::wstring& str)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
            return converter.to_bytes(str);
        }
        std::wstring utf16(const std::string& str)
        {
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
            return converter.from_bytes(str);
        }
        
#ifdef _WIN32
#include <direct.h>
        std::wstring GetCurrentWorkingDir(void)
        {
            wchar_t buff[FILENAME_MAX];
            _wgetcwd(buff, FILENAME_MAX);
            std::wstring current_working_dir(buff);
            return current_working_dir + L'\\';
        }
#else
#include <unistd.h>
        std::wstring GetCurrentWorkingDir(void)
        {
            char buff[FILENAME_MAX];
            getcwd(buff, FILENAME_MAX);
            std::string current_working_dir(buff);
            
            //TODO: change the way it converts
            std::wstring wcurrent_working_dir(current_working_dir.begin(), current_working_dir.end());
            
            return wcurrent_working_dir + L'\\';
        }
#endif
        
        
        
        bool FileExists(const std::wstring& path)
        {
#ifdef _WIN32
            return _waccess_s(path.c_str(), 0) == 0;
#else
            struct stat buffer;
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
            std::string u8str = converter.to_bytes(path);
            return (stat (u8str.c_str(), &buffer) == 0);
#endif
        }
        bool DoesFileExistWithResolutionClass(const std::wstring& currentPath)
        {
            if (!FileExists(currentPath))
            {
                std::wstring getExtention = GetExtentionFromString(currentPath);
                std::wstring woExtention = GetStringWithNoExtention(currentPath, getExtention);
                
                if (FileExists(woExtention + L"@3x" + getExtention))
                    return true;
                else if (FileExists(woExtention + L"@2x" + getExtention))
                    return true;
                else if (FileExists(woExtention + L"@1x" + getExtention))
                    return true;
                else if (FileExists(woExtention + L"@0x" + getExtention))
                    return true;
            }
            else
                return true;
            
            return false;
        }
        
        
        
        std::wstring GetFolderPath(const std::wstring& path)
        {
            std::wstring folder = L"";
            
            int pos{ -1 };
            if (path.length() != 0)
                for (unsigned int i = path.length() - 1; i >= 0 && pos == -1; i--)
                    if (path[i] == '/' || path[i] == '\\')
                        pos = i;
            
            for (int i = 0; i <= pos; i++)
                folder += path[i];
            
            return folder;
        }
        std::wstring GetExtentionFromString(std::wstring filename)
        {
            for (int i = filename.size(); i >= 0; --i)
                if (filename[i] == L'.')
                {
                    if (i != filename.size())
                        filename.erase(0,i);
                    return filename;
                }
            return L"";
        }
        std::wstring GetStringWithNoExtention(const std::wstring& filename, const std::wstring& extention)
        {
            std::wstring returned = L"";
            if (extention.length() != 0)
                for (int i = 0; i < filename.length() - extention.length(); i++)
                    returned += filename[i];
            if (returned != L"")
                return returned;
            return filename;
        }
    }
}

namespace nss
{
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
    
    std::string ParseUntil(std::string line, const char until, unsigned int from)
    {
        unsigned int pos{ from };
        std::string text = "";
        bool Found{ false };
        
        while (!Found && pos < line.length())
        {
            if (!(Found = (line[pos] == until)))
            {
                if (line[pos] != 13)
                    text += line[pos];
                pos++;
            }
        }
        
        return text;
    }
}
