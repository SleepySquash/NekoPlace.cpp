//
//  ItemDatabaseCollection.cpp
//  NekoPlace
//
//  Created by Никита Исаенко on 11/05/2019.
//  Copyright © 2019 Melanholy Hill. All rights reserved.
//

#include "ItemDatabaseCollection.hpp"

namespace NekoNinja
{
    void ItemDatabaseCollection_LoadItems()
    {
        Inventory::map.emplace("Cupcake", new idc::Cupcake());
        Inventory::map.emplace("Cake", new idc::Cake());
        Inventory::map.emplace("Candy", new idc::Candy());
        Inventory::map.emplace("Star", new idc::Star());
        Inventory::map.emplace("Icecream", new idc::Icecream());
        Inventory::map.emplace("Donut", new idc::Donut());
        Inventory::map.emplace("FractionOfPie", new idc::FractionOfPie());
        Inventory::map.emplace("Lootbox", new idc::Lootbox());
    }
    
    
    
    
    
    
    std::list<ItemBase*> Inventory::list;
    std::unordered_map<std::string, ItemBase*> Inventory::map;
    void Inventory::Push(const std::string& item, unsigned int count)
    {
        auto it = map.find(item); if (it == map.end()) return;
        if (it->second->count) it->second->count += count;
        else { list.push_back(it->second); it->second->count += count; }
    }
    void Inventory::Add(const std::string& item, unsigned int count)
    {
        auto it = map.find(item); if (it == map.end()) return;
        if (it->second->count) it->second->count += count;
        else { list.push_back(it->second); it->second->count += count; Sort(); }
    }
    void Inventory::Remove(const std::string& item, unsigned int count)
    {
        auto it = map.find(item); if (it == map.end()) return;
        if (it->second->count > count) it->second->count -= count;
        else { list.remove_if([&item](const ItemBase* base){ return (base->name == item); }); it->second->count = 0; }
    }
    void Inventory::Sort() { list.sort([](const ItemBase* a, const ItemBase* b) { return a->rarity > b->rarity; }); }
}
