//
//  RoomLibrary.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 18/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef RoomLibrary_hpp
#define RoomLibrary_hpp

#include <iostream>

#include <SFML/Graphics.hpp>

#include "../../Engine/Settings.hpp"
#include "../../Engine/Collectors.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    struct RoomLibrary
    {
        static float x, y, xWidth, yHeight, scale, roomScale;
        static int width, height;
        static sf::Image* mask;
    };
    
    typedef RoomLibrary Room;
    typedef RoomLibrary rm;
}

#endif /* RoomLibrary_hpp */
