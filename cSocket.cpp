// cSocket.cpp: implementation of the cSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cSocket.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



cSocket::cSocket(const HWND hmain)
{
 s1 = NULL;
 s2 = NULL;
 s3 = NULL;
 hWnd = hmain;
 InitWinsock();
 gASYNCEVENT = 0;
 data_size = 0;
 recv_done = false;
}



cSocket::~cSocket()
{
 WSACleanup(); 
}



bool cSocket::send_my_BasicPacket(const bool errors,char *data_to_send,unsigned const long data_len,const string &to,unsigned const int port)
{
  LPHOSTENT lpHostEntry = NULL;
  lpHostEntry = gethostbyname((char*)to.c_str());
  if(lpHostEntry == NULL)
  {
   if(errors == true)
   {
    DisplayLastErrorWSA();
   }
   return false;
  }
 
  //Initialize socket
  SOCKET sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(sock == INVALID_SOCKET)
  {
   if(errors == true)
   { 
    DisplayLastErrorWSA();
   }
   return false;
  }

  //fill server struct client needs this data......
  SOCKADDR_IN saServer;
  saServer.sin_family = AF_INET;
  saServer.sin_addr   = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
  saServer.sin_port   = htons(port); 
  
  //connect to socket
  int connect_ret = connect(sock,(LPSOCKADDR)&saServer,sizeof(saServer));
  if(connect_ret == SOCKET_ERROR)
  {
   if(errors == true)
   {
    DisplayLastErrorWSA();
   }
  return false;
  }

//  myBasicPacket *packet = new myBasicPacket(data_len,data_to_send,"qqqqrwa"); 

  //Send data to server
//  int bytes_to_sent = (int)packet->packetsize;
//  int ret = 0;
//  int total_bytes_send = 0;
//  do{
//  ret = send(sock,reinterpret_cast<char*>(packet) + total_bytes_send,data_len,0);
//  total_bytes_send = total_bytes_send + ret;
//   if(ret == SOCKET_ERROR)
//   {
//    if(errors == true)
//    {
//	 DisplayLastErrorWSA();
//    }
//	delete packet;
//	packet = NULL;
//   return false;
//   }
//  }while(total_bytes_send != (int)packet->packetsize);//do-loop
// if(lpHostEntry != NULL) closesocket(sock);
// delete packet;
// packet = NULL;
return true;
}



bool cSocket::send_raw_data(const bool errors,char *data_to_send,unsigned const long data_len,const string &to,unsigned const int port)
{
  LPHOSTENT lpHostEntry = NULL;
  lpHostEntry = gethostbyname((char*)to.c_str());
  if(lpHostEntry == NULL)
  {
   if(errors == true)
   {
    DisplayLastErrorWSA();
   }
   return false;
  }
 
  //Initialize socket
  SOCKET sock;
  sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(sock == INVALID_SOCKET)
  {
   if(errors == true)
   { 
    DisplayLastErrorWSA();
   }
   return false;
  }

  //fill server struct client needs this data......
  SOCKADDR_IN saServer;
  saServer.sin_family = AF_INET;
  saServer.sin_addr   = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
  saServer.sin_port   = htons(port); 
  
  //connect to socket
  int connect_ret = connect(sock,(LPSOCKADDR)&saServer,sizeof(saServer));
  if(connect_ret == SOCKET_ERROR)
  {
   if(errors == true)
   {
    DisplayLastErrorWSA();
   }
  return false;
  }
 
  //Send data to server
  int bytes_to_sent = (int)data_len;
  int ret = 0;
  int total_bytes_send = 0;
  do
  {
  ret = send(sock,data_to_send + total_bytes_send,data_len,0);
  total_bytes_send = total_bytes_send + ret;
   if(ret == SOCKET_ERROR)
   {
    if(errors == true)
    {
	 DisplayLastErrorWSA();
    }
   return false;
   }
  }while(total_bytes_send != data_len);//do-loop
 if(lpHostEntry != NULL) closesocket(sock);
return true;
}



void cSocket::DisplayLastErrorWSA()
{
  LPVOID lpMsgBuf;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                WSAGetLastError(),
                MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf,
                0,
                NULL);
  MessageBox(NULL,(LPCTSTR)lpMsgBuf,"Error!",MB_OK|MB_ICONINFORMATION);
  LocalFree( lpMsgBuf );
return;
}



bool cSocket::AsyncServerRun(SOCKET &s,const int port,int ASYNCEVENT,long lEvent,HWND thisHwnd)
{
 
 gASYNCEVENT = ASYNCEVENT; 
 
 s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
 if (s == INVALID_SOCKET)
 {
  DisplayLastErrorWSA(); 
  return false;
 }

 SOCKADDR_IN servdata;
 servdata.sin_family = AF_INET;
 servdata.sin_addr.s_addr = INADDR_ANY;
 servdata.sin_port = htons(port); //port     
  
 int nRet = bind(s,(LPSOCKADDR)&servdata,sizeof(struct sockaddr));
 if (nRet == SOCKET_ERROR)
 {
  DisplayLastErrorWSA(); 
 return false;
 }

 if(WSAAsyncSelect(s,thisHwnd,ASYNCEVENT,lEvent) != 0)
 {
  DisplayLastErrorWSA(); 
 return false;
 } 
    
 if(listen(s,SOMAXCONN) == SOCKET_ERROR)
 {
  DisplayLastErrorWSA(); 
  return false;
 } 
return true;	
}



bool cSocket::InitWinsock()
{
   WORD wVersionRequested = MAKEWORD(1,1);
    int  nRet;
    nRet = WSAStartup(wVersionRequested,&WinsockData);
    if (WinsockData.wVersion != wVersionRequested)
    {   
     return false;
    }
return true;
}



int cSocket::doNetEnumServerInDomain( int level, NETRESOURCE *pnr, vector<string> &hosts )
{
	DWORD rc, rc2;
	HANDLE hEnum;
	DWORD count, bufsize, ui;
	NETRESOURCE buf[200];
	const char *cont;

	rc = WNetOpenEnum( RESOURCE_GLOBALNET, RESOURCETYPE_ANY, 0, pnr, &hEnum );
	if ( rc == ERROR_ACCESS_DENIED )
	{
    	return 1;
	}

	
	while ( 1 )
	{
		count = (DWORD) -1L;
		bufsize = sizeof buf;
		rc = WNetEnumResource( hEnum, &count, buf, &bufsize );
		if ( rc != NO_ERROR ) break;
		for ( ui = 0; ui < count; ++ ui )
		{
			switch ( buf[ui].dwDisplayType )
			{
				//Domain
			    case RESOURCEDISPLAYTYPE_DOMAIN:
				break;

				//Generic	
				case RESOURCEDISPLAYTYPE_GENERIC:
				break;

                //Server - hostname
				case RESOURCEDISPLAYTYPE_SERVER:
					 hosts.push_back (buf[ui].lpRemoteName);
				break;

                //Share
				case RESOURCEDISPLAYTYPE_SHARE:
				break;

                //Unknown type
				default:
				break;
			}

			cont = ( buf[ui].dwUsage & RESOURCEUSAGE_CONTAINER )? "container": "";
			
			// now we recurse if it's a container
			if ( buf[ui].dwUsage & RESOURCEUSAGE_CONTAINER )
			{
				doNetEnumServerInDomain( level + 1, &buf[ui], hosts );
			}
		}
	}

	if ( rc != ERROR_NO_MORE_ITEMS ) // bad things
	{
	
	}

	WNetCloseEnum( hEnum );
	return 1;
}



int cSocket::doNetEnumDomain( int level, NETRESOURCE *pnr, vector<string> &domains  )
{
	DWORD rc, rc2;
	HANDLE hEnum;
	DWORD count, bufsize, ui;
	NETRESOURCE buf[200];
	const char *cont;

	rc = WNetOpenEnum( RESOURCE_GLOBALNET, RESOURCETYPE_ANY, 0, pnr, &hEnum );
	if ( rc == ERROR_ACCESS_DENIED )
	{
    	return 1;
	}
	
	while ( 1 )
	{
		count = (DWORD) -1L;
		bufsize = sizeof buf;
		rc = WNetEnumResource( hEnum, &count, buf, &bufsize );
		if ( rc != NO_ERROR ) break;
		for ( ui = 0; ui < count; ++ ui )
		{
			switch ( buf[ui].dwDisplayType )
			{
				//Domain
			    case RESOURCEDISPLAYTYPE_DOMAIN:
					 domains.push_back(buf[ui].lpRemoteName);
				break;

				//Generic	
				case RESOURCEDISPLAYTYPE_GENERIC:
				break;

                //Server - hostname
				case RESOURCEDISPLAYTYPE_SERVER:
				break;

                //Share
				case RESOURCEDISPLAYTYPE_SHARE:
				break;

                //Unknown type
				default:
				break;
			}

			cont = ( buf[ui].dwUsage & RESOURCEUSAGE_CONTAINER )? "container": "";
			
			// now we recurse if it's a container
			if ( buf[ui].dwUsage & RESOURCEUSAGE_CONTAINER )
			{
			doNetEnumDomain( level + 1, &buf[ui],domains );
			}
		}
	}

	if ( rc != ERROR_NO_MORE_ITEMS ) // bad things
	{
	
	}

	WNetCloseEnum( hEnum );
	return 1;
}



void cSocket::cpy_recv_data(char *buff)
{
 memcpy(buff,data,data_size);
 data_size = 0;
 recv_done = false;
 delete []data;
 data = NULL; 
return;
}



bool cSocket::PackDataPacket(dataPacket *dpacket)
{

return true;
}



bool cSocket::PackDataPacket(dataPacket *dpacket)
{


return true;
}



unsigned long cSocket::ret_recv_data_size()
{
 return data_size;
}



void cSocket::LastError()
{
  LPVOID lpMsgBuf;
  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|
                FORMAT_MESSAGE_FROM_SYSTEM|
                FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                GetLastError(),
                MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
                (LPTSTR)&lpMsgBuf,
                0,
                NULL);
  MessageBox(NULL,(LPCTSTR)lpMsgBuf,"Error!",MB_OK|MB_ICONINFORMATION);
  LocalFree(lpMsgBuf);
return;
}



bool cSocket::myconnect(SOCKET &s,const bool errors,string &to,const int port) 
{
  LPHOSTENT lpHostEntry = NULL;
  lpHostEntry = gethostbyname((char*)to.c_str());
  if(lpHostEntry == NULL)
  {
   if(errors == true)
   {
    DisplayLastErrorWSA();
   }
  return false;
  }
 
  //Initialize socket
  s = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(s == INVALID_SOCKET)
  {
   if(errors == true)
   {
    DisplayLastErrorWSA();
   }
  return false;
  }

  //fill server struct client needs this data......
  SOCKADDR_IN saServer;
  saServer.sin_family = AF_INET;
  saServer.sin_addr   = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
  saServer.sin_port   = htons(port); 
  
  //connect to socket
  int connect_ret = connect(s,(LPSOCKADDR)&saServer,sizeof(saServer));
  if(connect_ret == SOCKET_ERROR)
  {
   if(errors == true)
   {
    DisplayLastErrorWSA();
   }
  return false;
  }
return true;
}



void cSocket::dos(const bool errors,string &to,const int port,const int count) 
{
  LPHOSTENT lpHostEntry = NULL;
  lpHostEntry = gethostbyname((char*)to.c_str());
  if(lpHostEntry == NULL)
  {
   if(errors == true)
   {
    DisplayLastErrorWSA();
   }
  return;
  }
 
  //Initialize socket
  SOCKET sock;
  sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(sock == INVALID_SOCKET)
  {
   if(errors == true)
   {
    DisplayLastErrorWSA();
   }
  return;
  }

  //fill server struct client needs this data......
  SOCKADDR_IN saServer;
  saServer.sin_family = AF_INET;
  saServer.sin_addr   = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
  saServer.sin_port   = htons(port); 
  
  //connect to socket
  int loop;
  for(loop = 0;loop < count;loop++)
  {
   Sleep(500);
   int connect_ret = connect(sock,(LPSOCKADDR)&saServer,sizeof(saServer));
   if(connect_ret == SOCKET_ERROR)
   {
    if(errors == true)
    {
     DisplayLastErrorWSA();
    }
   return;
   }
  }
return;
}



void cSocket::send_text_message(const bool errors,const bool crpt,const bool log,string &msg,string &from,string &to,string &note,string &prefix,string &msgfilename,const int port)
{
  msg.append(prefix);
  msg.append(note);
  if(log == true) LogMessage((char*)msg.c_str(),msg.length(),msgfilename);
  if(crpt == true) crypt((char*)msg.c_str(),msg.length());
 	
  SOCKET sock;
  
  LPHOSTENT lpHostEntry = NULL;
  lpHostEntry = gethostbyname((char*)to.c_str());
  if(lpHostEntry == NULL)
  {
   if(errors == true)
   {
    DisplayLastErrorWSA();
   }
   return;
  }
  
 
  //Initialize socket
  sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
  if(sock == INVALID_SOCKET)
  {
   if(errors == true)
   { 
    DisplayLastErrorWSA();
   }
   return;
  }

  //fill server struct client needs this data......
  SOCKADDR_IN saServer;
  saServer.sin_family = AF_INET;
  saServer.sin_addr   = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
  saServer.sin_port   = htons(port); 
  
  //connect to socket
  int connect_ret = connect(sock,(LPSOCKADDR)&saServer,sizeof(saServer));
  if(connect_ret == SOCKET_ERROR)
  {
   if(errors == true)
   {
    DisplayLastErrorWSA();
   }
  return;
  }
  
  //Send data to server
  int bytes_to_sent = msg.length();
  int ret = 0;
  int total_bytes_send = 0;
  do{
  ret = send(sock,(char*)msg.c_str() + total_bytes_send,msg.length(),0);
  total_bytes_send = total_bytes_send + ret;
   if(ret == SOCKET_ERROR)
   {
    if(errors == true)
    {
	 DisplayLastErrorWSA();
    }
   return;
   }
  }while(total_bytes_send != msg.length());//do-loop
 if(lpHostEntry != NULL) closesocket(sock);
return;
}


//---------------------------------------------------
/*                                                 */
/*           prc_ProcessAsyncSelectMessage         */
/*                                                 */
//---------------------------------------------------   
void cSocket::prc_ProcessAsyncSelectMessage(SOCKET &server,SOCKET &saccepted_connection,WPARAM wParam, LPARAM lParam,HWND thishWnd)
{

 switch(WSAGETSELECTERROR(lParam))
 {		
 }
       
 switch(WSAGETSELECTEVENT(lParam)) 
 {
  //ACCEPT CONNECTION
  case FD_ACCEPT:
       if(wParam == server)
       {
        //MessageBox(NULL,"prc_ProcessAsyncSelectMessage FD_ACCEPT","Error!",MB_OK|MB_ICONINFORMATION);
 		saccepted_connection = WSAAccept(server,NULL,(LPINT)NULL,(LPCONDITIONPROC)NULL,(DWORD)NULL);
        if(saccepted_connection == INVALID_SOCKET)
        {  
         DisplayLastErrorWSA();
        return;
        }
         WSAAsyncSelect(saccepted_connection,thishWnd,gASYNCEVENT,FD_READ);
       }         
  break;
  
  //READ DATA
  case FD_READ:
       if(wParam == saccepted_connection)
       {
        //MessageBox(NULL,"prc_ProcessAsyncSelectMessage FD_READ","FD_READ!",MB_OK|MB_ICONINFORMATION);
        if(recv_done == false)
		{
		 to_buffer(saccepted_connection);
		}
       }
  break;
      
  //CLOSE CONNECTION
  case FD_CLOSE:
       if(wParam == server)
       {
        WSAAsyncSelect(server,thishWnd,gASYNCEVENT,FD_ACCEPT);
       } 
   break;
 }
return; 
}


void cSocket::crypt(TCHAR *inp, DWORD inplen, TCHAR* key, DWORD keylen)
{
    keylen = 0;
	key = "";
	TCHAR Sbox[257], Sbox2[257];
    unsigned long i, j, t, x;
    static const TCHAR  OurUnSecuredKey[] = "q6n9*!<titrimCezary:gJ;1@8&saqwerp[!]o-->>cezaryLupa-kurwa-huj-dupa-pizda" ;
    static const int OurKeyLen = lstrlen(OurUnSecuredKey);    
    TCHAR temp , k;
    i = j = k = t =  x = 0;
    temp = 0;
    ZeroMemory(Sbox, sizeof(Sbox));
    ZeroMemory(Sbox2, sizeof(Sbox2));
   for(i = 0; i < 256U; i++)
    {
        Sbox[i] = (TCHAR)i;
    }
    j = 0;
    if(keylen)
    {
        for(i = 0; i < 256U ; i++)
        {
            if(j == keylen)
            {
                j = 0;
            }
            Sbox2[i] = key[j++];
        }    
    }
    else
    {
        for(i = 0; i < 256U ; i++)
        {
            if(j == OurKeyLen)
            {
                j = 0;
            }
            Sbox2[i] = OurUnSecuredKey[j++];
        }
    }

    j = 0;
    for(i = 0; i < 256; i++)
    {
        j = (j + (unsigned long) Sbox[i] + (unsigned long) Sbox2[i]) % 256U ;
        temp =  Sbox[i];                    
        Sbox[i] = Sbox[j];
        Sbox[j] =  temp;
    }

   i = j = 0;
    for(x = 0; x < inplen; x++)
    {
        i = (i + 1U) % 256U;
         j = (j + (unsigned long) Sbox[i]) % 256U;
        temp = Sbox[i];
        Sbox[i] = Sbox[j] ;
        Sbox[j] = temp;
        t = ((unsigned long) Sbox[i] + (unsigned long) Sbox[j]) %  256U ;
        k = Sbox[t];
        inp[x] = (inp[x] ^ k);
    }    
}



void cSocket::LogMessage(char* msg,long msglen,string &filename)
{
 char pathbuf[100];
 string exename;
 ZeroMemory(pathbuf,sizeof(pathbuf));
 GetModuleFileName(NULL,pathbuf,sizeof(pathbuf));
 string s_path_buf = pathbuf;
 string slash = "\\";
 int spos = (s_path_buf.find_last_of(slash)+1);
 exename = s_path_buf.substr(spos); 
 string file_and_path = s_path_buf.substr(0,(s_path_buf.length() - exename.length()));
   
 HANDLE file;
 file_and_path.append(filename); 
 file = CreateFile((char*)file_and_path.c_str(),GENERIC_READ|GENERIC_WRITE,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
 if(file == INVALID_HANDLE_VALUE)
 {
  file = CreateFile((char*)file_and_path.c_str(),GENERIC_READ|GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
   if(file == INVALID_HANDLE_VALUE)
   {
    LastError();
   return;
   }
 }
 SetFilePointer(file,0,0,FILE_END);
 DWORD dwUseless;
 WriteFile(file,msg,msglen,&dwUseless,NULL);
 CloseHandle(file);
}



void cSocket::to_buffer(SOCKET &s)
{
 //MAX RECV DATA SIZE 204800 b = 200 Kb
 //BUFF 215040 = 210 Kb
 data  = new char[215040]; 
 ZeroMemory(data,215040);
 int ret;
 unsigned long dsize = 0;
 do
 {
  Sleep(200);
  ret = recv(s,data,204800,0);
  dsize = dsize + (unsigned long)ret;
   if(ret == SOCKET_ERROR)
   {
	DisplayLastErrorWSA();
	delete []data;
	data = NULL;
   return;
   } 
  }while(ret != 0);
 data_size = dsize;
 recv_done = true;
return;
}














//bool cSocket::send_my_BasicPacket(const bool errors,char *data_to_send,const long data_len,const string &to,const int port)
//{
//  LPHOSTENT lpHostEntry = NULL;
//  lpHostEntry = gethostbyname((char*)to.c_str());
//  if(lpHostEntry == NULL)
//  {
//   if(errors == true)
//   {
//    DisplayLastErrorWSA();
//   }
//   return false;
//  }
// 
//  //Initialize socket
//  SOCKET sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
//  if(sock == INVALID_SOCKET)
//  {
//   if(errors == true)
//   { 
//    DisplayLastErrorWSA();
//   }
//   return false;
//  }
//
//  //fill server struct client needs this data......
//  SOCKADDR_IN saServer;
//  saServer.sin_family = AF_INET;
//  saServer.sin_addr   = *((LPIN_ADDR)*lpHostEntry->h_addr_list);
//  saServer.sin_port   = htons(port); 
//  
//  //connect to socket
//  int connect_ret = connect(sock,(LPSOCKADDR)&saServer,sizeof(saServer));
//  if(connect_ret == SOCKET_ERROR)
//  {
//   if(errors == true)
//   {
//    DisplayLastErrorWSA();
//   }
//  return false;
//  }
//
//  myBasicPacket *packet = new myBasicPacket(data_len,data_to_send,"qqqqrwa"); 
//
//  //Send data to server
//  int bytes_to_sent = (int)packet->packetsize;
//  int ret = 0;
//  int total_bytes_send = 0;
//  do{
//  ret = send(sock,reinterpret_cast<char*>(packet) + total_bytes_send,data_len,0);
//  total_bytes_send = total_bytes_send + ret;
//   if(ret == SOCKET_ERROR)
//   {
//    if(errors == true)
//    {
//	 DisplayLastErrorWSA();
//    }
//	delete packet;
//   return false;
//   }
//  }while(total_bytes_send != (int)packet->packetsize);//do-loop
// if(lpHostEntry != NULL) closesocket(sock);
// delete packet;
//return true;
//}
//
//
//
//
//
//
//  //struct bytes
//  //{
//  // unsigned char b:8;
//  //};
//  
