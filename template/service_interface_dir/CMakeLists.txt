# [interface-begin]
#=========================================================
# A library providing the @{service_interface_name} interface
#---------------------------------------------------------

# Technically, this is not a bundle.

add_library(@{service_interface_name}
  @{service_interface_name}.h
  @{service_interface_name}.cpp
  )

target_include_directories(@{service_interface_name} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
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