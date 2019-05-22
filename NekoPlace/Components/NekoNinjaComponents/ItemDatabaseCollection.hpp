//
//  ItemDatabaseCollection.hpp
//  NekoPlace
//
//  Created by Никита Исаенко on 11/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#ifndef ItemDatabaseCollection_hpp
#define ItemDatabaseCollection_hpp

#include <iostream>
#include <list>
#include <unordered_map>

#include <SFML/Main.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include "../../Essentials/ResourcePath.hpp"
#include "../../Engine/Settings.hpp"

#include "ItemBase.hpp"

using std::cin;
using std::cout;
using std::endl;
using ns::base::utf8;
using ns::base::utf16;

namespace NekoNinja
{
    namespace idc
    {
        struct Cupcake : ItemBase { Cupcake() : ItemBase("Cupcake", L"Вкусный пирожочек с вишенкой.") { display = L"Пирожок"; } };
        struct Cake : ItemBase { Cake() : ItemBase("Cake", L"Шоколадный тортик с вишенкой наверху.", 10) { display = L"Тортик"; } };
        struct Candy : ItemBase { Candy() : ItemBase("Candy", L"М-м, тот самый ностальгический вкус.") { display = L"Конфетка"; } };
        struct Star : ItemBase { Star() : ItemBase("Star", L"Звёздочка с неба.", 90) { display = L"Звезда"; } };
        struct Icecream : ItemBase { Icecream() : ItemBase("Icecream", L"Приятно тает во рту.") { display = L"Мороженка"; } };
        struct Donut : ItemBase { Donut() : ItemBase("Donut", L"Шоколадный пончик в клубничной глазури.") { display = L"Пончик"; } };
        struct FractionOfPie : ItemBase { FractionOfPie() : ItemBase("FractionOfPie", L"Кусочек клубничного панкейка.") { display = L"Кусок тортика"; } };
        struct Lootbox : ItemBase { Lootbox() : ItemBase("Lootbox", L"Интересно, что внутри?", 100, false, true) { display = L"Лутбокс"; } };
    }
    
    void ItemDatabaseCollection_LoadItems();
    struct Inventory
    {
        static std::list<ItemBase*> list;
        static std::unordered_map<std::string, ItemBase*> map;
        static void Push(const std::string& item, unsigned int count = 1);
        static void Sort();
        static void Add(const std::string& item, unsigned int count = 1);
        static void Remove(const std::string& item, unsigned int count = 1);
    };
}

#endif /* ItemDatabaseCollection_hpp */
