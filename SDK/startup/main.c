/***************************************************************************//**
  @file     main.c
  @brief    FW main
  @author   Nicolás Magliola
 ******************************************************************************/

#include "hardware.h"
#include "taskHandler.h"

int main (void)
{
    hw_Init();
    hw_DisableInterrupts();
    tasksInit();
}
