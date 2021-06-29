cmake_minimum_required(VERSION 3.0.0)
project(Project-3New VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(Project-3New MainTrain.cpp SimpleAnomalyDetector.cpp timeseries.cpp anomaly_detection_util.cpp HybridAnomalyDetector.cpp minCircle.cpp CLI.cpp Server.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set(GCC_COVERAGE_COMPILE_FLAGS "-pthread -std=c++11")

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
