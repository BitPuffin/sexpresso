cmake_minimum_required(VERSION 3.0)

# Define project
project(sexpresso VERSION 1.0.0 DESCRIPTION "A C++ centric s-expression parser library.")

# Shared library specification
add_library(sexpresso SHARED sexpresso/sexpresso.cpp)

# Set up version
set_target_properties(sexpresso PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(sexpresso PROPERTIES SOVERSION 1)

# Set up headers (public API)
set_target_properties(sexpresso PROPERTIES PUBLIC_HEADER sexpresso/sexpresso.hpp)

# Install rule
include(GNUInstallDirs)
install(
    TARGETS sexpresso
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
