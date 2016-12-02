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
#include <thread>

// This class is exported from the limetednumbapp.dll
class LIMETEDNUMBAPP_API Climetednumbapp {
    const uint32_t _module;
    static HANDLE _tmpfile;
    static std::wstring pid; 
    static uint32_t address_from;
    static uint32_t address_to;
    static int countFiles(const std::wstring& path, const std::wstring& ext);
	 
    static void readAddressRange(std::wstring path2file);
    static bool checkOtherServer(const uint32_t& address);
    bool startProccess(const std::wstring& path, const int count);
    int readNumberLimit(std::wstring path2file) const;
	 int readNumberLimitNet(std::wstring path2file) const;
   
    
public:
   Climetednumbapp(const uint32_t module);
  static void clearModule();
	// TODO: add your methods here.
   BOOL checkCanWeRun();
   ~Climetednumbapp(); 
};

extern LIMETEDNUMBAPP_API int nlimetednumbapp;

LIMETEDNUMBAPP_API std::thread* fnlimetednumbapp(void);
#endif