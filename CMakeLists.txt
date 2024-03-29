cmake_minimum_required(VERSION 3.6)
project(Arch_Until_FeaturesExtraction)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CmakeModules")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp CustomCFGImpl.cpp CustomCFGImpl.h Disassembler.cpp Disassembler.h Aux.cpp Aux.h DisassemblingInfo.cpp DisassemblingInfo.h  CustomBasicBlockImpl.cpp CustomBasicBlockImpl.h Experimentation.cpp Experimentation.h DFG.cpp DFG.h X86Instruction.cpp X86Instruction.h FeaturesExtraction.cpp FeaturesExtraction.h MnemonicClassification.cpp MnemonicClassification.h PE32FileInfo.cpp PE32FileInfo.h MalwareOrganizedTools.cpp MalwareOrganizedTools.h)

find_package(CAPSTONE REQUIRED)
include_directories(${CAPSTONE_INCLUDE_DIRS})
set(LIBS ${LIBS} ${CAPSTONE_LIBRARIES})

find_package(Sqlite3 REQUIRED)
include_directories(${SQLITE3_INCLUDE_DIR})
set(LIBS ${LIBS} ${SQLITE3_LIBRARY})

add_executable(Arch_Until_FeaturesExtraction ${SOURCE_FILES})

target_link_libraries(Arch_Until_FeaturesExtraction ${LIBS})