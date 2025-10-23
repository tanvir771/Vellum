cmake_minimum_required(VERSION 3.16)

project(PDFBasic VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick)

qt_standard_project_setup(REQUIRES 6.8)

# --- BEGIN: Poppler discovery (config first, pkg-config fallback) ---
# Try CMake config (some environments provide it)
find_package(poppler CONFIG QUIET)

set(POPPLER_TGT "")
if (TARGET poppler::poppler-cpp)
    set(POPLER_TGT poppler::poppler-cpp)
else()
    # Fallback to pkg-config (vcpkg provides 'poppler-cpp.pc')
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(POPPLER_CPP REQUIRED IMPORTED_TARGET poppler-cpp)
    set(POPLER_TGT PkgConfig::POPPLER_CPP)
endif()
# --- END: Poppler discovery ---

qt_add_executable(appPDFBasic
    main.cpp
)

target_link_libraries(appPDFBasic
    PRIVATE
        Qt6::Quick
        Qt6::Concurrent
        ${POPLER_TGT}
)

qt_add_qml_module(appPDFBasic
    URI PDFBasic
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES pdfengine.h pdfengine.cpp
        SOURCES pdfprovider.h pdfprovider.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appPDFBasic PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appPDFBasic
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appPDFBasic
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appPDFBasic
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
