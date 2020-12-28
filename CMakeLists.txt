cmake_minimum_required(VERSION 3.3)

project(gurobi_template)

option(CXX "enable C++ compilation" ON)
if(CXX)
    enable_language(CXX)
endif()


set(CMAKE_CXX_STANDARD 11)

set(GRAPH_DIR fssimplewindow/macosx/)
set(PNG_DIR yspng/)
set(SOUND_DIR yssimplesound/)
set(SOUND_SUBDIR yssimplesound/macosx/)
set(FONT_DIR ysglfontdata/)
find_package(OpenGL REQUIRED)
set(PNG_DIR yspng/)
set(FONT_DIR ysglfontdata/)
set(SOUND_DIR yssimplesound/)
set(SRC_DIR src/)

include_directories(include)

if(MSVC)
    message("This is Windows.")
elseif(APPLE)
    message("This is MAC OS.")
    set(GRAPH_DIR fssimplewindow/macosx/)
    set(SOUND_SUBDIR yssimplesound/macosx/)
    find_library(COCOA_LIBRARY Cocoa REQUIRED)
    include_directories(include ${GRAPH_DIR} ${SOUND_DIR} ${SOUND_SUBDIR} ${PNG_DIR} ${OPENGL_INCLUDE_DIR})
elseif(UNIX)
    message("This is Linux.")
    set(GRAPH_DIR fssimplewindow/linux/)
    set(SOUND_SUBDIR yssimplesound/linux_alsa/)
    find_package(ALSA REQUIRED)
    find_package(X11 REQUIRED)
    include_directories(include ${GRAPH_DIR} ${SOUND_DIR} ${SOUND_SUBDIR} ${PNG_DIR} ${OPENGL_INCLUDE_DIR} ${ALSA_INCLUDE_DIRS} ${X11_INCLUDE_DIR})
else()
endif()

if(MSVC)
elseif(APPLE)
    add_library(yssimplesound ${SOUND_DIR}yssimplesound.cpp ${SOUND_SUBDIR}yssimplesound_macosx_cpp.cpp ${SOUND_SUBDIR}yssimplesound_macosx_objc.m)
    add_library(fssimplewindowcpp ${GRAPH_DIR}fssimplewindowcpp.cpp ${GRAPH_DIR}fssimplewindowobjc.m)
    target_link_libraries(fssimplewindowcpp ${OPENGL_LIBRARIES} )
elseif(UNIX)
    add_library(yssimplesound ${SOUND_DIR}yssimplesound.cpp ${SOUND_SUBDIR}yssimplesound_linux_alsa.cpp)
    target_link_libraries(yssimplesound ${ALSA_LIBRARIES} )
    add_library(fssimplewindowcpp ${GRAPH_DIR}fssimplewindow.cpp)
    target_link_libraries(fssimplewindowcpp ${OPENGL_LIBRARIES} ${X11_LIBRARIES})
else()
endif()

add_library(yspng ${PNG_DIR}yspng.cpp)

add_library(MyDraw ${SRC_DIR}MyDraw.cpp)
target_link_libraries(MyDraw fssimplewindowcpp)

add_library(PngManager ${SRC_DIR}PngManager.cpp)
target_link_libraries(PngManager MyDraw yspng)

add_library(Rect ${SRC_DIR}Rect.cpp)
target_link_libraries(Rect MyDraw)

add_library(Circle ${SRC_DIR}Circle.cpp)
target_link_libraries(Circle MyDraw)

add_library(GameObject ${SRC_DIR}GameObject.cpp)
target_link_libraries(GameObject Circle Rect MyDraw PngManager)

add_library(GameBar ${SRC_DIR}GameBar.cpp)
target_link_libraries(GameBar GameObject)

add_library(Menu ${SRC_DIR}Menu.cpp)
target_link_libraries(Menu GameObject)

add_library(ActiveObject ${SRC_DIR}ActiveObject.cpp)
target_link_libraries(ActiveObject GameObject)

add_library(Bullet ${SRC_DIR}Bullet.cpp)
target_link_libraries(Bullet GameObject)

add_library(Player ${SRC_DIR}Player.cpp)
target_link_libraries(Player GameObject ActiveObject Bullet)

add_library(Computer ${SRC_DIR}Computer.cpp)
target_link_libraries(Computer GameObject ActiveObject Bullet)

add_library(Tool ${SRC_DIR}Tool.cpp)
target_link_libraries(Tool Player)

add_library(Room ${SRC_DIR}Room.cpp)
target_link_libraries(Room Game)

add_library(Game ${SRC_DIR}Game.cpp)
target_link_libraries(Game Room Menu GameBar Player Computer Tool GameObject yssimplesound)

add_executable(game_main ${SRC_DIR}game_main.cpp)
if(MSVC)
elseif(APPLE)
    target_link_libraries(game_main Menu GameBar Room Game  ${COCOA_LIBRARY})
elseif(UNIX)
    target_link_libraries(game_main Menu GameBar Room Game )
else()
endif()

