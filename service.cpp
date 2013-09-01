#include "service.h"
#include <cassert>

Service* Service::m_service = nullptr;

Service::~Service() {
  if (m_account) {
    delete [] m_account;
  }

  if (m_password) {
    delete [] m_password;
  }
}

// static
bool Service::Run(Service& svc) {
  m_service = &svc;
  SERVICE_TABLE_ENTRY tableEntry[] = {
    {const_cast<TCHAR*>(m_service->GetName()), SvcMain},
    {nullptr, nullptr}
  };

  return ::StartServiceCtrlDispatcher(tableEntry) == TRUE;
}

// static
void WINAPI Service::SvcMain(DWORD argc, TCHAR* argv[]) {
  assert(m_service != nullptr);

  m_service->m_svcStatusHandle = ::RegisterServiceCtrlHandler(m_service->GetName(),
                                                              ServiceCtrlHandler);
  if (!m_service->m_svcStatusHandle) {
    // TODO(Olster): Add logging here notifying that service didn't start.
    return;
  }

  m_service->Start(argc, argv);
}

// static
void WINAPI Service::ServiceCtrlHandler(DWORD code) {
  switch (code) {
    case SERVICE_CONTROL_STOP:
      m_service->Stop();
    break;

    case SERVICE_CONTROL_PAUSE:
      m_service->Pause();
    break;

    case SERVICE_CONTROL_CONTINUE:
      m_service->Continue();
    break;

    case SERVICE_CONTROL_SHUTDOWN:
      m_service->Shutdown();
    break;

    default:
    break;
  }
}

Service::Service(TCHAR name[],
                 TCHAR displayName[],
                 DWORD dwStartType,
                 DWORD dwErrCtrlType,
                 DWORD dwAcceptedCmds,
                 TCHAR* depends,
                 TCHAR* account,
                 TCHAR* password)
 : m_depends(depends),
   m_dwStartType(dwStartType),
   m_dwErrorCtrlType(dwErrCtrlType),
   m_svcStatusHandle(nullptr) {
  _tcscpy_s(m_name, name);
  _tcscpy_s(m_displayName, displayName);
  
  if (account) {
    int accLength = _tcslen(account);
    m_account = new TCHAR[accLength + 1];
    m_account[accLength] = _T('\0');

    // NOTE(Olster): _tcscpy_s causes heap corruption for some bizzare reason.
    for (int i = 0; i < accLength; i++) {
      m_account[i] = account[i];
    }
  } else {
    m_account = nullptr;
  }

  if (password) {
    int passLength = _tcslen(password);
    m_password = new TCHAR[passLength + 1];
    m_password[passLength] = _T('\0');
    
    // NOTE(Olster): Same as with "account".
    for (int i = 0; i < passLength; i++) {
      m_password[i] = password[i];
    }
  } else {
    m_password = nullptr;
  }

  m_svcStatus.dwControlsAccepted = dwAcceptedCmds;
  m_svcStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
  m_svcStatus.dwCurrentState = SERVICE_START_PENDING;
  m_svcStatus.dwWin32ExitCode = NO_ERROR;
  m_svcStatus.dwServiceSpecificExitCode = 0;
  m_svcStatus.dwCheckPoint = 0;
  m_svcStatus.dwWaitHint = 0;
}

void Service::SetStatus(DWORD dwState, DWORD dwErrCode, DWORD dwWait) {
  m_svcStatus.dwCurrentState = dwState;
  m_svcStatus.dwWin32ExitCode = dwErrCode;
  m_svcStatus.dwWaitHint = dwWait;

  ::SetServiceStatus(m_svcStatusHandle, &m_svcStatus);
}

void Service::WriteToEventLog(TCHAR* msg, EventLogType type) {
  HANDLE hSource = RegisterEventSource(nullptr, m_name);
  if (hSource) {
    const TCHAR* msgData[2] = {m_name, msg};
    ReportEvent(hSource,
                type,
                0, 0,
                nullptr,
                2, 0,
                msgData,
                nullptr);
    DeregisterEventSource(hSource);
  }
}

void Service::Start(DWORD argc, TCHAR* argv[]) {
  SetStatus(SERVICE_START_PENDING);
  OnStart(argc, argv);
  SetStatus(SERVICE_RUNNING);
}

void Service::Stop() {
  SetStatus(SERVICE_STOP_PENDING);
  OnStop();
  SetStatus(SERVICE_STOPPED);
}

void Service::Pause() {
  SetStatus(SERVICE_PAUSE_PENDING);
  OnPause();
  SetStatus(SERVICE_PAUSED);
}

void Service::Continue() {
  SetStatus(SERVICE_CONTINUE_PENDING);
  OnContinue();
  SetStatus(SERVICE_RUNNING);
}

void Service::Shutdown() {
  OnShutdown();
  SetStatus(SERVICE_STOPPED);
}