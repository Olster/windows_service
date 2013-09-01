#ifndef LOGGER_SERVICE_H_
#define LOGGER_SERVICE_H_

#include "service.h"

class LoggerService final : public Service {
public:
  LoggerService();
  ~LoggerService();
protected:
  void OnStart(DWORD argc, TCHAR* argv[]) override;
  void OnStop() override;
private:
  HANDLE m_hStopHandle;
};

#endif // LOGGER_SERVICE_H_
