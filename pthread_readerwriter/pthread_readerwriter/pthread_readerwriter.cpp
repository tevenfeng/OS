// pthread_readerwriter.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

#include<windows.h>
#include<stdio.h>
#include<stdlib.h>

typedef HANDLE semaphore;
#define P(S) WaitForSingleObject(S,INFINITE);
#define V(S) ReleaseSemaphore(S,1,NULL);

//#define MAX_NUM 100;
const int MAX_NUM = 100;

int readercount = 0;
semaphore rmutex,db;

class myPara
{
public:
	int i;
};

DWORD WINAPI Reader(LPVOID para)
{
	myPara *p = (myPara*)para;
	int i = p->i;                        //第i个reader
	printf("线程号为%d的读者线程到达。", i);
	//while (TRUE)
	{
		P(rmutex);
		readercount++;
		if (readercount == 1) P(db);
		V(rmutex);

		//非临界区
		printf("当前进入的是线程号为%d的读者，共有%d个读者在读取数据库。\n",i,readercount);

		P(rmutex);
		readercount--;
		if (readercount == 0) V(db);
		V(rmutex);
	}

	return 0;
}

DWORD WINAPI Writer(LPVOID para)
{
	myPara *p = (myPara*)para;
	int i = p->i;                        //第i个writer
	printf("线程号为%d的写者线程到达。",i);
	//while (TRUE)
	{
		P(db);
		printf("当前在进行写操作的是线程号为%d的线程。\n",i);
		V(db);
	}

	return 0;
}

int main()
{
	HANDLE hThread[MAX_NUM+MAX_NUM];             //线程计数

	const int threadcount = 5;
	//信号量初始化
	rmutex = CreateSemaphore(NULL,1,MAX_NUM,(LPCWSTR)"Reader'sMutex");
	db = CreateSemaphore(NULL,1,1,(LPCWSTR)"Database'sMutex");


	printf("………………………………………程序开始………………………………………\n");

	myPara* mypara[threadcount * 2];
	for (int i = 0; i < threadcount; i++)
	{
		mypara[i] = new myPara();
		mypara[i + threadcount] = new myPara();

		mypara[i]->i = i;
		hThread[i] = CreateThread(NULL, 0, Reader, mypara[i], NULL, NULL);
		mypara[i+threadcount]->i = i + threadcount;
		hThread[i + threadcount] = CreateThread(NULL, 0, Writer, mypara[i + threadcount], NULL, NULL);
	}

	for (int i = 0; i < threadcount; i++)
	{
		WaitForSingleObject(hThread[i],INFINITE);
		WaitForSingleObject(hThread[i + threadcount], INFINITE);
	}



	system("pause");
}