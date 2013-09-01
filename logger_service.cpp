#include "logger_service.h"
#include "utility.h"

LoggerService::LoggerService()
 : Service(_T("Logger service"),
           _T("Logger service"),
           SERVICE_AUTO_START,
           SERVICE_ERROR_NORMAL,
           Service::STOP,
           nullptr,
           _T(".\\LocalSystem"),
           nullptr) {
  m_hStopHandle = ::CreateEvent(nullptr, TRUE, FALSE, nullptr);
  if (!m_hStopHandle) {
    _tprintf(_T("Didn't start service"));
  }
}

LoggerService::~LoggerService() {
  if (m_hStopHandle) {
    ::CloseHandle(m_hStopHandle);
  }
}

void LoggerService::OnStart(DWORD argc, TCHAR* argv[]) {
  (void)argc;
  (void)argv;

  WriteToEventLog(_T("Logging service started"));
  
  
}

void LoggerService::OnStop() {
  WriteToEventLog(_T("Stopping logging service"));
  WaitForSingleObject(m_hStopHandle, INFINITE);
}
