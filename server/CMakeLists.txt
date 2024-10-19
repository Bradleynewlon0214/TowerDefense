cmake_minimum_required(VERSION 3.29)
project(TowerDefense)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(TowerDefense
    main.cpp)

find_package(oatpp CONFIG REQUIRED C:/Users/Bradl/.vcpkg-clion/vcpkg/installed/x64-mingw-dynamic/include/oatpp-1.3.0/oatpp)
target_link_libraries(TowerDefense PRIVATE oatpp::oatpp)

find_package(oatpp-websocket CONFIG REQUIRED C:/Users/Bradl/.vcpkg-clion/vcpkg/installed/x64-mingw-dynamic/include/oatpp-1.3.0/oatpp-websocket)
target_link_libraries(TowerDefense PRIVATE oatpp::oatpp-websocket)
