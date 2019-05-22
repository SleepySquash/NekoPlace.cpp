//
//  StaticMethods.hpp
//  NovelSome
//
//  Created by Никита Исаенко on 26/08/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#ifndef StaticMethods_hpp
#define StaticMethods_hpp

#include <iostream>
#include <unordered_map>

#include <thread>
#include <codecvt>
#include <fstream>
#include <memory>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../Essentials/ResourcePath.hpp"
#ifdef _WIN32
    //#include "../Essentials/versionhelpers.h"
#endif
#include "../Essentials/Base.hpp"
#include "NovelSomeScript.hpp"

using std::cout;
using std::endl;
using std::pair;
using std::make_pair;
using base::utf8;
using base::utf16;

namespace ns
{
    struct GlobalSettings
    {
        static void Load(const std::wstring& path);
        static void Save(const std::wstring& path);
        
        static sf::RenderWindow* window;
        
        static unsigned int width, height;
        
        static float scale, scScale;
        static float scalex, scaley;
        static unsigned int relativeWidth, relativeHeight;
        static bool verticalOrientation;
        
        static float deltaVelocity;
        
        static std::pair<int, int> lastMousePos;
        static int windowPositionOffset;
        
        static bool isVerticalSyncEnabled;
        static int framerateLimit, framerateNoFocus;
        
        static bool isResolutionClassEnabled;
        static int resolutionClassSetting, resolutionClass;
        static bool prefer0xRatherThatpx;
        
        static bool isPause, pauseOnFocusLost, inGame;
        static bool ignoreEvent, ignoreDraw;
        static bool requestWindowRefresh;
        
        static std::vector<void*> activeInterfaces;
        static void PushInterface(void*);
        static void RemoveInterface(void*);
        static bool isActiveInterface(void*);
        
        static bool isParallaxEnabled;
        static float defaultParallaxBackground, defaultParallaxFar, defaultParallaxNormal, defaultParallaxClose, defaultParallaxFrontground;
        
        static float maxVolumeGlobal, maxVolumeMusic, maxVolumeAmbeint, maxVolumeSound;
    };
    
    typedef GlobalSettings gs;
}

#endif /* StaticMethods_hpp */
