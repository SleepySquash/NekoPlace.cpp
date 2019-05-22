//
//  Sound.hpp
//  NovelSome
//
//  Created by Никита Исаенко on 17/03/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef SoundCollector_hpp
#define SoundCollector_hpp

#include <iostream>
#include <unordered_map>

#include <thread>
#include <codecvt>
#include <fstream>
#include <memory>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>

#include "../../Essentials/ResourcePath.hpp"
#ifdef _WIN32
#include "../../Essentials/versionhelpers.h"
#endif
#include "../../Essentials/Base.hpp"
#include "../NovelSomeScript.hpp"
#include "etc.hpp"

using base::utf8;
using base::utf16;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

namespace ns
{
    struct scThreadsJoiner;
    struct SoundCollectorObject
    {
        sf::SoundBuffer* sound{ nullptr };
        int usage{ 0 };
        bool destroyable{ true };
        
        SoundCollectorObject(sf::SoundBuffer* sound = nullptr, int usage = 0, bool destroyable = true) : sound(sound), usage(usage), destroyable(destroyable) { }
    };
    class SoundCollector
    {
    private:
        SoundCollector() { }
        
    public:
        static std::unordered_map<std::wstring, SoundCollectorObject> sounds;
        static std::unordered_map<std::wstring, std::thread> threads;
        
        static scThreadsJoiner threadsJoiner;
        static sf::SoundBuffer* LoadSound(const std::wstring& soundName, unsigned int mode = 3);
        static bool LoadMusic(sf::Music& music, std::wstring fullPath, std::unique_ptr<char[]>& fileInMemory);
        static void ThreadSound(std::wstring soundName, unsigned int mode, bool destroyable = true);
        static void PreloadSound(const std::wstring& soundName, unsigned int mode = 3, bool destroyable = true);
        static void SetDestroyable(std::wstring soundName, bool destroyable);
        static void DeleteSound(const std::wstring& soundName);
        static void EraseSound(const std::wstring& soundName);
        static void FreeSounds();
    };
    struct scThreadsJoiner { ~scThreadsJoiner() { for (auto& t : SoundCollector::threads) t.second.join(); SoundCollector::threads.clear(); } };
    
    typedef SoundCollector sc;
}

#endif /* Sound_hpp */
