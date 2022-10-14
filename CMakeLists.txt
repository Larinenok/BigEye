cmake_minimum_required(VERSION 3.10)
project(            BigEye
    DESCRIPTION     "Detection and logging of 'leather bags' by camera"
    HOMEPAGE_URL    "https://github.com/Larinenok/BigEye"
    LANGUAGES       CXX
)

### COMPILE PROPERTIES ###
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter -fdiagnostics-color=always)

    if ("${CMAKE_BUILD_TYPE}" STRGREATER_EQUAL "DEBUG")
        add_compile_options(-O0 -g -fsanitize=undefined -fsanitize=address)
        add_link_options(-fsanitize=undefined -fsanitize=address)
    else ()
        add_compile_options(-O3)
    endif ()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter -fcolor-diagnostics)

    if ("${CMAKE_BUILD_TYPE}" STRGREATER_EQUAL "DEBUG")
        add_compile_options(-O0 -g -fsanitize=undefined -fsanitize=address)
        add_link_options(-fsanitize=undefined -fsanitize=address)
    else ()
        add_compile_options(-O3)
    endif ()

endif ()

### SRC FILES ###
file(GLOB_RECURSE SRCFILES "./src/*.cpp")

include_directories(AFTER SYSTEM ./include)
add_executable(${PROJECT_NAME} ${SRCFILES})
target_include_directories(${PROJECT_NAME} PRIVATE ./include)

#add_subdirectory("./modules")

### SUBMODULES ###
function (submodules_update)
    find_package(Git QUIET)
    if (GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
        option(GIT_SUBMODULE "Check submodules during build" ON)
        if (GIT_SUBMODULE)
            message (">> Update submodules")
            execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                            RESULT_VARIABLE GIT_SUBMOD_RESULT)
            if (NOT GIT_SUBMOD_RESULT EQUAL "0")
                message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
            endif ()
        endif ()
    endif ()
endfunction ()

### OpenCV ###
message         (">> Finding OpenCV")
find_package    (OpenCV QUIET)
if (OpenCV_FOUND)
    include_directories     (${OpenCV_INCLUDE_DIRS})
    link_directories        (${OpenCV_LIBRARY_DIRS})
    target_link_libraries   (${PROJECT_NAME} ${OpenCV_LIBS})
else ()
    message                 (WARNING "OpenCV Not found as system library!")
    # find_library            (opencv HINTS "modules/opencv/include")
    # include_directories     (${PROJECT_SOURCE_DIR}/modules/opencv)
    # link_directories        (${PROJECT_SOURCE_DIR}/modules/opencv)
    # target_link_libraries   (${PROJECT_NAME} opencv)
endif ()

### QT GUI ###
message         (">> Finding Qt")
find_package    (QT NAMES Qt6 Qt5 QUIET COMPONENTS Widgets)
find_package    (Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Widgets)
if (QT_FOUND)
    add_compile_definitions (USE_GUI)
    target_link_libraries   (${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Widgets)
else ()
    message                 (FATAL_ERROR "QT Not found!")
endif ()
