#ifndef SERVICE_INSTALLER_H_
#define SERVICE_INSTALLER_H_

#include "service.h"

class ServiceInstaller {
public:
  static bool Install(const Service& service);
  static bool Uninstall(const Service& service);
private:
  ServiceInstaller() {}
};

#endif // SERVICE_INSTALLER_H_

