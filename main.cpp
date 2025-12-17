#include "Dungeon.h"
#include "NPCFactory.h"
#include "BattleObserver.h"
#include <iostream>
#include <memory>
#include <fstream>

int main() {
    Dungeon dungeon;

    std::cout << "=== Dungeon Editor - Variant 19 ===\n";
    std::cout << "Rules: Bear eats all except bears\n";
    std::cout << "       Bittern doesn't attack anyone\n";
    std::cout << "       Desman kills bears (because it's our endemic)\n\n";

    dungeon.addNPC(NPCFactory::createNPC("Bear", "Bear1", 10, 10));
    dungeon.addNPC(NPCFactory::createNPC("Bittern", "Bittern1", 50, 50));
    dungeon.addNPC(NPCFactory::createNPC("Desman", "Desman1", 12, 12));
    dungeon.addNPC(NPCFactory::createNPC("Bear", "Bear2", 100, 100));

    std::cout << "All NPCs in dungeon:" << std::endl;
    dungeon.printAll();

    dungeon.saveToFile("dungeon.txt");
    std::cout << "\nSaved to 'dungeon.txt'\n";

    Dungeon dungeon2;
    dungeon2.loadFromFile("dungeon.txt");
    std::cout << "Loaded from file:" << std::endl;
    dungeon2.printAll();

    auto compositeObserver = std::make_shared<CompositeObserver>();
    compositeObserver->addObserver(std::make_shared<ConsoleObserver>());
    compositeObserver->addObserver(std::make_shared<FileObserver>("log.txt"));

    std::cout << "\n=== Battle Mode (range: 30 meters) ===" << std::endl;
    dungeon.battle(30.0, compositeObserver);
    
    std::cout << "\nSurvivors after battle:" << std::endl;
    dungeon.printAll();

    std::cout << "\n=== Battle Log ===" << std::endl;
    std::ifstream logFile("log.txt");
    if (logFile.is_open()) {
        std::string line;
        while (std::getline(logFile, line)) {
            std::cout << line << std::endl;
        }
        logFile.close();
    }

    return 0;
}
