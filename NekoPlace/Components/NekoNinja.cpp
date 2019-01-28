//
//  NekoNinjaComponents.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 25/01/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "NekoNinja.hpp"

namespace NekoNinja
{
    float NekoObject::gravity = 0.19f * 60 * 60;
    float NekoObject::yStartVelocity = -17.f * 60;
    int NekoObject::yStartRandomVelocity = 4*60;
    int NekoObject::xStartRandomVelocity = 6*60;
    
    Controller* NekoObject::control = nullptr;
    
    vector<NekoInfo> NekoLibrary::neko = {
        NekoInfo(L"Poop", 10000, 20, 20, 20, 20),
        NekoInfo(L"Shigure", 10000, 155, 30, 120, 70),
        NekoInfo(L"Vanilla", 10000, 185, 75, 167, 70),
        NekoInfo(L"Chocola", 10000, 168, 80, 160, 70),
        NekoInfo(L"Maple", 10000, 115, 90, 138, 100),
        NekoInfo(L"Azuki", 10000, 115, 75, 98, 80),
        NekoInfo(L"Cinnamon", 10000, 144, 50, 144, 70),
        NekoInfo(L"Coconut", 10000, 195, 75, 97, 80),
    };
    
    
    
    
    void SceneBackground::Destroy() { ic::DeleteImage(L"Data/Images/park.jpg"); }
    void SceneBackground::Init()
    {
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/park.jpg");
        if (texture != nullptr) { sprite.setTexture(*texture); spriteLoaded = true; }
    }
    void SceneBackground::Resize(unsigned int width, unsigned int height)
    {
        if (spriteLoaded)
        {
            float factorX = (float)width / sprite.getTexture()->getSize().x;
            float factorY = (float)height / sprite.getTexture()->getSize().y;
            
            float scaleFactor = factorX > factorY ? factorX : factorY;
            sprite.setScale(scaleFactor, scaleFactor);
        }
    }
    void SceneBackground::Draw(sf::RenderWindow* window)
    {
        if (spriteLoaded) window->draw(sprite);
    }
    
    
    
    
    void NekoObject::Init()
    {
        int nekoChance = rand() % nl::neko.size();
        nekoInfo = &nl::neko[nekoChance];
        
        x = rand() % gs::relativeWidth;
        y = gs::relativeHeight;
        
        if (x < gs::relativeWidth/13) x = gs::relativeWidth/13;
        if (x > gs::relativeWidth - gs::relativeWidth/13) x = gs::relativeWidth - gs::relativeWidth/13;
        
        if (x < gs::relativeWidth/2) xVelocity = (rand() % xStartRandomVelocity); else xVelocity = -(rand() % xStartRandomVelocity);
        yVelocity = yStartVelocity + (rand() % yStartRandomVelocity);
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Neko/" + nekoInfo->name + L".png");
        if (texture != nullptr)
        {
            sprite.setTexture(*texture);
            halfTheWidth = texture->getSize().x / 2;
            sprite.setOrigin(halfTheWidth, 0);
        }
    }
    void NekoObject::Destroy() { offline = true; if (nekoInfo != nullptr) ic::DeleteImage(L"Data/Neko/" + nekoInfo->name + L".png"); }
    void NekoObject::Update(const sf::Time& elapsedTime)
    {
        float yVelocity_prev = yVelocity;
        yVelocity += gravity * elapsedTime.asSeconds();
        if (y > (gs::relativeHeight + 100)) Destroy();
        
        x += xVelocity * elapsedTime.asSeconds();
        y += 0.5 * (yVelocity + yVelocity_prev) * elapsedTime.asSeconds();
        
        sprite.setPosition(x * gs::scalex, y * gs::scaley);
    }
    void NekoObject::CheckInterception(sf::Vector2<int>& point)
    {
        // TODO: Почему-то снизу коллизий будто бы больше. Например, ту же какашку снизу нажать слишком просто, даже когда ты далеко от какашки.
        /*tamed = (point.x > xSc - (halfTheWidth - nekoInfo->offsets.left)*sprite.getScale().x
                 && point.x < xSc + (halfTheWidth - nekoInfo->offsets.width)*sprite.getScale().x
                 && point.y > ySc + nekoInfo->offsets.top * gs::scaley
                 && point.y < ySc + sprite.getLocalBounds().height*gs::scaley - nekoInfo->offsets.height*sprite.getScale().x);*/
        sf::FloatRect rect = sprite.getGlobalBounds();
        rect.left += nekoInfo->offsets.left * sprite.getScale().x;
        rect.width -= nekoInfo->offsets.width * sprite.getScale().x;
        rect.top += nekoInfo->offsets.top * sprite.getScale().x;
        rect.height -= nekoInfo->offsets.height * sprite.getScale().x;
        tamed = rect.contains(point.x, point.y);
        if (tamed)
        {
            sprite.setColor(sf::Color(255,255,255,120));
            if (nekoInfo->name == L"Poop")
            {
                --control->lifes;
                if (control->lifes <= 0) control->GameOver();
                if (control->score < 100) control->score = 0; else control->score -= 100;
            }
            else control->score += 100;
        }
    }
    void NekoObject::Resize()
    {
        sprite.setPosition(x * gs::scalex, y * gs::scaley);
        sprite.setScale(gs::scale, gs::scale);
    }
    void NekoObject::Draw(sf::RenderWindow* window)
    {
        window->draw(sprite);
    }



    void Controller::Destroy()
    {
        for (int i = 0; i < nl::neko.size(); ++i)
            ic::SetDestroyable(L"Data/Neko/" + nl::neko[i].name + L".png", true);
    }
    void Controller::Init()
    {
        NekoObject::control = this;
        line[0] = sf::Vertex(sf::Vector2f(10, 10), sf::Color::Red);
        line[1] = sf::Vertex(sf::Vector2f(150, 150), sf::Color::Red);
        
        for (int i = 0; i < nl::neko.size(); ++i)
            ic::PreloadImage(L"Data/Neko/" + nl::neko[i].name + L".png", 0, false);
        for (int i = 0; i < 10; ++i)
        {
            nekos.push_back(NekoObject());
            nekos.back().Init(); nekos.back().Resize();
        }
        
        std::wstring filename = base::utf16(resourcePath()) + L"Data/topscore.txt";
        std::wifstream wif;
#ifdef _WIN32
        wif.open(filename);
#else
        wif.open(base::utf8(filename));
#endif
        wif.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
        
        if (wif.is_open())
        {
            std::wstring line; std::getline(wif, line);
            topScore = (unsigned int)std::atol(base::utf8(line).c_str());
            cout << topScore << endl;
        }
    }
    void Controller::Update(const sf::Time& elapsedTime)
    {
        if (!gs::isPause)
        {
            if (spawnNekos)
            {
                elapsedSeconds += elapsedTime.asSeconds();
                if (elapsedSeconds > 2.f)
                {
                    elapsedSeconds = 0.f;
                    int count = 1 + rand() % 5;
                    for (int i = 0; i < count; ++i)
                    {
                        nekos.push_back(NekoObject());
                        nekos.back().Init(); nekos.back().Resize();
                    }
                }
            }
            
            for (auto it = nekos.begin(); it != nekos.end(); ++it)
            {
                if ((*it).offline) {
                    if (!isGameOver && !(*it).tamed && (*it).nekoInfo->name != L"Poop") {--lifes; if (lifes <= 0) GameOver();}
                    it = nekos.erase(it); /*score += 100;*/
                }
                else (*it).Update(elapsedTime);
            }
            
            if (cursor.size() > 60)
            {
                auto it = cursor.begin(); std::advance(it, cursor.size() - 60);
                cursor.erase(cursor.begin(), it);
            }
            
            if (cursor.size() > 0)
            {
                elapsedCursor += elapsedTime.asSeconds();
                while (elapsedCursor > 0.04f)
                {
                    if (cursor.size() > 0)
                    {
                        elapsedCursor -= 0.04f;
                        cursor.erase(cursor.begin());
                    }
                    else elapsedCursor = 0.f;
                }
            }
        }
    }
    void Controller::Resize(unsigned int width, unsigned int height)
    {
        for (auto& n : nekos) n.Resize();
    }
    void Controller::Draw(sf::RenderWindow* window)
    {
        for (auto& n : nekos) n.Draw(window);
        for (auto it = cursor.begin(); it != cursor.end(); ++it)
        {
            if ((*it).x != -1)
            {
                auto it1 = it; std::advance(it1, 1);
                if (it1 != cursor.end())
                {
                    if ((*it1).x != -1)
                    {
                        line[0].position.x = (*it).x;
                        line[0].position.y = (*it).y;
                        line[1].position.x = (*it1).x;
                        line[1].position.y = (*it1).y;
                        window->draw(line, 2, sf::Lines);
                    }
                }
            }
        }
    }
    void Controller::PollEvent(sf::Event& event)
    {
        if (!gs::isPause && !isGameOver)
        {
            /*if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::TouchBegan)
            {
                cursor.push_back({ -1, -1 });
                
                sf::Vector2<int> dot;
                if (event.type == sf::Event::MouseButtonPressed) dot = {event.mouseButton.x, event.mouseButton.y};
                else dot = {event.touch.x, event.touch.y};
                
                startPoint.x = dot.x;
                startPoint.y = dot.y;
                endPoint.x = dot.x;
                endPoint.y = dot.y;
                
                swipeConfirmed = true;
            }
            else */if (event.type == sf::Event::MouseButtonReleased || (event.type == sf::Event::TouchEnded && event.touch.finger == 0))
            {
                //cursor.push_back({ -1, -1 });
                swipeConfirmed = false;
            }
            else if (event.type == sf::Event::MouseMoved || event.type == sf::Event::TouchMoved)
            {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || (sf::Touch::isDown(0) && event.touch.finger == 0))
                {
                    sf::Vector2<int> dot;
                    if (event.type == sf::Event::MouseMoved) dot = {event.mouseMove.x, event.mouseMove.y};
                    else dot = {event.touch.x, event.touch.y};
                    
                    endPoint.x = dot.x;
                    endPoint.y = dot.y;
                    
                    if (!swipeConfirmed)
                    {
                        cursor.push_back({ -1, -1 });
                        startPoint.x = dot.x;
                        startPoint.y = dot.y;
                        
                        swipeConfirmed = true;
                    }
                    
                    //cout << startPoint.x << " " << startPoint.y << "  " << endPoint.x << " " << endPoint.y << endl;
                    if (pow(startPoint.x - endPoint.x, 2) + pow(startPoint.y - endPoint.y, 2) >= 36*gs::scale)
                    {
                        if (abs(startPoint.x - endPoint.x) > abs(startPoint.y - endPoint.y))
                        {
                            sf::Vector2<int> point{ startPoint.x, startPoint.y };
                            int steps = (startPoint.y - endPoint.y) / 14;
                            if (startPoint.x < endPoint.x)
                            {
                                while (point.x < endPoint.x)
                                {
                                    cursor.push_back(point);
                                    for (auto& n : nekos) if (!n.tamed) { n.CheckInterception(point); if (isGameOver) break; }
                                    if (isGameOver) break;
                                    point.x += 14; point.y -= steps;
                                }
                            }
                            else
                            {
                                while (point.x > endPoint.x)
                                {
                                    cursor.push_back(point);
                                    for (auto& n : nekos) if (!n.tamed) { n.CheckInterception(point); if (isGameOver) break; }
                                    if (isGameOver) break;
                                    point.x -= 14; point.y -= steps;
                                }
                            }
                        }
                        else
                        {
                            sf::Vector2<int> point{ startPoint.x, startPoint.y };
                            int steps = (startPoint.x - endPoint.x) / 14;
                            if (startPoint.y < endPoint.y)
                            {
                                while (point.y < endPoint.y)
                                {
                                    cursor.push_back(point);
                                    for (auto& n : nekos) if (!n.tamed) { n.CheckInterception(point); if (isGameOver) break; }
                                    if (isGameOver) break;
                                    point.y += 14; point.x -= steps;
                                }
                            }
                            else
                            {
                                while (point.y > endPoint.y)
                                {
                                    cursor.push_back(point);
                                    for (auto& n : nekos) if (!n.tamed) { n.CheckInterception(point); if (isGameOver) break; }
                                    if (isGameOver) break;
                                    point.y -= 14; point.x -= steps;
                                }
                            }
                        }
                    }
                    
                    startPoint.x = endPoint.x;
                    startPoint.y = endPoint.y;
                } else swipeConfirmed = false;
            }
        }
    }
    void Controller::GameOver()
    {
        isGameOver = true;
        spawnNekos = false;
        gs::isPause = false;
        
        if (score > topScore)
        {
            topScore = score;
            
            std::wstring filename = base::utf16(resourcePath()) + L"Data/topscore.txt";
            std::wofstream wof;
#ifdef _WIN32
            wof.open(filename);
#else
            wof.open(base::utf8(filename));
#endif
            wof.imbue(std::locale(std::locale(), new std::codecvt_utf8<wchar_t>));
            if (wof.is_open()) wof << topScore << endl;
        }
    }
    void Controller::ResetGame()
    {
        gs::isPause = false;
        lifes = maxlifes;
        spawnNekos = true;
        isGameOver = false;
        score = 0;
        
        nekos.clear();
    }
    void Controller::DefaultGamemodeSettings()
    {
        NekoObject::gravity = 0.19f * 60 * 60;
        NekoObject::yStartVelocity = -17.f * 60;
        NekoObject::yStartRandomVelocity = 4*60;
        NekoObject::xStartRandomVelocity = 6*60;
    }
    
    
    
    void GUIOverlay::Destroy() { ic::DeleteImage(L"Data/Images/pause_button.png"); ic::DeleteImage(L"Data/Images/heart.png"); }
    void GUIOverlay::Init()
    {
        scoreText.setFillColor(sf::Color::White);
        scoreText.setOutlineColor(sf::Color::Black);
        scoreText.setOutlineThickness(1.f);
        scoreText.setCharacterSize(15);
        scoreText.setString(L"0");
        scoreText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        fontLoaded = (scoreText.getFont() != nullptr);
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/pause_button.png");
        if (texture != nullptr) pauseSprite.setTexture(*texture);
        
        texture = ic::LoadTexture(L"Data/Images/groping-hand.png");
        if (texture != nullptr) handSprite.setTexture(*texture);
        handSprite.setOrigin(170, 136);
        
        texture = ic::LoadTexture(L"Data/Images/heart.png");
        if (texture != nullptr)
        {
            heartSprite.setTexture(*texture);
            heartSprite.setTextureRect(sf::IntRect(268, 0, 268, 256));
        }
        
        pauseShape.setFillColor(sf::Color(0,0,0,150));
        
        pauseText.setFillColor(sf::Color::White);
        pauseText.setOutlineColor(sf::Color::Black);
        pauseText.setOutlineThickness(1.f);
        scoreText.setCharacterSize(60);
        pauseText.setString(L"GAME PAUSE");
        pauseText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        fontLoaded = (pauseText.getFont() != nullptr);
    }
    void GUIOverlay::Update(const sf::Time& elapsedTime)
    {
        if (control != nullptr) scoreText.setString(std::to_wstring(control->score));
    }
    void GUIOverlay::Draw(sf::RenderWindow* window)
    {
        float xx = heartsXX;
        for (int i = 0; i < control->lifes; ++i)
        {
            heartSprite.setPosition(xx, heartSprite.getPosition().y);
            window->draw(heartSprite);
            xx += heartSprite.getLocalBounds().width * heartSprite.getScale().x + 5*gs::scale;
        }
        if (control->lifes != control->maxlifes) heartSprite.setTextureRect(sf::IntRect(0, 0, 268, 256));
        for (int i = control->lifes; i < control->maxlifes; ++i)
        {
            heartSprite.setPosition(xx, heartSprite.getPosition().y);
            window->draw(heartSprite);
            xx += heartSprite.getLocalBounds().width * heartSprite.getScale().x + 5*gs::scale;
        }
        if (control->lifes != control->maxlifes) heartSprite.setTextureRect(sf::IntRect(268, 0, 268, 256));
        
        if (control->isGameOver)
        {
            window->draw(pauseShape);
            
            pauseText.setString(L"GAME IS OVER");
            pauseText.setPosition(gs::width/2 - pauseText.getLocalBounds().width/2, gs::height/4);
            window->draw(pauseText);
            
            pauseText.setString(L"(Top score: " + std::to_wstring(control->topScore) + L")");
            pauseText.setPosition(gs::width/2 - pauseText.getLocalBounds().width/2, gs::height/4 + pauseText.getLocalBounds().height);
            window->draw(pauseText);
            
            pauseText.setString(L"Try again?");
            pauseText.setPosition(gs::width/2 - pauseText.getLocalBounds().width/2, gs::height/4 + pauseText.getLocalBounds().height*3);
            window->draw(pauseText);
        }
        else if (gs::isPause)
        {
            window->draw(pauseShape);
            window->draw(pauseText);
        }
        
        if (fontLoaded) window->draw(scoreText);
        window->draw(pauseSprite);
        
        if (!gs::isPause && !control->isGameOver)
        {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                handSprite.setPosition(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
                window->draw(handSprite);
            }
            else if (sf::Touch::isDown(0))
            {
                handSprite.setPosition(sf::Touch::getPosition(0, *window).x, sf::Touch::getPosition(0, *window).y);
                window->draw(handSprite);
            }
        }
    }
    void GUIOverlay::Resize(unsigned int width, unsigned int height)
    {
        scoreText.setOutlineThickness(gs::scale);
        scoreText.setCharacterSize((unsigned int)(96 * gs::scale));
        scoreText.setPosition(8*gs::scale, -10*gs::scale);
        
        pauseText.setOutlineThickness(gs::scale);
        pauseText.setCharacterSize((unsigned int)(108 * gs::scale));
        pauseText.setPosition(gs::width/2 - pauseText.getLocalBounds().width/2, gs::height/4);
        
        handSprite.setScale(0.4 * gs::scale, 0.4 * gs::scale);
        
        heartSprite.setScale(0.3 * gs::scale, 0.3 * gs::scale);
        heartsXX = 10*gs::scale;
        heartSprite.setPosition(0, 10*gs::scale + scoreText.getLocalBounds().height + 10*gs::scale);
        
        pauseSprite.setScale(gs::scale, gs::scale);
        pauseSprite.setPosition(width - pauseSprite.getLocalBounds().width*gs::scale - 10*gs::scale, 10*gs::scale);
        
        pauseShape.setSize({(float)width, (float)height});
    }
    void GUIOverlay::PollEvent(sf::Event& event)
    {
        if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::TouchBegan)
        {
            sf::Vector2<int> dot;
            if (event.type == sf::Event::MouseButtonPressed) dot = {event.mouseButton.x, event.mouseButton.y};
            else dot = {event.touch.x, event.touch.y};
            
            if (control->isGameOver)
            {
                sf::IntRect rect(pauseText.getPosition().x, pauseText.getPosition().y,
                                 pauseText.getPosition().x + pauseText.getLocalBounds().width,
                                 pauseText.getPosition().y + pauseText.getLocalBounds().height);
                if (rect.contains(dot.x, dot.y))
                {
                    pauseText.setString(L"GAME PAUSE");
                    pauseText.setPosition(gs::width/2 - pauseText.getLocalBounds().width/2, gs::height/4);
                    
                    control->ResetGame();
                }
            }
            else
            {
                sf::IntRect rect(pauseSprite.getPosition().x, pauseSprite.getPosition().y,
                                 pauseSprite.getPosition().x + pauseSprite.getLocalBounds().width*gs::scale,
                                 pauseSprite.getPosition().y + pauseSprite.getLocalBounds().height*gs::scale);
                if (rect.contains(dot.x, dot.y)) gs::isPause = !gs::isPause;
            }
        }
    }
}
