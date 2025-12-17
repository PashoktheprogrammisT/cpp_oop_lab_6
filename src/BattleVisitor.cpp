#include "BattleVisitor.h"
#include "NPC.h"
#include "BattleObserver.h"
#include <iostream>

void BattleVisitor::visit(std::shared_ptr<NPC> npc, std::shared_ptr<NPC> other, std::shared_ptr<BattleObserver> observer) {
    if (npc->canAttack(*other)) {
        observer->onKill(npc->getName(), other->getName());
    } else if (other->canAttack(*npc)) {
        observer->onKill(other->getName(), npc->getName());
    }
}
