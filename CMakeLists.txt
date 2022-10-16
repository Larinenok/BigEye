cmake_minimum_required(VERSION 3.10)
project(            BigEye
    DESCRIPTION     "Detection and logging of 'leather bags' by camera"
    HOMEPAGE_URL    "https://github.com/Larinenok/BigEye"
    LANGUAGES       CXX
)

### COMPILE PROPERTIES ###
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BE_BUILD_SAN OFF)                   # Build project with sanitizers

function (no_msvc)
    if ("${CMAKE_BUILD_TYPE}" STRGREATER_EQUAL "DEBUG")
        add_compile_options(-O0 -g)
        if (BE_BUILD_SAN)
            add_compile_options(-fsanitize=undefined -fsanitize=address)
            add_link_options(-fsanitize=undefined -fsanitize=address)
        endif ()

    else ()
        add_compile_options(-O3)

    endif ()

endfunction()

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter -fdiagnostics-color=always)
    no_msvc()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter -fcolor-diagnostics)
    no_msvc()

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
            execute_process(
                COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT
            )
            if (NOT GIT_SUBMOD_RESULT EQUAL "0")
                message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
            endif ()
        endif ()
    endif ()
endfunction ()

### OpenCV ###
message         (">> Finding OpenCV")
find_package    (OpenCV QUIET)
if (NOT OpenCV_FOUND)
    message (WARNING
        "OpenCV Not found as system library!\n"
        "Install OpenCV and add it to PATH or override variables:\n"
        "'OpenCV_INCLUDE_DIRS', 'OpenCV_LIBRARY_DIRS', 'OpenCV_LIBS'"
    )
endif ()
include_directories     (${OpenCV_INCLUDE_DIRS})
link_directories        (${OpenCV_LIBRARY_DIRS})
target_link_libraries   (${PROJECT_NAME} ${OpenCV_LIBS})

### QT GUI ###
message         (">> Finding Qt")
find_package    (QT NAMES Qt6 Qt5 QUIET COMPONENTS Widgets Core)
find_package    (Qt${QT_VERSION_MAJOR} QUIET COMPONENTS Widgets Core)
if (QT_FOUND)
    add_compile_definitions (USE_GUI)
    target_link_libraries   (${PROJECT_NAME} Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core)
else ()
    message                 (FATAL_ERROR "QT Not found! Install QT and add it to PATH")
endif ()
