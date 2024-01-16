cmake_minimum_required(VERSION 3.16.3)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(async VERSION ${PROJECT_VERSION})

add_library(async SHARED 
    src/async.cpp
    src/commands.cpp
    src/parser.cpp
)


target_include_directories(async PRIVATE
    "${CMAKE_BINARY_DIR}"
    src
    include
)

configure_file(version.h.in version.h)


add_executable(async_cli src/main.cpp)

target_link_libraries(async_cli async)

target_include_directories(async_cli PRIVATE
    "${CMAKE_BINARY_DIR}"
    src
    include
)
  
set_target_properties(async async_cli PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)
#set_target_properties(async PROPERTIES PUBLIC_HEADER async.h)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(async PRIVATE 
        -g
        -Wall
        -Wextra
        -Werror
        -pedantic
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(async PRIVATE
        /EHsc
        /W4
        /WX
    )
endif()


#install(TARGETS async async_cli)
install(TARGETS async_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

