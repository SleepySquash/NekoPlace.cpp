//
//  NekoLibrary.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 27/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "NekoLibrary.hpp"

namespace NekoNinja
{
    vector<NekoBase*> nl::neko;
    namespace nekoLibrary { void clear() { for (auto& n : nl::neko) delete n; nl::neko.clear(); } }
}
