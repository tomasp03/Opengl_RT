#include "TimerClass.h"
#include <string>

TimerClass::TimerClass(double* Fdt)
{
	dt = Fdt;
	time0 = glfwGetTime();
}

TimerClass::~TimerClass()
{
	time = glfwGetTime();
	*dt = time - time0;
}

