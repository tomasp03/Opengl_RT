#pragma once
#include<GLFW/glfw3.h>
#include<iostream>

class TimerClass
{
	double* dt;
	double time0, time;
public:
	TimerClass(double* Fdt);
	~TimerClass();
};

