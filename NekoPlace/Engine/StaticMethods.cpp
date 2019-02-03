//
//  StaticMethods.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 26/08/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#include "StaticMethods.hpp"

namespace ns
{
    sf::Font* FontCollector::GetFont(const std::wstring& fontName)
    {
        if (fonts.find(fontName) != fonts.end())
            return fonts.at(fontName);
        else
        {
            sf::Font* font = new sf::Font();
            
            sf::String fullPath = sf::String(resourcePath()) + L"Data/Fonts/" + fontName;
            bool fontLoaded{ false };
#ifdef _WIN32
            //TODO: Fix memory leak
            std::ifstream ifStream(fullPath.toWideString(), std::ios::binary | std::ios::ate);
            if (!ifStream.is_open())
                std::cerr << "Unable to open file: " << fullPath.toAnsiString() << std::endl;
            else
            {
                auto filesize = ifStream.tellg();
                char* fileInMemory = new char[static_cast<unsigned int>(filesize)];
                ifStream.seekg(0, std::ios::beg);
                ifStream.read(fileInMemory, filesize);
                ifStream.close();
                
                fontLoaded = font->loadFromMemory(fileInMemory, filesize);
            }
#else
            std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
            std::string u8str = converter.to_bytes(fullPath.toWideString());
            if (!(fontLoaded = font->loadFromFile(u8str)))
                std::cerr << "Unable to open file: " << fullPath.toAnsiString() << std::endl;
#endif
            
            if (fontLoaded)
            {
                fonts.emplace(fontName, font);
                return font;
            }
            else return nullptr;
        }
    }
    void FontCollector::FreeFonts()
    {
        for (const auto& key : fonts)
            if (key.second != nullptr)
                delete key.second;
        fonts.clear();
    }
    std::unordered_map<std::wstring, sf::Font*> FontCollector::fonts = { {L"", nullptr} };
    
    
    
    
    
    
    //////////////////////////////////////////
    ///
    /// Modes:
    ///     1 - background
    ///     2 - sprites/images
    ///     3 - sounds
    ///     4 - music/ambient
    ///
    //////////////////////////////////////////
    std::wstring PathWithResolutionDetermination(const std::wstring& imageName, unsigned int mode)
    {
        
        std::wstring fullPath = sf::String(resourcePath()) + imageName;
        if (mode != 0 && !base::FileExists(fullPath))
        {
            std::wstring getExtention = base::GetExtentionFromString(fullPath);
            std::wstring woExtention = base::GetStringWithNoExtention(fullPath, getExtention);
            if (!base::FileExists(sf::String(woExtention + L"@0x" + getExtention)) &&
                !base::FileExists(sf::String(woExtention + L"@1x" + getExtention)) &&
                !base::FileExists(sf::String(woExtention + L"@2x" + getExtention)) &&
                !base::FileExists(sf::String(woExtention + L"@3x" + getExtention)))
            {
                bool found{ false };
                std::wstring onlyFolder = base::GetFolderPath(fullPath);
                
                std::wstring onlyFileName = L"";
                for (unsigned long i = onlyFolder.length(); i < fullPath.length(); i++)
                    onlyFileName += fullPath[i];
                
                switch (mode)
                {
                    case 1:
                        for (int i = 0; i <= 1 && !found; i++)
                        {
                            std::wstring currentPath;
                            switch (i)
                            {
                                case 0: currentPath = onlyFolder + L"backgrounds/" + onlyFileName; break;
                                case 1: currentPath = onlyFolder + L"Backgrounds/" + onlyFileName; break;
                                default: break;
                            }
                            if (((found = base::DoesFileExistWithResolutionClass(currentPath)))) fullPath = currentPath;
                        }
                        break;
                        
                    case 2:
                        for (int i = 0; i <= 3 && !found; i++)
                        {
                            std::wstring currentPath;
                            switch (i)
                            {
                                case 0: currentPath = onlyFolder + L"images/" + onlyFileName; break;
                                case 1: currentPath = onlyFolder + L"sprites/" + onlyFileName; break;
                                case 2: currentPath = onlyFolder + L"Images/" + onlyFileName; break;
                                case 3: currentPath = onlyFolder + L"Sprites/" + onlyFileName; break;
                                default: break;
                            }
                            if (((found = base::DoesFileExistWithResolutionClass(currentPath)))) fullPath = currentPath;
                        }
                        break;
                    case 3:
                        for (int i = 0; i <= 5 && !found; i++)
                        {
                            std::wstring currentPath;
                            switch (i)
                            {
                                case 0: currentPath = onlyFolder + L"sounds/" + onlyFileName; break;
                                case 1: currentPath = onlyFolder + L"sound/" + onlyFileName; break;
                                case 2: currentPath = onlyFolder + L"audio/" + onlyFileName; break;
                                case 3: currentPath = onlyFolder + L"Sounds/" + onlyFileName; break;
                                case 4: currentPath = onlyFolder + L"Sound/" + onlyFileName; break;
                                case 5: currentPath = onlyFolder + L"Audio/" + onlyFileName; break;
                                default: break;
                            }
                            if (((found = base::DoesFileExistWithResolutionClass(currentPath)))) fullPath = currentPath;
                        }
                        break;
                    case 4:
                        for (int i = 0; i <= 5 && !found; i++)
                        {
                            std::wstring currentPath;
                            switch (i)
                            {
                                case 0: currentPath = onlyFolder + L"music/" + onlyFileName; break;
                                case 1: currentPath = onlyFolder + L"ambient/" + onlyFileName; break;
                                case 2: currentPath = onlyFolder + L"audio/" + onlyFileName; break;
                                case 3: currentPath = onlyFolder + L"Music/" + onlyFileName; break;
                                case 4: currentPath = onlyFolder + L"Ambient/" + onlyFileName; break;
                                case 5: currentPath = onlyFolder + L"Audio/" + onlyFileName; break;
                                default: break;
                            }
                            if (((found = base::DoesFileExistWithResolutionClass(currentPath)))) fullPath = currentPath;
                        }
                        break;
                    default: break;
                }
            }
        }
        
        if (gs::isResolutionClassEnabled)
        {
            std::wstring getExtention = base::GetExtentionFromString(fullPath);
            std::wstring woExtention = base::GetStringWithNoExtention(fullPath, getExtention);
            
            switch (ns::gs::resolutionClass)
            {
                case 0:
                    if (base::FileExists(woExtention + L"@0x" + getExtention)) fullPath = woExtention + L"@0x" + getExtention;
                    else if (base::FileExists(woExtention + L"@1x" + getExtention)) fullPath = woExtention + L"@1x" + getExtention;
                    else if (base::FileExists(woExtention + L"@2x" + getExtention)) fullPath = woExtention + L"@2x" + getExtention;
                    else if (base::FileExists(woExtention + L"@3x" + getExtention)) fullPath = woExtention + L"@3x" + getExtention;
                    break;
                    
                case 1:
                    if (base::FileExists(woExtention + L"@1x" + getExtention)) fullPath = woExtention + L"@1x" + getExtention;
                    else if (base::FileExists(woExtention + L"@2x" + getExtention)) fullPath = woExtention + L"@2x" + getExtention;
                    else if (base::FileExists(woExtention + L"@0x" + getExtention)) fullPath = woExtention + L"@0x" + getExtention;
                    else if (base::FileExists(woExtention + L"@3x" + getExtention)) fullPath = woExtention + L"@3x" + getExtention;
                    break;
                    
                case 2:
                    if (base::FileExists(woExtention + L"@2x" + getExtention)) fullPath = woExtention + L"@2x" + getExtention;
                    else if (base::FileExists(woExtention + L"@1x" + getExtention)) fullPath = woExtention + L"@1x" + getExtention;
                    else if (base::FileExists(woExtention + L"@3x" + getExtention)) fullPath = woExtention + L"@3x" + getExtention;
                    else if (base::FileExists(woExtention + L"@0x" + getExtention)) fullPath = woExtention + L"@0x" + getExtention;
                    break;
                    
                case 3:
                    if (base::FileExists(woExtention + L"@3x" + getExtention)) fullPath = woExtention + L"@3x" + getExtention;
                    else if (base::FileExists(woExtention + L"@2x" + getExtention)) fullPath = woExtention + L"@2x" + getExtention;
                    else if (base::FileExists(woExtention + L"@1x" + getExtention)) fullPath = woExtention + L"@1x" + getExtention;
                    else if (base::FileExists(woExtention + L"@0x" + getExtention)) fullPath = woExtention + L"@0x" + getExtention;
                    break;
                    
                default: break;
            }
        }
        else
        {
            if (!base::FileExists(fullPath))
            {
                std::wstring getExtention = base::GetExtentionFromString(fullPath);
                std::wstring woExtention = base::GetStringWithNoExtention(fullPath, getExtention);
                
                if (base::FileExists(sf::String(woExtention + L"@3x" + getExtention))) fullPath = woExtention + L"@3x" + getExtention;
                else if (base::FileExists(sf::String(woExtention + L"@2x" + getExtention))) fullPath = woExtention + L"@2x" + getExtention;
                else if (base::FileExists(sf::String(woExtention + L"@1x" + getExtention))) fullPath = woExtention + L"@1x" + getExtention;
                else if (base::FileExists(sf::String(woExtention + L"@0x" + getExtention))) fullPath = woExtention + L"@0x" + getExtention;
            }
        }
        
        return fullPath;
    }
    sf::Image* ImageCollector::LoadImage(const std::wstring& imageName, unsigned int mode)
    {
        if (images.find(imageName) != images.end())
        {
            if (threads.find(imageName) != threads.end())
            {
                if (threads[imageName].joinable()) threads[imageName].join();
                threads.erase(imageName);
            }
            ++images[imageName].usage;
            return images[imageName].image;
        }
        else
        {
            if (threads.find(imageName) != threads.end())
            {
                threads[imageName].join();
                threads.erase(imageName);
                if (images.find(imageName) != images.end() && images[imageName].image != nullptr)
                {
                    ++images[imageName].usage;
                    return images[imageName].image;
                }
                
                return nullptr;
            }
            
            std::wstring fullPath = PathWithResolutionDetermination(imageName, mode);
            
            if (base::FileExists(fullPath))
            {
                sf::Image* image = new sf::Image();
                bool imageLoaded{ false };
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
                    
                    imageLoaded = image->loadFromMemory(fileInMemory, filesize);
                    delete[] fileInMemory;
                }
    #else
                if (!(imageLoaded = image->loadFromFile(base::utf8(fullPath))))
                    std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
    #endif
                
                if (imageLoaded)
                {
                    images.emplace(imageName, ImageCollectorObject(image, nullptr, 1));
                    return image;
                }
            }
            return nullptr;
        }
    }
    void ImageCollector::ThreadImage(std::wstring imageName, unsigned int mode, bool destroyable, bool loadTexture)
    {
        std::wstring fullPath = PathWithResolutionDetermination(imageName, mode);
        
        if (base::FileExists(fullPath))
        {
            sf::Image* image = new sf::Image();
            bool imageLoaded{ false };
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
                
                imageLoaded = image->loadFromMemory(fileInMemory, filesize);
                delete[] fileInMemory;
            }
    #else
            if (!(imageLoaded = image->loadFromFile(base::utf8(fullPath))))
                std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
    #endif
            
            if (imageLoaded)
            {
                images.emplace(imageName, ImageCollectorObject(image, nullptr, 0, destroyable));
                if (loadTexture) { LoadTexture(imageName); images[imageName].usage = 0; }
                //threads.erase(imageName);
            }
        }
    }
    void ImageCollector::PreloadImage(const std::wstring& imageName, unsigned int mode, bool destroyable)
    {
        if (images.find(imageName) == images.end())
        {
            /*if (threads.find(imageName) != threads.end())
            {
                threads[imageName].join();
                threads.erase(threads.find(imageName));
                if (images.find(imageName) != images.end() && images[imageName].image != nullptr)
                    return;
            }*/
            if (threads.find(imageName) == threads.end())
                threads.emplace(imageName, std::thread(&ThreadImage, imageName, mode, destroyable, false));
        }
    }
    sf::Texture* ImageCollector::LoadTexture(const std::wstring& imageName, unsigned int mode)
    {
        bool usagePlusPlus{ true };
        if (images.find(imageName) == images.end()) { ic::LoadImage(imageName, mode); usagePlusPlus = false; }
        if (images.find(imageName) != images.end())
        {
            if (images[imageName].image != nullptr)
            {
                if (usagePlusPlus) ++images[imageName].usage;
                ///cout << "LoadTexture! images[" << base::utf8(imageName) << "].usage = " << images[imageName].usage << endl;
                if (images[imageName].texture != nullptr) return images[imageName].texture;
                
                images[imageName].texture = new sf::Texture();
                sf::Image* imagePtr = images[imageName].image;
                if (imagePtr->getSize().x > sf::Texture::getMaximumSize() || imagePtr->getSize().y > sf::Texture::getMaximumSize())
                    images[imageName].texture->loadFromImage(*imagePtr, sf::IntRect(0, 0, imagePtr->getSize().x > sf::Texture::getMaximumSize() ? sf::Texture::getMaximumSize() : imagePtr->getSize().x, imagePtr->getSize().y > sf::Texture::getMaximumSize() ? sf::Texture::getMaximumSize() : imagePtr->getSize().y));
                else images[imageName].texture->loadFromImage(*images[imageName].image);
                if (images[imageName].texture) images[imageName].texture->setSmooth(true);
                return images[imageName].texture;
            }
        }
        return nullptr;
    }
    void ImageCollector::PreloadTexture(const std::wstring& imageName, unsigned int mode, bool destroyable)
    {
        if (images.find(imageName) == images.end())
        {
            /*if (threads.find(imageName) != threads.end())
            {
                threads[imageName].join();
                threads.erase(threads.find(imageName));
                if (images.find(imageName) != images.end() && images[imageName].image != nullptr)
                {
                    if (!images[imageName].texture) { LoadTexture(imageName); --images[imageName].usage; }
                    return;
                }
            }*/
            if (threads.find(imageName) == threads.end())
                threads.emplace(imageName, std::thread(&ThreadImage, imageName, mode, destroyable, true));
        }
    }
    sf::Image* ImageCollector::FindImage(const std::wstring& imageName)
    {
        if (images.find(imageName) != images.end()) return images[imageName].image;
        return nullptr;
    }
    sf::Texture* ImageCollector::FindTexture(const std::wstring& imageName)
    {
        if (images.find(imageName) != images.end()) return images[imageName].texture;
        return nullptr;
    }
    void ImageCollector::SetDestroyable(std::wstring imageName, bool destroyable)
    {
        if (images.find(imageName) != images.end())
        {
            images[imageName].destroyable = destroyable;
            if (destroyable) if (images[imageName].usage <= 0) ic::EraseImage(imageName);
        }
    }
    void ImageCollector::DeleteImage(const std::wstring& imageName)
    {
        if (images.find(imageName) != images.end())
        {
            --images[imageName].usage; ///cout << "DeleteImage! images[" << base::utf8(imageName) << "].usage = " << images[imageName].usage << endl;
            if (images[imageName].usage <= 0 && images[imageName].destroyable)
                ic::EraseImage(imageName);
        }
    }
    void ImageCollector::EraseImage(const std::wstring& imageName)
    {
        if (images.find(imageName) != images.end())
        {
            if (threads.find(imageName) != threads.end())
            {
                if (threads[imageName].joinable()) threads[imageName].join();
                threads.erase(imageName);
            }
            
            sf::Image* image = images[imageName].image;
            if (image != nullptr) delete image;
            sf::Texture* texture = images[imageName].texture;
            if (texture != nullptr) delete texture;
            
            images.erase(imageName);
        }
    }
    void ImageCollector::FreeImages()
    {
        for (const auto& key : images)
        {
            if (key.second.image != nullptr) delete key.second.image;
            if (key.second.texture != nullptr) delete key.second.texture;
        }
        images.clear();
    }
    std::unordered_map<std::wstring, ImageCollectorObject> ic::images;
    std::unordered_map<std::wstring, std::thread> ic::threads;
    icThreadsJoiner ic::threadsJoiner;
    
    
    
    
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
    
    
    
    
    
    
    sf::RenderWindow* gs::window = nullptr;
    
    unsigned int gs::width = 0;
    unsigned int gs::height = 0;
    unsigned int gs::relativeWidth = 0;
    unsigned int gs::relativeHeight = 0;
    float gs::scale = 1.f; float gs::scScale = 1.f;
    float gs::scalex = 1.f; float gs::scaley = 1.f;
    
    float gs::deltaVelocity = 1.f;
    
#ifdef _WIN32
    int gs::windowPositionOffset = 0;//IsWindows8OrGreater() ? GetSystemMetrics(SM_CXSIZEFRAME) : 0;
#else
    int gs::windowPositionOffset = 0;
#endif
    
    bool gs::isVerticalSyncEnabled = true;
    int gs::framerateLimit = 120;
    int gs::framerateNoFocus = 60;
    
    bool gs::isResolutionClassEnabled = true;
    int gs::resolutionClassSetting = -1;
    int gs::resolutionClass = 1;
    
    bool gs::isPause = false;
    bool gs::pauseOnFocusLost = true;
    bool gs::inGame = false;
    
    float gs::maxVolumeGlobal = 1.f;
    float gs::maxVolumeMusic = 1.f;
    float gs::maxVolumeAmbeint = 1.f;
    float gs::maxVolumeSound = 1.f;
}
