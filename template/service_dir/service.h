#include <@{service_interface_export_header}.h>

struct @{service_interface_export_macro} @{service_interface_name}
{
  virtual ~@{service_interface_name}();

  // Define your interface
  virtual void Ping() const = 0;
};
