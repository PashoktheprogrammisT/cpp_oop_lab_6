#pragma once
#include <string>
#include <memory>
#include <vector>

class BattleObserver {
public:
    virtual ~BattleObserver() = default;
    virtual void onKill(const std::string& killer, const std::string& victim) = 0;
};

class FileObserver : public BattleObserver {
    std::string filename;
public:
    FileObserver(const std::string& filename);
    void onKill(const std::string& killer, const std::string& victim) override;
};

class ConsoleObserver : public BattleObserver {
public:
    void onKill(const std::string& killer, const std::string& victim) override;
};

class CompositeObserver : public BattleObserver {
    std::vector<std::shared_ptr<BattleObserver>> observers;
public:
    void addObserver(std::shared_ptr<BattleObserver> observer) {
        observers.push_back(observer);
    }
    
    void onKill(const std::string& killer, const std::string& victim) override {
        for (auto& observer : observers) {
            observer->onKill(killer, victim);
        }
    }
};
