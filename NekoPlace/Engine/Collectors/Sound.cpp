//
//  Sound.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 17/03/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Sound.hpp"

namespace ns
{
    sf::SoundBuffer* SoundCollector::LoadSound(const std::wstring& soundName, unsigned int mode)
    {
        if (sounds.find(soundName) != sounds.end())
        {
            if (threads.find(soundName) != threads.end())
            {
                if (threads[soundName].joinable()) threads[soundName].join();
                threads.erase(soundName);
            }
            ++sounds[soundName].usage;
            return sounds[soundName].sound;
        }
        else
        {
            if (threads.find(soundName) != threads.end())
            {
                threads[soundName].join();
                threads.erase(soundName);
                if (sounds.find(soundName) != sounds.end() && sounds[soundName].sound != nullptr)
                {
                    ++sounds[soundName].usage;
                    return sounds[soundName].sound;
                }
                return nullptr;
            }
            
            std::wstring fullPath = PathWithResolutionDetermination(soundName, mode);
            if (base::FileExists(fullPath))
            {
                sf::SoundBuffer* sound = new sf::SoundBuffer();
                bool soundLoaded{ false };
#ifdef _WIN32
                std::ifstream ifStream(fullPath, std::ios::binary | std::ios::ate);
                if (!ifStream.is_open())
                    std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
                else
                {
                    auto filesize = ifStream.tellg();
                    char* fileInMemory = new char[static_cast<unsigned int>(filesize)];
                    ifStream.seekg(0, std::ios::beg);
                    ifStream.read(fileInMemory, filesize);
                    ifStream.close();
                    
                    soundLoaded = sound->loadFromMemory(fileInMemory, filesize);
                    delete[] fileInMemory;
                }
#else
                if (!(soundLoaded = sound->loadFromFile(base::utf8(fullPath))))
                    std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
#endif
                
                if (soundLoaded)
                {
                    sounds.emplace(soundName, SoundCollectorObject(sound, 1));
                    return sound;
                }
            }
            return nullptr;
        }
    }
    bool SoundCollector::LoadMusic(sf::Music &music, std::wstring fullPath, std::unique_ptr<char[]>& fileInMemory)
    {
        bool found{ false };
        if (!base::FileExists(fullPath))
        {
            for (int j = 0; j <= 1 && !found; ++j)
            {
                if (j == 1) fullPath = base::utf16(resourcePath()) + fullPath;
                
                std::wstring onlyFolder = base::GetFolderPath(fullPath);
                std::wstring onlyFileName = L"";
                for (int i = onlyFolder.length(); i < fullPath.length(); i++)
                    onlyFileName += fullPath[i];
                
                if (base::FileExists(onlyFolder + L"music/" + onlyFileName)) fullPath = onlyFolder + L"music/" + onlyFileName;
                else if (base::FileExists(onlyFolder + L"ambient/" + onlyFileName)) fullPath = onlyFolder + L"ambient/" + onlyFileName;
                else if (base::FileExists(onlyFolder + L"audio/" + onlyFileName)) fullPath = onlyFolder + L"audio/" + onlyFileName;
                else if (base::FileExists(onlyFolder + L"Music/" + onlyFileName)) fullPath = onlyFolder + L"Music/" + onlyFileName;
                else if (base::FileExists(onlyFolder + L"Ambient/" + onlyFileName)) fullPath = onlyFolder + L"Ambient/" + onlyFileName;
                else if (base::FileExists(onlyFolder + L"Audio/" + onlyFileName)) fullPath = onlyFolder + L"Audio/" + onlyFileName;
                found = base::FileExists(fullPath);
            }
        }
        
        if (!found) found = base::FileExists(fullPath);
        if (found)
        {
            bool audioLoaded{ false };
#ifdef _WIN32
            std::ifstream ifStream(fullPath, std::ios::binary | std::ios::ate);
            if (!ifStream.is_open()) std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
            else
            {
                auto filesize = ifStream.tellg();
                fileInMemory.reset(new char[static_cast<unsigned int>(filesize)]);
                ifStream.seekg(0, std::ios::beg);
                ifStream.read(fileInMemory.get(), filesize);
                ifStream.close();
                
                audioLoaded = music.openFromMemory(fileInMemory.get(), filesize);
            }
#else
            if (!(audioLoaded = music.openFromFile(base::utf8(fullPath))))
                std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
#endif
            return audioLoaded;
        }
        return false;
    }
    void SoundCollector::ThreadSound(std::wstring soundName, unsigned int mode, bool destroyable)
    {
        sf::SoundBuffer* sound = new sf::SoundBuffer();
        std::wstring fullPath = PathWithResolutionDetermination(soundName, mode);
        
        if (base::FileExists(fullPath))
        {
            bool soundLoaded{ false };
#ifdef _WIN32
            std::ifstream ifStream(fullPath, std::ios::binary | std::ios::ate);
            if (!ifStream.is_open())
                std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
            else
            {
                auto filesize = ifStream.tellg();
                char* fileInMemory = new char[static_cast<unsigned int>(filesize)];
                ifStream.seekg(0, std::ios::beg);
                ifStream.read(fileInMemory, filesize);
                ifStream.close();
                
                soundLoaded = sound->loadFromMemory(fileInMemory, filesize);
                delete[] fileInMemory;
            }
#else
            cout << base::utf8(fullPath) << endl;
            if (!(soundLoaded = sound->loadFromFile(base::utf8(fullPath))))
                std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
#endif
            
            if (soundLoaded)
            {
                sounds.emplace(soundName, SoundCollectorObject(sound, 0, destroyable));
                //threads.erase(imageName);
            }
        }
    }
    void SoundCollector::PreloadSound(const std::wstring& soundName, unsigned int mode, bool destroyable)
    {
        if (sounds.find(soundName) == sounds.end())
            if (threads.find(soundName) == threads.end())
            {
                std::wstring fullPath = PathWithResolutionDetermination(soundName, mode);
                if (base::FileExists(fullPath))
                    threads.emplace(soundName, std::thread(&ThreadSound, soundName, mode, destroyable));
            }
    }
    void SoundCollector::SetDestroyable(std::wstring soundName, bool destroyable)
    {
        if (sounds.find(soundName) != sounds.end())
        {
            sounds[soundName].destroyable = destroyable;
            if (destroyable) if (sounds[soundName].usage <= 0) sc::EraseSound(soundName);
        }
    }
    void SoundCollector::DeleteSound(const std::wstring& soundName)
    {
        if (sounds.find(soundName) != sounds.end())
        {
            --sounds[soundName].usage;
            if (sounds[soundName].usage <= 0 && sounds[soundName].destroyable)
                sc::EraseSound(soundName);
        }
    }
    void SoundCollector::EraseSound(const std::wstring& soundName)
    {
        if (sounds.find(soundName) != sounds.end())
        {
            if (threads.find(soundName) != threads.end())
            {
                if (threads[soundName].joinable()) threads[soundName].join();
                threads.erase(soundName);
            }
            
            sf::SoundBuffer* sound = sounds[soundName].sound;
            if (sound != nullptr) delete sound;
            sounds.erase(soundName);
        }
    }
    void SoundCollector::FreeSounds()
    {
        for (const auto& key : sounds)
            if (key.second.sound != nullptr) delete key.second.sound;
        sounds.clear();
    }
    std::unordered_map<std::wstring, SoundCollectorObject> sc::sounds;
    std::unordered_map<std::wstring, std::thread> sc::threads;
    scThreadsJoiner sc::threadsJoiner;
}
