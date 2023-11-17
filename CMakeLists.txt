cmake_minimum_required (VERSION 3.20)

project (LearnOpenGL)

set (CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
find_package(glew REQUIRED)

add_subdirectory(lib/glm)
set(GLM_INCLUDE_DIR "lib/glm")

add_subdirectory(lib/glfw)
set(GLFW_INCLUDE_DIR "lib/glfw/include")

set(IMGUI_INCLUDE_DIR "lib/imgui")
file(GLOB IMGUI_SOURCES
       ${IMGUI_INCLUDE_DIR}/*.cpp
       ${IMGUI_INCLUDE_DIR}/backends/imgui_impl_glfw.cpp
	   ${IMGUI_INCLUDE_DIR}/backends/imgui_impl_opengl3.cpp)

set(GLAD_INCLUDE_DIR "lib/glad")
file(GLOB GLAD_SOURCES
       ${GLAD_INCLUDE_DIR}/glad.c)


file(GLOB MAIN_HEADER_FILES
	src/*.h
	src/*.hpp
)

file(GLOB MAIN_SOURCE_FILES
	src/*.c
	src/*.cpp
)

include_directories(
	${GLEW_INCLUDE_PATH}
	${GLM_INCLUDE_DIR}
	${GLFW_INCLUDE_DIR}
	${IMGUI_INCLUDE_DIR}
	${GLAD_INCLUDE_DIR}
	${MAIN_HEADER_FILES}
)
add_executable(LearnOpenGL
	${MAIN_SOURCE_FILES}
	${IMGUI_SOURCES}
	${GLAD_SOURCES}
)
target_link_libraries(LearnOpenGL
	${OPENGL_LIBRARY} 
	glfw
	${GLEW_LIBRARY}
)

file(GLOB SHADER_SOURCES
	shaders/*.vert
	shaders/*.frag
	shaders/*.glsl
)

file(COPY ${SHADER_SOURCES} DESTINATION ${CMAKE_BINARY_DIR})

#FOREACH(MY_RESOURCE_FILE ${SHADER_SOURCES})
#    add_custom_command(
#        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${MY_RESOURCE_FILE}
#        COMMAND ${CMAKE_COMMAND} -E copy
#            ${CMAKE_CURRENT_SOURCE_DIR}/${MY_RESOURCE_FILE}
#            ${CMAKE_CURRENT_BINARY_DIR}/${MY_RESOURCE_FILE}
#        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${MY_RESOURCE_FILE}
#  )
#ENDFOREACH()
