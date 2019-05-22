//
//  NekoMenu.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 29/01/2019.
//  Copyright © 2019 Melancholy Hill. All rights reserved.
//

#include "MainMenu.hpp"

namespace NekoNinja
{
    int Room::roomWidth{ 1280 }, Room::roomHeight{ 650 };
    float Room::x{ 0 }, Room::y{ 0 }, Room::xWidth{ 0 }, Room::yHeight{ 0 };
    float Room::scale{ 1.f }, Room::roomScale{ 5.1 };
    sf::FloatRect Room::roomGlobalBounds;
    
    sf::Image* Room::mask{ nullptr };
    sf::Texture* Room::dialogueTexture{ nullptr };
    vector<RoomActivityPlace> Room::nekoActivity = {
        RoomActivityPlace(340, 214), /* \/  стол... \/ */
        RoomActivityPlace(370, 214),
        RoomActivityPlace(397, 214),
        RoomActivityPlace(409, 241),
        RoomActivityPlace(333, 240),
        RoomActivityPlace(340, 274),
        RoomActivityPlace(370, 274),
        RoomActivityPlace(397, 274), /* /\  стол... /\ */
        RoomActivityPlace(350, 456), //Диван снизу
    };
    
    RoomActivityPlace::RoomActivityPlace(int x, int y, const std::wstring& phrase) : x(x), y(y), phrase(phrase) { }
    void NekoEntity::Init()
    {
        inited = true;
        if (Room::dialogueTexture) {
            dialogueSprite.setTexture(*Room::dialogueTexture);
            dialogueSprite.setOrigin(Room::dialogueTexture->getSize().x/2, Room::dialogueTexture->getSize().y);
            dialogue.setFont(*fc::GetFont(L"Pacifica.ttf"));
            dialogue.setFillColor(sf::Color::White);
        }
        sf::Texture* texture = ic::LoadTexture(L"Data/Neko/Chibi/" + info->name + L".png");
        if ((spriteLoaded = texture)) {
            sprite.setTexture(*texture);
            sprite.setOrigin(texture->getSize().x/2, texture->getSize().y - texture->getSize().y/15);
            relScale = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * info->chibiScale;
        }
        
        int i{ 0 }; do {
            x = rand() % Room::roomWidth;
            y = rand() % Room::roomHeight; ++i;
        } while (Room::mask && Room::mask->getPixel(floor(x/Room::roomScale), floor(y/Room::roomScale)).r && i < 100);
        sprite.setPosition(x * Room::scale * gs::scale, y * Room::scale * gs::scale);
        
        newPoint = true; elapsedWaiting = (rand() % 1000)/1000.f;
    }
    void NekoEntity::Destroy() { ic::DeleteImage(L"Data/Neko/Chibi/" + info->name + L".png"); }
    void NekoEntity::Update(const sf::Time& elapsedTime)
    {
        if (beingActionedWith) return;
        if (elapsedDialogue > 0) elapsedDialogue -= elapsedTime.asSeconds();
        else
        {
            if (drawDialogue) { drawDialogue = false; elapsedDialogue = (rand() % 12000) / 1000.f; }
            else
            {
                drawDialogue = true;
                dialogue.setString(info->RandomRoomDialogue());
                elapsedDialogue = 2.1f + 0.08 * dialogue.getString().getSize();// + (rand() % 2000) / 2000.f;
                ResizeDialogue();
                dialogue.setOrigin(dialogue.getLocalBounds().width/2, dialogue.getLocalBounds().height + 44*gs::scale);
            }
        }
        
        if (elapsedWaiting > 0) elapsedWaiting -= elapsedTime.asSeconds();
        else
        {
            if (Room::mask)
            {
                float xMove{ 0 }, yMove{ 0 };
                if (moveLeft || moveRight) xMove = xySpd * elapsedTime.asSeconds();
                if (moveUp || moveDown) yMove = xySpd * elapsedTime.asSeconds();
                
                if (moveLeft) {
                    if (!Room::mask->getPixel(floor((x - xMove)/Room::roomScale), floor(y/Room::roomScale)).r)
                        x -= xMove; else moveLeft = false; }
                if (moveRight) {
                    if (!Room::mask->getPixel(floor((x + xMove)/Room::roomScale), floor(y/Room::roomScale)).r)
                        x += xMove; else moveRight = false; }
                if (moveUp) {
                    if (!Room::mask->getPixel(floor(x/Room::roomScale), floor((y - yMove)/Room::roomScale)).r)
                        y -= yMove; else moveUp = false; }
                if (moveDown) {
                    if (!Room::mask->getPixel(floor(x/Room::roomScale), floor((y + yMove)/Room::roomScale)).r)
                        y += yMove; else moveDown = false; }
            }
            else
            {
                if (moveLeft) x -= xySpd * elapsedTime.asSeconds();
                if (moveRight) x += xySpd * elapsedTime.asSeconds();
                if (moveUp) y -= xySpd * elapsedTime.asSeconds();
                if (moveDown) y += xySpd * elapsedTime.asSeconds();
            }
            if (moveLeft || moveRight || moveUp || moveDown)
            {
                UpdateDepthPosition();
                sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
                if (drawDialogue) { dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
                    dialogue.setPosition(dialogueSprite.getPosition()); }
            }
            
            
            
            if (newPoint)
            {
                if (occupiedAt != -1)
                {
                    Room::nekoActivity[occupiedAt].occupied = false;
                    occupiedAt = -1;
                }
                int occupyRandom = rand() % 20;
                if (!occupyRandom)
                {
                    occupiedAt = -1; int i{ 0 };
                    do { occupiedAt = rand() % Room::nekoActivity.size(); }
                    while (Room::nekoActivity[occupiedAt].occupied && i < 5);
                    if (i == 5) { occupiedAt = -1;
                        for (i = 0; i < Room::nekoActivity.size() && occupiedAt == -1; ++i)
                            if (!Room::nekoActivity[i].occupied) occupiedAt = i;
                    }
                    if (occupiedAt != -1)
                    {
                        Room::nekoActivity[occupiedAt].occupied = true;
                        movingTo_x = Room::nekoActivity[occupiedAt].x * Room::roomScale;
                        movingTo_y = Room::nekoActivity[occupiedAt].y * Room::roomScale;
                        cout << base::utf8(info->name) << ": I'm occupied at: " << movingTo_x << " " << movingTo_y << endl;
                    }
                }
                else
                {
                    int i{ 0 }; do {
                        movingTo_x = rand() % Room::roomWidth;
                        movingTo_y = rand() % Room::roomHeight; ++i;
                    } while (Room::mask && Room::mask->getPixel(floor(movingTo_x/Room::roomScale), floor(movingTo_y/Room::roomScale)).r && i < 100);
                }
                newPoint = false;
            }
            else
            {
                if (!newPoint && !moveUp && !moveDown && !moveLeft && !moveRight) {
                    newPoint = true; if (occupiedAt == -1) elapsedWaiting = waitingTime + (rand() % 1000)/1000.f;
                    else { elapsedWaiting = waitingTime*4 + (rand() % 5000)/5000.f; } }
                
                // TODO: Pathfinding algorithm!
                if (x < movingTo_x - 30) { moveRight = true; moveLeft = false; }
                else if (x > movingTo_x + 30) { moveLeft = true; moveRight = false; }
                else { moveLeft = false; moveRight = false; }
                
                if (y < movingTo_y - 30) { moveDown = true; moveUp = false; }
                else if (y > movingTo_y + 30) { moveUp = true; moveDown = false; }
                else { moveDown = false; moveUp = false; }
            }
        }
    }
    void NekoEntity::UpdateDepthPosition()
    {
        if (positionInArray > 0)
        {
            NekoEntity* prev = rl::neko[positionInArray - 1];
            if (prev->y > y) { rl::neko[positionInArray - 1] = this; rl::neko[positionInArray] = prev;
                --positionInArray; ++prev->positionInArray; }
        }
        if (positionInArray + 1 < rl::neko.size())
        {
            NekoEntity* next = rl::neko[positionInArray + 1];
            if (next->y < y) { rl::neko[positionInArray + 1] = this; rl::neko[positionInArray] = next;
                ++positionInArray; --next->positionInArray; }
        }
    }
    void NekoEntity::PollEvent(sf::Event& event) { }
    void NekoEntity::Resize()
    {
        sprite.setScale(0.7 * relScale * Room::scale * gs::scale, 0.7 * relScale * Room::scale * gs::scale);
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        
        if (drawDialogue) {ResizeDialogue();
            dialogue.setOrigin(dialogue.getLocalBounds().width/2, dialogue.getLocalBounds().height + 44*gs::scale); }
    }
    void NekoEntity::UpdatePosition()
    {
        sprite.setPosition((Room::x + x) * Room::scale * gs::scale, (Room::y + y) * Room::scale * gs::scale);
        if (drawDialogue)
        {
            dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
            dialogue.setPosition(dialogueSprite.getPosition());
        }
    }
    void NekoEntity::ResizeDialogue()
    {
        dialogue.setCharacterSize(24 * gs::scale);
        dialogue.setScale(Room::scale, Room::scale);
        
        dialogueSprite.setScale(1.1*dialogue.getGlobalBounds().width/dialogueSprite.getLocalBounds().width, 1.5 * Room::scale * gs::scale);
        dialogueSprite.setPosition(sprite.getPosition().x, sprite.getGlobalBounds().top);
        dialogue.setPosition(dialogueSprite.getPosition());
    }
    void NekoEntity::Draw(sf::RenderWindow* window)
    {
        if (x > -Room::x - sprite.getGlobalBounds().width/2 && y > -Room::y && x < -Room::x + Room::xWidth + sprite.getGlobalBounds().width/2 && y < -Room::y + Room::yHeight + sprite.getGlobalBounds().height)
        {
            if (spriteLoaded) window->draw(sprite);
            if (drawDialogue) { window->draw(dialogueSprite); window->draw(dialogue); }
        }
    }
    
    
    
    Room::Room() { LoadTextureLevel();
        dialogueTexture = ic::LoadTexture(L"Data/Images/nekodialogue.png");
        if (spriteLoaded) { scale = 0.7; //TODO: Focus on the main neko
            x = gs::width/(2*gs::scale*scale) - sprite.getLocalBounds().width*roomScale/2;
            y = gs::height/(2*gs::scale*scale) - sprite.getLocalBounds().height*roomScale/2; }
        
        nekoListButton.setTexture(L"Data/Images/room_buttons.png");
        nekoListButton.sprite.setTextureRect({ 0,0,160,160 });
        questListButton.setTexture(L"Data/Images/room_buttons.png");
        questListButton.sprite.setTextureRect({ 160,0,160,160 });
        nekoTeamButton.setTexture(L"Data/Images/room_buttons.png");
        nekoTeamButton.sprite.setTextureRect({ 320,0,160,160 });
        nekoListButton.valign = questListButton.valign = nekoTeamButton.valign = Valign::Bottom;
        nekoListButton.halign = questListButton.halign = nekoTeamButton.halign = Halign::Left;
        nekoListButton.scale = questListButton.scale = nekoTeamButton.scale = 0.88;
        
        blackScreenShape.setFillColor(sf::Color(0,0,0,170));
        quitButton.setTexture(L"Data/Images/room_buttons.png");
        quitButton.sprite.setTextureRect({ 477,0,64,64 });
        quitButton.setScale(2);
        
        roomBackButton.setTexture(L"Data/Neko/Chibi/Vanilla.png");
        roomBackButton.sprite.setRotation(-70);
        
        
        ntmNekoButton.valign = Valign::Bottom;
        ntmNekoShape.setFillColor(sf::Color(0,0,0,100));
        ntmNekoShape.setOutlineColor(sf::Color::White);
    }
    Room::~Room() { ic::DeleteImage(L"Data/Images/heart.png"); }
    void Room::Switch(bool on)
    {
        active = on;
        if (on)
        {
            roomLevel = (int)rl::neko.size()/14; if (roomLevel != lastLevel) LoadTextureLevel();
            for (auto& n : rl::neko) if (!n->inited) n->Init();
            
            if (Player::self->neko)
                if (Player::self->neko->positionInArray >= 0 && Player::self->neko->positionInArray < rl::neko.size()) {
                    hasFocusOnNeko = true; focusOnNeko = Player::self->neko; }
            
            if (firstTimeIn)
            {
                auto* novel = menu->entity->AddComponent<VNLightComponents::Novel>();
                novel->lines.push_back(L"blackend");
                if (Player::self->neko)
                {
                    novel->lines.push_back(L"show " + Player::self->neko->info->name + L" cleft message:no");
                    novel->lines.push_back(Player::self->neko->info->name + L" \"Добро пожаловать в комнату, хозяин!~\"");
                    novel->lines.push_back(Player::self->neko->info->name + L" \"Здесь бегают все-все-все кошкодевочки, которых ты приютил к себе, воть <.<\"");
                }
                novel->lines.push_back(L"disappear fade:0.2");
                firstTimeIn = false;
            } else { if (scale > 1.7) scale = 1.7; else if (scale < 1) scale = 1; }
        } else { /*if (page == Page::neko) nintNeko->beingActionedWith = false;*/ page = Page::no; }
        Resize(gs::width, gs::height);
    }
    void Room::Destroy() { for (auto& n : rl::neko) n->Destroy(); }
    void Room::Update(const sf::Time& elapsedTime)
    {
        if (!active) return;
        if (gs::inGame) { active = false; return; }
        gs::requestWindowRefresh = true;
        for (auto& n : rl::neko) n->Update(elapsedTime);
        
        if (hasFocusOnNeko)
        {
            x = gs::width/(2*gs::scale*scale) - (focusOnNeko)->x;
            y = gs::height/(2*gs::scale*scale) - (focusOnNeko)->y + (focusOnNeko)->sprite.getLocalBounds().height/4*focusOnNeko->relScale;
            CalculateCameraPosition();
        }
    }
    void Room::PollEvent(sf::Event& event)
    {
        if (!active) return;
        for (auto& n : rl::neko) n->PollEvent(event);
        
        if (page == Page::neko) { }
        else if (nekoListButton.PollEvent(event)) menu->entity->SendMessage({"NekoGridUI :: Switch"});
        else if (questListButton.PollEvent(event)) { if (page == Page::questlist) page = Page::no; else page = Page::questlist; }
        else if (nekoTeamButton.PollEvent(event))
        {
            if (page == Page::nekoteam)
            {
                if (Player::self->neko) ic::DeleteImage(L"Data/Neko/Avatar/" + Player::self->neko->info->name + L".png");
                for (auto& n : Player::self->passiveNeko) ic::DeleteImage(L"Data/Neko/Avatar/" + n->info->name + L".png");
                /*if (Player::self->level < 50)*/ ic::DeleteImage(L"Data/Images/lock.png");
                ic::DeleteImage(L"Data/Images/plus.png");
                page = Page::no;
            }
            else
            {
                if (Player::self->neko) ic::PreloadTexture(L"Data/Neko/Avatar/" + Player::self->neko->info->name + L".png", 2, true, true);
                for (auto& n : Player::self->passiveNeko) ic::PreloadTexture(L"Data/Neko/Avatar/" + n->info->name + L".png", 2, true, true);
                /*if (Player::self->level < 50)*/ ic::PreloadTexture(L"Data/Images/lock.png");
                ic::PreloadTexture(L"Data/Images/plus.png");
                page = Page::nekoteam;
            }
        }
        else if (page == Page::nekolist) { }
        else if (page == Page::questlist) { if (quitButton.PollEvent(event)) page = Page::no; }
        else if (page == Page::nekoteam)
        {
            if (quitButton.PollEvent(event))
            {
                if (Player::self->neko) ic::DeleteImage(L"Data/Neko/Avatar/" + Player::self->neko->info->name + L".png");
                for (auto& n : Player::self->passiveNeko) ic::DeleteImage(L"Data/Neko/Avatar/" + n->info->name + L".png");
                /*if (Player::self->level < 50)*/ ic::DeleteImage(L"Data/Images/lock.png");
                ic::DeleteImage(L"Data/Images/plus.png");
                page = Page::no;
            }
            else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::TouchBegan || event.type == sf::Event::TouchEnded)
            {
                NekoBase* n = nullptr;
                float xx = ntmNekoStartXX, yy = 240*gs::scaley;
                for (int i = 0, maxNekoTeam = 4; i < maxNekoTeam; ++i)
                {
                    bool locked = ((i == 1 && Player::self->level < 16) || (i == 2 && Player::self->level < 25) || (i == 3 && Player::self->level < 35));
                    if (!locked)
                    {
                        if (!i) n = Player::self->neko->info; else if (Player::self->passiveNeko.size() >= i) n = Player::self->passiveNeko[i - 1]->info; else n = nullptr;
                        if (n)
                        {
                            ntmNekoButton.index = i;
                            sf::Texture* texture = ic::FindTexture(L"Data/Neko/Avatar/" + n->name + L".png");
                            ntmNekoButton.setTexture(texture);
                            if (texture) ntmNekoButton.setScale(168.f*ntmNekoScaling / texture->getSize().x);
                            ntmNekoButton.setPosition(xx, yy);
                        }
                        else
                        {
                            ntmNekoButton.index = i; sf::Texture* texture = nullptr;
                            texture = ic::FindTexture(L"Data/Images/plus.png");
                            ntmNekoButton.setTexture(texture);
                            if (texture) ntmNekoButton.setScale(158.f*ntmNekoScaling / texture->getSize().y);
                            ntmNekoButton.setPosition(xx, yy);
                        }
                        if (ntmNekoButton.PollEvent(event))
                        {
                            ntmChoosing = (i != 0 && !n) ? -1 : i;
                            if (!i) menu->entity->SendMessage({"NekoGridUI :: ChooseActive"});
                            else menu->entity->SendMessage({"NekoGridUI :: ChoosePassive"});
                        }
                    }
                    xx += 178*gs::scale * ntmNekoScaling;
                }
            }
        }
        else
        {
            if (roomBackButton.PollEvent(event)) { menu->screen = MainMenu::Screen::main; Switch(false); }
            else if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) || (event.type == sf::Event::TouchBegan))
            {
                if (event.type == sf::Event::MouseButtonPressed) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                if (event.touch.finger == 0)
                {
                    bool interception{ false };
                    for (auto it = rl::neko.rbegin(); it != rl::neko.rend() && !interception; ++it) {
                        interception = (*it)->sprite.getGlobalBounds().contains(dot.x, dot.y);
                        if (interception) { pressedNeko = *it; pressedNekoPos = { dot.x, dot.y }; } }
                    
                    move_dx = x - dot.x/(scale*gs::scale);
                    move_dy = y - dot.y/(scale*gs::scale);
                }
                else if (event.touch.finger == 1)
                {
                    float x0{ static_cast<float>(sf::Touch::getPosition(0).x) },
                          y0{ static_cast<float>(sf::Touch::getPosition(0).y) },
                          x1{ static_cast<float>(dot.x) },
                          y1{ static_cast<float>(dot.y) };
                    prevDistanceZoom = sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
                    prevZoom = scale;
                }
            }
            else if ((event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) || (event.type == sf::Event::TouchEnded && event.touch.finger == 0))
            {
                if (event.type == sf::Event::MouseButtonReleased) dot = { event.mouseButton.x, event.mouseButton.y };
                else dot = { event.touch.x, event.touch.y };
                
                bool interception{ false };
                for (auto it = rl::neko.rbegin(); it != rl::neko.rend() && !interception; ++it)
                {
                    interception = (*it)->sprite.getGlobalBounds().contains(dot.x, dot.y);
                    if (interception && pressedNeko == *it && sqrt(pow(pressedNekoPos.x - dot.x, 2) + pow(pressedNekoPos.y - dot.y, 2)) < 15)
                    { nintReturnTo = Page::no; menu->entity->SendMessage({"NekoUI :: SelectNeko", *it}); menu->entity->SendMessage({"NekoUI :: Show"}); }
                }
            }
            else if (event.type == sf::Event::TouchMoved || (event.type == sf::Event::MouseMoved && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)))
            {
                if (event.type == sf::Event::MouseMoved) dot = { event.mouseMove.x, event.mouseMove.y };
                else dot = { event.touch.x, event.touch.y };
                
                if (sf::Touch::isDown(0) && event.touch.finger == 1 && prevDistanceZoom != 0)
                {
                    if (hasFocusOnNeko) { focusOnNeko->beingActionedWith = false; hasFocusOnNeko = false; }
                    float x0{ static_cast<float>(sf::Touch::getPosition(0).x) },
                          y0{ static_cast<float>(sf::Touch::getPosition(0).y) },
                          x1{ static_cast<float>(dot.x) },
                          y1{ static_cast<float>(dot.y) };
                    float distance = sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1));
                    scale = prevZoom * (distance/prevDistanceZoom);
                    if (scale < 0.3) scale = 0.3; else if (scale > 2.6) scale = 2.6;
                    xWidth = gs::width/(gs::scale * scale); yHeight = gs::height/(gs::scale * scale);
                    
                    sprite.setScale(roomScale * gs::scale * scale, roomScale * gs::scale * scale);
                    CalculateCameraScale();
                }
                else if (event.type != sf::Event::TouchMoved || event.touch.finger == 0)
                {
                    if (hasFocusOnNeko && (dot.x > (175 * gs::scale)*3*0.88 || dot.y < (gs::height - (160*gs::scale)*0.88 - 15*gs::scale))) {
                        focusOnNeko->beingActionedWith = false; hasFocusOnNeko = false; }
                    x = move_dx + dot.x/(scale*gs::scale);
                    y = move_dy + dot.y/(scale*gs::scale);
                    CalculateCameraPosition();
                }
            }
            else if (event.type == sf::Event::MouseWheelScrolled && event.mouseWheelScroll.delta != 0 && event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
            {
                float scalePrev = scale;
                scale += scrollSensitivity * event.mouseWheelScroll.delta;
                if (scale < 0.3) scale = 0.3; else if (scale > 2.6) scale = 2.6;
                xWidth = gs::width/(gs::scale * scale); yHeight = gs::height/(gs::scale * scale);
                
                x -= (gs::width/(gs::scale*scale) - gs::width/(gs::scale*scale) * (scalePrev/scale))/2;
                y -= (gs::height/(gs::scale*scale) - gs::height/(gs::scale*scale) * (scalePrev/scale))/2;
                
                sprite.setScale(roomScale * gs::scale * scale, roomScale * gs::scale * scale);
                CalculateCameraScale();
            }
        }
    }
    void Room::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        sprite.setScale(roomScale * gs::scale * scale, roomScale * gs::scale * scale);
        xWidth = gs::width/(gs::scale * scale); yHeight = gs::height/(gs::scale * scale);
        roomGlobalBounds = { -x, -y, xWidth, yHeight };
        CalculateCameraScale();
        
        nekoListButton.Resize(width, height); nekoListButton.setPosition(15*gs::scale, height - 15*gs::scale);
        questListButton.Resize(width, height); questListButton.setPosition(15*gs::scale + (175 * gs::scale)*0.88, height - 15*gs::scale);
        nekoTeamButton.Resize(width, height); nekoTeamButton.setPosition(15*gs::scale + (175 * gs::scale)*2*0.88, height - 15*gs::scale);
        
        blackScreenShape.setSize({(float)gs::width, (float)gs::height});
        quitButton.Resize(width, height); quitButton.setPosition(gs::width - gs::width/12, gs::height/8);
        
        roomBackButton.Resize(width, height);
        roomBackButton.setPosition(width, height/3.5);
        
        
        ntmNekoStartXX = 178*gs::scale * 4 + 20*gs::scale;
        if (ntmNekoStartXX > width/* - quitButton.sprite.getGlobalBounds().width*/)
            ntmNekoScaling = (width/* - quitButton.sprite.getGlobalBounds().width*/)/ntmNekoStartXX;
        ntmNekoStartXX = (width/* - quitButton.sprite.getGlobalBounds().width*/)/2 - (ntmNekoStartXX/2 - (168/2 + 10)*gs::scale)*ntmNekoScaling;
        ntmNekoButton.Resize(width, height);
        ntmNekoShape.setSize({168*gs::scale, 168*gs::scale});
        ntmNekoShape.setOrigin(ntmNekoShape.getSize().x/2, ntmNekoShape.getSize().y);
        ntmNekoShape.setOutlineThickness(3*gs::scale);
    }
    void Room::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        if (spriteLoaded) window->draw(sprite);
        for (auto& n : rl::neko) n->Draw(window);
        
        roomBackButton.Draw(window);
        if (page == Page::nekolist || page == Page::nekoteam || page == Page::questlist)
        {
            window->draw(blackScreenShape);
            quitButton.Draw(window);
        }
        if (page == Page::neko)
        {
            nekoListButton.Draw(window);
            questListButton.Draw(window);
            nekoTeamButton.Draw(window);
        }
        
        if (page == Page::neko) { }
        else if (page == Page::nekolist) { }
        else if (page == Page::nekoteam)
        {
            NekoBase* n = nullptr;
            //float xx = 168*gs::scale, yy = 240*gs::scale;
            float xx = ntmNekoStartXX, yy = 240*gs::scaley;
            for (int i = 0, maxNekoTeam = 4; i < maxNekoTeam; ++i)
            {
                if (!i) n = Player::self->neko->info; else if (Player::self->passiveNeko.size() >= i) n = Player::self->passiveNeko[i - 1]->info; else n = nullptr;
                if (n)
                {
                    ntmNekoShape.setPosition(xx, yy);
                    ntmNekoShape.setScale(ntmNekoScaling, ntmNekoScaling);
                    ntmNekoShape.setFillColor(sf::Color(n->color.r, n->color.g, n->color.b, 150));
                    if (i == 0) ntmNekoShape.setOutlineColor(sf::Color(255, 60, 60, 255));
                    else ntmNekoShape.setOutlineColor(sf::Color(60, 60, 255, 255));
                    window->draw(ntmNekoShape);
                    
                    ntmNekoButton.index = i;
                    sf::Texture* texture = ic::FindTexture(L"Data/Neko/Avatar/" + n->name + L".png");
                    ntmNekoButton.setTexture(texture);
                    if (texture) ntmNekoButton.setScale(168.f*ntmNekoScaling / texture->getSize().x);
                    ntmNekoButton.setPosition(xx, yy);
                    ntmNekoButton.Draw(window);
                }
                else
                {
                    ntmNekoShape.setPosition(xx, yy);
                    ntmNekoShape.setScale(ntmNekoScaling, ntmNekoScaling);
                    ntmNekoShape.setFillColor(sf::Color(90, 90, 90, 160));
                    ntmNekoShape.setOutlineColor(sf::Color::White);
                    window->draw(ntmNekoShape);
                    
                    bool locked = ((i == 1 && Player::self->level < 16) || (i == 2 && Player::self->level < 25) || (i == 3 && Player::self->level < 35));
                    ntmNekoButton.index = i; sf::Texture* texture = nullptr;
                    if (locked) texture = ic::FindTexture(L"Data/Images/lock.png");
                    else texture = ic::FindTexture(L"Data/Images/plus.png");
                    ntmNekoButton.setTexture(texture);
                    if (texture) ntmNekoButton.setScale(158.f*ntmNekoScaling / texture->getSize().y);
                    ntmNekoButton.setPosition(xx, yy - 5.f*gs::scale);
                    ntmNekoButton.Draw(window);
                }
                xx += 178*gs::scale * ntmNekoScaling;
                //if (xx > gs::width - 168*gs::scale) { xx = 168*gs::scale; yy += 178*gs::scale; }
            }
            quitButton.Draw(window);
        }
        
        if (page != Page::neko)
        {
            nekoListButton.Draw(window);
            questListButton.Draw(window);
            nekoTeamButton.Draw(window);
        }
    }
    void Room::CalculateCameraPosition()
    {
        if (sprite.getGlobalBounds().width <= gs::width) x = gs::width/(2*gs::scale*scale) - sprite.getLocalBounds().width*roomScale/2;
        else if (x > 100/(gs::scale*scale)) x = 100/(gs::scale*scale);
        else if (x < -sprite.getLocalBounds().width*roomScale + gs::width/(gs::scale*scale) - 100/(gs::scale*scale))
            x = -sprite.getLocalBounds().width*roomScale + gs::width/(gs::scale*scale) - 100/(gs::scale*scale);
        if (sprite.getGlobalBounds().height <= gs::height) y = gs::height/(2*gs::scale*scale) - sprite.getLocalBounds().height*roomScale/2;
        else if (y > 100/(gs::scale*scale)) y = 100/(gs::scale*scale);
        else if (y < -sprite.getLocalBounds().height*roomScale + gs::height/(gs::scale*scale) - 100/(gs::scale*scale))
            y = -sprite.getLocalBounds().height*roomScale + gs::height/(gs::scale*scale) - 100/(gs::scale*scale);
        
        for (auto& n : rl::neko) n->UpdatePosition();
        sprite.setPosition(x * gs::scale * scale, y * gs::scale * scale);
    }
    void Room::CalculateCameraScale()
    {
        if (sprite.getGlobalBounds().width <= gs::width) x = gs::width/(2*gs::scale*scale) - sprite.getLocalBounds().width*roomScale/2;
        else if (x > 100/(gs::scale*scale)) x = 100/(gs::scale*scale);
        else if (x < -sprite.getLocalBounds().width*roomScale + gs::width/(gs::scale*scale) - 100/(gs::scale*scale))
            x = -sprite.getLocalBounds().width*roomScale + gs::width/(gs::scale*scale) - 100/(gs::scale*scale);
        if (sprite.getGlobalBounds().height <= gs::height) y = gs::height/(2*gs::scale*scale) - sprite.getLocalBounds().height*roomScale/2;
        else if (y > 100/(gs::scale*scale)) y = 100/(gs::scale*scale);
        else if (y < -sprite.getLocalBounds().height*roomScale + gs::height/(gs::scale*scale) - 100/(gs::scale*scale))
            y = -sprite.getLocalBounds().height*roomScale + gs::height/(gs::scale*scale) - 100/(gs::scale*scale);
        
        for (auto& n : rl::neko) n->Resize();
        sprite.setPosition(x * gs::scale * scale, y * gs::scale * scale);
    }
    void Room::LoadTextureLevel()
    {
        if (lastLevel != roomLevel) {mask = nullptr;
            ic::DeleteImage(L"Data/Images/room_mask0_" + std::to_wstring(roomLevel) + L".png");
            ic::DeleteImage(L"Data/Images/room0_" + std::to_wstring(roomLevel) + L".jpg"); }
        lastLevel = roomLevel = (int)rl::neko.size() / 12;
        if (roomLevel > 1) roomLevel = 1;
        
        mask = ic::LoadImage(L"Data/Images/room_mask0_" + std::to_wstring(roomLevel) + L".png");
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/room0_" + std::to_wstring(roomLevel) + L".jpg");
        if ((spriteLoaded = texture))
        {
            texture->setSmooth(false);
            Room::roomWidth = (texture->getSize().x - 90) * roomScale;
            Room::roomHeight = (texture->getSize().y - 90) * roomScale;
            sprite.setTexture(*texture, true);
            for (auto& n : rl::neko) n->inited = false;
        }
    }
    void Room::RecieveMessage(MessageHolder& message)
    {
        if (!active) return;
                
        if (message.info == "NekoUI :: SelectNeko") { hasFocusOnNeko = true; focusOnNeko = reinterpret_cast<NekoEntity*>(message.address); }
        else if (message.info == "NekoUI :: Choosed" && page == Page::nekoteam)
        {
            if (ntmChoosing == 0)
            {
                if (message.address)
                {
                    NekoEntity* neko = reinterpret_cast<NekoEntity*>(message.address);
                    if (neko && neko != Player::self->neko)
                    {
                        ic::DeleteImage(L"Data/Neko/Avatar/" + Player::self->neko->info->name + L".png");
                        ic::DeleteImage(L"Data/Neko/Person/" + Player::self->neko->info->name + L".png");
                        Player::self->neko->highlightedDuringSelection = false;
                        Player::self->neko = neko; neko->highlightedDuringSelection = true;
                        ic::PreloadTexture(L"Data/Neko/Avatar/" + neko->info->name + L".png", 2, true, true);
                        if (Player::self->neko && menu)
                        {
                            sf::Texture* texture = ic::LoadTexture(L"Data/Neko/Person/" + Player::self->neko->info->name + L".png");
                            if (texture) {
                                menu->neko.setTexture(*texture, true);
                                menu->neko.setOrigin(texture->getSize().x/2 + Player::self->neko->info->personSprite_offsetX, texture->getSize().y + Player::self->neko->info->personSprite_offsetY);
                                menu->nekoRelScale = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * Player::self->neko->info->novelScale;
                            }
                            menu->nekoSpriteLoaded = texture;
                            menu->Resize(gs::width, gs::height);
                        }
                        Player::self->SaveRoom();
                    }
                }
            }
            else if (ntmChoosing == -1)
            {
                if (message.address)
                {
                    NekoEntity* neko = reinterpret_cast<NekoEntity*>(message.address);
                    Player::self->passiveNeko.push_back(neko); neko->highlightedDuringSelection = true;
                    ic::PreloadTexture(L"Data/Neko/Avatar/" + neko->info->name + L".png", 2, true, true);
                    Player::self->SaveRoom();
                }
            }
            else if (Player::self->passiveNeko.size() >= ntmChoosing)
            {
                NekoEntity* ours = Player::self->passiveNeko[ntmChoosing - 1];
                ours->highlightedDuringSelection = false;
                ic::DeleteImage(L"Data/Neko/Avatar/" + ours->info->name + L".png");
                std::vector<NekoEntity*>::iterator it = Player::self->passiveNeko.begin() + (ntmChoosing - 1);
                Player::self->passiveNeko.erase(it);
                if (message.address)
                {
                    NekoEntity* neko = reinterpret_cast<NekoEntity*>(message.address);
                    neko->highlightedDuringSelection = true;
                    ic::PreloadTexture(L"Data/Neko/Avatar/" + neko->info->name + L".png", 2, true, true);
                    Player::self->passiveNeko.insert(Player::self->passiveNeko.begin() + (ntmChoosing - 1), neko);
                }
                Player::self->SaveRoom();
            }
            menu->entity->SendMessage({"NekoUI :: Close"});
            menu->entity->SendMessage({"NekoGridUI :: Close"});
        }
        /*else if (message.info == "NekoUI :: Show" && page == Page::neko)
        else if (message.info == "NekoUI :: Close" && page == Page::neko) page = nintReturnTo;*/
    }
    
    
    
    LocationPlace::LocationPlace(const std::wstring& name, const std::wstring& background, const int& difficulty) : name(name), background(background), difficulty(difficulty) { }
    vector<LocationPlace*> MainMenu::places = {
        new LocationPlace(L"Парк", L"Data/Images/park.jpg", 0),
        new LocationPlace(L"Морская аллея", L"Data/Images/Background/Riverside.jpg", 3),
        new LocationPlace(L"Город", L"Data/Images/Background/TokyoSunset.jpg", 5),
        new LocationPlace(L"Одинокое дерево", L"Data/Images/Background/LonelyTree.jpg", 7),
        new LocationPlace(L"Пещера", L"Data/Images/Background/Cave.jpg", 10),
    };
    void MainMenu::Init()
    {
        Player::self->Init();
        room.menu = this;
        
        sf::Texture* texture = ic::LoadTexture(L"Data/Images/MainMenu1.jpg");
        if ((spriteLoaded = texture))
        {
            background.setTexture(*texture);
            background.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
        }
        
        if (Player::self->neko)
        {
            texture = ic::LoadTexture(L"Data/Neko/Person/" + Player::self->neko->info->name + L".png");
            if (texture)
            {
                nekoTextureName = Player::self->neko->info->name;
                neko.setTexture(*texture);
                neko.setOrigin(texture->getSize().x/2 + Player::self->neko->info->personSprite_offsetX, texture->getSize().y + Player::self->neko->info->personSprite_offsetY);
                nekoRelScale = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * Player::self->neko->info->novelScale;
            }
            nekoSpriteLoaded = texture;
        }
        
        playButton.setFont(L"Pacifica.ttf");
        playButton.setCharacterSize(140);
        playButton.setString(L"Прогулка");
        playButton.valign = Valign::Bottom;
        
        inventoryButton.setFont(L"Pacifica.ttf");
        inventoryButton.setCharacterSize(82);
        inventoryButton.setString(L"Шкафчик");
        inventoryButton.valign = Valign::Bottom;
        lootboxesCountText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        lootboxesCountText.setFillColor(sf::Color::White);
        auto it = Inventory::map.find("Lootbox");
        if (it != Inventory::map.end() && it->second->count != 0) lootboxesCountText.setString(std::to_string(it->second->count));
        lootboxesCircle.setFillColor(sf::Color(255,0,0,170));
        
        backButton.setFont(L"Pacifica.ttf");
        backButton.setCharacterSize(90);
        backButton.setString(L"Назад");
        backButton.valign = Valign::Bottom;
        
        walkButton.setFont(L"Pacifica.ttf");
        walkButton.setCharacterSize(110);
        walkButton.setString(L"Пойдём сюда!");
        walkButton.valign = Valign::Bottom;
        addDifficultyButton.setFont(L"Pacifica.ttf");
        addDifficultyButton.setCharacterSize(90);
        addDifficultyButton.setString(L"Сложность: 0");
        addDifficultyButton.valign = Valign::Center;
        
        accountButton.setFont(L"Pacifica.ttf");
        accountButton.setCharacterSize(72);
        accountButton.setString(L"[" + Player::self->username + L"]");
        accountButton.valign = Valign::Top;
        accountBackButton.setFont(L"Pacifica.ttf");
        accountBackButton.setCharacterSize(90);
        if (Player::self->username == L"Закрыть") accountBackButton.setString(L"[Назад]");
        else accountBackButton.setString(L"[Закрыть]");
        accountBackButton.valign = Valign::Top;
        
        resetButton.setFont(L"Pacifica.ttf");
        resetButton.setCharacterSize(48);
        resetButton.setString(L"[Reset NekoLibrary]");
        resetButton.halign = Halign::Left;
        resetButton.valign = Valign::Bottom;
        
        roomButton.setTexture(L"Data/Neko/Chibi/Chocola.png");
        roomButton.sprite.setRotation(-70);
        
        dialogueText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        dialogueText.setFillColor(sf::Color::White);
        dialogueText.setOutlineColor(sf::Color::Black);
        dialogueText.setString(L"С возвращением, хозяин!~ <3");
        dialogueButton.setTexture(L"Data/Images/dialogue1.png");
        dialogueButton.valign = Valign::Bottom;
        dialogueButton.characterScale = true;
        
        backShape.setFillColor(sf::Color(0, 0, 0, 185));
        accountBackShape.setFillColor(sf::Color(0, 0, 0, 185));
        expShape.setFillColor(sf::Color(30, 30, 30, 255));
        helperText.setFillColor(sf::Color::White);
        helperText.setOutlineColor(sf::Color::Black);
        helperText.setString(L"Уровень:");
        helperText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        
        
        locLeftButton.setTexture(L"Data/Images/stageArrow.png");
        locLeftButton.setScale(0.7);
        locLeftButton.halign = Halign::Left;
        locRightButton.setTexture(L"Data/Images/stageArrow.png");
        locRightButton.setScale(0.7); locRightButton.invertXAxes = true;
        locRightButton.halign = Halign::Right;
        locationNameText.setFont(*fc::GetFont(L"Pacifica.ttf"));
        locationNameText.setOutlineColor(sf::Color::Black);
        
        
        
        //ic::PreloadTexture(L"Data/Images/park.jpg", 0, false);
        ic::PreloadTexture(L"Data/Images/heartsshape.png", 0, false);
        ic::PreloadTexture(L"Data/Images/heart.png", 0, false);
        ic::PreloadTexture(L"Data/Images/groping-hand.png", 0, false);
        ic::PreloadTexture(L"Data/Images/pause_button.png", 0, false);
        for (int i = 0; i < nl::neko.size(); ++i)
        {
            //ic::PreloadTexture(L"Data/Neko/" + nl::neko[i]->name + L".png", 0, false);
            sc::PreloadSound(L"Data/Neko/Nya/" + nl::neko[i]->name + L".ogg", 0, false);
        }
        
        
        //entity->AddComponent<Popup>(L"Вы достигли 15-го уровня!", L"Теперь Вам доступна комната с кошкодевочками;\nА также Вы можете взять с собой в отряд на прогулку ещё одну кошкодевочку с пассивной способностью!");
        
        
        /*auto* novel = entity->AddComponent<VNLightComponents::Novel>();
        novel->lines.push_back(L"blackend fade:0 message:no");
        novel->lines.push_back(L"draw block");
        novel->lines.push_back(L"wait 0.4");
        novel->lines.push_back(L"background \"park.jpg\" ");
        
        novel->lines.push_back(L"\"Добро пожаловать в NekoScience!\"");
        novel->lines.push_back(L"show Vanilla left message:no");
        novel->lines.push_back(L"show Chocola right");
        novel->lines.push_back(L"show Maple center");
        novel->lines.push_back(L"Maple \"Приветь, ня!~ :з\"");
        
        novel->lines.push_back(L"\"Йа диалог!!! Отень-отень-отень длинный диалог (на удивление, уфууфу) >3<\"");
        
        novel->lines.push_back(L"draw unblock");
        novel->lines.push_back(L"disappear fade:0.5");*/
        
        
        /*auto* novel = entity->AddComponent<VNLightComponents::Novel>();
        novel->lines.push_back(L"blackend");
        
        novel->lines.push_back(L"Flandre \"Йа неко!! >3<\"");
        novel->lines.push_back(L"show Vanilla left message:no");
        novel->lines.push_back(L"show Chocola right");
        novel->lines.push_back(L"show Maple center");
        novel->lines.push_back(L"Maple \"Приветь, ня!~ :з\"");
        
        novel->lines.push_back(L"background \"park.jpg\" ");
        novel->lines.push_back(L"draw block");
        
        novel->lines.push_back(L"\"Йа диалог!!! Отень-отень-отень длинный диалог (на удивление, уфууфу) >3<\"");
        
        novel->lines.push_back(L"draw unblock");
        novel->lines.push_back(L"disappear fade:0.5");*/
    }
    void MainMenu::Destroy()
    {
        ic::DeleteImage(L"Data/Images/MainMenu1.jpg");
        ic::DeleteImage(L"Data/Neko/Person/Vanilla.png");
        
        //ic::SetDestroyable(L"Data/Images/park.jpg", true);
        ic::SetDestroyable(L"Data/Images/heartsshape.png", true);
        ic::SetDestroyable(L"Data/Images/heart.png", true);
        ic::SetDestroyable(L"Data/Images/groping-hand.png", true);
        ic::SetDestroyable(L"Data/Images/pause_button.png", true);
        /*for (int i = 0; i < nl::neko.size(); ++i)
        {
            ic::SetDestroyable(L"Data/Neko/" + nl::neko[i].name + L".png", true);
            sc::SetDestroyable(L"Data/Sounds/" + nl::neko[i].name + L".ogg", true);
        }*/
    }
    void MainMenu::Update(const sf::Time& elapsedTime)
    {
        if (!active) return;
        if (screen == Screen::room) room.Update(elapsedTime);
        else if (screen == Screen::stage && locationChanged)
        {
            locationNameText.setString(places[placeSelected]->name);
            locationNameText.setOrigin(locationNameText.getGlobalBounds().width/2, locationNameText.getGlobalBounds().height/2);
            sf::Texture* texture = ic::LoadTexture(places[placeSelected]->background);
            if (texture)
            {
                locationSprite.setTexture(*texture, true);
                locationSprite.setOrigin(texture->getSize().x/2, texture->getSize().y/2);
                float factorX = (float)gs::width / texture->getSize().x;
                float factorY = (float)gs::height / texture->getSize().y;
                float scaleFactor = factorX > factorY ? factorX : factorY;
                locationSprite.setScale(0.7*scaleFactor, 0.7*scaleFactor);
                locationSprite.setPosition(gs::width/2, gs::height/2);
            }
            locationChanged = false;
        }
    }
    void MainMenu::PollEvent(sf::Event& event)
    {
        if (!active) return;
        if (gs::ignoreEvent) return;
        
        float yy; int i; bool deselect;
        switch (screen)
        {
            case Screen::main:
                if (playButton.PollEvent(event)) {
                    if (placeSelected < 0) placeSelected = 0;
                    if (placeSelected > places.size()) placeSelected = places.size() - 1;
                    locationChanged = true;
                    screen = Screen::stage; /*gs::isPause = false; active = false; entity->AddComponent<NekoNinja::Controller>();*/ }
                else if (accountButton.PollEvent(event)) screen = Screen::account;
                else if (resetButton.PollEvent(event))
                {
                    Player::self->level = 1; Player::self->exp = 0;
                    Player::self->topScore = 0; Player::self->topStrike = 0;
                    Player::self->totalScore = 0;
                    Player::self->SaveData(); Player::self->SaveRoom();
                    Player::self->ResetNekos(); roomLibrary::clear();
                    for (auto& n : nl::neko) if (n->name == L"Shigure") n->tamed = true;
                    if (nl::neko.size())
                    {
                        vector<NekoBase*>::iterator it = nl::neko.begin(); bool found{ false };
                        while (it != nl::neko.end() && !found) { if (!(found = (*it)->name == L"Shigure")) ++it; }
                        if (!found) it = nl::neko.begin();
                        if (it != nl::neko.end()) rl::neko.emplace_back(new NekoEntity(*it, rl::neko.size()));
                    }
                }
                else if (dialogueButton.PollEvent(event)) dialogueButton.setVisible(false);
                else if (roomButton.PollEvent(event) && Player::self->roomUnlocked) { screen = Screen::room; room.Switch(true); }
                else if (inventoryButton.PollEvent(event)) {
                    if (!gs::verticalOrientation)
                    {
                        auto it = Inventory::map.find("Lootbox");
                        if (it != Inventory::map.end() && it->second->count != 0) entity->SendMessage({"Inventory :: SelectItem", it->second});
                    }
                    entity->SendMessage({"Inventory :: Show"}); screen = Screen::inventory; }
                break;
                
            case Screen::stage:
                if (walkButton.PollEvent(event)) { gs::isPause = false; active = false;
                    entity->AddComponent<Controller>(ControllerSettings(places[placeSelected]->difficulty, places[placeSelected]->background)); }
                else if (locLeftButton.PollEvent(event))
                {
                    locationChanged = true; ic::DeleteImage(places[placeSelected]->background);
                    --placeSelected; if (placeSelected < 0) placeSelected = places.size() - 1;
                }
                else if (locRightButton.PollEvent(event))
                {
                    locationChanged = true; ic::DeleteImage(places[placeSelected]->background);
                    ++placeSelected; if (placeSelected >= places.size()) placeSelected = 0;
                }
                /*else if (addDifficultyButton.PollEvent(event)) {
                    ++difficultyStartValue; addDifficultyButton.setString(L"Сложность: " + std::to_wstring(difficultyStartValue)); }*/
                else if (backButton.PollEvent(event)) { ic::DeleteImage(places[placeSelected]->background); screen = Screen::main; }
                break;
                
            case Screen::inventory: break;
            case Screen::account: if (accountBackButton.PollEvent(event)) screen = Screen::main; break;
            case Screen::room: room.PollEvent(event); break;
            default: break;
        }
    }
    void MainMenu::Resize(unsigned int width, unsigned int height)
    {
        if (!active) return;
        
        inventoryButton.Resize(width, height);
        playButton.Resize(width, height);
        verticalOrientation = ((float)width/height < 1.3);
        if (verticalOrientation)
        {
            inventoryButton.setPosition(width/2, height - 65*gs::scaley);
            playButton.setPosition(width/2, inventoryButton.text.getGlobalBounds().top - inventoryButton.text.getGlobalBounds().height + 20*gs::scale);
        }
        else
        {
            playButton.setPosition(width/2, height - height/10);
            inventoryButton.setPosition(gs::width - (gs::width - (playButton.text.getGlobalBounds().left + playButton.text.getGlobalBounds().width))/2, height - height/10);
            if (inventoryButton.text.getGlobalBounds().width > (gs::width - (playButton.text.getGlobalBounds().left + playButton.text.getGlobalBounds().width)))
            {
                float scalef = (gs::width - (playButton.text.getGlobalBounds().left + playButton.text.getGlobalBounds().width))/inventoryButton.text.getGlobalBounds().width;
                inventoryButton.text.setScale(scalef, 1);
            }
        }
        //lootboxesCountText.setOutlineThickness(1 * gs::scale);
        lootboxesCountText.setCharacterSize(30 * gs::scale);
        lootboxesCircle.setOutlineThickness(2 * gs::scale);
        lootboxesCircle.setRadius(lootboxesCountText.getGlobalBounds().width * 1.7);
        lootboxesCircle.setPosition(inventoryButton.text.getGlobalBounds().left + inventoryButton.text.getGlobalBounds().width - 16*gs::scale, inventoryButton.text.getGlobalBounds().top - 10*gs::scale);
        if (lootboxesCircle.getGlobalBounds().left + lootboxesCircle.getGlobalBounds().width > gs::width)
            lootboxesCircle.setPosition(gs::width - lootboxesCircle.getGlobalBounds().width, lootboxesCircle.getPosition().y);
        lootboxesCountText.setPosition(lootboxesCircle.getGlobalBounds().left + lootboxesCircle.getGlobalBounds().width/2 - lootboxesCountText.getGlobalBounds().width/2 - 1*gs::scale, lootboxesCircle.getGlobalBounds().top + lootboxesCircle.getGlobalBounds().height/2 - lootboxesCountText.getGlobalBounds().height/2 - 7*gs::scale);
        
        dialogueButton.Resize(width, height);
        if (gs::width < dialogueButton.sprite.getGlobalBounds().width)
            dialogueButton.setScale((float)gs::width/dialogueButton.sprite.getGlobalBounds().width);
        dialogueButton.setPosition(width/2, playButton.text.getGlobalBounds().top);
        dialogueText.setOutlineThickness(1 * gs::scale);
        dialogueText.setCharacterSize(40 * gs::scale);
        dialogueText.setPosition(dialogueButton.sprite.getGlobalBounds().left + 80*gs::scale, dialogueButton.sprite.getGlobalBounds().top + 20*gs::scale);
        dialogueText.setScale(1, 1);
        if (dialogueText.getGlobalBounds().width > (dialogueButton.sprite.getGlobalBounds().width - 160*gs::scale))
            dialogueText.setScale((dialogueButton.sprite.getGlobalBounds().width - 160*gs::scale) / dialogueText.getGlobalBounds().width, dialogueText.getScale().y);
        
        accountButton.Resize(width, height); accountButton.setPosition(width/2, -5*gs::scale);
        resetButton.Resize(width, height); resetButton.setPosition(30*gs::scale, height - 20*gs::scale);
        if (Player::self->roomUnlocked) { roomButton.Resize(width, height); roomButton.setPosition(width, height/3.5); }
        else roomButton.setPosition(-width, 0);
        
        if (spriteLoaded)
        {
            float factorX = (float)width / background.getTexture()->getSize().x;
            float factorY = (float)height / background.getTexture()->getSize().y;
            float scaleFactor = factorX > factorY ? factorX : factorY;
            background.setScale(scaleFactor, scaleFactor);
            background.setPosition(width/2, height/2);
            
            if (nekoSpriteLoaded)
            {
                neko.setScale(nekoRelScale*1.5*gs::scScale, nekoRelScale*1.5*gs::scScale);
                if (neko.getGlobalBounds().height + 20*gs::scale > gs::height)
                    neko.setPosition(width/2, neko.getGlobalBounds().height + 20*gs::scale);
                else neko.setPosition(width/2, height);
            }
        }
        
        room.Resize(width, height);
        
        backShape.setSize({ (float)width, (float)height });
        accountBackShape.setSize({width * 5.f/6, height * 5.f/6});
        accountBackShape.setPosition((width * 1.f/6)/2, (height * 1.f/6)/2);
        accountBackButton.Resize(width, height);
        accountBackButton.setPosition(width/2, /*accountBackShape.getPosition().y*/ -5*gs::scale);
        
        helperText.setOutlineThickness(gs::scale);
        helperText.setCharacterSize((unsigned int)(78 * gs::scale));
        helperText.setString(L"Уровень:");
        expShape.setSize({accountBackShape.getSize().x * 0.9f, helperText.getGlobalBounds().height + 10*gs::scale});
        expShape.setPosition(accountBackShape.getPosition().x + accountBackShape.getSize().x*0.05f, 0);
        
        
        if (screen == Screen::stage)
        {
            float factorX = (float)width / locationSprite.getTexture()->getSize().x;
            float factorY = (float)height / locationSprite.getTexture()->getSize().y;
            float scaleFactor = factorX > factorY ? factorX : factorY;
            locationSprite.setScale(0.7*scaleFactor, 0.7*scaleFactor);
            locationSprite.setPosition(width/2, height/2);
        }
        locationNameText.setCharacterSize(106*gs::scale);
        locationNameText.setOutlineThickness(2*gs::scale);
        locationNameText.setPosition(gs::width/2, 40*gs::scale);
        locationNameText.setOrigin(locationNameText.getGlobalBounds().width/2, locationNameText.getGlobalBounds().height/2);
        locLeftButton.Resize(width, height);
        locRightButton.Resize(width, height);
        locLeftButton.setPosition(gs::width/10, gs::height/2);
        locRightButton.setPosition(gs::width - gs::width/10 - locRightButton.sprite.getGlobalBounds().width, gs::height/2);
        
        backButton.Resize(width, height); backButton.setPosition(width/2, height - 72*gs::scaley);
        walkButton.Resize(width, height); walkButton.setPosition(width/2, height - height/5);
    }
    void MainMenu::Draw(sf::RenderWindow* window)
    {
        if (!active) return;
        if (gs::ignoreDraw) return;
        switch (screen)
        {
            case Screen::main: case Screen::account:
                if (spriteLoaded) window->draw(background);
                if (nekoSpriteLoaded) window->draw(neko);
                playButton.Draw(window);
                accountButton.Draw(window);
                resetButton.Draw(window);
                roomButton.Draw(window);
                inventoryButton.Draw(window);
                window->draw(lootboxesCircle);
                window->draw(lootboxesCountText);
            
                dialogueButton.Draw(window);
                if (dialogueButton.visible) window->draw(dialogueText);
                
                if (screen == Screen::account)
                {
                    //window->draw(accountBackShape);
                    window->draw(backShape);
                    accountBackButton.Draw(window);
                    
                    float yy = gs::height/3;
                    
                    helperText.setCharacterSize((unsigned int)(78 * gs::scale));
                    helperText.setString(L"Уровень:    " + std::to_wstring(Player::self->level));
                    helperText.setPosition(accountBackShape.getPosition().x + 10*gs::scalex, yy);
                    window->draw(helperText); yy += helperText.getGlobalBounds().height + 30*gs::scaley;
                    
                    expShape.setPosition(expShape.getPosition().x, yy);
                    window->draw(expShape); expShape.setFillColor(sf::Color(255, 37, 142, 255));
                    expShape.setSize({expShape.getSize().x * (Player::self->expRatio), expShape.getSize().y});
                    window->draw(expShape); expShape.setFillColor(sf::Color(30, 30, 30, 255));
                    expShape.setSize({accountBackShape.getSize().x * 0.9f, expShape.getSize().y});
                    
                    helperText.setString(std::to_wstring(Player::self->exp) + L" / " + std::to_wstring(Player::self->expNeeded));
                    helperText.setPosition(gs::width/2 - helperText.getGlobalBounds().width/2, yy - 15*gs::scale);
                    window->draw(helperText); yy += expShape.getGlobalBounds().height + 5*gs::scaley;
                    
                    helperText.setCharacterSize((unsigned int)(70 * gs::scale));
                    helperText.setString(L"Топ результат:  " + std::to_wstring(Player::self->topScore));
                    helperText.setPosition(accountBackShape.getPosition().x + 40*gs::scalex, yy);
                    window->draw(helperText); yy += helperText.getGlobalBounds().height + 10*gs::scaley;
                    
                    helperText.setString(L"Всего очков:  " + std::to_wstring(Player::self->totalScore));
                    helperText.setPosition(accountBackShape.getPosition().x + 40*gs::scalex, yy);
                    window->draw(helperText); yy += helperText.getGlobalBounds().height + 10*gs::scaley;
                    
                    helperText.setString(L"Топ страйк:  " + std::to_wstring(Player::self->topStrike));
                    helperText.setPosition(accountBackShape.getPosition().x + 40*gs::scalex, yy);
                    window->draw(helperText); yy += helperText.getGlobalBounds().height + 10*gs::scaley;
                }
                break;
            case Screen::stage:
                if (spriteLoaded)
                {
                    window->draw(background);
                    window->draw(backShape);
                    window->draw(locationSprite);
                }
                locLeftButton.Draw(window);
                locRightButton.Draw(window);
                window->draw(locationNameText);
                walkButton.Draw(window);
                backButton.Draw(window);
                break;
            case Screen::inventory:
                if (spriteLoaded) window->draw(background);
                //window->draw(backShape);
                //window->draw(lootboxSprite);
                break;
            case Screen::room: room.Draw(window); break;
            default: break;
        }
        
        //if (gs::ignoreEvent) window->draw(backShape);
    }
    void MainMenu::RecieveMessage(MessageHolder& message)
    {
        if (room.active) room.RecieveMessage(message);
        if (message.info == "NekoNinjaComponents :: NekoNinja :: Returning to the menu")
        {
            active = true; auto it = Inventory::map.find("Lootbox");
            if (it != Inventory::map.end() && it->second->count != 0) lootboxesCountText.setString(std::to_string(it->second->count));
            
            dialogueButton.setVisible(true);
            int randomDialogue = rand() % 4;
            switch (randomDialogue)
            {
                case 0: dialogueText.setString(L"Как тебе прогулка, мур?~"); break;
                case 1: dialogueText.setString(L"Чем займёмся сегодня?~~~ \n   Пойдём гулять? :3"); break;
                case 2: dialogueText.setString(L"Хозяин... Я скучала >///<"); break;
                default: break;
            }
            
            screen = Screen::main;
            Resize(gs::width, gs::height);
            message = MessageHolder();
        }
        else if (message.info == "Inventory :: Close") { if (screen == Screen::inventory) screen = Screen::main; }
        else if (message.info == "NekoNinjaComponents :: Room :: Main neko changed")
        {
            ic::DeleteImage(L"Data/Neko/Person/" + nekoTextureName + L".png");
            if (Player::self->neko)
            {
                sf::Texture* texture = ic::LoadTexture(L"Data/Neko/Person/" + Player::self->neko->info->name + L".png");
                if (texture)
                {
                    nekoTextureName = Player::self->neko->info->name;
                    neko.setTexture(*texture, true);
                    neko.setOrigin(texture->getSize().x/2 + Player::self->neko->info->personSprite_offsetX, texture->getSize().y + Player::self->neko->info->personSprite_offsetY);
                    nekoRelScale = (static_cast<float>(gs::relativeHeight)/static_cast<float>(texture->getSize().y)) * Player::self->neko->info->novelScale;
                }
                nekoSpriteLoaded = texture;
            }
            message = MessageHolder();
        }
    }
}
