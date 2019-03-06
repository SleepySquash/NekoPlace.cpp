//
//  Popup.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 03/03/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef Popup_hpp
#define Popup_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../Engine/EntitySystem.hpp"
#include "../../Engine/StaticMethods.hpp"
#include "../../Engine/GUIInterface.hpp"
#include "../../Engine/NovelSomeScript.hpp"

using std::cin;
using std::cout;
using std::endl;

using namespace ns;

namespace NekoNinja
{
    struct PopupInfo
    {
        std::wstring desc, imagePath;
        PopupInfo(const std::wstring& desc = L"", const std::wstring& imagePath = L"");
    };
    struct Popup : Component
    {
        sf::RectangleShape shape, backfade, line;
        std::wstring title, desc;
        GUI::TextButton button;
        sf::Text caption, info;
        
        Popup(const std::wstring& title = L"Caption", const std::wstring& desc = L"");
        void Init() override;
        void Destroy() override;
        void PollEvent(sf::Event&) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
    };
}

#endif /* Popup_hpp */
