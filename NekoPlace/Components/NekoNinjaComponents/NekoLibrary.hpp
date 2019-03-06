//
//  NekoLibrary.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 27/02/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef NekoLibrary_hpp
#define NekoLibrary_hpp

#include <iostream>
#include <vector>
#include <memory>

#include "NekoBase.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using ns::base::utf8;
using ns::base::utf16;

using namespace ns;

namespace NekoNinja
{
    struct NekoLibrary { static vector<NekoBase*> neko; };
    namespace nekoLibrary { void clear(); }
    typedef NekoLibrary nl;
}

#endif /* NekoLibrary_hpp */
