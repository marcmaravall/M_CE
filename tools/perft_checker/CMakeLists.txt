﻿cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("perft_checker")

add_executable (perft_checker "perft_checker.cpp" "perft_checker.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET perft_checker PROPERTY CXX_STANDARD 20)
endif()

