include_directories("../odeint")
include_directories("../include")

if(DEBUG)
    add_definitions( -DDEBUG=1 )
endif()


add_executable( test01 test01.cpp)
TARGET_LINK_LIBRARIES(test01 inumerics)

add_executable( test02 test02.cpp)
TARGET_LINK_LIBRARIES(test02 inumerics)

install (TARGETS test01 DESTINATION ./examples/)
install (TARGETS test02 DESTINATION ./examples/)
install (DIRECTORY "../include" DESTINATION .)
