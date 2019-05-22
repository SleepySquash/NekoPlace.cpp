//
//  Novel.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 24/02/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "Novel.hpp"

namespace ns
{
    namespace VNLightComponents
    {
        void Waiting::Update(const sf::Time& elapsedTime)
        {
            if (currentTime < waitingTime) currentTime += elapsedTime.asSeconds();
            else { if (novel) novel->UnHold(this); novelSystem->PopComponent(this); }
        }
        
        
        
        
        
        
        void RectangleShape::Init() { shape.setFillColor(sf::Color(0,0,0,0)); }
        void RectangleShape::Resize(unsigned int width, unsigned int height) { shape.setSize({(float)width, (float)height}); }
        void RectangleShape::Update(const sf::Time& elapsedTime)
        {
            switch (mode)
            {
                case appearing:
                    if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= appearTime)
                    {
                        alpha = maxAlpha; currentTime = 0.f; mode = existing;
                        if (novel && sendMessageBack == atAppearance) novel->UnHold(this);
                    }
                    else alpha = (sf::Int8)(maxAlpha * (currentTime / appearTime));
                    shape.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, alpha));
                    gs::requestWindowRefresh = true;
                    break;
                case disappearing:
                    if (currentTime < disappearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= disappearTime)
                    {
                        alpha = 0; currentTime = 0.f; mode = deprecated;
                        if (novel && sendMessageBack == atDeprecated) novel->UnHold(this);
                    }
                    else alpha = (sf::Int8)(maxAlpha - (maxAlpha * (currentTime / disappearTime)));
                    shape.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, alpha));
                    gs::requestWindowRefresh = true;
                    break;
                case deprecated: novelSystem->PopComponent(this); break;
                default: break;
            }
        }
        void RectangleShape::Draw(sf::RenderWindow* window) { if (visible) window->draw(shape); }
        
        
        
        
        
        
        
        
        void Background::LoadImage(const sf::String& path)
        {
            spriteLoaded = false;
            if (novel != nullptr)
            {
                sf::Texture* texturePtr = ic::LoadTexture(novel->GetFolderPath() + path, 1);
                if (texturePtr != nullptr)
                {
                    imagePath = novel->GetFolderPath() + path;
                    spriteLoaded = true;
                    sprite.setTexture(*texturePtr);
                    Resize(gs::width, gs::height);
                }
                
                if (!spriteLoaded)
                {
                    if (sendMessageBack != noMessage) novel->UnHold(this);
                    novelSystem->PopComponent(this);
                }
            }
            else
                cout << "Error :: BackgroundComponent :: LoadImage :: No novel was loaded, pointer is NULL" << endl;
        }
        void Background::Resize(unsigned int width, unsigned int height)
        {
            CalculateScale(width, height);
            if (doParallax && !gs::isPause)
                CalculateParallax(sf::Mouse::getPosition(*ns::gs::window).x, sf::Mouse::getPosition(*ns::gs::window).y);
        }
        void Background::Update(const sf::Time& elapsedTime)
        {
            switch (mode)
            {
                case appearing:
                    if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= appearTime)
                    {
                        alpha = maxAlpha; currentTime = 0.f;
                        mode = existing;
                        if (novel && sendMessageBack == atAppearance) novel->UnHold(this);
                    }
                    else
                        alpha = (sf::Int8)(maxAlpha * (currentTime / appearTime));
                    sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, alpha));
                    gs::requestWindowRefresh = true;
                    break;
                    
                case disappearing:
                    if (currentTime < disappearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= disappearTime)
                    {
                        alpha = 0; currentTime = 0.f;
                        mode = deprecated;
                        if (novel && sendMessageBack == atDeprecated) novel->UnHold(this);
                    }
                    else
                        alpha = (sf::Int8)(maxAlpha - (maxAlpha * (currentTime / disappearTime)));
                    sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, alpha));
                    gs::requestWindowRefresh = true;
                    break;
                    
                case deprecated: novelSystem->PopComponent(this); break;
                default: break;
            }
        }
        void Background::Draw(sf::RenderWindow* window) { if (spriteLoaded && visible) window->draw(sprite); }
        void Background::Destroy() { if (imagePath.length() != 0) ic::DeleteImage(imagePath); }
        void Background::PollEvent(sf::Event& event)
        {
            if (event.type == sf::Event::MouseMoved && mode != deprecated && visible && doParallax && parallaxPower > 0)
                CalculateParallax(event.mouseMove.x, event.mouseMove.y);
        }
        void Background::CalculateScale(unsigned int width, unsigned int height)
        {
            if (spriteLoaded)
            {
                float scaleFactorX, scaleFactorY, scaleFactor;
                scaleFactorX = (float)width / (sprite.getTexture()->getSize().x) * (doParallax? 1 + parallaxPower : 1) * scaleX;
                scaleFactorY = (float)height / (sprite.getTexture()->getSize().y) * (doParallax? 1 + parallaxPower : 1) * scaleY;
                
                scaleFactor = (scaleFactorX > scaleFactorY) ? scaleFactorX : scaleFactorY;
                sprite.setScale(scaleFactor, scaleFactor);
                defaultPositionX = (float)ns::gs::width/2 - sprite.getLocalBounds().width/2*sprite.getScale().x - sprite.getOrigin().x*sprite.getScale().x;
                defaultPositionY = (float)ns::gs::height/2 - sprite.getLocalBounds().height/2*sprite.getScale().y - sprite.getOrigin().y*sprite.getScale().y;
                sprite.setPosition(defaultPositionX, defaultPositionY);
            }
        }
        void Background::CalculateParallax(int mouseX, int mouseY)
        {
            if (mouseX >= 0 && mouseY >= 0 && mouseX <= gs::width && mouseY <= gs::height)
            {
                float posX = defaultPositionX + (int)(mouseX - gs::width/2) * parallaxPower;
                float posY = defaultPositionY + (int)(mouseY - gs::height/2) * parallaxPower;
                sprite.setPosition(posX, posY);
            }
        }
        
        
        
        
        
        
        
        Dialogue::Dialogue(Novel* novel) { this->novel = novel; }
        void Dialogue::Init()
        {
            sf::Texture* texture = ic::LoadTexture(L"Data/Images/dialogue1.png");
            if (texture)
            {
                dialogueSprite.setTexture(*texture);
                dialogueSprite.setOrigin(texture->getSize().x/2, texture->getSize().y);
            }
        }
        void Dialogue::Update(const sf::Time& elapsedTime)
        {
            if (mode != deprecated && textAppearMode == textAppearModeEnum::printing && textAppearPos < textAppearMax)
            {
                elapsedCharacterSum += elapsedTime.asSeconds();
                while (elapsedCharacterSum > characterInSecond && textAppearPos < textAppearMax)
                {
                    ++textAppearPos; elapsedCharacterSum -= characterInSecond;
                    while (printingString[textAppearI] == L'\n' || printingString[textAppearI] == L' ')
                        currentString += printingString[textAppearI++];
                    currentString += printingString[textAppearI++];
                    text.setString(currentString);
                }
            }
            switch (mode)
            {
                case appearing:
                    if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= appearTime)
                    {
                        alpha = maxAlpha; currentTime = 0.f;
                        mode = afterAppearSwitchTo;
                        if (novel && sendMessageBack == atAppearance) novel->UnHold(this);
                    }
                    else alpha = (sf::Int8)(maxAlpha * (currentTime / appearTime));
                    gs::requestWindowRefresh = true;
                    
                    text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, alpha));
                    dialogueSprite.setColor(sf::Color(dialogueSprite.getColor().r, dialogueSprite.getColor().g, dialogueSprite.getColor().b, alpha));
                    if (text.getOutlineThickness() != 0)
                        text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, alpha));
                    if (drawCharacterName)
                    {
                        nameSprite.setColor(sf::Color(nameSprite.getColor().r, nameSprite.getColor().g, nameSprite.getColor().b, alpha));
                        charText.setFillColor(sf::Color(charText.getFillColor().r, charText.getFillColor().g, charText.getFillColor().b, alpha));
                        if (charText.getOutlineThickness() != 0)
                            charText.setOutlineColor(sf::Color(charText.getOutlineColor().r, charText.getOutlineColor().g, charText.getOutlineColor().b, alpha));
                    }
                    break;
                    
                case disappearing:
                    if (currentTime < disappearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= disappearTime)
                    {
                        alpha = 0; currentTime = 0.f; mode = deprecated;
                        if (novel && sendMessageBack == atDeprecated) novel->UnHold(this);
                    }
                    else alpha = (sf::Int8)(maxAlpha - (maxAlpha * (currentTime / disappearTime)));
                    gs::requestWindowRefresh = true;
                    
                    text.setFillColor(sf::Color(text.getFillColor().r, text.getFillColor().g, text.getFillColor().b, alpha));
                    dialogueSprite.setColor(sf::Color(dialogueSprite.getColor().r, dialogueSprite.getColor().g, dialogueSprite.getColor().b, alpha));
                    if (text.getOutlineThickness() != 0)
                        text.setOutlineColor(sf::Color(text.getOutlineColor().r, text.getOutlineColor().g, text.getOutlineColor().b, alpha));
                    if (drawCharacterName)
                    {
                        nameSprite.setColor(sf::Color(nameSprite.getColor().r, nameSprite.getColor().g, nameSprite.getColor().b, alpha));
                        charText.setFillColor(sf::Color(charText.getFillColor().r, charText.getFillColor().g, charText.getFillColor().b, alpha));
                        if (charText.getOutlineThickness() != 0)
                            charText.setOutlineColor(sf::Color(charText.getOutlineColor().r, charText.getOutlineColor().g, charText.getOutlineColor().b, alpha));
                    }
                    break;
                case deprecated: novelSystem->PopComponent(this); break;
                case waitingForTime:
                    if (currentTime < waitingTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= waitingTime)
                    {
                        currentTime = 0.f; mode = disappearing;
                        if (novel && sendMessageBack == atDisappearing) novel->UnHold(this);
                    }
                    break;
                default: break;
            }
        }
        void Dialogue::PollEvent(sf::Event& event)
        {
            if (mode == waitingForInput)
            {
                if (visible && (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::TouchEnded))
                {
                    bool pressed{ ((event.type == sf::Event::MouseButtonPressed && workingArea.contains(event.mouseButton.x, event.mouseButton.y)) || (event.type == sf::Event::TouchEnded && workingArea.contains(event.touch.x, event.touch.y))) };
                    
                    if (pressed)
                    {
                        bool fadeAway{ true };
                        if (textAppearMode == textAppearModeEnum::printing && textAppearPos != textAppearMax)
                            currentString = printingString, text.setString(currentString), textAppearPos = textAppearMax, fadeAway = false;
                        
                        event = sf::Event();
                        if (fadeAway)
                        {
                            mode = disappearing;
                            if (novel && sendMessageBack == atDisappearing) novel->UnHold(this);
                        }
                    }
                }
            }
            else if (mode == waitingForChoose)
            {
                if (visible && (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::TouchEnded) && textAppearMode == textAppearModeEnum::printing && textAppearPos != textAppearMax)
                {
                    bool pressed{ ((event.type == sf::Event::MouseButtonPressed && workingArea.contains(event.mouseButton.x, event.mouseButton.y)) || (event.type == sf::Event::TouchEnded && workingArea.contains(event.touch.x, event.touch.y))) };
                    if (pressed)
                    {
                        currentString = printingString;
                        text.setString(currentString);
                        textAppearPos = textAppearMax;
                        
                        event = sf::Event();
                    }
                }
            }
            if ((mode == waitingForChoose || mode == waitingForInput) && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
                visible = !visible;
        }
        void Dialogue::Draw(sf::RenderWindow* window)
        {
            if (visible && fontLoaded)
            {
                window->draw(dialogueSprite);
                window->draw(text);
                if (drawCharacterName)
                {
                    window->draw(nameSprite);
                    window->draw(charText);
                }
            }
        }
        void Dialogue::Destroy()
        {
            ic::DeleteImage(L"Data/Images/dialogue1.png");
            if (drawCharacterName) ic::DeleteImage(L"Data/Images/dialogue1_n.png");
        }
        void Dialogue::Resize(unsigned int width, unsigned int height)
        {
            charText.setCharacterSize((unsigned int)(characterSize * gs::scale));
            text.setCharacterSize((unsigned int)(characterSize * gs::scale));
            
            if (fontLoaded)
            {
                text.setOutlineThickness(outlineThickness * gs::scale);
                if (drawCharacterName && nekoInfo) charText.setOutlineThickness(outlineThickness * gs::scale);
                
                int textWidth = width, textXOffset = 0, textYOffset = 0;
                int nameTextWidth = width, nameTextXOffset = 0, nameTextYOffset = 0;
                dialogueSprite.setPosition(width/2, height - 10*gs::scaley);
                dialogueSprite.setScale(gs::scScale, gs::scScale);
                if (dialogueSprite.getGlobalBounds().width > gs::width)
                    dialogueSprite.setScale(gs::scScale*gs::width/(dialogueSprite.getGlobalBounds().width), dialogueSprite.getScale().y);
                workingArea = { (int)dialogueSprite.getGlobalBounds().left, (int)dialogueSprite.getGlobalBounds().top, (int)dialogueSprite.getGlobalBounds().width, (int)dialogueSprite.getGlobalBounds().height };
                
                if (drawCharacterName)
                {
                    if (charString != L"") charText.setString(nss::GetStringWithLineBreaks(charText, charString, nameTextWidth, 0));
                    nameSprite.setPosition(dialogueSprite.getGlobalBounds().left + dialogueSprite.getGlobalBounds().width/13, dialogueSprite.getGlobalBounds().top + 32*gs::scale);
                    nameSprite.setScale((charText.getGlobalBounds().width + 30*gs::scale)/(nameSprite.getLocalBounds().width), gs::scale);
                    nameTextXOffset = nameSprite.getGlobalBounds().left + 15*gs::scale;
                    nameTextYOffset = nameSprite.getGlobalBounds().top + 7*gs::scale;
                }
                
                textXOffset = workingArea.left + 90*gs::scScale;
                textYOffset = workingArea.top + 20*gs::scScale;
                textWidth = workingArea.width - 2*90*gs::scScale;
                
                if (textString != L"")
                {
                    if (textAppearMode ==  textAppearModeEnum::printing)
                    {
                        currentString = L""; textAppearI = 0;
                        printingString = nss::GetStringWithLineBreaks(text, textString, textWidth, 0);
                        for (size_t i = 0; i < textAppearPos; ++i)
                        {
                            while (printingString[textAppearI] == L'\n' || printingString[textAppearI] == L' ')
                                currentString += printingString[textAppearI++];
                            currentString += printingString[textAppearI++];
                        }
                        text.setString(currentString);
                    }
                    else text.setString(nss::GetStringWithLineBreaks(text, textString, textWidth, 0));
                }
                
                text.setPosition(textXOffset, textYOffset);
                if (drawCharacterName) charText.setPosition(nameTextXOffset, nameTextYOffset);
            }
        }
        void Dialogue::SetNeko(NekoBase *info)
        {
            if (info)
            {
                nekoInfo = info;
                charText.setOutlineThickness(outlineThickness);
                charText.setFillColor(sf::Color(nekoInfo->color.r, nekoInfo->color.g, nekoInfo->color.b, alpha));
                if ((nekoInfo->color.r + nekoInfo->color.g + nekoInfo->color.b)/3 > 127)
                    charText.setOutlineColor(sf::Color(0, 0, 0, alpha));
                else charText.setOutlineColor(sf::Color(255, 255, 255, alpha));
                //charText.setOutlineColor(sf::Color(nekoInfo->color.r, nekoInfo->color.g, nekoInfo->color.b, alpha));
            }
        }
        void Dialogue::SetCharacterName(const sf::String& characterName)
        {
            charString = characterName;
            drawCharacterName = true;
            
            charText.setString(charString);
            charText.setFont(*ns::FontCollector::GetFont(fontName));
            fontLoaded = (text.getFont() != nullptr);
            
            charText.setCharacterSize(characterSize);
            charText.setFillColor(sf::Color::White);
            
            charText.setFillColor(sf::Color(charText.getFillColor().r, charText.getFillColor().g, charText.getFillColor().b, alpha));
            if (charText.getOutlineThickness() != 0)
                charText.setOutlineColor(sf::Color(charText.getOutlineColor().r, charText.getOutlineColor().g, charText.getOutlineColor().b, alpha));
            
            sf::Texture* texture = ic::LoadTexture(L"Data/Images/dialogue1_n.png");
            if (texture)
            {
                nameSprite.setTexture(*texture);
                nameSprite.setOrigin(0, texture->getSize().y);
            }
        }
        void Dialogue::SetDialogue(const sf::String& dialogue)
        {
            textString = dialogue;
            printingString = textString;
            if (textAppearMode == textAppearModeEnum::printing)
            {
                currentString = L"", textAppearMax = base::GetLengthWONewLinesAndSpaces(textString);
                text.setString(currentString);
            } else text.setString(printingString);
            
            text.setFont(*ns::FontCollector::GetFont(fontName));
            fontLoaded = (text.getFont() != nullptr);
            
            text.setCharacterSize(characterSize);
            text.setFillColor(sf::Color::White);
            
            Resize(gs::width, gs::height);
        }
        
        
        
        
        
        
        
        
        void Character::LoadState(const sf::String& state)
        {
            spriteLoaded = false;
            if (nekoInfo && novel)
            {
                imagePath = L"Data/Neko/Person/" + nekoInfo->name + L".png";
                sf::Texture* texture = ic::LoadTexture(imagePath);
                if ((spriteLoaded = texture))
                {
                    sprite.setTexture(*texture, true);
                    relScale = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * nekoInfo->novelScale;
                    
                    if (state == L"c" || state == L"close") { parallaxPower = gs::defaultParallaxClose; scaleX = scaleY = 2; }
                    else if (state == L"f" || state == L"far") { parallaxPower = gs::defaultParallaxFar; scaleX = scaleY = 1; }
                    else { parallaxPower = gs::defaultParallaxNormal; scaleX = scaleY = 1.5;  }
                    
                    Resize(gs::width, gs::height);
                } else imagePath = L"";
            }
            
            if (novel && !spriteLoaded)
            {
                if (sendMessageBack != noMessage) novel->UnHold(this);
                novelSystem->PopComponent(this);
            }
        }
        void Character::Resize(unsigned int width, unsigned int height)
        {
            if (spriteLoaded)
            {
                sprite.setScale(relScale * scaleX * (doParallax ? (1 + parallaxPower) : 1) * gs::scScale, relScale * scaleY * (doParallax ? (1 + parallaxPower) : 1) * gs::scScale);
                sprite.setOrigin((nekoInfo ? nekoInfo->personSprite_offsetX : 0) + sprite.getLocalBounds().width/2, (nekoInfo ? nekoInfo->personSprite_offsetY : 0) + ((sprite.getLocalBounds().height > height/sprite.getScale().y) ? height/sprite.getScale().y : sprite.getLocalBounds().height));
                
                switch (position)
                {
                    case left: sprite.setPosition((float)width/6, height); break;
                    case cleft: sprite.setPosition((float)width/3, height); break;
                    case center: sprite.setPosition((float)width/2, height); break;
                    case cright: sprite.setPosition((width - (float)width/3), height); break;
                    case right: sprite.setPosition((width - (float)width/6), height); break;
                    default: break;
                }
                
                defaultPositionX = sprite.getPosition().x;
                defaultPositionY = sprite.getPosition().y;
                if (doParallax && !gs::isPause)
                    CalculateParallax(sf::Mouse::getPosition(*ns::gs::window).x, sf::Mouse::getPosition(*ns::gs::window).y);
            }
        }
        void Character::Update(const sf::Time& elapsedTime)
        {
            switch (mode)
            {
                case appearing:
                    if (currentTime < appearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= appearTime)
                    {
                        alpha = maxAlpha; currentTime = 0.f;
                        mode = afterAppearSwitchTo;
                        if (novel && sendMessageBack == atAppearance) novel->UnHold(this);
                    }
                    else alpha = (sf::Int8)(maxAlpha * (currentTime / appearTime));
                    sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, alpha));
                    gs::requestWindowRefresh = true;
                    break;
                    
                case disappearing:
                    if (currentTime < disappearTime) currentTime += elapsedTime.asSeconds();
                    if (currentTime >= disappearTime)
                    {
                        alpha = 0; currentTime = 0.f;
                        mode = deprecated;
                        if (novel && sendMessageBack == atDeprecated) novel->UnHold(this);
                    }
                    else
                        alpha = (sf::Int8)(maxAlpha - (maxAlpha * (currentTime / disappearTime)));
                    sprite.setColor(sf::Color(sprite.getColor().r, sprite.getColor().g, sprite.getColor().b, alpha));
                    gs::requestWindowRefresh = true;
                    break;
                case deprecated: novelSystem->PopComponent(this); break;
                default: break;
            }
        }
        void Character::Draw(sf::RenderWindow* window) { if (spriteLoaded && visible) window->draw(sprite); }
        void Character::Destroy() { if (imagePath.length() != 0) ic::DeleteImage(imagePath); }
        void Character::PollEvent(sf::Event& event)
        {
            if (event.type == sf::Event::MouseMoved && mode != deprecated && visible && doParallax && parallaxPower > 0)
                CalculateParallax(event.mouseMove.x, event.mouseMove.y);
        }
        void Character::CalculateParallax(int mouseX, int mouseY)
        {
            if (mouseX >= 0 && mouseY >= 0 && mouseX <= ns::gs::width && mouseY <= ns::gs::height)
            {
                float posX = defaultPositionX + (int)(mouseX - ns::gs::width/2) * parallaxPower;
                float posY = defaultPositionY + (int)(mouseY - ns::gs::height/2) * parallaxPower;
                sprite.setPosition(posX, posY);
            }
        }
        
        
        
        
        
        
        
        
        Novel::Novel() { }
        void Novel::Init() { gs::PushInterface(this); }
        void Novel::Destroy()
        {
            gs::RemoveInterface(this);
            gs::ignoreDraw = false;
            entity->SendMessage(MessageHolder("NovelComponents :: Novel :: Destroying"));
            layers.clear();
        }
        void Novel::Update(const sf::Time& elapsedTime)
        {
            eof = lines.empty();
            while (!eof && onHold.empty())
            {
                command.Command(lines.front()); lines.pop_front();
                eof = lines.empty(); gs::requestWindowRefresh = true;
                
                bool backgroundAddingMode{ false };
                if (nss::Command(command, L"//")) { /* oh, that's a comment... */ }
                ///---------------------------------------DIALOGUE---------------------------------------
                ///---------------------------------------DIALOGUE---------------------------------------
                ///---------------------------------------DIALOGUE---------------------------------------
                else if (nss::Command(command, L"\""))
                {
                    command.lastPos = command.lastPos - 1;
                    std::wstring dialogueLine = nss::ParseAsQuoteString(command);
                    auto* component = layers.PrioritizeComponent<Dialogue>(10000, this);
                    
                    vector<std::wstring> arguments;
                    nss::ParseArguments(command, arguments);
                    for (auto it = arguments.begin(); it != arguments.end(); ++it)
                    {
                        nss::CommandSettings argument; argument.Command(*it);
                        
                        if (nss::Command(argument, L"fade:"))
                        {
                            float value = nss::ArgumentAsFloat(argument);
                            component->appearTime = value;
                            component->disappearTime = value;
                        }
                        else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                            component->appearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                            component->disappearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"alpha:") || nss::Command(argument, L"maxalpha:"))
                            component->maxAlpha = nss::ArgumentAsInt(argument);
                        else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"atappearance" || stringValue == L"appearance")
                                component->sendMessageBack = component->atAppearance;
                            else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                component->sendMessageBack = component->atDisappearing;
                            else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                component->sendMessageBack = component->atDeprecated;
                            else if (stringValue == L"nomessage" || stringValue == L"no")
                                component->sendMessageBack = component->noMessage;
                        }
                        else if (nss::Command(argument, L"afterappearswitchto:") || nss::Command(argument, L"switchby:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"waitingforinput" || stringValue == L"input")
                                component->afterAppearSwitchTo = component->waitingForInput;
                            else if (stringValue == L"waitingfortime" || stringValue == L"time")
                                component->afterAppearSwitchTo = component->waitingForTime;
                        }
                        else if (nss::Command(argument, L"waitingtime:") || nss::Command(argument, L"time:"))
                            component->waitingTime = nss::ArgumentAsFloat(argument);
                    }
                    
                    if (component->sendMessageBack != component->noMessage) OnHold(component);
                    component->SetDialogue(dialogueLine);
                }
                ///----------------------------------------MISC----------------------------------------
                ///----------------------------------------MISC----------------------------------------
                ///----------------------------------------MISC----------------------------------------
                else if (nss::Command(command, L"wait "))
                {
                    nss::SkipSpaces(command);
                    float amount = ArgumentAsFloat(command);
                    
                    if (amount > 0.f)
                    {
                        auto* component = layers.PrioritizeComponent<Waiting>(-32000);
                        OnHold(component);
                        component->novel = this; component->waitingTime = amount;
                    }
                    else cout << "Warning :: NovelComponent :: Couldn't init \"wait\" command for " << amount << " seconds." << endl;
                }
                else if (nss::Command(command, L"fade ") || nss::Command(command, L"fadeout ") || nss::Command(command, L"disappear "))
                {
                    nss::SkipSpaces(command);
                    float disappearTime = ArgumentAsFloat(command);
                    
                    if (disappearTime >= 0.f)
                    {
                        enum sendMessageBackEnum{ atDisappearing, atDeprecated, noMessage };
                        sendMessageBackEnum sendMessageBack{ atDeprecated };
                        
                        vector<std::wstring> arguments;
                        nss::ParseArguments(command, arguments);
                        for (auto it = arguments.begin(); it != arguments.end(); ++it)
                        {
                            nss::CommandSettings argument;
                            argument.Command(*it);
                            
                            if (nss::Command(argument, L"for") || nss::Command(argument, L"seconds")) { }
                            else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                            {
                                std::wstring stringValue = nss::ArgumentAsString(argument);
                                if (stringValue == L"atdisappearing" || stringValue == L"disappearing") sendMessageBack = atDisappearing;
                                else if (stringValue == L"atdeprecated" || stringValue == L"deprecated") sendMessageBack = atDeprecated;
                                else if (stringValue == L"nomessage" || stringValue == L"no") sendMessageBack = noMessage;
                            }
                        }
                        
                        for (auto n : layers.objects)
                        {
                            Background* b{ nullptr };
                            Character* c{ nullptr };
                            RectangleShape* rs{ nullptr };
                            Dialogue* d{ nullptr };
                            if ((b = dynamic_cast<Background*>(n)))
                            {
                                if (sendMessageBack != noMessage) OnHold(n);
                                switch (sendMessageBack)
                                {
                                    case atDeprecated: b->sendMessageBack = b->atDeprecated; break;
                                    case atDisappearing: b->sendMessageBack = b->atDisappearing; break;
                                    case noMessage: b->sendMessageBack = b->noMessage; break;
                                    default: b->sendMessageBack = b->atDeprecated; break;
                                }
                                b->mode = b->disappearing;
                                if (disappearTime >= 0) b->disappearTime = disappearTime;
                            }
                            else if ((c = dynamic_cast<Character*>(n)))
                            {
                                if (sendMessageBack != noMessage) OnHold(n);
                                switch (sendMessageBack)
                                {
                                    case atDeprecated: c->sendMessageBack = c->atDeprecated; break;
                                    case atDisappearing: c->sendMessageBack = c->atDisappearing; break;
                                    case noMessage: c->sendMessageBack = c->noMessage; break;
                                    default: c->sendMessageBack = c->atDeprecated; break;
                                }
                                c->mode = c->disappearing;
                                if (disappearTime >= 0) c->disappearTime = disappearTime;
                            }
                            else if ((rs = dynamic_cast<RectangleShape*>(n)))
                            {
                                if (sendMessageBack != noMessage) OnHold(n);
                                switch (sendMessageBack)
                                {
                                    case atDeprecated: rs->sendMessageBack = rs->atDeprecated; break;
                                    case atDisappearing: rs->sendMessageBack = rs->atDisappearing; break;
                                    case noMessage: rs->sendMessageBack = rs->noMessage; break;
                                    default: rs->sendMessageBack = rs->atDeprecated; break;
                                }
                                rs->mode = rs->disappearing;
                                if (disappearTime >= 0) rs->disappearTime = disappearTime;
                            }
                            else if ((d = dynamic_cast<Dialogue*>(n)))
                            {
                                if (sendMessageBack != noMessage) OnHold(n);
                                switch (sendMessageBack)
                                {
                                    case atDeprecated: d->sendMessageBack = d->atDeprecated; break;
                                    case atDisappearing: d->sendMessageBack = d->atDisappearing; break;
                                    case noMessage: d->sendMessageBack = d->noMessage; break;
                                    default: d->sendMessageBack = d->atDeprecated; break;
                                }
                                d->mode = d->disappearing;
                                if (disappearTime >= 0) d->disappearTime = disappearTime;
                            }
                        }
                    }
                    else
                        cout << "Warning :: NovelComponent :: Couldn't init \"fade\" command for " << disappearTime << " seconds." << endl;
                }
                /*else if (nss::Command(command, L"event block")) gs::ignoreEvent = true;
                else if (nss::Command(command, L"event unblock")) gs::ignoreEvent = false;*/
                else if (nss::Command(command, L"draw block")) gs::ignoreDraw = true;
                else if (nss::Command(command, L"draw unblock")) gs::ignoreDraw = false;
                else if (nss::Command(command, L"blackend hide"))
                {
                    float disappearTime{ -1.f };
                    enum sendMessageBackEnum{ atDisappearing, atDeprecated, noMessage };
                    sendMessageBackEnum sendMessageBack{ atDeprecated };
                    
                    vector<std::wstring> arguments;
                    nss::ParseArguments(command, arguments);
                    for (auto it = arguments.begin(); it != arguments.end(); ++it)
                    {
                        nss::CommandSettings argument;
                        argument.Command(*it);
                        
                        if (nss::Command(argument, L"fade:")|| nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                            disappearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"atdisappearing" || stringValue == L"disappearing") sendMessageBack = atDisappearing;
                            else if (stringValue == L"atdeprecated" || stringValue == L"deprecated") sendMessageBack = atDeprecated;
                            else if (stringValue == L"nomessage" || stringValue == L"no") sendMessageBack = noMessage;
                        }
                    }
                    
                    for (auto n : layers.objects)
                    {
                        RectangleShape* b{ nullptr };
                        if ((b = dynamic_cast<RectangleShape*>(n)))
                        {
                            if (sendMessageBack != noMessage) OnHold(n);
                            switch (sendMessageBack)
                            {
                                case atDeprecated: b->sendMessageBack = b->atDeprecated; break;
                                case atDisappearing: b->sendMessageBack = b->atDisappearing; break;
                                case noMessage: b->sendMessageBack = b->noMessage; break;
                                default: b->sendMessageBack = b->atDeprecated; break;
                            }
                            b->mode = b->disappearing;
                            if (disappearTime >= 0) b->disappearTime = disappearTime;
                        }
                    }
                }
                else if (nss::Command(command, L"blackend"))
                {
                    auto* component = layers.PrioritizeComponent<RectangleShape>(0);
                    component->novel = this;
                    
                    vector<std::wstring> arguments;
                    nss::ParseArguments(command, arguments);
                    for (auto it = arguments.begin(); it != arguments.end(); ++it)
                    {
                        nss::CommandSettings argument;
                        argument.Command(*it);
                        
                        if (nss::Command(argument, L"fade:"))
                        {
                            float value = nss::ArgumentAsFloat(argument);
                            component->appearTime = value;
                            component->disappearTime = value;
                        }
                        else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                            component->appearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                            component->disappearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"alpha:") || nss::Command(argument, L"maxalpha:"))
                            component->maxAlpha = nss::ArgumentAsInt(argument);
                        else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"atappearance" || stringValue == L"appearance")
                                component->sendMessageBack = component->atAppearance;
                            else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                component->sendMessageBack = component->atDisappearing;
                            else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                component->sendMessageBack = component->atDeprecated;
                            else if (stringValue == L"nomessage" || stringValue == L"no")
                                component->sendMessageBack = component->noMessage;
                        }
                    }
                    
                    if (component->sendMessageBack != component->noMessage) OnHold(component);
                }
                ///--------------------------------------BACKGROUND--------------------------------------
                ///--------------------------------------BACKGROUND--------------------------------------
                ///--------------------------------------BACKGROUND--------------------------------------
                else if (nss::Command(command, L"background hide"))
                {
                    float disappearTime{ -1.f };
                    enum sendMessageBackEnum{ atDisappearing, atDeprecated, noMessage };
                    sendMessageBackEnum sendMessageBack{ atDeprecated };
                    
                    vector<std::wstring> arguments;
                    nss::ParseArguments(command, arguments);
                    for (auto it = arguments.begin(); it != arguments.end(); ++it)
                    {
                        nss::CommandSettings argument;
                        argument.Command(*it);
                        
                        if (nss::Command(argument, L"fade:")||
                            nss::Command(argument, L"fadeout:") ||
                            nss::Command(argument, L"disappear:"))
                            disappearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                sendMessageBack = atDisappearing;
                            else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                sendMessageBack = atDeprecated;
                            else if (stringValue == L"nomessage" || stringValue == L"no")
                                sendMessageBack = noMessage;
                        }
                    }
                    
                    for (auto n : layers.objects)
                    {
                        Background* b{ nullptr };
                        if ((b = dynamic_cast<Background*>(n)))
                        {
                            if (sendMessageBack != noMessage) OnHold(n);
                            switch (sendMessageBack)
                            {
                                case atDeprecated: b->sendMessageBack = b->atDeprecated; break;
                                case atDisappearing: b->sendMessageBack = b->atDisappearing; break;
                                case noMessage: b->sendMessageBack = b->noMessage; break;
                                default: b->sendMessageBack = b->atDeprecated; break;
                            }
                            b->mode = b->disappearing;
                            if (disappearTime >= 0) b->disappearTime = disappearTime;
                        }
                    }
                }
                else if ((backgroundAddingMode = nss::Command(command, L"background add ")) || nss::Command(command, L"background ") || nss::Command(command, L"задний фон "))
                {
                    std::wstring filePath = nss::ParseAsQuoteString(command);
                    auto* component = layers.PrioritizeComponent<Background>(0);
                    component->novel = this;
                    
                    vector<std::wstring> arguments;
                    nss::ParseArguments(command, arguments);
                    for (auto it = arguments.begin(); it != arguments.end(); ++it)
                    {
                        nss::CommandSettings argument;
                        argument.Command(*it);
                        
                        if (nss::Command(argument, L"fade:"))
                        {
                            float value = nss::ArgumentAsFloat(argument);
                            component->appearTime = value;
                            component->disappearTime = value;
                        }
                        else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                            component->appearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                            component->disappearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"alpha:") || nss::Command(argument, L"maxalpha:"))
                            component->maxAlpha = nss::ArgumentAsInt(argument);
                        else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"atappearance" || stringValue == L"appearance")
                                component->sendMessageBack = component->atAppearance;
                            else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                component->sendMessageBack = component->atDisappearing;
                            else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                component->sendMessageBack = component->atDeprecated;
                            else if (stringValue == L"nomessage" || stringValue == L"no")
                                component->sendMessageBack = component->noMessage;
                        }
                        else if (nss::Command(argument, L"parallax:"))
                        {
                            std::wstring possibleParallax = nss::ArgumentAsString(argument);
                            if (possibleParallax == L"normal" || possibleParallax == L"n")
                                component->parallaxPower = ns::GlobalSettings::defaultParallaxNormal;
                            else if (possibleParallax == L"close" || possibleParallax == L"c")
                                component->parallaxPower = ns::GlobalSettings::defaultParallaxClose;
                            else if (possibleParallax == L"far" || possibleParallax == L"f")
                                component->parallaxPower = ns::GlobalSettings::defaultParallaxFar;
                            else if (possibleParallax == L"background" || possibleParallax == L"back" || possibleParallax == L"b")
                                component->parallaxPower = ns::GlobalSettings::defaultParallaxBackground;
                            else if (possibleParallax == L"frontground" || possibleParallax == L"front" || possibleParallax == L"f")
                                component->parallaxPower = ns::GlobalSettings::defaultParallaxFrontground;
                            else
                                component->parallaxPower = base::atof(possibleParallax);
                        }
                    }
                    
                    if (component->sendMessageBack != component->noMessage) OnHold(component);
                    component->LoadImage(filePath);
                }
                ///---------------------------------------CHARACTER--------------------------------------
                ///---------------------------------------CHARACTER--------------------------------------
                ///---------------------------------------CHARACTER--------------------------------------
                else if (nss::Command(command, L"show "))
                {
                    std::wstring possibleName = nss::ParseUntil(command, ' ');
                    if (possibleName.length() != 0)
                    {
                        vector<NekoBase*>::iterator neko = nl::neko.end();
                        for (auto it = nl::neko.begin(); it != nl::neko.end() && neko == nl::neko.end(); ++it)
                            if ((*it)->name == possibleName) neko = it;
                        if (neko != nl::neko.end())
                        {
                            auto* component = layers.PrioritizeComponent<Character>(5000);
                            component->novel = this;
                            component->nekoInfo = (*neko);
                            component->nekoIterator = neko;
                            component->position = component->center;
                            
                            std::wstring state{ L"" };
                            
                            vector<std::wstring> arguments;
                            nss::ParseArguments(command, arguments);
                            for (auto it = arguments.begin(); it != arguments.end(); ++it)
                            {
                                nss::CommandSettings argument;
                                argument.Command(*it);
                                
                                if (nss::Command(argument, L"fade:"))
                                {
                                    float value = nss::ArgumentAsFloat(argument);
                                    component->appearTime = value;
                                    component->disappearTime = value;
                                }
                                else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                                    component->appearTime = nss::ArgumentAsFloat(argument);
                                else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                                    component->disappearTime = nss::ArgumentAsFloat(argument);
                                else if (nss::Command(argument, L"alpha:") || nss::Command(argument, L"maxalpha:"))
                                    component->maxAlpha = nss::ArgumentAsInt(argument);
                                else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                                {
                                    std::wstring stringValue = nss::ArgumentAsString(argument);
                                    if (stringValue == L"atappearance" || stringValue == L"appearance")
                                        component->sendMessageBack = component->atAppearance;
                                    else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                        component->sendMessageBack = component->atDisappearing;
                                    else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                        component->sendMessageBack = component->atDeprecated;
                                    else if (stringValue == L"nomessage" || stringValue == L"no")
                                        component->sendMessageBack = component->noMessage;
                                }
                                else
                                {
                                    std::wstring possibleStateOrPos;
                                    if (argument.line[0] == L'"')
                                        possibleStateOrPos = nss::ParseAsQuoteString(argument);
                                    else possibleStateOrPos = argument.line;
                                    
                                    if (possibleStateOrPos == L"l" || possibleStateOrPos == L"left")
                                        component->position = component->left;
                                    else if (possibleStateOrPos == L"cl" || possibleStateOrPos == L"cleft")
                                        component->position = component->cleft;
                                    else if (possibleStateOrPos == L"c" || possibleStateOrPos == L"center")
                                        component->position = component->center;
                                    else if (possibleStateOrPos == L"cr" || possibleStateOrPos == L"cright")
                                        component->position = component->cright;
                                    else if (possibleStateOrPos == L"r" || possibleStateOrPos == L"right")
                                        component->position = component->right;
                                    else state = possibleStateOrPos;
                                }
                            }
                            
                            if (component->sendMessageBack != component->noMessage) OnHold(component);
                            component->LoadState(state);
                        }
                    }
                }
                else if (nss::Command(command, L"hide all"))
                {
                    float disappearTime{ -1.f };
                    enum sendMessageBackEnum{ atDisappearing, atDeprecated, noMessage };
                    sendMessageBackEnum sendMessageBack{ atDeprecated };
                    
                    vector<std::wstring> arguments;
                    nss::ParseArguments(command, arguments);
                    for (auto it = arguments.begin(); it != arguments.end(); ++it)
                    {
                        nss::CommandSettings argument;
                        argument.Command(*it);
                        
                        if (nss::Command(argument, L"fade:")||
                            nss::Command(argument, L"fadeout:") ||
                            nss::Command(argument, L"disappear:"))
                            disappearTime = nss::ArgumentAsFloat(argument);
                        else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                        {
                            std::wstring stringValue = nss::ArgumentAsString(argument);
                            if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                sendMessageBack = atDisappearing;
                            else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                sendMessageBack = atDeprecated;
                            else if (stringValue == L"nomessage" || stringValue == L"no")
                                sendMessageBack = noMessage;
                        }
                    }
                    
                    for (auto n : layers.objects)
                    {
                        Character* c{ nullptr };
                        if ((c = dynamic_cast<Character*>(n)))
                        {
                            if (sendMessageBack != noMessage) OnHold(c);
                            switch (sendMessageBack)
                            {
                                case atDeprecated: c->sendMessageBack = c->atDeprecated; break;
                                case atDisappearing: c->sendMessageBack = c->atDisappearing; break;
                                case noMessage: c->sendMessageBack = c->noMessage; break;
                                default: c->sendMessageBack = c->atDeprecated; break;
                            }
                            c->mode = c->disappearing;
                            if (disappearTime >= 0) c->disappearTime = disappearTime;
                        }
                    }
                }
                else if (nss::Command(command, L"hide "))
                {
                    std::wstring possibleName = nss::ParseUntil(command, ' ');
                    if (possibleName.length() != 0)
                    {
                        float disappearTime{ -1.f };
                        enum sendMessageBackEnum{ atDisappearing, atDeprecated, noMessage };
                        sendMessageBackEnum sendMessageBack{ atDeprecated };
                        vector<std::wstring> arguments;
                        nss::ParseArguments(command, arguments);
                        for (auto it = arguments.begin(); it != arguments.end(); ++it)
                        {
                            nss::CommandSettings argument;
                            argument.Command(*it);
                            
                            if (nss::Command(argument, L"fade:")||
                                nss::Command(argument, L"fadeout:") ||
                                nss::Command(argument, L"disappear:"))
                                disappearTime = nss::ArgumentAsFloat(argument);
                            else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                            {
                                std::wstring stringValue = nss::ArgumentAsString(argument);
                                if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                    sendMessageBack = atDisappearing;
                                else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                    sendMessageBack = atDeprecated;
                                else if (stringValue == L"nomessage" || stringValue == L"no")
                                    sendMessageBack = noMessage;
                            }
                        }
                        
                        for (auto n : layers.objects)
                        {
                            Character* c{ nullptr };
                            if ((c = dynamic_cast<Character*>(n)))
                            {
                                if (c->nekoInfo && c->nekoInfo->name == possibleName)
                                {
                                    if (sendMessageBack != noMessage) OnHold(c);
                                    switch (sendMessageBack)
                                    {
                                        case atDeprecated: c->sendMessageBack = c->atDeprecated; break;
                                        case atDisappearing: c->sendMessageBack = c->atDisappearing; break;
                                        case noMessage: c->sendMessageBack = c->noMessage; break;
                                        default: c->sendMessageBack = c->atDeprecated; break;
                                    }
                                    c->mode = c->disappearing;
                                    if (disappearTime >= 0) c->disappearTime = disappearTime;
                                }
                            }
                        }
                    }
                }
                ///---------------------------------------SPECIFIC---------------------------------------
                ///---------------------------------------SPECIFIC---------------------------------------
                ///---------------------------------------SPECIFIC---------------------------------------
                else
                {
                    std::wstring possibleName = nss::ParseUntil(command, ' ');
                    if (possibleName.length() != 0)
                    {
                        nss::SkipSpaces(command);
                        std::wstring possibleDialogue = nss::ParseAsQuoteString(command);
                        if (possibleDialogue.length() != 0)
                        {
                            NekoBase* nekoInfo{ nullptr };
                            vector<NekoBase*>::iterator neko = nl::neko.end();
                            for (auto it = nl::neko.begin(); it != nl::neko.end() && neko == nl::neko.end(); ++it)
                                if ((*it)->name == possibleName) neko = it;
                            if (neko != nl::neko.end()) nekoInfo = (*neko);
                            
                            std::wstring characterName = (nekoInfo != nullptr) ? nekoInfo->display : possibleName;
                            for (auto n : layers.objects) {
                                Dialogue* d{ nullptr };
                                if ((d = dynamic_cast<Dialogue*>(n))) ++d->priority; }
                            auto* component = layers.PrioritizeComponent<Dialogue>(10000, this);
                            
                            vector<std::wstring> arguments;
                            nss::ParseArguments(command, arguments);
                            for (auto arg : arguments)
                            {
                                nss::CommandSettings argument; argument.Command(arg);
                                if (nss::Command(argument, L"fade:"))
                                {
                                    float value = nss::ArgumentAsFloat(argument);
                                    component->appearTime = value;
                                    component->disappearTime = value;
                                }
                                else if (nss::Command(argument, L"fadein:") || nss::Command(argument, L"appear:"))
                                    component->appearTime = nss::ArgumentAsFloat(argument);
                                else if (nss::Command(argument, L"fadeout:") || nss::Command(argument, L"disappear:"))
                                    component->disappearTime = nss::ArgumentAsFloat(argument);
                                else if (nss::Command(argument, L"alpha:") || nss::Command(argument, L"maxalpha:"))
                                    component->maxAlpha = nss::ArgumentAsInt(argument);
                                else if (nss::Command(argument, L"messageback:") || nss::Command(argument, L"message:"))
                                {
                                    std::wstring stringValue = nss::ArgumentAsString(argument);
                                    if (stringValue == L"atappearance" || stringValue == L"appearance")
                                        component->sendMessageBack = component->atAppearance;
                                    else if (stringValue == L"atdisappearing" || stringValue == L"disappearing")
                                        component->sendMessageBack = component->atDisappearing;
                                    else if (stringValue == L"atdeprecated" || stringValue == L"deprecated")
                                        component->sendMessageBack = component->atDeprecated;
                                    else if (stringValue == L"nomessage" || stringValue == L"no")
                                        component->sendMessageBack = component->noMessage;
                                }
                                else if (nss::Command(argument, L"afterappearswitchto:") || nss::Command(argument, L"switchby:"))
                                {
                                    std::wstring stringValue = nss::ArgumentAsString(argument);
                                    if (stringValue == L"waitingforinput" || stringValue == L"input")
                                        component->afterAppearSwitchTo = component->waitingForInput;
                                    else if (stringValue == L"waitingfortime" || stringValue == L"time")
                                        component->afterAppearSwitchTo = component->waitingForTime;
                                }
                                else if (nss::Command(argument, L"waitingtime:") || nss::Command(argument, L"time:"))
                                    component->waitingTime = nss::ArgumentAsFloat(argument);
                            }
                            
                            if (component->sendMessageBack != component->noMessage) OnHold(component);
                            
                            component->SetCharacterName(characterName);
                            if (nekoInfo) component->SetNeko(nekoInfo);
                            component->SetDialogue(possibleDialogue);
                        }
                    }
                }
            }
            
            if (!gs::isPause) layers.Update(elapsedTime);
            if (eof && onHold.empty()) entity->PopComponent(this);
        }
        void Novel::PollEvent(sf::Event& event) { if (!gs::isPause) layers.PollEvent(event); }
        void Novel::Resize(unsigned int width, unsigned int height) { layers.Resize(width, height); }
        void Novel::Draw(sf::RenderWindow* window) { layers.Draw(window); }
        void Novel::OnHold(NovelObject* component) { onHold.insert(onHold.begin(), component); }
        void Novel::UnHold(NovelObject* component)
        {
            std::list<NovelObject*>::iterator it = std::find(onHold.begin(), onHold.end(), component);
            if (it != onHold.end()) onHold.erase(it);
        }
        std::wstring Novel::GetFolderPath() { return folderPath; }
    }
}
