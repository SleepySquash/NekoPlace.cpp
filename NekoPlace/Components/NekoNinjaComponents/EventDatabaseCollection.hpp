//
//  EventDatabaseCollection.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 07/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef EventDatabaseCollection_hpp
#define EventDatabaseCollection_hpp

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <list>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/Settings.hpp"

#include "EventBase.hpp"
#include "NekoNinjaSettings.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoNinja
{
    namespace edc
    {
        struct GravityChanged : EventBase
        {
            GravityChanged() : EventBase("GravityChanged", L"Гравитация изменила своё направление") { }
            
            // Меняем y-координату спавна, меняем знак стартовой velocity и знак гравитации.
            nns::yySpawnMode befspawnCoordsYYmode;
            
            void OnAction() override;
            void OnEnd() override;
        };
        
        struct GravityShuffle : EventBase
        {
            GravityShuffle() : EventBase("GravityShuffle", L"Гравитация меняет своё направление") { requireAlert = false; }
            nns::yySpawnMode befspawnCoordsYYmode;
            
            void OnAction() override;
            void OnEnd() override;
        };
    }
}

#endif /* EventDatabaseCollection_hpp */
