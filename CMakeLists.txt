cmake_minimum_required(VERSION 3.10)
project(            BigEye
    DESCRIPTION     "Detection and logging of 'leather bags' by camera"
    HOMEPAGE_URL    "https://github.com/Larinenok/BigEye"
    LANGUAGES       CXX
)

### COMPILE PROPERTIES ###
# Instruct CMake to create code from Qt designer ui files
set(CMAKE_AUTOUIC ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# CMake will handle the Qt rcc code generator automatically
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BE_BUILD_SAN "Build project with sanitizers (Debug only!)" OFF)
option(BE_USE_COLORS "Use colors in console (Unix only)" ON)

# Normal-like compile properties
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

### Finding libs ###
message         (">> Finding OpenCV")
find_package    (OpenCV REQUIRED)

message         (">> Finding Qt")
find_package    (QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Widgets)
find_package    (Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

message         (">> Finding PostgreSQL")
find_package    (PostgreSQL REQUIRED)

### PROJECT FILES AND LIBRARIES ###
file                (GLOB_RECURSE SRCFILES "./src/*.cpp")
include_directories (AFTER SYSTEM ./include)

if (UNIX AND BE_USE_COLORS)
    add_compile_definitions (USE_COLORS)
endif ()

### QT
if (QT_FOUND)
    add_compile_definitions (USE_GUI)
    if (QT_VERSION_MAJOR GREATER_EQUAL 6)
        qt_add_executable       (${PROJECT_NAME} MANUAL_FINALIZATION ${SRCFILES})
        qt_finalize_executable  (${PROJECT_NAME})
    else ()
        add_executable          (${PROJECT_NAME} ${SRCFILES})
    endif ()
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
else ()
    message (FATAL_ERROR "QT Not found! Install QT and add it to PATH")
endif ()

### OpenCV
if (OpenCV_FOUND)
    include_directories     (${OpenCV_INCLUDE_DIRS})
    link_directories        (${OpenCV_LIBRARY_DIRS})
    target_link_libraries   (${PROJECT_NAME} PRIVATE ${OpenCV_LIBS})
else ()
    message (FATAL_ERROR
        "OpenCV Not found as system library!\n"
        "Install OpenCV and add it to PATH or override variables:\n"
        "'OpenCV_INCLUDE_DIRS', 'OpenCV_LIBRARY_DIRS', 'OpenCV_LIBS'"
    )
endif ()

### PostreSQL
if (PostgreSQL_FOUND)
    include_directories     (${PostgreSQL_INCLUDE_DIRS})
    link_directories        (${PostgreSQL_LIBRARY_DIRS})
    target_link_libraries   (${PROJECT_NAME} PRIVATE ${PostgreSQL_LIBS})
else ()
    message (FATAL_ERROR "PostgreSQL libraries not found!")
endif()

### PQXX
find_library (PQXX_LIB pqxx)
if (PQXX_LIB)
    target_link_libraries   (${PROJECT_NAME} PRIVATE ${PQXX_LIB})
else ()
    submodules_update()
    add_subdirectory        (./libs/libpqxx)
    find_library            (pqxx HINTS "libs/libpqxx/include")
    include_directories     (${PROJECT_SOURCE_DIR}/libs/libpqxx/include)
    link_directories        (${PROJECT_SOURCE_DIR}/libs/libpqxx/src)
    target_link_libraries   (${PROJECT_NAME} PRIVATE pqxx)
endif ()

target_include_directories  (${PROJECT_NAME} PRIVATE ./include)

### POST-TRANSACTION TASKS ###
message (">> Downloading latest OpenCV face-detection files...")

set     (BE_OPENCV_CAFFEMODEL_URL "https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20180205_fp16/res10_300x300_ssd_iter_140000_fp16.caffemodel")
if (NOT EXISTS "./haarcascade_frontalface_default.xml")
    file    (DOWNLOAD ${BE_OPENCV_HCASCADE_URL} "./haarcascade_frontalface_default.xml")
endif ()

set     (BE_OPENCV_DEPLOYPROTO_URL "https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt")
if (NOT EXISTS "./deploy.prototxt")
file    (DOWNLOAD ${BE_OPENCV_DEPLOYPROTO_URL} "./deploy.prototxt")
endif ()

set     (BE_OPENCV_HCASCADE_URL "https://raw.githubusercontent.com/opencv/opencv/master/data/haarcascades/haarcascade_frontalface_default.xml")
if (NOT EXISTS "./res10_300x300_ssd_iter_140000_fp16.caffemodel")
file    (DOWNLOAD ${BE_OPENCV_CAFFEMODEL_URL} "./res10_300x300_ssd_iter_140000_fp16.caffemodel")
endif ()
