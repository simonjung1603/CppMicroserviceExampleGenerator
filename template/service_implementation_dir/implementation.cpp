#include <@{service_interface_name}.h>

#include <cppmicroservices/BundleActivator.h>

#include <iostream>

using namespace cppmicroservices;

class @{service_implementation_name} : public @{service_interface_name}
{
  void Ping() const override
  {
    std::cout << "Ping received!\n";
  }
};

class @{service_implementation_name}Activator : public BundleActivator
{
  void Start(BundleContext ctx)
  {
    auto service = std::make_shared<@{service_implementation_name}>();
    ctx.RegisterService<@{service_interface_name}>(service);
  }

  void Stop(BundleContext)
  {
    // Nothing to do
  }
};

CPPMICROSERVICES_EXPORT_BUNDLE_ACTIVATOR(@{service_implementation_name}Activator)

// [no-cmake]
// The code below is required if the CMake
// helper functions are not used.
#ifdef NO_CMAKE
CPPMICROSERVICES_INITIALIZE_BUNDLE(@{service_implementation_dir})
#endif
