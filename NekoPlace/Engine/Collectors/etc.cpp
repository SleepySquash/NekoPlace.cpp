//
//  etc.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 17/03/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "etc.hpp"

namespace ns
{
    //////////////////////////////////////////
    ///
    /// Modes:
    ///     1 - background
    ///     2 - sprites/images
    ///     3 - sounds
    ///     4 - music/ambient
    ///
    //////////////////////////////////////////
    std::wstring PathWithFolderDetermination(const std::wstring& imageName, unsigned int mode)
    {
        std::wstring fullPath = imageName;
        if (mode != 0 && !base::DoesFileExistWithResolutionClass(fullPath))
        {
            if (!base::DoesFileExistWithResolutionClass(base::utf16(resourcePath()) + fullPath))
            {
                bool found{ false };
                std::wstring onlyFolder = base::GetFolderPath(fullPath);
                std::wstring onlyFileName = L"";
                for (unsigned long i = onlyFolder.length(); i < fullPath.length(); i++) onlyFileName += fullPath[i];
                
                for (int j = 0; j <= 1 && !found; ++j)
                {
                    if (j == 1) {onlyFolder = base::utf16(resourcePath()) + onlyFolder; fullPath = base::utf16(resourcePath()) + fullPath;}
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
            } else fullPath = base::utf16(resourcePath()) + fullPath;
        } else if (mode == 0 && !base::DoesFileExistWithResolutionClass(fullPath)) fullPath = base::utf16(resourcePath()) + fullPath;
        
        return fullPath;
    }
    std::wstring PathWithResolutionDetermination(const std::wstring& imageName, unsigned int mode)
    {
        std::wstring fullPath = PathWithFolderDetermination(imageName, mode);
        if (gs::isResolutionClassEnabled)
        {
            std::wstring getExtention = base::GetExtentionFromString(fullPath);
            std::wstring woExtention = base::GetStringWithNoExtention(fullPath);
            
            switch (gs::resolutionClass)
            {
                case -1: break;
                case 0:
                    if (base::FileExists(woExtention + L"@0x" + getExtention)) fullPath = woExtention + L"@0x" + getExtention;
                    else if (base::FileExists(fullPath));
                    else if (base::FileExists(woExtention + L"@1x" + getExtention)) fullPath = woExtention + L"@1x" + getExtention;
                    else if (base::FileExists(woExtention + L"@2x" + getExtention)) fullPath = woExtention + L"@2x" + getExtention;
                    else if (base::FileExists(woExtention + L"@3x" + getExtention)) fullPath = woExtention + L"@3x" + getExtention;
                    break;
                    
                case 1:
                    if (base::FileExists(woExtention + L"@1x" + getExtention)) fullPath = woExtention + L"@1x" + getExtention;
                    else if (base::FileExists(fullPath));
                    else if (base::FileExists(woExtention + L"@2x" + getExtention)) fullPath = woExtention + L"@2x" + getExtention;
                    else if (base::FileExists(woExtention + L"@0x" + getExtention)) fullPath = woExtention + L"@0x" + getExtention;
                    else if (base::FileExists(woExtention + L"@3x" + getExtention)) fullPath = woExtention + L"@3x" + getExtention;
                    break;
                    
                case 2:
                    if (base::FileExists(woExtention + L"@2x" + getExtention)) fullPath = woExtention + L"@2x" + getExtention;
                    else if (base::FileExists(fullPath));
                    else if (base::FileExists(woExtention + L"@1x" + getExtention)) fullPath = woExtention + L"@1x" + getExtention;
                    else if (base::FileExists(woExtention + L"@3x" + getExtention)) fullPath = woExtention + L"@3x" + getExtention;
                    else if (base::FileExists(woExtention + L"@0x" + getExtention)) fullPath = woExtention + L"@0x" + getExtention;
                    break;
                    
                case 3:
                    if (base::FileExists(woExtention + L"@3x" + getExtention)) fullPath = woExtention + L"@3x" + getExtention;
                    else if (base::FileExists(fullPath));
                    else if (base::FileExists(woExtention + L"@2x" + getExtention)) fullPath = woExtention + L"@2x" + getExtention;
                    else if (base::FileExists(woExtention + L"@1x" + getExtention)) fullPath = woExtention + L"@1x" + getExtention;
                    else if (base::FileExists(woExtention + L"@0x" + getExtention)) fullPath = woExtention + L"@0x" + getExtention;
                    break;
                    
                default:
                    if (base::FileExists(woExtention + L"@" + std::to_wstring(gs::resolutionClass) + L"x" + getExtention)) fullPath = woExtention + L"@" + std::to_wstring(gs::resolutionClass) + L"x" + getExtention;
                    else if (base::FileExists(fullPath));
                    else if (base::FileExists(woExtention + L"@3x" + getExtention)) fullPath = woExtention + L"@3x" + getExtention;
                    else if (base::FileExists(woExtention + L"@2x" + getExtention)) fullPath = woExtention + L"@2x" + getExtention;
                    else if (base::FileExists(woExtention + L"@1x" + getExtention)) fullPath = woExtention + L"@1x" + getExtention;
                    else if (base::FileExists(woExtention + L"@0x" + getExtention)) fullPath = woExtention + L"@0x" + getExtention;
                    break;
            }
        }
        else
        {
            if (!base::FileExists(fullPath))
            {
                std::wstring getExtention = base::GetExtentionFromString(fullPath);
                std::wstring woExtention = base::GetStringWithNoExtention(fullPath);
                
                if (base::FileExists(sf::String(woExtention + L"@3x" + getExtention))) fullPath = woExtention + L"@3x" + getExtention;
                else if (base::FileExists(sf::String(woExtention + L"@2x" + getExtention))) fullPath = woExtention + L"@2x" + getExtention;
                else if (base::FileExists(sf::String(woExtention + L"@1x" + getExtention))) fullPath = woExtention + L"@1x" + getExtention;
                else if (base::FileExists(sf::String(woExtention + L"@0x" + getExtention))) fullPath = woExtention + L"@0x" + getExtention;
            }
        }
        
        return fullPath;
    }
}
