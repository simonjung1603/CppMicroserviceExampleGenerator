cmake_minimum_required(VERSION 3.7)
# [proj-begin]
project(@{project_name})

find_package(CppMicroServices REQUIRED)
# [proj-end]

include(GenerateExportHeader)

# [interface-begin]
#=========================================================
# A library providing the @{service_interface_name} interface
#---------------------------------------------------------

# Technically, this is not a bundle.

# Allow the developer to select if Dynamic or Static libraries are built
OPTION (BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
# Set the LIB_TYPE variable to STATIC
SET (LIB_TYPE STATIC)
IF (BUILD_SHARED_LIBS)
  # User wants to build Dynamic Libraries, so change the LIB_TYPE variable to CMake keyword 'SHARED'
  SET (LIB_TYPE SHARED)
ENDIF (BUILD_SHARED_LIBS)

add_library(@{service_interface_name}
  @{service_interface_dir}/@{service_interface_name}.h
  @{service_interface_dir}/@{service_interface_name}.cpp
  )

target_include_directories(@{service_interface_name} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/@{service_interface_dir}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
)

generate_export_header(@{service_interface_name})

if(BUILD_SHARED_LIBS)
  set_target_properties(@{service_interface_name} PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
    )
endif()
# [interface-end]

# [publisher-begin]
#=========================================================
# A bundle implementing the @{service_interface_name} interface
#---------------------------------------------------------

set(_srcs
  @{service_implementation_dir}/@{service_implementation_name}.cpp
  )

# Set up dependencies to resources to track changes
usFunctionGetResourceSource(TARGET @{service_implementation_name} OUT _srcs)
# Generate bundle initialization code
usFunctionGenerateBundleInit(TARGET @{service_implementation_name} OUT _srcs)

add_library(@{service_implementation_name} ${_srcs})

target_link_libraries(@{service_implementation_name} CppMicroServices @{service_interface_name})

set(_bundle_name @{service_implementation_dir})

set_target_properties(@{service_implementation_name} PROPERTIES
  # This is required for every bundle
  COMPILE_DEFINITIONS US_BUNDLE_NAME=${_bundle_name}
  # This is for convenience, used by other CMake functions
  US_BUNDLE_NAME ${_bundle_name}
  )

if(BUILD_SHARED_LIBS)
  set_target_properties(@{service_implementation_name} PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
    )
endif()

# Embed meta-data from a manifest.json file
usFunctionEmbedResources(TARGET @{service_implementation_name}
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/@{service_implementation_dir}
  FILES
    manifest.json
  )
# [publisher-end]

#=========================================================
# A bundle consuming a @{service_interface_name} service
#---------------------------------------------------------

set(_srcs
  @{service_consumer_dir}/@{service_consumer_name}.cpp
  )

usFunctionGetResourceSource(TARGET @{service_consumer_name} OUT _srcs)
usFunctionGenerateBundleInit(TARGET @{service_consumer_name} OUT _srcs)

add_library(@{service_consumer_name} ${_srcs})

target_link_libraries(@{service_consumer_name} CppMicroServices @{service_interface_name})

set(_bundle_name @{service_consumer_dir})

set_target_properties(@{service_consumer_name} PROPERTIES
  COMPILE_DEFINITIONS US_BUNDLE_NAME=${_bundle_name}
  US_BUNDLE_NAME ${_bundle_name}
  )

if(BUILD_SHARED_LIBS)
  set_target_properties(@{service_consumer_name} PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN 1
    )
endif()

usFunctionEmbedResources(TARGET @{service_consumer_name}
  WORKING_DIRECTORY
    ${CMAKE_CURRENT_SOURCE_DIR}/@{service_consumer_dir}
  FILES
    manifest.json
  )


# [exec-begin]
#=========================================================
# The executable managing the CppMicroServices framework
#---------------------------------------------------------

set(_srcs
  main.cpp
  )

if(NOT US_BUILD_SHARED_LIBS)
  # Set up dependencies to resources from static bundles
  usFunctionGetResourceSource(TARGET @{project_name} OUT _srcs)
endif()

add_executable(@{project_name} ${_srcs})

target_link_libraries(@{project_name} CppMicroServices)

if(NOT US_BUILD_SHARED_LIBS)
  set(_static_bundles @{service_implementation_name} @{service_consumer_name})
  target_link_libraries(@{project_name} ${_static_bundles})
  usFunctionEmbedResources(TARGET @{project_name}
    # The executable is not a bundle itself but we still
    # need a bundle name for it here.
    BUNDLE_NAME getting_started
    # Merge in all resources from static bundles
    ZIP_ARCHIVES ${_static_bundles}
    )
endif()
# [exec-end]
