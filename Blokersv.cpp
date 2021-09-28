// Blokersv.cpp : Defines the entry point for the application.
//



#include "stdafx.h"
#include "resource.h"
#include "cSocket.h"
#include "blowfish.h"

#define MAX_LOADSTRING 100
#define WSA_SERVER_READ 1112


//Declaration of proc function
LRESULT CALLBACK    prc_MainDlgProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK    prc_SniffDlgProc(HWND l_hWnd,UINT Message,WPARAM wParam, LPARAM lParam);
void Blokers();
void idb_send_clicked();
void idb_stop_clicked();
void idb_dos_clicked();
void disable_input_controls();
void enable_input_controls();
void idb_Sniff_clicked();
void idb_Sniff_start_clicked();
void idb_Sniff_show_data();
void test();


//Global Variables
HWND hWnd = NULL;
HWND hWnd_sniff = NULL;
HINSTANCE hInst = NULL;
cSocket *mycsocket;
SOCKET server;
SOCKET saccepted_connection;


void test(string &muka)
{
// MessageBox(NULL,muka.c_str(),"Fatal error!",MB_OK);

 vector<string> hosts;
 vector<string> domains;

// mycsocket->doNetEnum(0,NULL,hosts,domains);

 for(int lop = 0;lop<domains.size();lop++)
 {
   MessageBox(NULL,(char*)domains[lop].c_str(),"DOMAINS",MB_OK);
 }
 
 for(int lop = 0;lop<hosts.size();lop++)
 {
   MessageBox(NULL,(char*)hosts[lop].c_str(),"HOSTS",MB_OK);
 }


return;
}



void idb_Sniff_clicked()
{
    //CREATE DIALOG
	hWnd_sniff = CreateDialog(hInst,MAKEINTRESOURCE(IDD_DIALOG_SNIFF),hWnd,(DLGPROC)prc_SniffDlgProc); 
    if (hWnd_sniff == NULL)
    {
     MessageBox(NULL,"Abnormal program termination!","Fatal error!",MB_OK);
     return;   
    }
return;
}



void idb_Sniff_show_data()
{
	unsigned long datasize = mycsocket->ret_recv_data_size();
	if(datasize == 0) return;
	char *data = new char[datasize+10];
	ZeroMemory(data,datasize+10);
	mycsocket->cpy_recv_data(data);
	SetWindowText(GetDlgItem(hWnd_sniff,IDC_EDIT1_SNIFF),NULL);
	SetWindowText(GetDlgItem(hWnd_sniff,IDC_EDIT1_SNIFF),(LPCTSTR)data);
	delete []data;
	data = NULL;
return;
}



void idb_Sniff_stop_clicked()
{
 closesocket(server);
 SetWindowText(GetDlgItem(hWnd_sniff,IDC_STATIC_SNIFF_STATUSTEXT),"Server now is disabled.....");
 MessageBox(hWnd_sniff,"SERVER OFF!","Information!",MB_OK);
return;
}



void idb_Sniff_start_clicked()
{
 //Get port
 char port[21];
 ZeroMemory(port,sizeof(port));
 if(GetWindowText(GetDlgItem(hWnd_sniff,IDC_EDIT2_SNIFF_PORT),port,20) == 0)
 {
  MessageBox(hWnd_sniff,"Please input port number!","Error!",MB_OK);
 return;
 }
 int iport = atoi(port);
 bool res = mycsocket->AsyncServerRun(server,iport,WSA_SERVER_READ,FD_ACCEPT,hWnd_sniff);
 if(res == false)
 {
   MessageBox(hWnd_sniff,"Serwer failed!","Information!",MB_OK);
 }
 SetWindowText(GetDlgItem(hWnd_sniff,IDC_STATIC_SNIFF_STATUSTEXT),"Server now waiting for data.....");
 MessageBox(hWnd_sniff,"SERVER ON!","Information!",MB_OK);
return;
}



//---------------------------------------------------
/*           Blokers()				               */
//---------------------------------------------------
void Blokers()
{
    //CREATE DIALOG
	hWnd = CreateDialog(hInst,MAKEINTRESOURCE(IDD_MAIN_WINDOW1),GetDesktopWindow(),(DLGPROC)prc_MainDlgProc); 
    if (hWnd == NULL)
    {
     MessageBox(NULL,"Abnormal program termination!","Fatal error!",MB_OK);
     return;   
    }
    
	//INITIALIZE CONTROLS
	SendMessage(GetDlgItem(hWnd,IDC_EDIT_DATA),(UINT)EM_SETLIMITTEXT,(WPARAM)1000,(LPARAM)0);  
    SendMessage(GetDlgItem(hWnd,IDC_EDIT_COUNT),(UINT)EM_SETLIMITTEXT,(WPARAM)9,(LPARAM)0);  
	SendMessage(GetDlgItem(hWnd,IDC_EDIT_HOST),(UINT)EM_SETLIMITTEXT,(WPARAM)50,(LPARAM)0);  
	SendMessage(GetDlgItem(hWnd,IDC_EDIT_PORT),(UINT)EM_SETLIMITTEXT,(WPARAM)5,(LPARAM)0);
	SendMessage(GetDlgItem(hWnd,IDC_EDIT_HEX),(UINT)EM_SETLIMITTEXT,(WPARAM)200,(LPARAM)0);
	SendMessage(GetDlgItem(hWnd,IDC_EDIT_BIN),(UINT)EM_SETLIMITTEXT,(WPARAM)200,(LPARAM)0);
	SendMessage(GetDlgItem(hWnd,IDC_EDIT_DEC),(UINT)EM_SETLIMITTEXT,(WPARAM)200,(LPARAM)0);
	SendMessage(GetDlgItem(hWnd,IDC_CHECK_TEXTDATA),(UINT)BM_SETCHECK,(WPARAM)BST_CHECKED,(LPARAM)0);
	EnableWindow(GetDlgItem(hWnd,IDC_EDIT_HEX),false);
	EnableWindow(GetDlgItem(hWnd,IDC_EDIT_BIN),false);
	EnableWindow(GetDlgItem(hWnd,IDC_EDIT_DEC),false);
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_BINDATA),false);
	EnableWindow(GetDlgItem(hWnd,IDC_CHECK_DECDATA),false);
	UpdateWindow(hWnd);
	//CREATE SOCKET OBJECT
	mycsocket = new cSocket(hWnd);  
return;
}



//---------------------------------------------------
/*           idb_dos_clicked()                     */
//---------------------------------------------------
void idb_dos_clicked()
{
 
return;
}



//---------------------------------------------------
/*           disable_input_controls()              */
//---------------------------------------------------
void disable_input_controls()
{
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_HOST),false);
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_COUNT),false);
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_PORT),false);
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_DATA),false);
 EnableWindow(GetDlgItem(hWnd,IDC_CHECK_LOOP),false);
 EnableWindow(GetDlgItem(hWnd,IDC_CHECK_ERRORS),false);
 EnableWindow(GetDlgItem(hWnd,IDC_CHECK_RANDOM),false);
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_HEX),false);
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_BIN),false);
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_DEC),false);
 EnableWindow(GetDlgItem(hWnd,IDC_CHECK_TEXTDATA),false);
 EnableWindow(GetDlgItem(hWnd,IDC_CHECK_BINDATA),false);
 EnableWindow(GetDlgItem(hWnd,IDC_CHECK_DECDATA),false);
 UpdateWindow(hWnd);
return;
}



//---------------------------------------------------
/*           enable_input_controls()               */
//---------------------------------------------------
void enable_input_controls()
{
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_HOST),true);
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_COUNT),true);
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_PORT),true);
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_DATA),true);
 EnableWindow(GetDlgItem(hWnd,IDC_CHECK_LOOP),true);
 EnableWindow(GetDlgItem(hWnd,IDC_CHECK_ERRORS),true);
 EnableWindow(GetDlgItem(hWnd,IDC_CHECK_RANDOM),true);
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_HEX),true);
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_BIN),true);
 EnableWindow(GetDlgItem(hWnd,IDC_EDIT_DEC),true);
 EnableWindow(GetDlgItem(hWnd,IDC_CHECK_TEXTDATA),true);
 EnableWindow(GetDlgItem(hWnd,IDC_CHECK_BINDATA),true);
 EnableWindow(GetDlgItem(hWnd,IDC_CHECK_DECDATA),true);
 UpdateWindow(hWnd);
return;
}



//---------------------------------------------------
/*           idb_send_clicked()                    */
//---------------------------------------------------
void idb_send_clicked()
{
 
 //	
 //TEXT DATA
 //
 if(SendMessage(GetDlgItem(hWnd,IDC_CHECK_TEXTDATA),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_CHECKED)
 {
  char* textbuff = new char[102400];//100Kb	 
  int textlen = GetWindowText(GetDlgItem(hWnd,IDC_EDIT_DATA),textbuff,65000);
  if( textlen == 0)
  {
   delete []textbuff;
   textbuff = NULL;
   MessageBox(hWnd,"Please input data!","Error!",MB_OK);
  return;
  }
  
  //Get host
 char host[50];
 memset(host,0,sizeof(host));
 if(GetWindowText(GetDlgItem(hWnd,IDC_EDIT_HOST),host,50) == 0)
 {
  delete []textbuff;
  textbuff = NULL;
  MessageBox(hWnd,"Please input host name!","Error!",MB_OK);
 return;
 }
 string to = host;

 //Get port
 char chPort[20];
 ZeroMemory(chPort,sizeof(chPort));
 if(GetWindowText(GetDlgItem(hWnd,IDC_EDIT_PORT),chPort,20) == 0)
 {
  delete []textbuff;
  textbuff = NULL;
  MessageBox(hWnd,"Please input port number!","Error!",MB_OK);
 return;
 }
 const unsigned int iport = atoi(chPort);
 if(iport > 65000)
 {
  delete []textbuff;
  textbuff = NULL; 
  MessageBox(hWnd,"Too big PORT number!","Warning!",MB_OK);
 return;
 }
 
 //Send data
 bool ret = mycsocket->send_raw_data(true,textbuff,textlen,to,iport);
 if(ret == false)
 {
   MessageBox(hWnd,"Send failed!","Error!",MB_OK);
 }

 delete []textbuff;
 textbuff = NULL;
 }






return;
}



//---------------------------------------------------
/*           MainDlgProc.....                      */
//---------------------------------------------------    
LRESULT CALLBACK prc_MainDlgProc(HWND l_hWnd,UINT Message,WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_INITDIALOG:
        return TRUE;

        case WM_COMMAND:
             switch(LOWORD(wParam))
             {
                case  IDEXIT:
                      DestroyWindow(l_hWnd); 
                break;
				
				case IDC_test:
                     test((string)"muka");
                break;

				case  IDC_BUTTON_SNIFF:
				 	  idb_Sniff_clicked();
				break;

                case  ID_RUN_SEND_DATA:
                      idb_send_clicked();
                break;
                
				case  IDC_BUTTON_STOP:
                        
                break;
				
				case  IDC_BUTTON_DOS:
					  idb_dos_clicked();
				break;
				
				case  IDC_CHECK_RANDOM:
					  if(SendMessage(GetDlgItem(hWnd,IDC_CHECK_RANDOM),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_CHECKED)
					  {
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_PORT),false);
					   UpdateWindow(hWnd);
					  }else
					  {
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_PORT),true);
					   UpdateWindow(hWnd);
					  }
                break;
				
				case  IDC_CHECK_LOOP:
                      if(SendMessage(GetDlgItem(hWnd,IDC_CHECK_LOOP),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_CHECKED)
					  {
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_COUNT),false);
					   UpdateWindow(hWnd);
					  }else
					  {
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_COUNT),true);
					   UpdateWindow(hWnd);
					  }
                break;

				case  IDC_CHECK_TEXTDATA:
					  if(SendMessage(GetDlgItem(hWnd,IDC_CHECK_TEXTDATA),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_CHECKED)
					  {
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_HEX),false);
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_BIN),false);
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_DEC),false);
					   EnableWindow(GetDlgItem(hWnd,IDC_CHECK_BINDATA),false);
					   EnableWindow(GetDlgItem(hWnd,IDC_CHECK_DECDATA),false);
					   UpdateWindow(hWnd);
					  }else
					  {
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_HEX),true);
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_BIN),true);
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_DEC),true);
					   EnableWindow(GetDlgItem(hWnd,IDC_CHECK_BINDATA),true);
					   EnableWindow(GetDlgItem(hWnd,IDC_CHECK_DECDATA),true);
					   UpdateWindow(hWnd);
					  }
				break;
				
				case  IDC_CHECK_BINDATA:
				      if(SendMessage(GetDlgItem(hWnd,IDC_CHECK_BINDATA),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_CHECKED)
					  {
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_HEX),false);
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_DATA),false);
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_DEC),false);
					   EnableWindow(GetDlgItem(hWnd,IDC_CHECK_TEXTDATA),false);
					   EnableWindow(GetDlgItem(hWnd,IDC_CHECK_DECDATA),false);
					   UpdateWindow(hWnd);
					  }else
					  {
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_HEX),true);
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_DATA),true);
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_DEC),true);
					   EnableWindow(GetDlgItem(hWnd,IDC_CHECK_TEXTDATA),true);
					   EnableWindow(GetDlgItem(hWnd,IDC_CHECK_DECDATA),true);
					   UpdateWindow(hWnd);
					  } 
				break;
				case IDC_CHECK_DECDATA:
				     if(SendMessage(GetDlgItem(hWnd,IDC_CHECK_DECDATA),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_CHECKED)
					  {
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_HEX),false);
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_DATA),false);
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_BIN),false);
					   EnableWindow(GetDlgItem(hWnd,IDC_CHECK_TEXTDATA),false);
					   EnableWindow(GetDlgItem(hWnd,IDC_CHECK_BINDATA),false);
					   UpdateWindow(hWnd);
					  }else
					  {
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_HEX),true);
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_DATA),true);
					   EnableWindow(GetDlgItem(hWnd,IDC_EDIT_BIN),true);
					   EnableWindow(GetDlgItem(hWnd,IDC_CHECK_TEXTDATA),true);
					   EnableWindow(GetDlgItem(hWnd,IDC_CHECK_BINDATA),true);
					   UpdateWindow(hWnd);
					  }  
	             break;
               }
        break;
	        
        case WM_PAINT:
        break;
     
        case WM_DESTROY:
             delete mycsocket;
			 mycsocket = NULL;
             PostQuitMessage(0);
        break;
        
        case WM_CLOSE:
             DestroyWindow(hWnd);
        break;
        
    }
    return FALSE;
}



//---------------------------------------------------
/*           SniffDlgProc.....                      */
//---------------------------------------------------    
LRESULT CALLBACK prc_SniffDlgProc(HWND l_hWnd,UINT Message,WPARAM wParam, LPARAM lParam)
{
    switch(Message)
    {
        case WM_INITDIALOG:
        return TRUE;

        case WM_COMMAND:
             switch(LOWORD(wParam))
             {
                case  SNIFF_IDB_CANCEL:
					  closesocket(server);
                      DestroyWindow(l_hWnd); 
                break;
				
				case  IDC_BUTTON1_START_SNIFF:
				      idb_Sniff_start_clicked();
				break;

				case  IDC_BUTTON1_SNIFF_STOP:
					  idb_Sniff_stop_clicked();	
				break;
			 }
        break;
	        
        case WM_PAINT:
        break;
     
        //Incomming message..........
        case  WSA_SERVER_READ:
	       	  mycsocket->prc_ProcessAsyncSelectMessage(server,saccepted_connection,wParam,lParam,hWnd_sniff);
	          idb_Sniff_show_data();	
	    break;

		case WM_DESTROY:
             
        break;
        
        case WM_CLOSE:
             DestroyWindow(l_hWnd);
        break;
        
    }
    return FALSE;
}



//---------------------------------------------------
/*           int APIENTRY WinMain                  */
//---------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR     lpCmdLine,
                   int       nCmdShow)
{
        
    MSG msg;
    HACCEL hAccelTable = NULL;
    hInst = hInstance;
   
	srand( (unsigned)time( NULL ) );
    Blokers();
   
    while (GetMessage(&msg,NULL,0,0)) 
    {
		if(!IsWindow(hWnd) || !IsDialogMessage(hWnd, &msg))  
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}



//COMMENNTS------+++++------>>>>>>>>>>>>>>>
/*



 //BLOCK 
 if((SendMessage(GetDlgItem(hWnd,IDC_CHECK_TEXTDATA),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_UNCHECKED) &&
	(SendMessage(GetDlgItem(hWnd,IDC_CHECK_BINDATA),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_UNCHECKED) &&
	(SendMessage(GetDlgItem(hWnd,IDC_CHECK_DECDATA),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_UNCHECKED))
 {
    MessageBox(hWnd,"Please select data source to send!","Error!",MB_OK);
 return;
 }
	
	
 bool errors; 	
 if(SendMessage(GetDlgItem(hWnd,IDC_CHECK_ERRORS),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_UNCHECKED)
 {
	errors = false; 
 }else
 {
    errors = true;
 }

 char tobuf[201];
 char databuf[201];
 char port[21];
 int iport = 0;
 char count[21];
 int cnt = 0;
 ZeroMemory(count,sizeof(count));
 ZeroMemory(port,sizeof(port));
 ZeroMemory(tobuf,sizeof(tobuf));
 ZeroMemory(databuf,sizeof(databuf));
 //Get host
 if(GetWindowText(GetDlgItem(hWnd,IDC_EDIT_HOST),tobuf,200) == 0)
 {
  MessageBox(hWnd,"Please input host name!","Error!",MB_OK);
 return;
 }
 string to = tobuf;

 
 //Get count
 if(GetWindowText(GetDlgItem(hWnd,IDC_EDIT_COUNT),count,20) == 0)
 {
  MessageBox(hWnd,"Please input count number!","Error!",MB_OK);
 return;
 }
 cnt = atoi(count);
 
 //Get port
 if(GetWindowText(GetDlgItem(hWnd,IDC_EDIT_PORT),port,20) == 0)
 {
  MessageBox(hWnd,"Please input port number!","Error!",MB_OK);
 return;
 }
 iport = atoi(port);
 
 bool random;
 if(SendMessage(GetDlgItem(hWnd,IDC_CHECK_RANDOM),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_CHECKED)
 {
  random = true;
 }
 
 //get data
 if(GetWindowText(GetDlgItem(hWnd,IDC_EDIT_DATA),databuf,1000) == 0)
 {
  MessageBox(hWnd,"Please input data!","Error!",MB_OK);
 return;
 }
 string sdatabuf = databuf;

 if(SendMessage(GetDlgItem(hWnd,IDC_CHECK_LOOP),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_UNCHECKED)
 {	 
  int lop;
  for(lop = 0;lop<cnt;lop++)
  {
   if(random == true) iport = rand()%64000; 
   mycsocket->send_text_message(errors,0,0,(string)databuf,(string)"",(string)tobuf,(string)"",(string)"",iport);
  }
 }else
 {
  stop = false;
  while(stop == false)
  {
   if(random == true) iport = rand()%64000;
   mycsocket->send_text_message(errors,0,0,(string)databuf,(string)"",(string)tobuf,(string)"",(string)"",iport);
  }
 }









             //GetClientRect(l_hWnd,&rt); 
             //hbitmap = LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP1));
	         //hdc = BeginPaint(l_hWnd,&ps);
             //li_res = GetObject(hbitmap,sizeof(bmp),&bmp);
	         //chdc = CreateCompatibleDC(hdc);
	         //SelectObject(chdc,hbitmap);
	         //BitBlt(hdc,0,0,bmp.bmWidth,bmp.bmHeight,chdc,0,0,SRCCOPY);
	         //SetTextColor(hdc,RGB(255,0,0));
	         //SetBkMode(hdc,TRANSPARENT);
             // TextOut(hdc,25,25,"HOST :",6);
             // TextOut(hdc,25,64,"PORT :",6);
             // TextOut(hdc,195,25,"COUNT :",7);
             // TextOut(hdc,225,64,"LOOP",4);
             //DeleteObject(hbitmap);
	         //EndPaint(l_hWnd,&ps);  
  

 //DOS
 bool errors; 	
 if(SendMessage(GetDlgItem(hWnd,IDC_CHECK_ERRORS),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_UNCHECKED)
 {
	errors = false; 
 }else
 {
    errors = true;
 }
	
 char tobuf[201];
 char databuf[201];
 char port[21];
 int iport = 0;
 char count[21];
 int cnt = 0;
 ZeroMemory(count,sizeof(count));
 ZeroMemory(port,sizeof(port));
 ZeroMemory(tobuf,sizeof(tobuf));
 ZeroMemory(databuf,sizeof(databuf));
 //Get host
 if(GetWindowText(GetDlgItem(hWnd,IDC_EDIT_HOST),tobuf,200) == 0)
 {
  MessageBox(hWnd,"Please input host name!","Error!",MB_OK);
 return;
 }
 
 //Get count
 if(GetWindowText(GetDlgItem(hWnd,IDC_EDIT_COUNT),count,20) == 0)
 {
  MessageBox(hWnd,"Please input count number!","Error!",MB_OK);
 return;
 }
 cnt = atoi(count);
 
 //Get port
 if(GetWindowText(GetDlgItem(hWnd,IDC_EDIT_PORT),port,20) == 0)
 {
  MessageBox(hWnd,"Please input port number!","Error!",MB_OK);
 return;
 }
 iport = atoi(port);
 
 //get data
 if(GetWindowText(GetDlgItem(hWnd,IDC_EDIT_DATA),databuf,1000) == 0)
 {
  MessageBox(hWnd,"Please input data!","Error!",MB_OK);
 return;
 }
 
 if(SendMessage(GetDlgItem(hWnd,IDC_CHECK_LOOP),(UINT)BM_GETCHECK,(WPARAM)0,(LPARAM)0) == BST_UNCHECKED)
 {	 
  int lop;
  for(lop = 0;lop<cnt;lop++)
  {
   mycsocket->dos(errors,tobuf,iport);
  }
 }else
 {
  stop = false;
  while(stop == false)
  {
   mycsocket->dos(errors,tobuf,iport);
  }
 }






*/




