cmake_minimum_required(VERSION 3.20)

Message("")
Message(STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR})
Message(STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR})
Message(STATUS "CMAKE_COMMAND: " ${CMAKE_COMMAND})
Message(STATUS "CMAKE_VERSION: " ${CMAKE_VERSION})
Message(STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER})
Message(STATUS "CMAKE_PROJECT_NAME: " ${CMAKE_PROJECT_NAME})
Message(STATUS "CMAKE_FINDCMAKE: " ${CMAKE_SOURCE_DIR}/CMAKE)

project(leveleditor)
# ------- Set Vars ------- #
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMAKE")
set(CMAKE_XCODE_SCHEME_WORKING_DIRECTORY ./)
#[[set(SOURCE_FILES src/main.cpp src/global.hpp src/gamebase.hpp src/gamebase.cpp src/recthelper.hpp src/CubeGame.cpp
        src/CubeGame.hpp src/States/LevelSelector.cpp src/States/LevelSelector.hpp src/States/Level.cpp
        src/States/Level.hpp src/GameObjects/GameObject.cpp src/GameObjects/GameObject.hpp
        src/GameObjects/Player.cpp src/GameObjects/Player.hpp src/GameObjects/CubeMap.cpp
        src/GameObjects/CubeMap.hpp src/GameObjects/WorldMap.cpp src/GameObjects/WorldMap.hpp
        src/GameObjects/CubeField.cpp src/GameObjects/CubeField.hpp src/data/WorldField.hpp
        src/data/LevelData.hpp src/data/MoveDirections.hpp src/data/DiceData.hpp src/data/DiceData.cpp
        src/data/EmptyData.hpp src/data/SpriteStorage.hpp src/GameObjects/Text.cpp src/GameObjects/Text.hpp
        src/data/Colors.hpp src/States/ComplexGameState.hpp src/States/ComplexGameState.cpp
        src/GameObjects/CubeMap_CubeMapSide.cpp src/GameObjects/CubeMap_playerMovement.cpp src/data/WorldField.cpp
        src/data/paths.hpp src/stringhelper.hpp src/data/LevelLoader.cpp src/data/LevelLoader.hpp src/filehelper.hpp
        src/AudioHandler.hpp src/AudioHandler.cpp src/matrix.hpp src/GameObjects/CubeMapMiniMap.cpp
        src/GameObjects/CubeMapMiniMap.hpp src/data/bezier.hpp src/mathhelper.hpp src/data/spriteDefs.hpp)]]

# find all source files
file(GLOB_RECURSE SOURCE_FILES src/*.*)

source_group(TREE "${CMAKE_CURRENT_LIST_DIR}" FILES ${SOURCE_FILES})
Message("")
Message(STATUS "SOURCE entry point : " ${SOURCE_FILES})
Message("")

# ------- End Set Vars --- #

# ------- Finds ---------- #
IF (WIN32 OR MINGW)
    set(SDL2_INCLUDE_DIR ./SDL2-2.24.2/include)
    set(SDL2_LIB_DIR ./SDL2-2.24.2/lib)

    set(SDL2_TTF_INCLUDE_DIR ./SDL2_ttf-2.20.1/include)
    set(SDL2_TTF_LIB_DIR ./SDL2_ttf-2.20.1/lib)

    set(SDL2_IMAGE_INCLUDE_DIR ./SDL2_image-2.6.2/include)
    set(SDL2_IMAGE_LIB_DIR ./SDL2_image-2.6.2/lib)

    set(SDL2_MIXER_INCLUDE_DIR ./SDL2_mixer-2.6.2/include)
    set(SDL2_MIXER_LIB_DIR ./SDL2_mixer-2.6.2/lib)

    include_directories(${SDL2_INCLUDE_DIR})
    link_directories(${SDL2_LIB_DIR})

    include_directories(${SDL2_TTF_INCLUDE_DIR})
    link_directories(${SDL2_TTF_LIB_DIR})

    include_directories(${SDL2_IMAGE_INCLUDE_DIR})
    link_directories(${SDL2_IMAGE_LIB_DIR})

    include_directories(${SDL2_MIXER_INCLUDE_DIR})
    link_directories(${SDL2_MIXER_LIB_DIR})

    ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})

    target_link_libraries(${PROJECT_NAME} SDL2 SDL2main SDL2_ttf SDL2_image SDL2_mixer)

    file(COPY res DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    file(COPY level DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

else ()
    ADD_EXECUTABLE(${PROJECT_NAME} ${SOURCE_FILES})

    FIND_PACKAGE(SDL2 REQUIRED)
    Message("")
    Message(STATUS "FINDING SDL2")
    IF (${SDL2_FOUND})
        Message(STATUS "SDL2_FOUND: " ${SDL2_FOUND})
        Message(STATUS "SDL2_INCLUDE_DIR:" ${SDL2_INCLUDE_DIR})
        Message(STATUS "SDL2_LIBRARY: " ${SDL2_LIBRARY})
    ELSE ()
        Message(STATUS "SDL2_FOUND: " ${SDL2_FOUND})
        Message(FATAL_ERROR "SDL2 NOT FOUND")
    ENDIF ()

    FIND_PACKAGE(SDL2_IMAGE)
    Message("")
    Message(STATUS "FINDING SDL2_IMAGE")
    Message(STATUS "SDL2_IMAGE_FOUND: " ${SDL2_IMAGE_FOUND})
    IF (${SDL2_IMAGE_FOUND})
        Message(STATUS "SDL2_IMAGE_INCLUDE_DIR: " ${SDL2_IMAGE_INCLUDE_DIR})
        Message(STATUS "SDL2_IMAGE_LIBRARY: " ${SDL2_IMAGE_LIBRARY})
    ELSE ()
        Message(FATAL_ERROR "SDL2_IMAGE NOT FOUND")
    ENDIF ()

    FIND_PACKAGE(SDL2TTF)
    Message("")
    Message(STATUS "FINDING SDL2TTF")
    Message(STATUS "SDL2TTF_FOUND: " ${SDL2TTF_FOUND})
    IF (${SDL2TTF_FOUND})
        Message(STATUS "SDL2TTF_INCLUDE_DIR: " ${SDL2TTF_INCLUDE_DIR})
        Message(STATUS "SDL2TTF_LIBRARY: " ${SDL2TTF_LIBRARY})
    ELSE ()
        Message(FATAL_ERROR "SDL2TTF NOT FOUND")
    ENDIF ()
    Message("")

    FIND_PACKAGE(SDL2Mixer)

    Message(STATUS "FINDING SDL2Mixer")
    Message(STATUS "SDL2Mixer_FOUND: " ${SDL2Mixer_FOUND})

    Message("")

    file(COPY res DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # ------- End Finds ------ #

    # ------- Inc & Link ---- #

    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2Mixer_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2Mixer_LIBRARY})

    # ------- End ----------- #
ENDIF ()