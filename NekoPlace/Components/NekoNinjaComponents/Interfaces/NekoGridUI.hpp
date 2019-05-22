//
//  NekoGridUI.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 16/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef NekoGridUI_hpp
#define NekoGridUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Essentials/ResourcePath.hpp"
#include "../../../Engine/Settings.hpp"
#include "../../../Engine/Collectors/Font.hpp"
#include "../../../Engine/Collectors/Image.hpp"
#include "../../../Engine/EntitySystem.hpp"
#include "../../../Engine/GUIInterface.hpp"

#include "../RoomLibrary.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

namespace NekoNinja
{
    struct NekoGridUI : Component
    {
        bool active{ false };
        
        sf::RectangleShape blackScreenShape;
        bool nekoSelection{ false }, selectActive{ false }, selectPassive{ false };
        sf::Texture* crossTexture{ nullptr };
        
        NekoBase* highlightedNeko{ nullptr };
        sf::RectangleShape nlstNekoShape;
        GUI::SpriteButtons nlstNekoButton;
        GUI::SpriteButton quitButton;
        
        void Init() override;
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void RecieveMessage(MessageHolder& message) override;
        void Switch(const bool& on);
    };
}

#endif /* NekoGridUI_hpp */
