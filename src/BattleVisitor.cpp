#include "BattleVisitor.h"
#include "NPC.h"
#include "BattleObserver.h"
#include <iostream>

void BattleVisitor::visit(std::shared_ptr<NPC> npc, std::shared_ptr<NPC> other, std::shared_ptr<BattleObserver> observer) {
    bool npcCanAttack = npc->canAttack(*other);
    bool otherCanAttack = other->canAttack(*npc);
    
    if (npcCanAttack && otherCanAttack) {
        observer->onKill(npc->getName(), other->getName());
        observer->onKill(other->getName(), npc->getName());
    } else if (npcCanAttack) {
        observer->onKill(npc->getName(), other->getName());
    } else if (otherCanAttack) {
        observer->onKill(other->getName(), npc->getName());
    }
}
