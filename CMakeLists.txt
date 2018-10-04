cmake_minimum_required(VERSION 3.8)

include_directories("include")

add_executable(quiz "main.cpp")

set_target_properties(quiz
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
set_property(TARGET quiz PROPERTY CXX_STANDARD_REQUIRED 17)

target_link_libraries(quiz "stdc++fs")

add_custom_command(TARGET quiz POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/res $<TARGET_FILE_DIR:quiz>/quizzes
)