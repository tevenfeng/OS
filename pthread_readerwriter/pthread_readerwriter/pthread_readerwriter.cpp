// pthread_readerwriter.cpp : �������̨Ӧ�ó������ڵ㡣
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
	int i = p->i;                        //��i��reader
	printf("�̺߳�Ϊ%d�Ķ����̵߳��", i);
	//while (TRUE)
	{
		P(rmutex);
		readercount++;
		if (readercount == 1) P(db);
		V(rmutex);

		//���ٽ���
		printf("��ǰ��������̺߳�Ϊ%d�Ķ��ߣ�����%d�������ڶ�ȡ���ݿ⡣\n",i,readercount);

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
	int i = p->i;                        //��i��writer
	printf("�̺߳�Ϊ%d��д���̵߳��",i);
	//while (TRUE)
	{
		P(db);
		printf("��ǰ�ڽ���д���������̺߳�Ϊ%d���̡߳�\n",i);
		V(db);
	}

	return 0;
}

int main()
{
	HANDLE hThread[MAX_NUM+MAX_NUM];             //�̼߳���

	const int threadcount = 5;
	//�ź�����ʼ��
	rmutex = CreateSemaphore(NULL,1,MAX_NUM,(LPCWSTR)"Reader'sMutex");
	db = CreateSemaphore(NULL,1,1,(LPCWSTR)"Database'sMutex");


	printf("����������������������������������ʼ������������������������������\n");

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