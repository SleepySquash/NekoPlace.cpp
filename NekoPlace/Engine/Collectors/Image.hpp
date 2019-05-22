//
//  Image.hpp
//  NovelSome
//
//  Created by Никита Исаенко on 17/03/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef ImageCollector_hpp
#define ImageCollector_hpp

#include <iostream>
#include <unordered_map>

#include <thread>
#include <codecvt>
#include <fstream>
#include <memory>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#ifdef _WIN32
#include "../../Essentials/versionhelpers.h"
#endif
#include "../../Essentials/Base.hpp"
#include "../NovelSomeScript.hpp"
#include "../MessageHolder.hpp"
#include "etc.hpp"

using base::utf8;
using base::utf16;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

namespace ns
{
    struct icThreadsJoiner;
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
        static std::unordered_map<std::wstring, std::thread*> threads;
        
        static icThreadsJoiner threadsJoiner;
        
        static sf::Image* LoadImage(const std::wstring& imageName, unsigned int mode = 2);
        static void ThreadImage(std::wstring imageName, unsigned int mode, bool destroyable = true, bool loadTexture = false, bool countAsUsed = false, std::string message = "", MessageSender* sender = nullptr);
        static void PreloadImage(const std::wstring& imageName, unsigned int mode = 2, bool destroyable = true);
        static sf::Texture* LoadTexture(const std::wstring& imageName, unsigned int mode = 2);
        static void PreloadTexture(const std::wstring& imageName, unsigned int mode = 2, bool destroyable = true, bool countAsUsed = false, MessageSender* sender = nullptr);
        
        static sf::Texture* RequestTexture(const std::wstring& imageName, MessageSender* sender, unsigned int mode = 2);
        static sf::Texture* RequestHigherTexture(const std::wstring& imageName, MessageSender* sender, unsigned int mode = 2);
        
        static sf::Image* FindImage(const std::wstring& imageName);
        static sf::Texture* FindTexture(const std::wstring& imageName);
        
        static void SetDestroyable(std::wstring imageName, bool destroyable);
        static void DeleteImage(const std::wstring& imageName);
        static void EraseImage(const std::wstring& imageName);
        static void DrawbackImage(const std::wstring& imageName);
        static void FreeImages();
    };
    struct icThreadsJoiner { ~icThreadsJoiner() { for (auto& t : ImageCollector::threads) t.second->join(); ImageCollector::threads.clear(); } };
    
    typedef ImageCollector ic;
}

#endif /* Image_hpp */
