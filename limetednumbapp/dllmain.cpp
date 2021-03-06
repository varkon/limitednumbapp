// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "limetednumbapp.h"
#include <string>



BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
   
   if (ul_reason_for_call == DLL_PROCESS_ATTACH)
   {
      Climetednumbapp app(GetCurrentProcessId());
      return app.checkCanWeRun();
   }
   if (ul_reason_for_call == DLL_PROCESS_DETACH)
      Climetednumbapp::clearModule();
	
   return TRUE;
}

