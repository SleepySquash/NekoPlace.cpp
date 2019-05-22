//
//  etc.hpp
//  NovelSome
//
//  Created by Никита Исаенко on 17/03/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef SupportingCollectorMethods_hpp
#define SupportingCollectorMethods_hpp

#include <iostream>
#include <unordered_map>

#include <thread>
#include <codecvt>
#include <fstream>
#include <memory>

#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Essentials/Base.hpp"
#include "../Settings.hpp"

using base::utf8;
using base::utf16;
using std::pair;
using std::make_pair;
using std::cout;
using std::endl;

namespace ns
{
    std::wstring PathWithFolderDetermination(const std::wstring& fileName, unsigned int mode);
    std::wstring PathWithResolutionDetermination(const std::wstring& fileName, unsigned int mode);
}

#endif /* SupportingCollectorMethods_hpp */
