// limetednumbapp.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "limetednumbapp.h"
#include <sstream>



std::wstring Climetednumbapp::pid;
HANDLE Climetednumbapp::_tmpfile;
// This is an example of an exported variable
LIMETEDNUMBAPP_API int nlimetednumbapp=0;
std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
// This is an example of an exported function.
LIMETEDNUMBAPP_API int fnlimetednumbapp(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see limetednumbapp.h for the class definition
Climetednumbapp::Climetednumbapp(const uint32_t module):_module(module)
{
   
}
Climetednumbapp::~Climetednumbapp()
{
  
}
int Climetednumbapp::countFiles(const std::wstring& path, const std::wstring& ext)
{
   std::wstring tmp = path +s2ws("*")+ ext;
    WIN32_FIND_DATA FindFileData;
   HANDLE hFind = FindFirstFile(tmp.c_str(), &FindFileData);
   int count = 1;
   if (hFind != INVALID_HANDLE_VALUE)
   {
      do{
         ++count;
      }while (FindNextFile(hFind,&FindFileData)!= FALSE);
		
   }
   FindClose(hFind);
   return count;

}
int Climetednumbapp::readNumberLimit(std::wstring path2file)
{
  DWORD dwAttrib = ::GetFileAttributes(path2file.c_str());
   if (dwAttrib == INVALID_FILE_ATTRIBUTES)
   {
      std::ofstream fout(path2file);
      fout << "[local]"<<std::endl;
      fout << "COUNT_OF_PROCESS_LOC"<<":"<<COUNT_OF_PROCESS_LOC << std::endl;
      fout.close();    
   }
   std::ifstream fin (path2file);
   std::string lineread;
   while(!fin.eof())
   {
      std::getline(fin,lineread);
      if (lineread.find("COUNT_OF_PROCESS_LOC:") !=std::string::npos )
      {
         int pos = lineread.find(":")+1;
         std::istringstream buf(lineread.substr(pos));
         int num;
         buf >> num;
         return num;
      }
   }
   return COUNT_OF_PROCESS_LOC;
}
BOOL Climetednumbapp::checkCanWeRun()
{
   std::wstring path(s2ws(getenv("APPDATA")));
   std::wstring path2dir = path +s2ws("\\limitednumbapp\\");
   DWORD dwAttrib = ::GetFileAttributes(path2dir.c_str());
   bool isExsists =  dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_DIRECTORY);
   if (!isExsists && !CreateDirectory(path2dir.c_str(),NULL) )
   {
      return false;
   }
          
   std::string m = std::to_string(HandleToULong(_module));
   std::wstring ext = s2ws(EXT_FILE);
   Climetednumbapp::pid = path2dir+ s2ws(m)+ext;
   if (Climetednumbapp::countFiles(path2dir,ext) > readNumberLimit(path2dir + s2ws(".settings")) ) 
   {
      return FALSE;
   }
   return(_tmpfile = CreateFile(Climetednumbapp::pid.c_str(),GENERIC_READ,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL)) !=  INVALID_HANDLE_VALUE;

}
void Climetednumbapp::clearModule()
{
   CloseHandle(_tmpfile);
   DeleteFile(Climetednumbapp::pid.c_str());
}
