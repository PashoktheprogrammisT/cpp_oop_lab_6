#pragma once
#include <string>
#include <memory>

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
