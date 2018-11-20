CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(Coherent-Line-Drawing)

FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(wxWidgets REQUIRED)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

INCLUDE(${wxWidgets_USE_FILE})
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

ADD_EXECUTABLE(CLD-gui
"${PROJECT_SOURCE_DIR}/src/main.cpp" 
"${PROJECT_SOURCE_DIR}/src/ETF.cpp" 
"${PROJECT_SOURCE_DIR}/src/CLD.cpp" 
"${PROJECT_SOURCE_DIR}/src/postProcessing.cpp" 
"${PROJECT_SOURCE_DIR}/src/gui.cpp"  
"${PROJECT_SOURCE_DIR}/include/ETF.h" 
"${PROJECT_SOURCE_DIR}/include/CLD.h" 
"${PROJECT_SOURCE_DIR}/include/postProcessing.h" 
"${PROJECT_SOURCE_DIR}/include/gui.h" 
)

ADD_EXECUTABLE(CLD-cli
"${PROJECT_SOURCE_DIR}/src/cld-cli.cpp" 
"${PROJECT_SOURCE_DIR}/src/ETF.cpp" 
"${PROJECT_SOURCE_DIR}/src/CLD.cpp" 
"${PROJECT_SOURCE_DIR}/src/postProcessing.cpp" 
"${PROJECT_SOURCE_DIR}/include/ETF.h" 
"${PROJECT_SOURCE_DIR}/include/CLD.h" 
"${PROJECT_SOURCE_DIR}/include/postProcessing.h" 
)

ADD_EXECUTABLE(ETF-cli
"${PROJECT_SOURCE_DIR}/src/etf-cli.cpp" 
"${PROJECT_SOURCE_DIR}/src/ETF.cpp" 
"${PROJECT_SOURCE_DIR}/src/CLD.cpp" 
"${PROJECT_SOURCE_DIR}/src/postProcessing.cpp" 
"${PROJECT_SOURCE_DIR}/include/ETF.h" 
"${PROJECT_SOURCE_DIR}/include/CLD.h" 
"${PROJECT_SOURCE_DIR}/include/postProcessing.h" 
)

TARGET_LINK_LIBRARIES(CLD-gui ${OpenCV_LIBS} ${wxWidgets_LIBRARIES})
TARGET_LINK_LIBRARIES(CLD-cli ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(ETF-cli ${OpenCV_LIBS})
