#ifndef SERVICE_H_
#define SERVICE_H_

#include <Windows.h>
#include <tchar.h>

// Base Service class used to create windows services.
class Service {
public:
  enum AcceptedControls {
    STOP = SERVICE_ACCEPT_STOP,
    SHUTDOWN = SERVICE_ACCEPT_SHUTDOWN,
    PAUSE_CONTINUE = SERVICE_ACCEPT_PAUSE_CONTINUE
  };

  virtual ~Service();
  static bool Run(Service& svc);

  const TCHAR* GetName() const { return m_name; }
  const TCHAR* GetDisplayName() const { return m_displayName; }
  const DWORD GetStartType() const { return m_dwStartType; }
  const DWORD GetErrorControlType() const { return m_dwErrorCtrlType; }
  const TCHAR* GetDependencies() const { return m_depends; }

  // Defines what account service runs under.
  const TCHAR* GetAccount() const { return m_account; }
  const TCHAR* GetPassword() const { return m_password; }
protected:
  Service(TCHAR name[],
          TCHAR displayName[],
          DWORD dwStartType,
          DWORD dwErrCtrlType,
          DWORD dwAcceptedCmds,
          TCHAR* depends,
          TCHAR* account = nullptr,
          TCHAR* password = nullptr);
  void SetStatus(DWORD dwState, DWORD dwErrCode = NO_ERROR, DWORD dwWait = 0);

  enum EventLogType {
    EVT_LOG_ERROR = EVENTLOG_ERROR_TYPE,
    EVT_LOG_WARNING = EVENTLOG_WARNING_TYPE,
    EVT_LOG_INFO = EVENTLOG_INFORMATION_TYPE
  };
  void WriteToEventLog(TCHAR* msg, EventLogType type = EVT_LOG_INFO);

  virtual void OnStart(DWORD argc, TCHAR* argv[]) = 0;
  virtual void OnStop() {}
  virtual void OnPause() {}
  virtual void OnContinue() {}
  virtual void OnShutdown() {}
private:
  // Registers handle and starts the service.
  static void WINAPI SvcMain(DWORD argc, TCHAR* argv[]);

  // Called whenever control managed updates service status.
  static void WINAPI ServiceCtrlHandler(DWORD code);

  void Start(DWORD argc, TCHAR* argv[]);
  void Stop();
  void Pause();
  void Continue();
  void Shutdown();

  // Service name and display name size is restricted by Microsoft.
  // see: http://msdn.microsoft.com/en-us/library/ms682450(v=vs.85).ASPX
  TCHAR m_name[256];
  TCHAR m_displayName[256];
  DWORD m_dwStartType;
  DWORD m_dwErrorCtrlType;
  TCHAR* m_depends;
  TCHAR* m_account;
  TCHAR* m_password;

  SERVICE_STATUS m_svcStatus;
  SERVICE_STATUS_HANDLE m_svcStatusHandle;

  static Service* m_service;
};

#endif // SERVICE_H_
