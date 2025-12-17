#pragma once
#include <string>
#include <memory>
#include <vector>

class BattleVisitor;
class BattleObserver;

class NPC {
protected:
    std::string name;
    int x, y;
public:
    NPC(const std::string& name, int x, int y);
    virtual ~NPC() = default;

    virtual std::string getType() const = 0;
    std::string getName() const;
    int getX() const;
    int getY() const;
    double distanceTo(const NPC& other) const;

    virtual bool canAttack(const NPC& other) const = 0;
    virtual void accept(BattleVisitor& visitor, std::shared_ptr<NPC> attacker, 
                        std::shared_ptr<BattleObserver> observer) = 0;
};
