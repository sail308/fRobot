cmake_minimum_required(VERSION 2.6)
project(Follower)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_FLAGS "-O0 -g -std=c++0x -lpthread -DSQLITE_LOCK_TRACE -DSQLITE_DEBUG")

find_package(Threads)
find_package(PCAP)
find_package(Sqlite3)
find_package(ZeroMQ)
find_package(OpenCV REQUIRED)

add_executable(follower main.cpp rssi.cpp motion.cpp vision.cpp motor.cpp comm.cpp wifi_sniffer.cpp radiotap-parser.c db_utils.cpp robotctrl.cpp Particle.cpp ParticleFilter.cpp objectDetection.cpp obstacle.cpp vff.cpp)
target_link_libraries(follower ${CMAKE_THREAD_LIBS_INIT} ${PCAP_LIBRARY} ${SQLITE3_LIBRARIES} ${ZEROMQ_LIBRARIES} ${OpenCV_LIBS})
