#include "inc/sleep.h"

void mySleep(unsigned long ms)
{
    QTime _timer = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < _timer)
    {
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    }
}
