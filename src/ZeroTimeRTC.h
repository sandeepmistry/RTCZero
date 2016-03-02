#ifndef ZERO_TIME_RTC_H
#define ZERO_TIME_RTC_H

#include "api/RTCAPI.h"

class ZeroTimeRTC : public RTCAPI
{
public:
    ZeroTimeRTC();

    virtual unsigned long getTime();
    virtual void setTime(unsigned long t);

private:
    void setup();
    bool isSetup;
};

#endif // ZERO_TIME_RTC_H
