cmake_minimum_required(VERSION 3.31)
project(api)

set(CMAKE_CXX_STANDARD 20)

find_package(Crow CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(GTest CONFIG REQUIRED)

add_executable(api main.cpp)

target_link_libraries(api PRIVATE Crow::Crow asio::asio nlohmann_json::nlohmann_json cpr::cpr)

if (WIN32)
    target_link_libraries(api PRIVATE ws2_32 Mswsock)
endif()

add_custom_command(TARGET api POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/static $<TARGET_FILE_DIR:api>/static
)

# Testing
enable_testing()

add_executable(api_tests test_todos.cpp)

target_link_libraries(api_tests PRIVATE
        GTest::gtest_main
        Crow::Crow
        asio::asio
        nlohmann_json::nlohmann_json
        cpr::cpr
)

add_test(NAME TodoApiTests COMMAND api_tests)
