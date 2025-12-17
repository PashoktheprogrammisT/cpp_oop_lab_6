#include "BattleObserver.h"
#include <iostream>
#include <fstream>

FileObserver::FileObserver(const std::string& filename) : filename(filename) {
    std::ofstream file(filename, std::ios::trunc);
    file.close();
}

void FileObserver::onKill(const std::string& killer, const std::string& victim) {
    std::ofstream file(filename, std::ios::app);
    file << killer << " killed " << victim << std::endl;
    file.close();
}

void ConsoleObserver::onKill(const std::string& killer, const std::string& victim) {
    std::cout << "[Battle] " << killer << " killed " << victim << std::endl;
}
