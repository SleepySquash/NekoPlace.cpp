//
//  Player.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 17/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef Player_hpp
#define Player_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/EntitySystem.hpp"
#include "../../Engine/Settings.hpp"
#include "../../Engine/Collectors.hpp"
#include "../../Engine/GUIInterface.hpp"

#include "Player.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoUI
{
    struct Player
    {
        static void Load();
    };
}

#endif /* Player_hpp */
