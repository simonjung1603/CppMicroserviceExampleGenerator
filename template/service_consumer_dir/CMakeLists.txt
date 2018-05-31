#=========================================================
# A bundle consuming a @{service_interface_name} service
#---------------------------------------------------------

set(_srcs
  @{service_consumer_name}.cpp
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
    ${CMAKE_CURRENT_SOURCE_DIR}
  FILES
    manifest.json
  )