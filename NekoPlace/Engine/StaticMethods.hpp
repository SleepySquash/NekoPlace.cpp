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

#ifdef _WIN32
//std::ifstream in GetFont
#include <fstream>
#endif

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../Essentials/ResourcePath.hpp"
#ifdef _WIN32
//#include "../Essentials/versionhelpers.h"
#endif
#include "../Essentials/Base.hpp"

using std::cout;
using std::endl;

using std::pair;
using std::make_pair;

namespace ns
{
    class FontCollector
    {
    private:
        FontCollector() { }
        
    public:
        static std::unordered_map<std::wstring, sf::Font*> fonts;
        static sf::Font* GetFont(const std::wstring& fontName);
        static void FreeFonts();
    };
    
    
    
    std::wstring PathWithResolutionDetermination(const std::wstring& fileName, unsigned int mode);
    struct ThreadsJoiner;
    struct ImageCollectorObject
    {
        sf::Image* image{ nullptr };
        sf::Texture* texture{ nullptr };
        int usage{ 0 };
        bool destroyable{ true };
        
        ImageCollectorObject(sf::Image* image = nullptr, sf::Texture* texture = nullptr, int usage = 0, bool destroyable = true) : image(image), texture(texture), usage(usage), destroyable(destroyable) { }
    };
    class ImageCollector
    {
    private:
        ImageCollector() { }
        
    public:
        static std::unordered_map<std::wstring, ImageCollectorObject> images;
        static std::unordered_map<std::wstring, std::thread> threads;
        
        static ThreadsJoiner threadsJoiner;
        static sf::Image* LoadImage(const std::wstring& imageName, unsigned int mode = 0);
        static void ThreadImage(std::wstring imageName, unsigned int mode, bool destroyable = true);
        static void PreloadImage(const std::wstring& imageName, unsigned int mode = 0, bool destroyable = true);
        static sf::Texture* LoadTexture(const std::wstring& imageName, unsigned int mode = 0);
        static void SetDestroyable(std::wstring imageName, bool destroyable);
        static void DeleteImage(const std::wstring& imageName);
        static void EraseImage(const std::wstring& imageName);
        static void FreeImages();
    };
    struct ThreadsJoiner { ~ThreadsJoiner() { for (auto& t : ImageCollector::threads) t.second.join(); ImageCollector::threads.clear(); } };
    /*class TextureCollector
    {
    private:
        TextureCollector() { }
        
    public:
        static std::unordered_map<std::wstring, pair<sf::Texture*, int>> textures;
        
        static sf::Texture* LoadTexture(const std::wstring& imageName, unsigned int mode = 0);
        static void DeleteTexture(const std::wstring& imageName);
        static void FreeTextures();
    };*/
    
    
    
    class GlobalSettings
    {
    public:
        static sf::RenderWindow* window;
        
        static unsigned int width;
        static unsigned int height;
        
        static float scale;
        static float scalex, scaley;
        static unsigned int relativeWidth;
        static unsigned int relativeHeight;
        
        static float deltaVelocity;
        
        static int windowPositionOffset;
        
        static bool isVerticalSyncEnabled;
        static int framerateLimit;
        static int framerateNoFocus;
        
        static bool isResolutionClassEnabled;
        static int resolutionClassSetting;
        static int resolutionClass;
        
        static bool isPause;
        static bool pauseOnFocusLost;
    };
    
    typedef GlobalSettings gs;
    typedef FontCollector fc;
    typedef ImageCollector ic;
}

#endif /* StaticMethods_hpp */
