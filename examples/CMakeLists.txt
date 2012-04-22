include_directories("../odeint")
include_directories("../include")

if(DEBUG)
    add_definitions( -DDEBUG=1 )
endif()


add_executable( test01 test01.cpp)

TARGET_LINK_LIBRARIES(test01 inumerics)

install (TARGETS test01 DESTINATION ./examples/)
install (DIRECTORY "../include" DESTINATION .)
