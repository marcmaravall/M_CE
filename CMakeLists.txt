cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("M_CE")


#target_include_directories(M_CE PUBLIC
#    ${CMAKE_CURRENT_SOURCE_DIR}/tools
#)

add_executable(M_CE
    "src/main.cpp"
    "src/utils.cpp"
    "src/uci.cpp"
    "src/engine.cpp"
    "src/board.cpp"
    "src/search.cpp"
    "src/evaluation.cpp"
    "src/move_generation.cpp" 
    "src/book.h" 
    "src/book.cpp")


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET M_CE PROPERTY CXX_STANDARD 20)

endif()


