cmake_minimum_required(VERSION 3.10)

project(BirdGame)

find_package(Curses REQUIRED)

add_executable(game 
    ./depedencies/configmanaging.c
    ./depedencies/windowmanaging.c
    ./depedencies/bird.c
    ./depedencies/stars.c
    ./depedencies/hunters.c
    ./depedencies/ranking.c
    ./depedencies/albatross.c
    ./depedencies/fog.c
    main.c 
)

target_link_libraries(game 
    ${CURSES_LIBRARIES} 
    m
)
