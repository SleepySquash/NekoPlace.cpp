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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "ResourcePath.hpp"

#ifdef __APPLE__
    #ifdef SFML_SYSTEM_IOS
        #import <Foundation/Foundation.h>
        #import <sys/utsname.h>

        std::string iOSDeviceName()
        {
            struct utsname systemInfo;
            uname(&systemInfo);
            
            std::string rpath;
            NSString* path = [NSString stringWithCString:systemInfo.machine
                                                encoding:NSUTF8StringEncoding];
            return [path UTF8String];
        }
        #else
        #import <Foundation/Foundation.h>

        ////////////////////////////////////////////////////////////
        std::string resourcePath(void)
        {
            NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
            
            std::string rpath;
            NSBundle* bundle = [NSBundle mainBundle];
            
            if (bundle == nil) {
        #ifdef DEBUG
                NSLog(@"bundle is nil... thus no resources path can be found.");
        #endif
            } else {
                NSString* path = [bundle resourcePath];
                rpath = [path UTF8String] + std::string("/");
            }
            
            [pool drain];
            
            return rpath;
        }

        std::wstring executablePath(void)
        {
            std::string resPath = resourcePath();
            
            //TODO: change the way it converts
            std::wstring exePath(resPath.begin(), resPath.end());
            return exePath;
        }
    #endif
#endif
