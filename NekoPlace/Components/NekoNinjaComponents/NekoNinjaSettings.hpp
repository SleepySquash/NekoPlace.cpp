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
    };
    typedef NekoNinjaSettings nns;
}

#endif /* NekoNinjaSettings_hpp */
