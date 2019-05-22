//
//  Image.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 17/03/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Image.hpp"

namespace ns
{
    sf::Image* ImageCollector::LoadImage(const std::wstring& imageName, unsigned int mode)
    {
        if (images.find(imageName) != images.end())
        {
            if (threads.find(imageName) != threads.end())
            {
                if (threads[imageName]->joinable()) threads[imageName]->join();
                delete threads[imageName]; threads.erase(imageName);
            }
            ++images[imageName].usage;
            return images[imageName].image;
        }
        else
        {
            if (threads.find(imageName) != threads.end())
            {
                threads[imageName]->join();
                delete threads[imageName]; threads.erase(imageName);
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
#ifdef SFML_SYSTEM_ANDROID
                if (fullPath[0] == L'/')
                {
                    std::ifstream ifStream(base::utf8(fullPath), std::ios::binary | std::ios::ate);
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
                }
                else
                {
#endif
                    if (!(imageLoaded = image->loadFromFile(base::utf8(fullPath))))
                        std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
#ifdef SFML_SYSTEM_ANDROID
                }
#endif
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
    void ImageCollector::ThreadImage(std::wstring imageName, unsigned int mode, bool destroyable, bool loadTexture, bool countAsUsed, std::string message, MessageSender* sender)
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
#ifdef SFML_SYSTEM_ANDROID
            if (fullPath[0] == L'/')
            {
                std::ifstream ifStream(base::utf8(fullPath), std::ios::binary | std::ios::ate);
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
            }
            else
            {
#endif
                if (!(imageLoaded = image->loadFromFile(base::utf8(fullPath))))
                    std::cerr << "Unable to open file: " << base::utf8(fullPath) << std::endl;
#ifdef SFML_SYSTEM_ANDROID
            }
#endif
#endif
            
            if (imageLoaded)
            {
                images.emplace(imageName, ImageCollectorObject(image, nullptr, countAsUsed? 1 : 0, destroyable));
                if (loadTexture) { LoadTexture(imageName); images[imageName].usage = countAsUsed? 1 : 0; }
                if (sender && message.length() > 0 && message != "")
                {
                    //cout << "original quality loaded, sending message" << endl;
                    gs::requestWindowRefresh = true;
                    sender->SendMessage({message, imageName, images[imageName].texture});
                    if (message == "Request0")
                    {
                        std::wstring imageExtention = base::GetExtentionFromString(imageName);
                        std::wstring imageWoExtention = base::GetStringWithNoExtention(imageName);
                        DrawbackImage(imageWoExtention + L"@0x" + imageExtention);
                    } else if (message == "Requestp")
                    {
                        std::wstring imageExtention = base::GetExtentionFromString(imageName);
                        std::wstring imageWoExtention = base::GetStringWithNoExtention(imageName);
                        DrawbackImage(imageWoExtention + L"@px" + imageExtention);
                    }
                    if (message != "RequestP")
                    {
                        //cout << "deleting and usage: " << images[imageName].usage << endl;
                        if (images[imageName].usage <= 0) { DrawbackImage(imageName); threads.erase(imageName); }
                    } //else cout << "preloading done " << images[imageName].usage << " " << utf8(imageName) << endl;
                }
                //threads.erase(imageName);
            }
        }
    }
    void ImageCollector::PreloadImage(const std::wstring& imageName, unsigned int mode, bool destroyable)
    {
        //cout << "[][][][][][] PRELOADING: " << base::utf8(imageName) << " [][][][][][]" << endl;
        if (images.find(imageName) == images.end())
            if (threads.find(imageName) == threads.end())
                threads.emplace(imageName, new std::thread(&ThreadImage, imageName, mode, destroyable, false, false, "", nullptr));
    }
    sf::Texture* ImageCollector::LoadTexture(const std::wstring& imageName, unsigned int mode)
    {
        bool usagePlusPlus{ true };
        if (images.find(imageName) == images.end()) { ic::LoadImage(imageName, mode); usagePlusPlus = false; }
        if (images.find(imageName) != images.end())
        {
            if (images[imageName].image)
            {
                if (usagePlusPlus) ++images[imageName].usage;
                if (images[imageName].texture) return images[imageName].texture;
                
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
    void ImageCollector::PreloadTexture(const std::wstring& imageName, unsigned int mode, bool destroyable, bool countAsUsed, MessageSender* sender)
    {
        if (images.find(imageName) == images.end())
        {
            if (threads.find(imageName) == threads.end())
                threads.emplace(imageName, new std::thread(&ThreadImage, imageName, mode, destroyable, true, countAsUsed, "RequestP", sender));
        } else if (countAsUsed) ++images[imageName].usage;
    }
    sf::Texture* ImageCollector::RequestTexture(const std::wstring& imageName, MessageSender* sender, unsigned int mode)
    {
        if (images.find(imageName) != images.end()) return LoadTexture(imageName, mode);
        else if (threads.find(imageName) == threads.end())
            threads.emplace(imageName, new std::thread(&ThreadImage, imageName, mode, true, true, false, "Request", sender));
        return nullptr;
    }
    sf::Texture* ImageCollector::RequestHigherTexture(const std::wstring& imageName, MessageSender* sender, unsigned int mode)
    {
        if (images.find(imageName) == images.end())
        {
            if (gs::isResolutionClassEnabled && gs::resolutionClass == 0) return LoadTexture(imageName, mode);
            std::wstring fullPath = PathWithFolderDetermination(imageName, mode);
            std::wstring getExtention = base::GetExtentionFromString(fullPath);
            std::wstring woExtention = base::GetStringWithNoExtention(fullPath);
            std::wstring prefix = L"@px";
            if (gs::prefer0xRatherThatpx || !base::FileExists(woExtention + L"@px" + getExtention)) prefix = L"@0x";
            if (base::FileExists(woExtention + prefix + getExtention))
            {
                std::wstring imageExtention = base::GetExtentionFromString(imageName);
                std::wstring imageWoExtention = base::GetStringWithNoExtention(imageName);
                if (images.find(imageWoExtention + prefix + imageExtention) != images.end())
                    return LoadTexture(imageWoExtention + prefix + imageExtention, mode);
                sf::Texture* texture = LoadTexture(imageWoExtention + prefix + imageExtention, mode);
                if (texture)
                {
                    // Start loading with the help of thread
                    //if (prefix == L"@0x") cout << "0x found, starting loading" << endl; else cout << "px found, starting loading" << endl;
                    if (threads.find(imageName) == threads.end())
                    {
                        std::string message = "Requestp"; if (prefix == L"@0x") message = "Request0";
                        threads.emplace(imageName, new std::thread(&ThreadImage, imageName, mode, true, true, false, message, sender));
                    }
                    else --images[imageWoExtention + prefix + imageExtention].usage;
                    return texture;
                } else return LoadTexture(imageName, mode);
            } else return LoadTexture(imageName, mode);
        }
        return LoadTexture(imageName, mode);
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
            --images[imageName].usage; //cout << "[][][][][][] images[" << base::utf8(imageName) << "].usage: " << images[imageName].usage << " [][][][][][]" << endl;
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
                if (threads[imageName]->joinable()) threads[imageName]->join();
                delete threads[imageName]; threads.erase(imageName);
            }
            
            if (images[imageName].image) delete images[imageName].image;
            if (images[imageName].texture) delete images[imageName].texture;
            images.erase(imageName);
        }
    }
    void ImageCollector::DrawbackImage(const std::wstring& imageName)
    {
        if (images.find(imageName) != images.end())
        {
            --images[imageName].usage;
            if (images[imageName].usage <= 0 && images[imageName].destroyable)
            {
                if (images[imageName].image) delete images[imageName].image;
                if (images[imageName].texture) delete images[imageName].texture;
                images.erase(imageName);
            }
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
    std::unordered_map<std::wstring, std::thread*> ic::threads;
    icThreadsJoiner ic::threadsJoiner;
}
