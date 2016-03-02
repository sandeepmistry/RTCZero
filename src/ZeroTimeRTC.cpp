/*
  RTC library for Arduino Zero.
  Copyright (c) 2015 Arduino LLC. All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "ZeroTimeRTC.h"

ZeroTimeRTC zeroRTC;
RTCAPI* rtcAPI= &zeroRTC;

ZeroTimeRTC::ZeroTimeRTC() :
    RTCAPI(),
    isSetup(false)
{
}

unsigned long ZeroTimeRTC::getTime()
{
    if (!isSetup) {
      setup();
    }

    return RTC->MODE0.COUNT.reg;
}

void ZeroTimeRTC::setTime(unsigned long t)
{
    if (!isSetup) {
      setup();
    }

    RTC->MODE0.COUNT.reg = t;
    while (RTC->MODE0.STATUS.bit.SYNCBUSY);
}

void ZeroTimeRTC::setup()
{  
    PM->APBAMASK.reg |= PM_APBAMASK_RTC; // turn on digital interface clock

    SYSCTRL->XOSC32K.reg = SYSCTRL_XOSC32K_ONDEMAND |
                           SYSCTRL_XOSC32K_RUNSTDBY |
                           SYSCTRL_XOSC32K_EN32K |
                           SYSCTRL_XOSC32K_XTALEN |
                           SYSCTRL_XOSC32K_STARTUP(6) |
                           SYSCTRL_XOSC32K_ENABLE;

    // Setup clock GCLK2 with OSC32K divided by 32
    GCLK->GENDIV.reg = GCLK_GENDIV_ID(2)|GCLK_GENDIV_DIV(4);
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

    GCLK->GENCTRL.reg = (GCLK_GENCTRL_GENEN | GCLK_GENCTRL_SRC_XOSC32K | GCLK_GENCTRL_ID(2) | GCLK_GENCTRL_DIVSEL );
    while (GCLK->STATUS.reg & GCLK_STATUS_SYNCBUSY);

    GCLK->CLKCTRL.reg = (uint32_t)((GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK2 | (RTC_GCLK_ID << GCLK_CLKCTRL_ID_Pos)));
    while (GCLK->STATUS.bit.SYNCBUSY);

    RTC->MODE0.CTRL.reg &= ~RTC_MODE0_CTRL_ENABLE; // disable RTC
    while (RTC->MODE0.STATUS.bit.SYNCBUSY);

    RTC->MODE0.CTRL.reg |= RTC_MODE0_CTRL_SWRST; // software reset
    while (RTC->MODE0.STATUS.bit.SYNCBUSY);

    RTC->MODE0.CTRL.reg = RTC_MODE0_CTRL_MODE_COUNT32 | RTC_MODE0_CTRL_PRESCALER_DIV1024 | RTC_MODE0_CTRL_ENABLE;
    while (RTC->MODE0.STATUS.bit.SYNCBUSY);

    isSetup = true;
}
