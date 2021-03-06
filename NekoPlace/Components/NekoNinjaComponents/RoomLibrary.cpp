//
//  RoomLibrary.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 27/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "RoomLibrary.hpp"

namespace NekoNinja
{
    vector<NekoEntity*> RoomLibrary::neko;
    vector<NekoEntity*> RoomLibrary::nekolist;
    namespace roomLibrary
    {
        void clear() { for (auto& n : rl::neko) { n->Destroy(); delete n; } rl::neko.clear(); }
        void sortlist() { std::sort(rl::nekolist.begin(), rl::nekolist.end(), [](const NekoEntity* a, const NekoEntity* b) { return a->info->chance < b->info->chance; }); }
    }
}
