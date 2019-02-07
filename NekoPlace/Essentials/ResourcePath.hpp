////////////////////////////////////////////////////////////
//
// SFML - Simple and Fast Multimedia Library
// Copyright (C) 2007-2018 Marco Antognini (antognini.marco@gmail.com),
//                         Laurent Gomila (laurent@sfml-dev.org)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
////////////////////////////////////////////////////////////

#ifndef RESOURCE_PATH_HPP
#define RESOURCE_PATH_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <string>

////////////////////////////////////////////////////////////
/// \brief Return the path to the resource folder.
///
/// \return The path to the resource folder associate
/// with the main bundle or an empty string is there is no bundle.
///
////////////////////////////////////////////////////////////
#include <SFML/Config.hpp>

#ifdef SFML_SYSTEM_IOS
    std::string iOSDeviceName();
    std::string documentsPath(void);
    inline std::string resourcePath()
    {
        return "";
    }
    inline std::wstring executablePath()
    {
        return L"";
    }
#else
    #ifdef SFML_SYSTEM_ANDROID
        #include <jni.h>
        #include <android/native_activity.h>
        #include <android/log.h>
        #define LOGE(...) ((void)__android_log_print(ANDROID_LOG_INFO, "sfml-activity", __VA_ARGS__))
        #include <SFML/System/NativeActivity.hpp>

        std::string documentsPath();
        std::string apkPath();
        inline std::string resourcePath()
        {
            return "";
        }
        inline std::wstring executablePath()
        {
            return L"";
        }
    #else
        std::string resourcePath(void);
        std::wstring executablePath(void);
        std::string documentsPath(void);
    #endif
#endif

#endif
