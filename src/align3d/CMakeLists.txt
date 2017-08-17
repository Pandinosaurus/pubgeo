SET(ALIGN3D_HEADER_FILES
        align3d.h)

SET(ALIGN3D_SOURCE_FILES
        align3d.cpp)

ADD_LIBRARY(ALIGN3D_LIB STATIC ${ALIGN3D_HEADER_FILES} ${ALIGN3D_SOURCE_FILES})
TARGET_INCLUDE_DIRECTORIES(ALIGN3D_LIB
        PUBLIC
        PUBGEO_LIB)

TARGET_LINK_LIBRARIES(ALIGN3D_LIB
        PUBLIC
        PUBGEO_LIB)

### Add some executables to play with.
### Main exe will showcase all features
ADD_EXECUTABLE(align3d main.cpp)
TARGET_LINK_LIBRARIES(align3d
        PUBLIC
        ALIGN3D_LIB)

if(TESTING)
    ADD_EXECUTABLE(align3dTest test-main.cpp)
    TARGET_LINK_LIBRARIES(align3dTest
            PUBLIC
            ALIGN3D_LIB)
endif()