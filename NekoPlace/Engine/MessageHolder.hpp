//
//  MessageHolder.hpp
//  NovelSome
//
//  Created by Никита Исаенко on 06/03/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#ifndef MessageHolder_hpp
#define MessageHolder_hpp

#include <iostream>

namespace ns
{
    struct MessageHolder
    {
        std::string info{ "" }; std::wstring additional{ L"" };
        void* address{ nullptr };
        
        MessageHolder();
        MessageHolder(const std::string& info, void* address = nullptr);
        MessageHolder(const std::string& info, const std::wstring& additional, void* address = nullptr);
    };
    
    struct MessageSender
    {
        virtual void SendMessage(MessageHolder message) = 0;
    };
}

#endif /* MessageHolder_hpp */
