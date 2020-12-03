#pragma once

// Initialize system (Clock, systick & NVIC) without starting the OS
#define SYSTEM_INIT()  System::GetInstance()

// OS_START initializes the system as well
#define OS_START()     System::GetInstance().OsStart()

class System
{
public:
	static System& GetInstance();
	void OsStart();

private:
	System();
	void Clock_Config();
};
