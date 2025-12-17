#pragma once
#include <vector>
#include <memory>
#include "NPC.h"
#include "BattleObserver.h"

class Dungeon {
    std::vector<std::shared_ptr<NPC>> npcs;
public:
    void addNPC(std::shared_ptr<NPC> npc);
    void printAll() const;
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);
    void battle(double range, std::shared_ptr<BattleObserver> observer);
};
