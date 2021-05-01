project(HighscoreDatabase)

include_directories(include) # Local include directory

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.hpp")

file(GLOB_RECURSE SOURCE_FILES_COMMANDS "src/Commands/*.cpp")
file(GLOB_RECURSE HEADER_FILES_COMMANDS "include/Commands/*.hpp")

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src" PREFIX "Source" FILES ${SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include" PREFIX "Headers" FILES ${HEADER_FILES})

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src/Commands" PREFIX "Source/Commands/" FILES ${SOURCE_FILES_COMMANDS})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/Commands" PREFIX "Headers/Commands/" FILES ${HEADER_FILES_COMMANDS})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})