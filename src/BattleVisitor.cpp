#include "BattleVisitor.h"
#include "NPC.h"
#include "BattleObserver.h"
#include <iostream>

void BattleVisitor::visit(std::shared_ptr<NPC> npc, std::shared_ptr<NPC> other, 
                          std::shared_ptr<BattleObserver> observer,
                          std::vector<std::shared_ptr<NPC>>& deadNPCs) {
    bool npcCanAttack = npc->canAttack(*other);
    bool otherCanAttack = other->canAttack(*npc);
    
    if (npcCanAttack && otherCanAttack) {
        observer->onKill(npc->getName(), other->getName());
        observer->onKill(other->getName(), npc->getName());
        deadNPCs.push_back(npc);
        deadNPCs.push_back(other);
    } else if (npcCanAttack) {
        observer->onKill(npc->getName(), other->getName());
        deadNPCs.push_back(other);
    } else if (otherCanAttack) {
        observer->onKill(other->getName(), npc->getName());
        deadNPCs.push_back(npc);
    }
}
