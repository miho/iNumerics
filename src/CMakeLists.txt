include_directories("../odeint")
include_directories("../include")
include_directories("../include")

set(SRC
	Trajectory.cpp
	Problem.cpp
	ODESolver.cpp
)


add_library(inumerics ${SRC})

set (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/lib")

install (TARGETS inumerics DESTINATION .)
install (DIRECTORY "../include" DESTINATION .)
