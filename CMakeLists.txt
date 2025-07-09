cmake_minimum_required(VERSION 3.10)
project(LinearRegressionFromScratch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set(SOURCES
    src/Matrix.cpp
    src/CSVReader.cpp
    src/LinearRegression.cpp
)

add_executable(bostonHousing
    examples/bostonHousing.cpp
    ${SOURCES}
)

