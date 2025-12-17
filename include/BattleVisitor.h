#pragma once
#include <memory>
#include "NPC.h"
#include "BattleObserver.h"

class BattleVisitor {
public:
    void visit(std::shared_ptr<NPC> npc, std::shared_ptr<NPC> other, std::shared_ptr<BattleObserver> observer);
};
