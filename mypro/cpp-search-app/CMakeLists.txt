cmake_minimum_required(VERSION 3.16)
project(CPPSearchApp VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required Qt6 components
find_package(Qt6 REQUIRED COMPONENTS 
    Core 
    Widgets 
)

## No network/cURL needed in offline-only mode

# Include directories
include_directories(include)
## No external include dirs required

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Source files
set(SOURCES
    src/main.cpp
    src/MainWindow.cpp
    src/ResultsWidget.cpp
    src/SearchHistory.cpp
    src/LoadingScreen.cpp
    src/OfflineQADatabase.cpp
)

# Header files
set(HEADERS
    include/MainWindow.h
    include/ResultsWidget.h
    include/SearchHistory.h
    include/SearchResult.h
    include/LoadingScreen.h
    include/OfflineQADatabase.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Enable Qt MOC for headers with Q_OBJECT
set_target_properties(${PROJECT_NAME} PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Widgets
)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -O2)
endif()

# Install target
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

# Copy resources
file(COPY resources/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/)
file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/data)
