#include "user_tracker_service.h"
#include "service_installer.h"

int _tmain(int argc, TCHAR* argv[]) {
  UserTrackerService service;

  if (argc > 1) {
    if (_tcscmp(argv[1], _T("install")) == 0) {
      _tprintf(_T("Installing service\n"));
      if (!ServiceInstaller::Install(service)) {
        _tprintf(_T("Couldn't install service: %d\n"), ::GetLastError());
        return -1;
      }

      _tprintf(_T("Service installed\n"));
      return 0;
    }
    
    if (_tcscmp(argv[1], _T("uninstall")) == 0) {
      _tprintf(_T("Uninstalling service\n"));
      if (!ServiceInstaller::Uninstall(service)) {
        _tprintf(_T("Couldn't uninstall service: %d\n"), ::GetLastError());
        return -1;
      }

      _tprintf(_T("Service uninstalled\n"));
      return 0;
    }

    _tprintf(_T("Invalid argument\n"));
    return -1;
  }
  
  service.Run();

  return 0;
}