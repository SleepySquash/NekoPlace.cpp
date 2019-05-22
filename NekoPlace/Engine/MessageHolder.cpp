//
//  MessageHolder.cpp
//  NovelSome
//
//  Created by Никита Исаенко on 06/03/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "MessageHolder.hpp"

namespace ns
{
    MessageHolder::MessageHolder() { }
    MessageHolder::MessageHolder(const std::string& info, void* address) : info(info), address(address) { }
    MessageHolder::MessageHolder(const std::string& info, const std::wstring& additional, void* address) : info(info), additional(additional), address(address) { }
}
