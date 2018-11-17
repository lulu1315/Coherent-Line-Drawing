CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(Coherent-Line-Drawing)

FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(wxWidgets REQUIRED)

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
"${PROJECT_SOURCE_DIR}/src/cli.cpp" 
"${PROJECT_SOURCE_DIR}/src/ETF.cpp" 
"${PROJECT_SOURCE_DIR}/src/CLD.cpp" 
"${PROJECT_SOURCE_DIR}/src/postProcessing.cpp" 
"${PROJECT_SOURCE_DIR}/include/ETF.h" 
"${PROJECT_SOURCE_DIR}/include/CLD.h" 
"${PROJECT_SOURCE_DIR}/include/postProcessing.h" 
)

TARGET_LINK_LIBRARIES(CLD-gui ${OpenCV_LIBS} ${wxWidgets_LIBRARIES})
TARGET_LINK_LIBRARIES(CLD-cli ${OpenCV_LIBS})
