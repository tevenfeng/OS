// pthread_WriterFirst.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

#include<windows.h>
#include<stdlib.h>
#include<stdio.h>

typedef HANDLE semaphore;
#define P(S) WaitForSingleObject(S,INFINITE);
#define V(S) ReleaseSemaphore(S,1,NULL);

//最大读者写者线程数
const int MAX_NUM = 100;

int writercount = 0;
int readercount = 0;
semaphore rmutex, wmutex, wpmutex, db;

class Para
{
public:
	int i;
};

HANDLE WINAPI Reader(LPVOID para)
{
	//第i个reader
	Para *myPara = (Para*)para;
	int i = myPara->i;
	printf("线程号为%d的读者线程到达。", i);

	//while (TRUE)
	{
		P(wpmutex);
		P(rmutex);
		readercount++;
		if (readercount == 1) P(db);
		V(rmutex);
		V(wpmutex);

		printf("当前是线程号为%d的读者线程在操作数据库。\n",i);

		P(rmutex);
		readercount--;
		if (readercount == 0) V(db);
		V(rmutex);
	}

	return 0;
}

HANDLE WINAPI Writer(LPVOID para)
{
	Para *myPara = (Para*)para;
	int i = myPara->i;
	printf("线程号为%d的写者线程到达。",i);

	while (TRUE)
	{
		P(wpmutex);
		P(wmutex)
		writercount++;
		if (writercount == 1) P(db);
		V(wmutex);

		printf("当前是线程号为%d的写者线程在操作数据库。\n",i);

		P(wmutex);
		writercount--;
		if (writercount == 0) V(db);
		V(wmutex);
		V(wpmutex);
	}
}

int main()
{
	//初始化信号量
	rmutex = CreateSemaphore(NULL, 1, 1, (LPCWSTR)"Reader'sMutex");
	wmutex = CreateSemaphore(NULL, 1, 1, (LPCWSTR)"Writer'sMutex");
	wpmutex = CreateSemaphore(NULL, 1, 1, (LPCWSTR)"WriterFirstMark");
	db = CreateSemaphore(NULL, 1, 1, (LPCWSTR)"Database'sMutex");



	system("pause");
}