//
//  NekoInterfaceUI.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 16/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef NekoInterfaceUI_hpp
#define NekoInterfaceUI_hpp

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
    struct NekoInterfaceUI : Component
    {
        bool active{ false };
        
        sf::RectangleShape blackScreenShape;
        enum class lookingMode { room, select, look };
        lookingMode looking{ lookingMode::room };
        
        NekoEntity* nintNeko{ nullptr };
        sf::RectangleShape nintShape; sf::Text nintNameText, nintText;
        sf::Sprite nintNekoSprite; GUI::TextButton nintCloseButton, nintSelectMainButton;
        GUI::SpriteButton quitButton;
        sf::Color nintRarityColor{ sf::Color::White }, nintMoodColor{ sf::Color::White };
        std::wstring nintDescriptionText{ L"" }, nintRarityText{ L"" }, nintMoodText{ L"" };
        float nintYYInfoStart{ 0 };
        bool nintDontDrawPersonNeko{ false };
        sf::Sprite nintBodyNeko, nintHeart; float nintBodyNekoScale{ 1.f };
        GUI::TextButton nintTalkButton, nintWardrobeButton, nintGiftButton, nintActionButton, nintChoose;
        
        AbilityBase* nintAbility{ nullptr };
        sf::RectangleShape nintAbilityShape;
        sf::Text nintAbilityText;
        std::wstring nintAbilityDescription{ L"" };
        sf::Sprite nintAbilitySprite; bool nintAbilitySpriteLoaded{ false };
        
        void Init() override;
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void RecieveMessage(MessageHolder& message) override;
        void Switch(const bool& on);
        void OpenNekoInterface(NekoEntity* entity);
    };
}

#endif /* NekoInterfaceUI_hpp */
