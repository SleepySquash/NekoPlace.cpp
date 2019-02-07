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
#ifdef __ANDROID__
            if (path.length() != 0)
            {
                if (path[0] == L'/') return access( utf8(path).c_str(), F_OK ) != -1;
                else {
                    LOGE("FileExists looking for: %s", (apkPath() + utf8(path)).c_str());
                    //return access( (apkPath() + utf8(path)).c_str(), F_OK ) != -1;
                    
                    ANativeActivity* activity = sf::getNativeActivity();
                    JNIEnv* lJNIEnv = NULL;
                    (activity->vm)->AttachCurrentThread(&lJNIEnv, 0);
                    
                    // Retrieve the NativeActivity
                    jobject ObjectNativeActivity = activity->clazz;
                    jclass ClassNativeActivity = lJNIEnv->GetObjectClass(ObjectNativeActivity);
                    
                    // Retrieve the ActivityInfo
                    jmethodID MethodGetAssetManager = lJNIEnv->GetMethodID(ClassNativeActivity, "getAssets", "()Landroid/content/res/AssetManager;");
                    jobject ObjectAssetManager = lJNIEnv->CallObjectMethod(ObjectNativeActivity, MethodGetAssetManager);
                    
                    AAssetManager* mgr = AAssetManager_fromJava(lJNIEnv, ObjectAssetManager);
                    AAsset* asset = AAssetManager_open(mgr, (utf8(path)).c_str(), AASSET_MODE_UNKNOWN);
                    
                    (activity->vm)->DetachCurrentThread();
                    return (asset != nullptr);
                }
            }
#endif
            
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
        /// Thanks to Maxim Suslov ( https://stackoverflow.com/users/3364871/maxim-suslov ) ///
        bool CreateDirectory(const std::wstring& path)
        {
#ifdef _WIN32
            int ret = _wmkdir(path.c_str());
#else
            std::string upath = utf8(path);
            mode_t mode = 0755;
            int ret = mkdir(upath.c_str(), mode);
#endif
            if (ret == 0)
                return true;
            
            switch (errno)
            {
                case ENOENT:
                    // parent didn't exist, try to create it
                    {
                        unsigned long pos = path.find_last_of('/');
                        if (pos == std::string::npos)
    #ifdef _WIN32
                            pos = path.find_last_of('\\');
                        if (pos == std::string::npos)
    #endif
                            return false;
                        if (!CreateDirectory( path.substr(0, pos) ))
                            return false;
                    }
                    // now, try to create again
#ifdef _WIN32
                    return (0 == _wmkdir(path.c_str()));
#else
                    return (0 == mkdir(upath.c_str(), mode));
#endif
                    
                case EEXIST:
                    // done!
                    return FileExists(path);
                    
                default:
                    return false;
            }
        }

        
        
        
        std::wstring GetFolderPath(const std::wstring& path)
        {
            std::wstring folder = L"";
            
            int pos{ -1 };
            if (path.length() != 0)
                for (unsigned long i = path.length() - 1; i >= 0 && pos == -1; i--)
                {
                    if (path[i] == '/' || path[i] == '\\')
                        pos = i;
                    if (i == 0) break;
                }
            
            for (int i = 0; i <= pos; i++)
                folder += path[i];
            
            return folder;
        }
        std::wstring GetExtentionFromString(std::wstring filename)
        {
            for (unsigned long i = filename.size(); i >= 0; --i)
            {
                if (filename[i] == L'.')
                {
                    if (i != filename.size())
                        filename.erase(0,i);
                    return filename;
                }
                if (i == 0) break;
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
        
        
        
        int atoi(const std::wstring& stringValue)
        {
            std::string parsingString = "";
            for (int i = 0; stringValue[i] != '\0'; i++)
                if ((stringValue[i] >= 48 && stringValue[i] <= 57) || stringValue[i] == 45)
                    parsingString += (char)stringValue[i];
            
            return std::atoi(parsingString.c_str());
        }
        float atof(const std::wstring& stringValue)
        {
            std::string parsingString = "";
            for (int i = 0; stringValue[i] != '\0'; i++)
                if ((stringValue[i] >= 48 && stringValue[i] <= 57) || stringValue[i] == 44 || stringValue[i] == 46 || stringValue[i] == '-')
                    parsingString += (char)stringValue[i];
            
            return std::atof(parsingString.c_str());
        }
        bool atob(const std::wstring& stringValue)
        {
            if (stringValue == L"true" || stringValue == L"True" || stringValue == L"T" || stringValue == L"истина" || stringValue == L"1")
                return true;
            else
                return false;
        }
    }
}
