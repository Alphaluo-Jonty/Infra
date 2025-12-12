cmake_minimum_required(VERSION 3.10)
project(fastdds_demo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the vendor directory to the search path
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/FastDDS)

# Add include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../vendor/FastDDS/include)

# Define the executable targets
add_executable(publisher src/publisher.cpp src/typesupport.cpp)
add_executable(subscriber src/subscriber.cpp src/typesupport.cpp)

# Check if we are on Windows or Linux and adjust library linking accordingly
if(WIN32)
    # On Windows, look for .lib files
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/FastDDS/lib")
        target_link_libraries(publisher
            ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/FastDDS/lib/fastdds.lib
            ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/FastDDS/lib/fastcdr.lib
        )
        target_link_libraries(subscriber
            ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/FastDDS/lib/fastdds.lib
            ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/FastDDS/lib/fastcdr.lib
        )
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/FastDDS/dll")
        # If libs are not in lib dir, try to find them in dll directory or system
        find_library(FASTDDS_LIB 
            NAMES fastdds libfastdds
            PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/FastDDS/dll
                  ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/FastDDS
            NO_DEFAULT_PATH
        )
        if(FASTDDS_LIB)
            target_link_libraries(publisher ${FASTDDS_LIB})
            target_link_libraries(subscriber ${FASTDDS_LIB})
        else()
            # Fallback: try to find installed FastDDS
            find_package(fastdds REQUIRED)
            target_link_libraries(publisher fastdds)
            target_link_libraries(subscriber fastdds)
        endif()
    endif()
else()
    # On Linux/macOS, look for .so/.a files
    find_package(PkgConfig REQUIRED)
    # Try to find system installed FastDDS
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_check_modules(FASTDDS fastdds)
    endif()
    
    if(FASTDDS_FOUND)
        target_link_libraries(publisher ${FASTDDS_LIBRARIES})
        target_link_libraries(subscriber ${FASTDDS_LIBRARIES})
        target_include_directories(publisher PRIVATE ${FASTDDS_INCLUDE_DIRS})
        target_include_directories(subscriber PRIVATE ${FASTDDS_INCLUDE_DIRS})
    else()
        # Direct linking assuming FastDDS is installed in standard locations
        target_link_libraries(publisher fastrtps fastdds fastcdr)
        target_link_libraries(subscriber fastrtps fastdds fastcdr)
    endif()
endif()