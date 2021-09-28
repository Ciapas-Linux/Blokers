// cSocket.h: interface for the cSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSOCKET_H__45D33F30_A9D0_40A1_AB60_8D4A86E5FC1B__INCLUDED_)
#define AFX_CSOCKET_H__45D33F30_A9D0_40A1_AB60_8D4A86E5FC1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class cSocket  
{
public:
	cSocket(const HWND hmain);
	~cSocket();
	bool AsyncServerRun(SOCKET &s,const int port,int ASYNCEVENT,long lEvent,HWND thisHwnd);
	void send_text_message(const bool errors,const bool crpt,const bool log,string &msg,string &from,string &to,string &note,string &prefix,string &msgfilename,const int port);
    bool send_raw_data(const bool errors,char *data_to_send,unsigned const long data_len,const string &to,unsigned const int port);
	bool send_my_BasicPacket(const bool errors,char *data_to_send,unsigned const long data_len,const string &to,unsigned const int port);
	void dos(const bool errors,string &to,const int port,const int count); 
    void prc_ProcessAsyncSelectMessage(SOCKET &server,SOCKET &saccepted_connection,WPARAM wParam, LPARAM lParam,HWND thishWnd);
    void cpy_recv_data(char *buff);
	unsigned long ret_recv_data_size();
	int doNetEnumDomain( int level, NETRESOURCE *pnr, vector<string> &domains );
	int doNetEnumServerInDomain( int level, NETRESOURCE *pnr, vector<string> &hosts );
	bool PackDataPacket(dataPacket *dpacket);
	bool UnPackDataPacket(char *dpacket);
private:
	bool myconnect(SOCKET &s,const bool errors,string &to,const int port);
	void DisplayLastErrorWSA();
	void LastError();
    bool InitWinsock();
	void crypt(TCHAR *inp, DWORD inplen, TCHAR* key = "", DWORD keylen = 0);
    void LogMessage(char* msg,long msglen,string &filename);
   	void to_buffer(SOCKET &s);
	WSADATA WinsockData;
	HWND hWnd;

	SOCKET s1,s2,s3,s4,s5,s6;
    int gASYNCEVENT;
	bool recv_done;
	unsigned long data_size;
	char* data;
		
	typedef struct dataPacket
	{
     dataPacket()//struct constructor
	 {
      ZeroMemory(packetID,sizeof(packetID));
	  ZeroMemory(packetType,sizeof(packetType));
	  ZeroMemory(sendDateTime,sizeof(sendDateTime));
	  ZeroMemory(sendFrom,sizeof(sendFrom));
	  ZeroMemory(sendTo,sizeof(sendTo));
	  size = 0;
	  data = NULL;
	 }
	 unsigned long size;
	 char packetID[21];
	 char packetType[21]; //FILE,DATA,TEXTMESSAGE
	 char sendDateTime[21];
	 char sendFrom[101];
	 char sendTo[101];
	 char *data; 
    };
};




#endif // !defined(AFX_CSOCKET_H__45D33F30_A9D0_40A1_AB60_8D4A86E5FC1B__INCLUDED_)
