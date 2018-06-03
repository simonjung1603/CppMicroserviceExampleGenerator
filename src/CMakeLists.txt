set(_srcs
  CppMsGenerator.cpp
  )
set(_headers
  CppMsGenerator.h
  )
add_executable (${PROJECT_NAME} ${_srcs} ${headers})

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	target_link_libraries(${PROJECT_NAME} PRIVATE /usr/lib/gcc/x86_64-linux-gnu/6/libstdc++fs.a)
endif()