#include<iostream>
#include<WS2tcpip.h>
#include<string>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

 void main()
 {
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);
	int WsOk = WSAStartup(ver, &wsData);
	if (WsOk != 0)
	{
		cerr << "Ba?lant? hatas?" << endl;
		return;
	}

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET)
	{
		cerr << "soket olu?turulamad?" << endl;
		return;
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(5002);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));

	listen(listening, SOMAXCONN);

	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
	
	char host[NI_MAXHOST];
	char service[NI_MAXHOST];


	ZeroMemory(host, NI_MAXHOST);

	ZeroMemory(service, NI_MAXSERV) ;
	
	
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " Baglandi " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " porta ba?lan?ld?" <<  ntohs(client.sin_port) << endl;
	}

	closesocket(listening);

	char buf[4096];
	while (true)
	{
		ZeroMemory(buf, 4096);
		//data bekleniyor

		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		/*if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "rec hatas?" << endl;
			break;
		}*/

		//string(buf, 0, bytesReceived) == "exit" || 
		if (bytesReceived == 0) 
		{
			cout << "istemci ayr?ld?" << endl;
			break;
		}

		send(clientSocket, buf, bytesReceived + 1, 0);
	}

	closesocket(clientSocket);
	WSACleanup();
 }
