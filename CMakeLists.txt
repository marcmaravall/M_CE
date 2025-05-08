cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("M_CE")

add_executable (M_CE  "src/main.cpp" "src/constants.h" "src/uci.cpp" "src/uci.h" "src/engine.h" "src/engine.cpp" "src/board.h" "src/board.cpp" "src/move.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET M_CE PROPERTY CXX_STANDARD 20)
endif()


