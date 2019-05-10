cmake_minimum_required (VERSION 3.12)

# Project stuff
project (bulk_src
	VERSION 0.0.${TRAVIS_BUILD_NUMBER}
	DESCRIPTION "Homework 7 Bulks of commands implementation program"
	HOMEPAGE_URL "https://github.com/edwdmkrv/bulk/tree/master/src"
	LANGUAGES CXX
)

# Program stuff
add_executable (bulk bulk.cpp)

set_target_properties (bulk PROPERTIES
	CXX_STANDARD 17
	CXX_EXTENSIONS OFF
	CXX_STANDARD_REQUIRED ON
	COMPILE_OPTIONS "-pedantic;-Wall;-Wextra"
	LINK_LIBRARIES "lib;${CMAKE_THREAD_LIBS_INIT}"
)

install (TARGETS bulk RUNTIME DESTINATION Debug/bin CONFIGURATIONS Debug)
install (TARGETS bulk RUNTIME DESTINATION bin CONFIGURATIONS Release)
