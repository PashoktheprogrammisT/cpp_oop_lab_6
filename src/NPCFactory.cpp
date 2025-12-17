#include "NPCFactory.h"
#include "NPC.h"
#include "BattleVisitor.h"
#include "BattleObserver.h"
#include "Utils.h"
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>

class Bear : public NPC {
public:
    Bear(const std::string& name, int x, int y) : NPC(name, x, y) {}
    std::string getType() const override { return "Bear"; }
    bool canAttack(const NPC& other) const override {
        return other.getType() != "Bear";
    }
    void accept(BattleVisitor& visitor, std::shared_ptr<NPC> attacker, std::shared_ptr<BattleObserver> observer) override {
        std::shared_ptr<NPC> self = std::make_shared<Bear>(*this);
        visitor.visit(self, attacker, observer);
    }
};

class Bittern : public NPC {
public:
    Bittern(const std::string& name, int x, int y) : NPC(name, x, y) {}
    std::string getType() const override { return "Bittern"; }
    bool canAttack(const NPC& other) const override {
        return false;
    }
    void accept(BattleVisitor& visitor, std::shared_ptr<NPC> attacker, std::shared_ptr<BattleObserver> observer) override {
        std::shared_ptr<NPC> self = std::make_shared<Bittern>(*this);
        visitor.visit(self, attacker, observer);
    }
};

class Desman : public NPC {
public:
    Desman(const std::string& name, int x, int y) : NPC(name, x, y) {}
    std::string getType() const override { return "Desman"; }
    bool canAttack(const NPC& other) const override {
        return other.getType() == "Bear";
    }
    void accept(BattleVisitor& visitor, std::shared_ptr<NPC> attacker, std::shared_ptr<BattleObserver> observer) override {
        std::shared_ptr<NPC> self = std::make_shared<Desman>(*this);
        visitor.visit(self, attacker, observer);
    }
};

std::shared_ptr<NPC> NPCFactory::createNPC(const std::string& type, const std::string& name, int x, int y) {
    if (x < 0 || x > 500 || y < 0 || y > 500) {
        std::cerr << "Error: Coordinates must be between 0 and 500. Got (" << x << ", " << y << ")" << std::endl;
        return nullptr;
    }
    
    if (type == "Bear") {
        return std::make_shared<Bear>(name, x, y);
    } else if (type == "Bittern") {
        return std::make_shared<Bittern>(name, x, y);
    } else if (type == "Desman") {
        return std::make_shared<Desman>(name, x, y);
    }
    
    std::cerr << "Unknown NPC type: " << type << std::endl;
    return nullptr;
}

std::shared_ptr<NPC> NPCFactory::loadFromString(const std::string& data) {
    if (data.empty()) {
        return nullptr;
    }
    
    auto parts = split(data, ',');
    if (parts.size() != 4) {
        return nullptr;
    }
    
    std::string type = parts[0];
    std::string name = parts[1];
    
    try {
        int x = std::stoi(parts[2]);
        int y = std::stoi(parts[3]);
        
        if (x < 0 || x > 500 || y < 0 || y > 500) {
            return nullptr;
        }
        
        return createNPC(type, name, x, y);
    } 
    catch (const std::invalid_argument& e) {
        return nullptr;
    } 
    catch (const std::out_of_range& e) {
        return nullptr;
    }
    catch (...) {
        return nullptr;
    }
}

std::string NPCFactory::saveToString(const NPC& npc) {
    return npc.getType() + "," + npc.getName() + "," + 
           std::to_string(npc.getX()) + "," + std::to_string(npc.getY());
}
