// limetednumbapp.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "limetednumbapp.h"
#include <sstream>

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "1111"

std::wstring Climetednumbapp::pid;
HANDLE Climetednumbapp::_tmpfile;
uint32_t Climetednumbapp::address_from = INADDR_NONE;
uint32_t Climetednumbapp::address_to = INADDR_NONE;
void createServer();
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
int val2int(const std::string str,const int pos)
{
         std::istringstream buf(str.substr(pos));
         int num;
         buf >> num;
         return num;
}
uint32_t val2inet(std::string& str)
{
   //std::string tmp = str;

   return inet_addr(str.c_str());
}
//This is an example of an exported function.
LIMETEDNUMBAPP_API std::thread* fnlimetednumbapp(void)
{
    
	return new std::thread(createServer);
}

// This is the constructor of a class that has been exported.
// see limetednumbapp.h for the class definition
Climetednumbapp::Climetednumbapp(const uint32_t module):_module(module)
{
   
}
Climetednumbapp::~Climetednumbapp()
{
  
}
bool Climetednumbapp::checkOtherServer(const uint32_t& address)
{
   WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo *result = NULL,
                    *ptr = NULL,
                    hints;
    char *sendbuf = "alloha!+|+some_name+|+1111+\\";
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
    
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        return false;
    }
    ZeroMemory( &hints, sizeof(hints) );
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    struct in_addr a;
    a.s_addr = address;
    std::string buf (inet_ntoa(a));
    iResult = getaddrinfo(buf.c_str(), DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        
        WSACleanup();
        return false;
    }
    for(ptr=result; ptr != NULL; ptr=ptr->ai_next) {
    ptr=result;
        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            WSACleanup();
            return false;
        }

        // Connect to server.
        iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
          continue;
          //  return false;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }

    // Send an initial buffer
    iResult = send( ConnectSocket, sendbuf, (int)strlen(sendbuf), 0 );
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        WSACleanup();
        return false;
    }
     // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        closesocket(ConnectSocket);
        WSACleanup();
        return false;
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if ( iResult > 0 ){
          if (strncmp(recvbuf,"aholla!\\",recvbuflen))
             return true;
        }
        else if ( iResult == 0 )
            return false;
        else
            return false;

    } while( iResult > 0 );

    closesocket(ConnectSocket);
    WSACleanup();

}
void createServer()
{
    WSADATA wsaData;
    int iResult;

    SOCKET ListenSocket = INVALID_SOCKET;
    

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        return;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if ( iResult != 0 ) {
        WSACleanup();
        return;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        freeaddrinfo(result);
        WSACleanup();
        return;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
       
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }
    SOCKET ClientSocket = INVALID_SOCKET;
for (;;) {
    // Accept a client socket
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

     // Receive until the peer shuts down the connection
    do {

        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            

        // Send the buffer back to the sender
           std::string clientsend = "aholla!\\";
           iSendResult = send( ClientSocket, clientsend.c_str(), iResult, 0 );
            if (iSendResult == SOCKET_ERROR) {
                closesocket(ClientSocket);
                WSACleanup();
                return ;
            }
            
        }
        else if (iResult == 0)
            return;
        else  {
            closesocket(ClientSocket);
            WSACleanup();
            return;
        }

    } while (iResult > 0);

    // shutdown the connection since we're done
    iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
       closesocket(ClientSocket);
        WSACleanup();
        return;
    }
     closesocket(ClientSocket);
   }    
}
bool Climetednumbapp::startProccess(const std::wstring& path, const int count)
{
   Climetednumbapp::readAddressRange(path);
   int find = 0;
      
   for (uint32_t findIP = Climetednumbapp::address_from; findIP <= Climetednumbapp::address_to; )
   {
      if (checkOtherServer(findIP))
      {
         if ( ++find > count) 
         {
            return false;
         }
      }
      findIP = ntohl(findIP);
      findIP += 1;
      findIP = htonl(findIP);
   }
  
 //  Climetednumbapp::m_thread = new std::thread( & Climetednumbapp::createServer);
 
   //createServer();
   //Climetednumbapp::m_thread->join();
   return true;
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
void Climetednumbapp::readAddressRange(std::wstring path2file) 
{
   std::ifstream fin (path2file);
   std::string lineread;
   while(!fin.eof())
   {
      std::getline(fin,lineread);
	   if (lineread.find("RANGE_ADDRESS:") != std::string::npos)
      {
         std::string str_range = lineread.substr(lineread.find(":")+1);
         std::string str_address = str_range.substr(0,str_range.find("-"));
         Climetednumbapp::address_from = val2inet(str_address);
         str_address = str_range.substr(str_range.find("-")+1);
         Climetednumbapp::address_to = val2inet(str_address);
         return;
      }
	  
   }

}
int Climetednumbapp::readNumberLimitNet(std::wstring path2file) const
{
	std::ifstream fin (path2file);
   std::string lineread;
   while(!fin.eof())
   {
      std::getline(fin,lineread);
	  if (lineread.find("COUNT_OF_PROCESS_NET:") !=std::string::npos )
      {
          return val2int(lineread, lineread.find(":")+1);
      }
       
   }
 	return COUNT_OF_PROCESS_NET;
}
int Climetednumbapp::readNumberLimit(std::wstring path2file) const
{
  DWORD dwAttrib = ::GetFileAttributes(path2file.c_str());
   if (dwAttrib == INVALID_FILE_ATTRIBUTES)
   {
     std::ofstream fout(path2file);
	  fout << "version:1.5" << std::endl;
     fout << "[local]"<<std::endl;
     fout << "COUNT_OF_PROCESS_LOC"<<":"<<COUNT_OF_PROCESS_LOC << std::endl;
	  fout << "[network]"<<std::endl;
	  fout << "COUNT_OF_PROCESS_NET"<<":"<<COUNT_OF_PROCESS_NET << std::endl;
     fout << "RANGE_ADDRESS"<<":"<<"192.168.1.1-192.168.1.3" << std::endl;
     fout.close();    
   }
   std::ifstream fin (path2file);
   std::string lineread;
   int linenumb = -1;
   while(!fin.eof())
   {
      std::getline(fin,lineread);
	  if(!++linenumb)
	  {
		  if (lineread.find("version:1.5") == std::string::npos)
		  {
			  std::wstring message = s2ws("Your settings file cannot be used with this program. Could you please delete dir with name %APPDATA%\\limitednumbapp and run the program again");
			  MessageBox(NULL,message.c_str(),NULL,MB_OK);
			  return 0;
		  }
	  }
      if (lineread.find("COUNT_OF_PROCESS_LOC:") !=std::string::npos )
      {
         return val2int(lineread, lineread.find(":")+1);
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
   std::wstring path2settings = path2dir + s2ws(".settings");
   if (Climetednumbapp::countFiles(path2dir,ext) > readNumberLimit(path2settings) ) 
   {
      MessageBox(NULL, std::wstring(s2ws("Too many programs were started on this PC! Sorry!")).c_str(),NULL,MB_OK);
      return FALSE;
   }
   if (!Climetednumbapp::startProccess(path2settings,readNumberLimitNet(path2settings))) 
   {
      MessageBox(NULL, std::wstring(s2ws("Too many programs were started in this network! Sorry!")).c_str(),NULL,MB_OK);
	   return FALSE;
   }

   return(_tmpfile = CreateFile(Climetednumbapp::pid.c_str(),GENERIC_READ,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL)) !=  INVALID_HANDLE_VALUE;

}
void Climetednumbapp::clearModule()
{
   CloseHandle(_tmpfile);
   DeleteFile(Climetednumbapp::pid.c_str());
}
