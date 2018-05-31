# [publisher-begin]
#=========================================================
# A bundle implementing the @{service_interface_name} interface
#---------------------------------------------------------

set(_srcs
  @{service_implementation_name}.cpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}
  FILES
    manifest.json
  )
# [publisher-end]
