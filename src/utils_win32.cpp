#include "logger.hpp"
#include <stdio.h>
#include <conio.h>
#include <windows.h>

bool is_own_console()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    if ( !GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi ) )
    {
        printf( "GetConsoleScreenBufferInfo failed: %lu\n", GetLastError() );
        return FALSE;
    }

    // if cursor position is (0,0) then we were launched in a separate console
    return ( ( !csbi.dwCursorPosition.X ) && ( !csbi.dwCursorPosition.Y ) );
}

void wait_keyboard()
{
    while ( !_kbhit() )
    {
        Sleep( 100 );
    }
    (void)_getch();
}

bool go_realtime()
{
	/*
    LUID PrivilegeRequired;
    BOOL bRes = FALSE;
	if (!LookupPrivilegeValue(NULL, SE_INC_BASE_PRIORITY_NAME, &PrivilegeRequired))
	{
        LOG_ERROR("LookupPrivilegeValue() - failed with {}.", GetLastError());
		return false;
	} */


	 // Windows scheduling is a mess, see
	//    https://learn.microsoft.com/en-us/windows/win32/procthread/scheduling-priorities
	// First change the process to the REALTIME_PRIORITY_CLASS - the only way to allow 
	// threads to have priorities 16 and up. 
	// Then this process's threads rule all. There is NO WAY to have some real-time threads
	// and some system wide background threads in the same process.
	// UPDATE: Not true! Use: NfYtSetInformationThread()
	bool success = ( 0 != SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS) );
	DWORD err = GetLastError();


    if (!success) 
    {
        LOG_ERROR("SetPriorityClass() - failed with {}.", GetLastError());
		success = false;
    } 
	else
	{
		DWORD dwPriClass = GetPriorityClass(GetCurrentProcess());
		if (dwPriClass == REALTIME_PRIORITY_CLASS)
		{
			LOG_INFO("Process is REALTIME.")
			success = true;
		}
		else
		{
			LOG_ERROR("Process priority is actually {}.", dwPriClass);
			success = false;
		}
	}
	
	DWORD_PTR processAffinityMask = 0;
	DWORD_PTR systemAffinityMask = 0;
	if (GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &systemAffinityMask))
	{
		LOG_INFO("Affinity masks are processs={:08X}, system={:08X}.", processAffinityMask, systemAffinityMask);
	}
	else
	{
		LOG_ERROR("GetProcessAffinityMask() - failed with {}.", GetLastError());
		success = false;
	}
	// Try reserving this set so smaller distance between threads
	/*SetProcessAffinityMask(GetCurrentProcess(), 0xFF);
	if (GetProcessAffinityMask(GetCurrentProcess(), &processAffinityMask, &systemAffinityMask))
	{
		LOG_INFO("Updated affinity masks are processs={:08X}, system={:08X}.", processAffinityMask, systemAffinityMask);
	}
	else
	{
		LOG_ERROR("GetProcessAffinityMask() - failed with {}.", GetLastError());
		success = false;
	}*/

	LOG_INFO("Current thread priority is: {}", GetThreadPriority(GetCurrentThread()));
	SetThreadPriority(GetCurrentThread(), 1);
	//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL); highest
	LOG_INFO("Updated thread priority is: {}", GetThreadPriority(GetCurrentThread()));
	
	//DWORD_PTR new_mask = processAffinityMask & 0xF0;
	//DWORD_PTR old_mask = SetThreadAffinityMask(GetCurrentThread(), new_mask);
	//LOG_INFO("Thread priority mask from {:08X} to {:08X}.", old_mask, new_mask);


	// See https://stackoverflow.com/questions/2964941/not-able-to-include-ntifs-h-in-win32-project
	//NtCreateFile _NtCreateFile = (NtCreateFile)GetProcAddress(GetModuleHandle("ntdll.dll"),"NtSetInformationThread ");
	//ThreadInformation ti;
	//NtSetInformationThread(GetCurrentThread(), THREADINFOCLASS ::)
THREADINFOCLASS::
	return success;
}
