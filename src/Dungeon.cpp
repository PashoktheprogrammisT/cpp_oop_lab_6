#include "Dungeon.h"
#include "NPCFactory.h"
#include "BattleVisitor.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <memory>

void Dungeon::addNPC(std::shared_ptr<NPC> npc) {
    npcs.push_back(npc);
}

void Dungeon::printAll() const {
    for (const auto& npc : npcs) {
        std::cout << npc->getType() << " " << npc->getName() 
                  << " at (" << npc->getX() << ", " << npc->getY() << ")" << std::endl;
    }
}

void Dungeon::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    for (const auto& npc : npcs) {
        file << NPCFactory::saveToString(*npc) << std::endl;
    }
    file.close();
}

void Dungeon::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        auto npc = NPCFactory::loadFromString(line);
        if (npc) npcs.push_back(npc);
    }
    file.close();
}

void Dungeon::battle(double range, std::shared_ptr<BattleObserver> observer) {
    BattleVisitor visitor;
    std::vector<std::shared_ptr<NPC>> deadNPCs;
    
    for (size_t i = 0; i < npcs.size(); ++i) {
        for (size_t j = i + 1; j < npcs.size(); ++j) {
            if (npcs[i] && npcs[j] && npcs[i]->distanceTo(*npcs[j]) <= range) {
                std::vector<std::shared_ptr<NPC>> tempDeadNPCs;
                visitor.visit(npcs[i], npcs[j], observer, tempDeadNPCs);
                
                for (auto& dead : tempDeadNPCs) {
                    if (std::find(deadNPCs.begin(), deadNPCs.end(), dead) == deadNPCs.end()) {
                        deadNPCs.push_back(dead);
                    }
                }
            }
        }
    }
    
    for (auto& dead : deadNPCs) {
        auto it = std::find(npcs.begin(), npcs.end(), dead);
        if (it != npcs.end()) {
            npcs.erase(it);
        }
    }
}
