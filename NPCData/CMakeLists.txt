project(NPCData)

include_directories(
	include # Local include directory
	../raylib/include
	)

add_executable(NPCData
				src/main.cpp
				src/Record.cpp
				src/DataFile.cpp
				include/DataFile.hpp
			  )
target_link_libraries(NPCData raylib)

add_custom_command(
	TARGET NPCData
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${CMAKE_CURRENT_SOURCE_DIR}/npc_data.dat
		$<TARGET_FILE_DIR:NPCData>
)