#include "MyBaseClient.h"
#include<thread>
#include<iostream>
#include<functional> 
using namespace std;

int port=0;
char ip[32];

bool g_bRun = true;
void cmdThread()
{
	while (true)
	{
		char cmdBuf[256] = {};
		scanf("%s", cmdBuf);
		if (0 == strcmp(cmdBuf, "exit"))
		{
			g_bRun = false;
			printf("退出cmdThread线程\n");
			break;
		}
		else {
			printf("不支持的命令。\n");
		}
	}
}

BaseClient* pClientSocket = NULL;
void InitSocket()
{
	cout<< "Start Create Tcp Connection..." << endl;
	pClientSocket = new BaseClient();
	pClientSocket->Connect(ip, port);
	//pClientSocket->OnRun();

	std::chrono::milliseconds t(2000);
	std::this_thread::sleep_for(t);

	//Login login;
	//strcpy(login.userName, "lzx");
	//strcpy(login.PassWord, "lzx123");

	//const int nLen = sizeof(login);
	//pClientSocket->SendData(&login, nLen);
	//pClientSocket->InitSelect();
	//pClientSocket->OnRun();

	cout<< "client and server start communicate with connction." << endl;

	{
		TickRequest tick;
		int type = 0;
		printf("please input tick type : ");
		scanf("%d",&type);
		tick.type = type;
		int len = sizeof(TickRequest);
		printf("tick type : %d, len : %d\n", type,len);
		pClientSocket->SendData(&tick, len);
		
		//pClientSocket->RecvData(pClientSocket->GetSocket());
	}

}

int main(int argc, char *argv[])
{
	//控制开关，输入exit直接关闭socket退出
	//std::thread t1(cmdThread);
	//t1.detach();
	if(argc < 2)
		printf("please input ip and port.\n");
	port = atoi(argv[2]);
	printf("port : %s\n",argv[2]);
	strcpy(ip,argv[1]);
	printf("ip : %s\n",argv[1]);
	//InitSocket();
	std::thread t1(InitSocket);
	t1.detach();
	
	sleep(1);
	pClientSocket->OnRun();
	while (g_bRun)
		;

	printf("已退出。\n");
	return 0;
}