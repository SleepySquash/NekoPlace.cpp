//
//  NekoNinjaSettings.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 28/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "NekoNinjaSettings.hpp"

namespace NekoNinja
{
    float nns::timeMultiplier = 1.f, nns::getHarderTimeMultiplier = 1.f, nns::velocityMultiplier = 1.f;
    float nns::scoreMultiplier = 1.f;
    unsigned int nns::lifeScoreDelta = 2000, nns::maxLifes = 3;
    float nns::difficultyLevelUpTime = 24.f; // 36.f;
    int nns::poopPosibility = 28;
    
    float nns::gravity = 0.19f * 60 * 60, nns::yStartVelocity = -17.f * 60;
    int nns::yStartRandomVelocity = 4*60, nns::xStartRandomVelocity = 6*60;
    
    nns::yySpawnMode nns::spawnCoordsYYmode = nns::yySpawnMode::bottom;
    bool nns::randomSpawnYYmode = false;
    
    bool nns::showEventDescription = false;
}
