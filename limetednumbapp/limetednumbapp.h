#ifndef LIMETEDNUMBAPP_H_STOP
#define LIMETEDNUMBAPP_H_STOP
#ifdef LIMETEDNUMBAPP_EXPORTS
#define LIMETEDNUMBAPP_API __declspec(dllexport)
#else
#define LIMETEDNUMBAPP_API __declspec(dllimport)
#endif

#define COUNT_OF_PROCESS_LOC 4
#define COUNT_OF_PROCESS_NET 4
#define EXT_FILE             ".pid"

#include <iostream>
#include <string>
#include <stdint.h>
#include <fstream>

// This class is exported from the limetednumbapp.dll
class LIMETEDNUMBAPP_API Climetednumbapp {
    const uint32_t _module;
    static HANDLE _tmpfile;
    static std::wstring pid; 
    static int countFiles(const std::wstring& path, const std::wstring& ext);
	static int countProccess();
    int readNumberLimit(std::wstring path2file);
	int readNumberLimitNet(std::wstring path2file);
public:
   Climetednumbapp(const uint32_t module);
  static void clearModule();
	// TODO: add your methods here.
   BOOL checkCanWeRun();
   ~Climetednumbapp(); 
};

extern LIMETEDNUMBAPP_API int nlimetednumbapp;

LIMETEDNUMBAPP_API int fnlimetednumbapp(void);
#endif