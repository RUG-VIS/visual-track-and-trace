#ifndef TIMERCALLBACKCOMMAND_H
#define TIMERCALLBACKCOMMAND_H

#include <vtkCallbackCommand.h>
#include "../helperClasses/Program.h"

class TimerCallbackCommand : public vtkCallbackCommand {
public:
  TimerCallbackCommand();
  static TimerCallbackCommand* New(Program *program);
  void Execute(vtkObject* caller, unsigned long eventId, void* vtkNotUsed(callData)) override;

  void setDefaults();


private:
  int time;
  int dt;
  int maxTime;
};

#endif
