#include "service_installer.h"

#include <Windows.h>
#include <tchar.h>

#include "utility.h"

//static
bool ServiceInstaller::Install(const Service& service) {
  TCHAR modulePath[MAX_PATH] = _T("");

  if (::GetModuleFileName(nullptr, modulePath, ARR_SIZE(modulePath)) == 0) {
    _tprintf(_T("Couldn't get module file name: %d\n"), GetLastError());
    return false;
  }

  SC_HANDLE svcControlManager = ::OpenSCManager(nullptr, nullptr,
                                                SC_MANAGER_CONNECT | 
                                                SC_MANAGER_CREATE_SERVICE);
  if (!svcControlManager) {
    _tprintf(_T("Couldn't open service control manager: %d\n"), GetLastError());
    return false;
  }

  SC_HANDLE servHandle = ::CreateService(svcControlManager,
                                         service.GetName(),
                                         service.GetDisplayName(),
                                         SERVICE_QUERY_STATUS,
                                         SERVICE_WIN32_OWN_PROCESS,
                                         service.GetStartType(),
                                         service.GetErrorControlType(),
                                         modulePath,
                                         nullptr,
                                         nullptr,
                                         service.GetDependencies(),
                                         service.GetAccount(),
                                         service.GetPassword());
  if (!servHandle) {
    _tprintf(_T("Couldn't create service: %d\n"), GetLastError());
    ::CloseServiceHandle(svcControlManager);
    return false;
  }

  ::CloseServiceHandle(svcControlManager);
  ::CloseServiceHandle(servHandle);
  return true;
}

//static
bool ServiceInstaller::Uninstall(const Service& service) {
  SC_HANDLE svcControlManager = ::OpenSCManager(nullptr, nullptr,
                                                SC_MANAGER_CONNECT);

  if (!svcControlManager) {
    _tprintf(_T("Couldn't open service control manager: %d\n"), GetLastError());
    return false;
  }

  SC_HANDLE servHandle = ::OpenService(svcControlManager, service.GetName(),
                                       SERVICE_QUERY_STATUS | 
                                       SERVICE_STOP |
                                       DELETE);

  if (!servHandle) {
    ::CloseServiceHandle(svcControlManager);
    _tprintf(_T("Couldn't open service control manager: %d\n"), GetLastError());
    return false;
  }

  SERVICE_STATUS servStatus = {};
  if (::ControlService(servHandle, SERVICE_CONTROL_STOP, &servStatus)) {
    _tprintf(_T("Stoping service %s\n"), service.GetName());
    
    while (::QueryServiceStatus(servHandle, &servStatus)) {
      if (servStatus.dwCurrentState != SERVICE_STOP_PENDING) {
        break;
      }
    }

    if (servStatus.dwCurrentState != SERVICE_STOPPED) {
      _tprintf(_T("Failed to stop the service\n"));
    } else {
      _tprintf(_T("Service stopped\n"));
    }
  } else {
    _tprintf(_T("Didn't control service: %d\n"), GetLastError());
  }

  if (!DeleteService(servHandle)) {
    _tprintf(_T("Failed to delete the service: %d\n"), GetLastError());
    ::CloseServiceHandle(svcControlManager);
    ::CloseServiceHandle(servHandle);
    return false;
  }

  return true;
}