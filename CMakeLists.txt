cmake_minimum_required(VERSION 3.7)
project(fantasia-final)


set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(SOURCE_FILES main.cpp player.cpp audiomanager.cpp gamemanager.cpp monster.cpp uimanager.cpp scoremanager.cpp)
add_executable(fantasia-final ${SOURCE_FILES})
SET(ALLEGRO_ROOT allegro/)

INCLUDE_DIRECTORIES( ${ALLEGRO_ROOT}/include)
LINK_DIRECTORIES( /${ALLEGRO_ROOT}/lib )


TARGET_INCLUDE_DIRECTORIES(fantasia-final PUBLIC ${ALLEGRO_ROOT})
TARGET_LINK_LIBRARIES(fantasia-final allegro
        allegro_color
        allegro_main
        allegro_primitives
        allegro_ttf
        allegro_acodec
        allegro_audio
        allegro_font
        allegro_dialog
        allegro_image
        )