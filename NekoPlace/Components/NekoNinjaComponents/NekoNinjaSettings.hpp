//
//  NekoNinjaSettings.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 28/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef NekoNinjaSettings_hpp
#define NekoNinjaSettings_hpp

#include <iostream>

using std::cin;
using std::cout;
using std::endl;

namespace NekoNinja
{
    struct NekoNinjaSettings
    {
        static float timeMultiplier, getHarderTimeMultiplier, velocityMultiplier;
        static float scoreMultiplier;
        static unsigned int lifeScoreDelta, maxLifes;
        static float difficultyLevelUpTime;
        static int poopPosibility;
        
        static float gravity;
        static float yStartVelocity;
        static int yStartRandomVelocity, xStartRandomVelocity;
        
        enum class yySpawnMode { top, bottom, random };
        static yySpawnMode spawnCoordsYYmode;
        static bool randomSpawnYYmode;
        
        static bool showEventDescription;
    };
    typedef NekoNinjaSettings nns;
}

#endif /* NekoNinjaSettings_hpp */
