# CMakeList.txt : CMake project for adventuregame, include source and define
# project specific logic here.
#

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

# Add in Json to exe file 
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/jsonfiles/example.json ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

# Add source to this project's executable.
add_executable (adventuregame "adventuregame.cpp" "adventuregame.h")

target_link_libraries(adventuregame PRIVATE nlohmann_json::nlohmann_json)
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET adventuregame PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
