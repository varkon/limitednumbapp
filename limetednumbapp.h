
#ifdef LIMETEDNUMBAPP_EXPORTS
#define LIMETEDNUMBAPP_API __declspec(dllexport)
#else
#define LIMETEDNUMBAPP_API __declspec(dllimport)
#endif

#define COUNT_OF_PROCESS_LOC 4
#define EXT_FILE             ".pid"

#include <string>
#include <stdint.h>

// This class is exported from the limetednumbapp.dll
class LIMETEDNUMBAPP_API Climetednumbapp {
    const uint32_t _module;
    static HANDLE _tmpfile;
    static std::wstring pid; 
    static int countFiles(const std::wstring& path, const std::wstring& ext);
public:
   Climetednumbapp(const uint32_t module);
  static void clearModule();
	// TODO: add your methods here.
   BOOL checkCanWeRun();
   ~Climetednumbapp(); 
};

extern LIMETEDNUMBAPP_API int nlimetednumbapp;

LIMETEDNUMBAPP_API int fnlimetednumbapp(void);
