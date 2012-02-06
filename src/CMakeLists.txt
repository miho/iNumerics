include_directories("../odeint")
include_directories("../include")
include_directories("../include")

set(SRC
	Trajectory.cpp
	Problem.cpp
	ODESolver.cpp
        Interpolation.cpp
)


add_library(inumerics ${SRC})


install (TARGETS inumerics DESTINATION .)
install (DIRECTORY "../include" DESTINATION .)
