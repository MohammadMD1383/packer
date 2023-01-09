cmake_minimum_required(VERSION 3.22)
project(packer)

set(CMAKE_CXX_STANDARD 20)

if (DEFINED WINDOWS)
	add_definitions(-DWINDOWS)
	set(CMAKE_EXECUTABLE_SUFFIX .exe)
	set(CMAKE_INSTALL_PREFIX build.win)
	
	include_directories(/fakepath/cpp-tools/boost.win.include)
	link_directories(/fakepath/cpp-tools/boost.win/stage/lib)
endif ()

add_compile_options(-Wall -Wextra)

add_executable(pack pack.cpp)
add_executable(unpack unpack.cpp)

target_link_libraries(pack boost_filesystem boost_program_options)
target_link_libraries(unpack boost_filesystem boost_program_options)

install(
	TARGETS pack unpack
	CONFIGURATIONS Release
	RUNTIME DESTINATION bin
)
