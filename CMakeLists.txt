cmake_minimum_required(VERSION 3.21.1)
project(cloud-bi-native-lib C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/build/libs)

set(NATIVE_TOOLS_SRC
        src/nativetools.c
        )

find_path(LIBEV_INCLUDE NAMES ev.h      PATHS /usr/local/include/libev)
find_path(JNI_H         NAMES jni.h     PATHS /Users/tubetrue01/Applications/Java/jdk1.8.0_171.jdk/Contents/Home/include)
find_path(JNI_MOD       NAMES jni_md.h  PATHS /Users/tubetrue01/Applications/Java/jdk1.8.0_171.jdk/Contents/Home/include/darwin)
find_library(LIBEV_LIB  NAMES ev        PATHS /usr/local/lib/libev)

list(APPEND INCLUDES
        ${LIBEV_INCLUDE}
        ${JNI_H}
        ${JNI_MOD}
        )


add_library(NativeTools SHARED ${NATIVE_TOOLS_SRC})
target_include_directories(NativeTools PRIVATE ${INCLUDES})
target_link_libraries(NativeTools ${LIBEV_LIB})

INSTALL(TARGETS NativeTools
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
        )
