#include<iostream>
#include<string>
#include<WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

using namespace std;

void main()
{
	string ipAddress = "127.0.0.1";
	int port = 5002;
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int WSResult = WSAStartup(ver, &data);
	if (WSResult != 0)
	{
		cerr << "Winsock baþlatýlamadý" << WSResult<<endl;
		return;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		cerr << "Soket oluþturulamadý" << WSAGetLastError() << endl;
		return; 
	}
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cerr << "Sunucu ile baðlanamadý" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	char buf[4096];
	string userInput;

	do 
	{
		cout << ">";
		getline(cin, userInput);

		if (userInput.size() > 0)
		{
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR)
			{
				ZeroMemory(buf, 4096);
				int byteReceived = recv(sock, buf, 4096, 0);
				if (byteReceived > 0)
				{
					cout << "sunucu " << string(buf, 0, byteReceived) << endl;
				}
			}
		}
		else {

		}
	}while (userInput.size() > 0);

	closesocket(sock);
	WSACleanup();
}