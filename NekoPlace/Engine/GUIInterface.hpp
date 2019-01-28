//
//  GUIInterface.hpp
//  AlternativeTransport-MLaToA2018
//
//  Created by Никита Исаенко on 22/11/2018.
//  Copyright © 2018 Melancholy Hill. All rights reserved.
//

#ifndef GUIInterface_hpp
#define GUIInterface_hpp

#include <SFML/Graphics.hpp>

#include "../Essentials/Base.hpp"
#include "StaticMethods.hpp"

namespace ns
{
    class GUIButton
    {
    public:
        bool isPressed{ false };
        float pressedAfterColor{ 0.f };
        bool fontLoaded{ false };
        
        sf::Text text;
        sf::String string;
        unsigned int characterSize{ 20 };
        float thickness{ 1.f };
        
        enum class halignEnum { left, center, right };
        enum class valignEnum { top, center, bottom };
        halignEnum halign{ halignEnum::center };
        valignEnum valign{ valignEnum::top };
        
        GUIButton();
        void Update(const sf::Time& elapsedTime);
        void Draw(sf::RenderTarget* window);
        void Resize(unsigned int width, unsigned int height);
        void PollEvent(sf::Event& event);
        bool IsPressed();
        void SetPosition(float x, float y);
        void SetFont(const std::wstring& fontname);
        void SetString(const std::wstring& string);
        void SetCharacterSize(const unsigned int size);
    };
}

#endif /* GUIInterface_hpp */
