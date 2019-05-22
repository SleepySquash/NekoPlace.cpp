//
//  ItemBase.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 11/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef ItemBase_hpp
#define ItemBase_hpp

#include <iostream>

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Color.hpp>

#include "../../Engine/MessageHolder.hpp"

using std::cin;
using std::cout;
using std::endl;

namespace NekoNinja
{
    struct ItemBase
    {
        std::string name; std::wstring display{ L"Item" }, description;
        unsigned char rarity; // 0 - 100
        sf::Color color;
        unsigned int count{ 0 };
        bool usable, giftable;
        
        ItemBase(const std::string& name = "Item", const std::wstring& description = L"", const unsigned char& rarity = 0, bool giftable = true, bool usable = false);
    };
}

#endif /* ItemBase_hpp */
