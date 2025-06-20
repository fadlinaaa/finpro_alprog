cmake_minimum_required(VERSION 3.20) # Increased version for FetchContent best practices
project(finpro LANGUAGES CXX)

# Set CMP0135 to NEW to use current timestamps for extracted FetchContent
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directory
include_directories(include)

# --- nlohmann-json ---
# This needs to be available before tests subdirectory is added if tests use it,
# and before any other library that might use it.
include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz)
FetchContent_MakeAvailable(json) # This makes the nlohmann_json::nlohmann_json target available

# --- Libraries for modules ---
# Data Processing Module
add_library(finpro_data_processing src/data_processing/AnomalyDetector.cpp)
target_include_directories(finpro_data_processing PUBLIC include)


# Storage Module
add_library(finpro_storage src/storage/DataStorage.cpp)
target_include_directories(finpro_storage PUBLIC include)
# If DataStorage.cpp itself needed nlohmann::json, you would link it here:
# target_link_libraries(finpro_storage PUBLIC nlohmann_json::nlohmann_json)


# Query & Synchronization Module
add_library(finpro_query_sync src/query_sync/DataManager.cpp)
target_include_directories(finpro_query_sync PUBLIC include)
target_link_libraries(finpro_query_sync PRIVATE finpro_data_processing)

# --- Core Library for Client & Server ---
add_library(finpro_core src/Client.cpp src/Server.cpp)
target_include_directories(finpro_core PUBLIC include)

# --- Main Executable ---

add_executable(finpro src/main.cpp)
# Link main executable against the module libraries
target_link_libraries(finpro PRIVATE
        finpro_data_processing
        finpro_storage
        finpro_query_sync
)

if (WIN32)
    target_link_libraries(finpro PRIVATE Ws2_32)
endif()

# --- Google Test Setup ---
# Enable testing
enable_testing()

# Add the tests subdirectory
# Now, when tests/CMakeLists.txt is processed, nlohmann_json::nlohmann_json will exist.
add_subdirectory(tests)

