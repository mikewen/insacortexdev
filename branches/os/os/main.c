#include "os_call.h"

void func_call(void)
{
//	_SVCALL_();
	asm("SVC 0");
}

int main (void)
{

	func_call();

	for (;;);
}
