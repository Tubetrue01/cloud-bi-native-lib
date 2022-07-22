cmake_minimum_required(VERSION 3.21.1)
project(cloud-bi-native-lib C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_INSTALL_PREFIX /usr/cloud-bi-native-lib)

aux_source_directory(src SRC_PATH)
add_executable(nativetools  ${SRC_PATH})
