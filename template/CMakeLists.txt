cmake_minimum_required(VERSION 3.7)
# [proj-begin]
project(@{project_name})

find_package(CppMicroServices REQUIRED @{cppmicroservices_path})
# [proj-end]

include(GenerateExportHeader)

# Allow the developer to select if Dynamic or Static libraries are built
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" ON)
# Set the LIB_TYPE variable to STATIC
SET (LIB_TYPE STATIC)
IF (BUILD_SHARED_LIBS)
  # User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
  SET (LIB_TYPE SHARED)
ENDIF (BUILD_SHARED_LIBS)

add_subdirectory(@{service_interface_dir})

add_subdirectory(@{service_implementation_dir})

add_subdirectory(@{service_consumer_dir})

# [exec-begin]
#=========================================================
# The executable managing the CppMicroServices framework
#---------------------------------------------------------

set(_srcs
  main.cpp
  )

add_executable(@{project_name} ${_srcs})

target_link_libraries(@{project_name} CppMicroServices)

if(NOT US_BUILD_SHARED_LIBS)
  set(_static_bundles @{service_implementation_name} @{service_consumer_name})
  target_link_libraries(@{project_name} ${_static_bundles})
endif()
# [exec-end]
# [config-begin]
configure_file(project.vcxproj.user.in @{project_name}.vcxproj.user)
# [config-end]