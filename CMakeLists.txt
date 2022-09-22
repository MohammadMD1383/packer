cmake_minimum_required(VERSION 3.22)
project(packer)

set(CMAKE_CXX_STANDARD 20)

set(Boost_USE_STATIC_LIBS OFF)

find_package(Boost 1.80.0 COMPONENTS program_options filesystem REQUIRED)

link_libraries(Boost::program_options Boost::filesystem)

add_compile_options(-Wall -Wextra)

add_executable(pack pack.cpp)
add_executable(unpack unpack.cpp)

install(
	TARGETS pack unpack
	CONFIGURATIONS Release
	RUNTIME DESTINATION $HOME/bin
)
