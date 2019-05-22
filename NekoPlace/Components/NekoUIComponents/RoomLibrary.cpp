//
//  RoomLibrary.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 18/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "RoomLibrary.hpp"

namespace NekoUI
{
    float Room::x{ 0 }, Room::y{ 0 }, Room::xWidth{ 0 }, Room::yHeight{ 0 }, Room::scale{ 1.f }, Room::roomScale{ 3.3f };
    int Room::width{ 1 }, Room::height{ 1 };
    sf::Image* Room::mask{ nullptr };
}
