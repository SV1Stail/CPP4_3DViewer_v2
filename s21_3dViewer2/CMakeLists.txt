cmake_minimum_required(VERSION 3.5)

project(s21_3dViewer2 VERSION 0.3 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
# in some cases it is needed to add path to QT/cmake
#set(CMAKE_PREFIX_PATH "/Users/rsaga/Qt/6.6.1/macos/lib/cmake")
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets OpenGL Gui REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets OpenGL Gui REQUIRED)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS OpenGLWidgets REQUIRED)
endif()

set(PROJECT_SOURCES
        main.cpp
        Model/v2Model.cpp
        Model/v2Model.h
        View/v2MainView.cpp
        View/v2MainView.h
        View/v2MainView.ui
        View/MyQOpenGLWidget.h
        View/MyQOpenGLWidget.cpp
        Controller/v2Controller.cpp
        Controller/v2Controller.h
        resource.qrc
        ../objparser/objparser.cpp
        ../objparser/objparser.h
)
# for gif animation
add_subdirectory(QtGifImage-master)

# for serialization
add_library(cereal INTERFACE)
add_library(cereal::cereal ALIAS cereal)
target_include_directories(cereal INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cereal)

## какие то мутные проверки по умолчанию, сгенерированные QTCreator-ом
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET untitled APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
        )
    endif()
endif()
## end 

# Qt Library
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::OpenGL
    Qt${QT_VERSION_MAJOR}::Gui
    QtGifImage::QtGifImage
    cereal
)
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::OpenGLWidgets)
endif()


target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()


# We don't care about DOWNLOAD_EXTRACT_TIMESTAMP; prefer the new behavior
# Error in ubuntu, warn without it on Mac
if (${CMAKE_SYSTEM} MATCHES "Darwin")
    cmake_policy(SET CMP0135 NEW)
endif()


# GTEST
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(TEST_SRC
    ${PROJECT_SOURCE_DIR}/../tests/tests.h
    ${PROJECT_SOURCE_DIR}/../tests/tests.cpp
    ${PROJECT_SOURCE_DIR}/../objparser/objparser.cpp
    ${PROJECT_SOURCE_DIR}/../objparser/objparser.h
    ${PROJECT_SOURCE_DIR}/../objparser/parsertypes.h

)

enable_testing()

add_executable(tests
  ${TEST_SRC}
)
target_link_libraries(tests PUBLIC
  GTest::gtest_main
)
target_link_libraries(tests PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

include(GoogleTest)
gtest_discover_tests(tests)

# compile and link code instrumented for coverage analysis
SET(CMAKE_CXX_FLAGS "--coverage")

# Create the gcov target. Run coverage tests with 'make gcov'
add_custom_target(gcov
    COMMAND mkdir -p gcoverage
    COMMAND ${CMAKE_MAKE_PROGRAM} test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
add_custom_command(TARGET gcov
    COMMAND echo "=================== GCOV ===================="
    COMMAND gcov -b ${CMAKE_BINARY_DIR}/CMakeFiles/tests.dir/Model/*
    COMMAND echo "-- Source diretorie: ${CMAKE_BINARY_DIR}/CMakeFiles/tests.dir/Model/"
    COMMAND echo "-- Coverage files have been output to ${CMAKE_BINARY_DIR}/gcoverage"
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/gcoverage
    )
add_dependencies(gcov tests)
