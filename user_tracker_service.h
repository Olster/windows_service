#ifndef USER_TRACKER_SERVICE_H_
#define USER_TRACKER_SERVICE_H_

#include <fstream>

#include "service_base.h"

class UserTrackerService : public ServiceBase {
 public:
  UserTrackerService(const UserTrackerService& other) = delete;
  UserTrackerService& operator=(const UserTrackerService& other) = delete;

  UserTrackerService(UserTrackerService&& other) = delete;
  UserTrackerService& operator=(UserTrackerService&& other) = delete;

  UserTrackerService()
   : ServiceBase(_T("UserTrackerService"),
                 _T("User tracker service"),
                 SERVICE_DEMAND_START,
                 SERVICE_ERROR_NORMAL,
                 SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SESSIONCHANGE) {}
 private:
   void OnStart(DWORD argc, TCHAR* argv[]) override;
   void OnStop() override;
   void OnSessionChange(DWORD evtType,
                        WTSSESSION_NOTIFICATION* notification) override;

#ifdef UNICODE
   using tofstream = std::wofstream;
#else
   using tofstream = std::ofstream;
#endif

   tofstream m_logFile;
};

#endif // USER_TRACKER_SERVICE_H_
