//
//  EventDatabaseCollection.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 07/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "EventDatabaseCollection.hpp"

namespace NekoNinja
{
    namespace edc
    {
        void GravityChanged::OnAction()
        {
            befspawnCoordsYYmode = nns::spawnCoordsYYmode;
            nns::spawnCoordsYYmode = nns::yySpawnMode::top;
        }
        void GravityChanged::OnEnd() { nns::spawnCoordsYYmode = befspawnCoordsYYmode; }
        
        
        void GravityShuffle::OnAction()
        {
            befspawnCoordsYYmode = nns::spawnCoordsYYmode;
            nns::spawnCoordsYYmode = nns::yySpawnMode::random;
        }
        void GravityShuffle::OnEnd() { nns::spawnCoordsYYmode = befspawnCoordsYYmode; }
    }
}
