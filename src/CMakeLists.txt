include_directories("../odeint")
include_directories("../include")
include_directories("../include")

set(SRC
	Trajectory.cpp
	Problem.cpp
	ODESolver.cpp
        Interpolation.cpp
        inbyte.cpp
        invector.cpp
        inmatrix.cpp
        inbaseobject.cpp
)


add_library(inumerics ${SRC})

target_link_libraries(inumerics ${ACCELERATE})


if(DEBUG)
    add_definitions( -DDEBUG=1 )
endif()

install (TARGETS inumerics DESTINATION .)
install (DIRECTORY "../include" DESTINATION .)
