project(TicTacToe)

include_directories(include) # Local include directory

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.hpp")

option(TTT_CONSOLE_UTF8_SUPPORT "Enable UTF-8 Support in the console for TicTacToe" ON)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

if(TTT_CONSOLE_UTF8_SUPPORT)
    add_definitions(-DTTT_UTF8_SUPPORT)
endif()