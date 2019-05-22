//
//  ItemBase.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 11/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "ItemBase.hpp"

namespace NekoNinja
{
    ItemBase::ItemBase(const std::string& name, const std::wstring& description, const unsigned char& rarity, bool giftable, bool usable) : name(name), description(description), rarity(rarity), giftable(giftable), usable(usable)
    {
        if (rarity == 100)     color = sf::Color(255, 219, 0, 255);
        else if (rarity >= 85) color = sf::Color(244, 200, 68, 120);
        else if (rarity >= 70) color = sf::Color(188, 106, 167, 120);
        else if (rarity >= 50) color = sf::Color(218, 113, 114, 120);
        else if (rarity >= 30) color = sf::Color(87, 173, 98, 120);
        else if (rarity >= 10) color = sf::Color(106, 143, 203, 120);
        else                   color = sf::Color(60, 60, 60, 100);
    }
}
