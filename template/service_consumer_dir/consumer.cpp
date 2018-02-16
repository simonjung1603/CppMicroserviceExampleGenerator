#include <cppmicroservices/BundleActivator.h>
#include <cppmicroservices/BundleContext.h>
#include <cppmicroservices/GetBundleContext.h>

#include <@{service_interface_name}.h>

#include <iostream>

using namespace cppmicroservices;

class @{service_consumer_name}Activator : public BundleActivator
{
  typedef ServiceReference<@{service_interface_name}> @{service_interface_name}Ref;

  void Start(BundleContext ctx)
  {
    auto ref = ctx.GetServiceReference<@{service_interface_name}>();
	if (!ref)
    {
      std::cout << "@{service_interface_name} reference invalid" << std::endl;
      return;
    }

    // We could also get the bundle context like this
    //auto ctx = GetBundleContext();

    // Get the @{service_interface_name} service
    auto service = ctx.GetService(ref);
    if (!service)
    {
      std::cout << "@{service_interface_name} not available" << std::endl;
    }
    else
    {
      DoSomething(service);
    }
  }

  void Stop(BundleContext)
  {
    // Nothing to do
  }

  void DoSomething(const std::shared_ptr<@{service_interface_name}>& service) const
  {
    
  }
};

CPPMICROSERVICES_EXPORT_BUNDLE_ACTIVATOR(@{service_consumer_name}Activator)

// [no-cmake]
// The code below is required if the CMake
// helper functions are not used.
#ifdef NO_CMAKE
CPPMICROSERVICES_INITIALIZE_BUNDLE(@{service_consumer_dir})
#endif
