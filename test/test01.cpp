#include <gtest/gtest.h>
#include <memory>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "NPC.h"
#include "NPCFactory.h"
#include "Dungeon.h"
#include "BattleObserver.h"
#include "BattleVisitor.h"

TEST(NPCTest, BearCreation) {
    auto bear = NPCFactory::createNPC("Bear", "Boris", 10, 20);
    ASSERT_NE(bear, nullptr);
    EXPECT_EQ(bear->getType(), "Bear");
    EXPECT_EQ(bear->getName(), "Boris");
    EXPECT_EQ(bear->getX(), 10);
    EXPECT_EQ(bear->getY(), 20);
}

TEST(NPCTest, BitternCreation) {
    auto bittern = NPCFactory::createNPC("Bittern", "Vasya", 30, 40);
    ASSERT_NE(bittern, nullptr);
    EXPECT_EQ(bittern->getType(), "Bittern");
    EXPECT_EQ(bittern->getName(), "Vasya");
}

TEST(NPCTest, DesmanCreation) {
    auto desman = NPCFactory::createNPC("Desman", "Hohol", 50, 60);
    ASSERT_NE(desman, nullptr);
    EXPECT_EQ(desman->getType(), "Desman");
    EXPECT_EQ(desman->getName(), "Hohol");
}

TEST(NPCTest, InvalidType) {
    auto invalid = NPCFactory::createNPC("Dragon", "Smog", 0, 0);
    EXPECT_EQ(invalid, nullptr);
}

TEST(NPCTest, DistanceCalculation) {
    auto npc1 = NPCFactory::createNPC("Bear", "Bear1", 0, 0);
    auto npc2 = NPCFactory::createNPC("Bittern", "Bittern1", 3, 4);
    EXPECT_DOUBLE_EQ(npc1->distanceTo(*npc2), 5.0);
}

TEST(AttackRulesTest, BearAttacksNonBear) {
    auto bear = NPCFactory::createNPC("Bear", "Bear", 0, 0);
    auto bittern = NPCFactory::createNPC("Bittern", "Bittern", 0, 0);
    auto desman = NPCFactory::createNPC("Desman", "Desman", 0, 0);
    auto anotherBear = NPCFactory::createNPC("Bear", "Bear2", 0, 0);

    EXPECT_TRUE(bear->canAttack(*bittern));
    EXPECT_TRUE(bear->canAttack(*desman));
    EXPECT_FALSE(bear->canAttack(*anotherBear));
}

TEST(AttackRulesTest, BitternAttacksNoOne) {
    auto bittern = NPCFactory::createNPC("Bittern", "Bittern", 0, 0);
    auto bear = NPCFactory::createNPC("Bear", "Bear", 0, 0);
    auto desman = NPCFactory::createNPC("Desman", "Desman", 0, 0);

    EXPECT_FALSE(bittern->canAttack(*bear));
    EXPECT_FALSE(bittern->canAttack(*desman));
    EXPECT_FALSE(bittern->canAttack(*bittern));
}

TEST(AttackRulesTest, DesmanAttacksOnlyBear) {
    auto desman = NPCFactory::createNPC("Desman", "Desman", 0, 0);
    auto bear = NPCFactory::createNPC("Bear", "Bear", 0, 0);
    auto bittern = NPCFactory::createNPC("Bittern", "Bittern", 0, 0);
    auto anotherDesman = NPCFactory::createNPC("Desman", "Desman2", 0, 0);

    EXPECT_TRUE(desman->canAttack(*bear));
    EXPECT_FALSE(desman->canAttack(*bittern));
    EXPECT_FALSE(desman->canAttack(*anotherDesman));
}

TEST(FactoryTest, SaveToString) {
    auto bear = NPCFactory::createNPC("Bear", "Boris", 100, 200);
    std::string data = NPCFactory::saveToString(*bear);
    EXPECT_EQ(data, "Bear,Boris,100,200");
}

TEST(FactoryTest, LoadFromString) {
    std::string data = "Desman,Hohol,55,66";
    auto npc = NPCFactory::loadFromString(data);
    ASSERT_NE(npc, nullptr);
    EXPECT_EQ(npc->getType(), "Desman");
    EXPECT_EQ(npc->getName(), "Hohol");
    EXPECT_EQ(npc->getX(), 55);
    EXPECT_EQ(npc->getY(), 66);
}

TEST(FactoryTest, LoadInvalidString) {
    EXPECT_NO_THROW({
        auto npc1 = NPCFactory::loadFromString("BadData");
        EXPECT_EQ(npc1, nullptr);
    });

    EXPECT_NO_THROW({
        auto npc2 = NPCFactory::loadFromString("Bear,Name,not_int,10");
        EXPECT_EQ(npc2, nullptr);
    });

    EXPECT_NO_THROW({
        auto npc3 = NPCFactory::loadFromString("Bear,Name,10,not_int");
        EXPECT_EQ(npc3, nullptr);
    });

    EXPECT_NO_THROW({
        auto npc4 = NPCFactory::loadFromString("");
        EXPECT_EQ(npc4, nullptr);
    });

    EXPECT_NO_THROW({
        auto npc5 = NPCFactory::loadFromString(",,,");
        EXPECT_EQ(npc5, nullptr);
    });
}

TEST(DungeonTest, AddAndPrint) {
    Dungeon dungeon;
    dungeon.addNPC(NPCFactory::createNPC("Bear", "Bear1", 0, 0));
    dungeon.addNPC(NPCFactory::createNPC("Desman", "Desman1", 10, 10));
    EXPECT_NO_THROW(dungeon.printAll());
}

TEST(DungeonTest, SaveAndLoadFile) {
    const std::string filename = "test_save.txt";
    
    Dungeon dungeon1;
    dungeon1.addNPC(NPCFactory::createNPC("Bear", "Boris", 1, 2));
    dungeon1.addNPC(NPCFactory::createNPC("Bittern", "Vasya", 3, 4));
    dungeon1.addNPC(NPCFactory::createNPC("Desman", "Hohol", 5, 6));
    
    dungeon1.saveToFile(filename);
    
    Dungeon dungeon2;
    dungeon2.loadFromFile(filename);
    
    EXPECT_NO_THROW(dungeon2.printAll());
    
    std::remove(filename.c_str());
}

TEST(DungeonTest, BattleRemovesDeadNPCs) {
    Dungeon dungeon;
    
    dungeon.addNPC(NPCFactory::createNPC("Bear", "Bear1", 0, 0));
    dungeon.addNPC(NPCFactory::createNPC("Desman", "Desman1", 5, 5));
    dungeon.addNPC(NPCFactory::createNPC("Bittern", "Bittern1", 100, 100));
    
    auto observer = std::make_shared<ConsoleObserver>();
    testing::internal::CaptureStdout();
    dungeon.battle(10.0, observer);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NE(output.find("Bear1 killed Desman1"), std::string::npos);
    EXPECT_NE(output.find("Desman1 killed Bear1"), std::string::npos);
}

TEST(ObserverTest, ConsoleObserver) {
    testing::internal::CaptureStdout();
    
    auto observer = std::make_shared<ConsoleObserver>();
    observer->onKill("Bear", "Bittern");
    
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Bear killed Bittern"), std::string::npos);
}

TEST(ObserverTest, FileObserver) {
    const std::string filename = "test_log.txt";
    {
        auto observer = std::make_shared<FileObserver>(filename);
        observer->onKill("Desman", "Bear");
        observer->onKill("Bear", "Desman");
    }
    
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());
    
    std::string line;
    std::vector<std::string> lines;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    file.close();
    
    EXPECT_GE(lines.size(), 2);
    EXPECT_NE(lines[0].find("Desman killed Bear"), std::string::npos);
    
    std::remove(filename.c_str());
}

TEST(ObserverTest, CompositeObserver) {
    testing::internal::CaptureStdout();
    const std::string filename = "test_composite.txt";
    
    auto composite = std::make_shared<CompositeObserver>();
    composite->addObserver(std::make_shared<ConsoleObserver>());
    composite->addObserver(std::make_shared<FileObserver>(filename));
    
    composite->onKill("Bear", "Desman");
    
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Bear killed Desman"), std::string::npos);
    
    std::ifstream file(filename);
    ASSERT_TRUE(file.is_open());
    std::string line;
    std::getline(file, line);
    EXPECT_NE(line.find("Bear killed Desman"), std::string::npos);
    file.close();
    
    std::remove(filename.c_str());
}

TEST(BattleVisitorTest, BattleLogic) {
    auto observer = std::make_shared<ConsoleObserver>();
    std::vector<std::shared_ptr<NPC>> deadNPCs;
    BattleVisitor visitor;
    
    auto bear = NPCFactory::createNPC("Bear", "Bear", 0, 0);
    auto desman = NPCFactory::createNPC("Desman", "Desman", 0, 0);
    auto bittern = NPCFactory::createNPC("Bittern", "Bittern", 0, 0);
    
    EXPECT_NO_THROW(visitor.visit(bear, desman, observer, deadNPCs));
    EXPECT_NO_THROW(visitor.visit(bear, bittern, observer, deadNPCs));
    EXPECT_NO_THROW(visitor.visit(desman, bittern, observer, deadNPCs));
}

TEST(IntegrationTest, FullBattleScenario) {
    Dungeon dungeon;
    
    dungeon.addNPC(NPCFactory::createNPC("Bear", "Bear1", 0, 0));
    dungeon.addNPC(NPCFactory::createNPC("Desman", "Desman1", 5, 5));
    dungeon.addNPC(NPCFactory::createNPC("Bittern", "Bittern1", 10, 10));
    
    auto consoleObserver = std::make_shared<ConsoleObserver>();
    
    testing::internal::CaptureStdout();
    dungeon.battle(10.0, consoleObserver);
    std::string output = testing::internal::GetCapturedStdout();
    
    EXPECT_NO_THROW();
}

TEST(IntegrationTest, CoordinatesValidation) {
    auto npc1 = NPCFactory::createNPC("Bear", "Bear", -10, 600);
    EXPECT_EQ(npc1, nullptr);
    
    auto npc2 = NPCFactory::createNPC("Bear", "Bear", 250, 250);
    ASSERT_NE(npc2, nullptr);
    EXPECT_EQ(npc2->getX(), 250);
    EXPECT_EQ(npc2->getY(), 250);
    
    auto npc3 = NPCFactory::createNPC("Bear", "Bear", 0, 0);
    EXPECT_NE(npc3, nullptr);
    
    auto npc4 = NPCFactory::createNPC("Bear", "Bear", 500, 500);
    EXPECT_NE(npc4, nullptr);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
