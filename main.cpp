#include "service_installer.h"
#include "logger_service.h"

int _tmain(int argc, TCHAR* argv[]) {
  LoggerService logger;
  if (argc > 1) {
    if (_tcscmp(argv[1], _T("install")) == 0) {
      _tprintf(_T("Installing service\n"));
      if (!ServiceInstaller::Install(logger)) {
        _tprintf(_T("Couldn't install service: %d\n"), GetLastError());
        return -1;
      }

      _tprintf(_T("Service installed\n"));
      return 0;
    }
    
    if (_tcscmp(argv[1], _T("uninstall")) == 0) {
      _tprintf(_T("Uninstalling service\n"));
      if (!ServiceInstaller::Uninstall(logger)) {
        _tprintf(_T("Couldn't uninstall service: %d\n"), GetLastError());
        return -1;
      }

      _tprintf(_T("Service uninstalled\n"));
      return 0;
    }

    _tprintf(_T("Invalid argument\n"));
    return -1;
  }
  
  Service::Run(logger);

  return 0;
}