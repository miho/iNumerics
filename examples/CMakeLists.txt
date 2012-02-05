include_directories("../odeint")
include_directories("../include")
include_directories("../include")

set(SRC
	test01.cpp
)




add_executable( test01 ${SRC})

TARGET_LINK_LIBRARIES(test01 inumerics)

install (TARGETS test01 DESTINATION ./examples/)
install (DIRECTORY "../include" DESTINATION .)
