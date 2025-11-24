# 1. Required CMake version (3.10 is standard/safe)
cmake_minimum_required(VERSION 3.10)

# 2. Name of your project
project(BirdGame)

# 3. Find the Ncurses library automatically
# This replaces the need to manually verify paths
find_package(Curses REQUIRED)

# 4. Define your executable and its source files
# We list every .c file we want to compile here
add_executable(my_game 
    ./depedencies/configmenaging.c
    ./depedencies/windowmenaging.c
    ./depedencies/bird.c
    ./depedencies/collisions.c
    ./depedencies/stars.c
    ./depedencies/hunters.c
    ./depedencies/ranking.c
    main.c 
)

# 5. Link libraries
# ${CURSES_LIBRARIES} is the variable CMake found in step 3
# 'm' stands for the math library (-lm)
target_link_libraries(my_game 
    ${CURSES_LIBRARIES} 
    m
)
