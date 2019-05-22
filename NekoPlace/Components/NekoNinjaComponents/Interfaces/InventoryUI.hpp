//
//  Inventory.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 12/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef InventoryUI_hpp
#define InventoryUI_hpp

#include <iostream>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../../Essentials/ResourcePath.hpp"
#include "../../../Engine/Settings.hpp"
#include "../../../Engine/Collectors/Font.hpp"
#include "../../../Engine/Collectors/Image.hpp"
#include "../../../Engine/EntitySystem.hpp"
#include "../../../Engine/GUIInterface.hpp"

#include "../ItemDatabaseCollection.hpp"
#include "../Player.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

namespace NekoNinja
{
    struct InventoryUI : Component
    {
        bool active{ false };
        
        bool itemIsSelected{ false }, giftSelection{ false }; ItemBase* selectedItem{ nullptr };
        sf::Sprite itemSprite; std::wstring itemDescription{ L"" }, itemTexturePath{ L"" };
        GUI::RectangleButtons itemButtons; sf::Texture* itemTexture{ nullptr };
        GUI::RectangleButton itemUseButton;
        sf::RectangleShape itemDescriptionShape, backShape;
        sf::Text itemText, helperText; float itemScrollYY{ 100 };
        GUI::TextButton closeButton;
        
        void Init() override;
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void RecieveMessage(MessageHolder& message) override;
        void SelectItem(ItemBase* item);
        void Switch(const bool& on);
    };
    
    struct LootboxUI : Component
    {
        bool active{ false };
        
        sf::RectangleShape lootboxWindowShape, backShape;
        sf::Sprite lootboxSprite;
        GUI::TextButton openButton, closeButton;
        
        void Init() override;
        void PollEvent(sf::Event& event) override;
        void Resize(unsigned int width, unsigned int height) override;
        void Draw(sf::RenderWindow* window) override;
        void RecieveMessage(MessageHolder& message) override;
        void Switch(const bool& on);
    };
}

#endif /* InventoryUI_hpp */
