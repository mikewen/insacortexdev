#include "stm32f10x_type.h"
#include "config.h"
#include "kernel.h"
#include "alarm.h"

struct st_Alarm
{
	st_AlarmInfo *alarminfo;
	u32 counter;
} Alarm_List[MAX_ALARM_NBR];

/* Alarm services declaration */
AlarmType	DeclareAlarm(st_AlarmInfo *AlarmInfo)
{
	return 0;
}

StatusType	GetAlarmBase(AlarmType AlarmID, AlarmBaseRefType Info)
{
	return E_OK;
}

StatusType	GetAlarm(AlarmType AlarmID, TickRefType Tick)
{
	return E_OK;
}

StatusType	SetRelAlarm_Int(AlarmType AlarmID, TickType Increment, TickType Cycle)
{
	return E_OK;
}

StatusType	SetAbsAlarm_Int(AlarmType AlarmID, TickType Start, TickType Cycle)
{
	return E_OK;
}

StatusType	CancelAlarm_Int(AlarmType AlarmID)
{
	return E_OK;
}

void Alarm_Init(void)
{
u8 i;

	for(i=0; i<	MAX_ALARM_NBR; i++)
	{
		Alarm_List[i].alarminfo = 0x0;
	}
}
