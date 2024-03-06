#include "inc/sleep.h"

void Zy::mSleep(unsigned long ms)
{
    QTime _timer = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < _timer) {
        QCoreApplication::processEvents(QEventLoop::AllEvents);
    }
}
