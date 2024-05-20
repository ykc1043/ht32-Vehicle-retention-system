#include "WDT.h"

void WDT_Configuration(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.WDT = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  /* Enable WDT APB clock                                                                                   */
  /* Reset WDT                                                                                              */
  WDT_DeInit();
  /* Set Prescaler Value, 32K/64 = 500 Hz                                                                   */
  WDT_SetPrescaler(WDT_PRESCALER_64);//看门狗时钟分频
  /* Set Prescaler Value, 500 Hz/1000 = 0.5 Hz                                                            */
  WDT_SetReloadValue(1000);         //看门狗定时器重装载值1000，定时2秒，计数溢出就复位（要开启WDT_ResetCmd）
  /* Set Delta Value, 500 Hz/1000 = 0.5 Hz                                                                */
  //WDT_SetDeltaValue(1000);
  WDT_Restart();                    // 重置看门狗计数值
  //#if 0
  WDT_ResetCmd(ENABLE);             // 使能看门狗计数溢出时复位单片机
  //#endif
  WDT_Cmd(ENABLE);                  // 使能看门狗
  WDT_ProtectCmd(ENABLE);           // Enable WDT Protection
}
