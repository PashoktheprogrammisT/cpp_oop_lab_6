cmake_minimum_required(VERSION 3.10)
project(DungeonEditor)

set(CMAKE_CXX_STANDARD 17)

add_executable(dungeon_editor
    src/NPC.cpp
    src/NPCFactory.cpp
    src/BattleVisitor.cpp
    src/BattleObserver.cpp
    src/Dungeon.cpp
    src/Utils.cpp
    main.cpp
)

target_include_directories(dungeon_editor PRIVATE include)

enable_testing()
find_package(GTest REQUIRED)

add_executable(test01
    test/test01.cpp
    src/NPC.cpp
    src/NPCFactory.cpp
    src/BattleVisitor.cpp
    src/BattleObserver.cpp
    src/Dungeon.cpp
    src/Utils.cpp
)

target_include_directories(test01 PRIVATE include)
target_link_libraries(test01 GTest::gtest GTest::gtest_main)

add_test(NAME NPC_Unit_Tests COMMAND test01)
