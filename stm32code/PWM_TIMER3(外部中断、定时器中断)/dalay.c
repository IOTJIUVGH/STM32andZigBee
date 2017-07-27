#include "stm32f10x.h"

void Delayms(u16 ms)
{
	unsigned int i = 0;
	while (ms--)
	{
		for (i = 0; i < 8000; ++i)
		{
			;
		}
	}
}