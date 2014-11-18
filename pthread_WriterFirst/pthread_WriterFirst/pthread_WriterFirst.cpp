// pthread_WriterFirst.cpp : �������̨Ӧ�ó������ڵ㡣
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

//������д���߳���
const int MAX_NUM = 100;

int writercount = 0;
int readercount = 0;
semaphore rmutex, wmutex, wpmutex, db;

class Para
{
public:
	int i;
};

DWORD WINAPI Reader(LPVOID para)
{
	//��i��reader
	Para *myPara = (Para*)para;
	int i = myPara->i;
	printf("�̺߳�Ϊ%d�Ķ����̵߳��", i);

	//while (TRUE)
	{
		P(wpmutex);
		P(rmutex);
		readercount++;
		if (readercount == 1) P(db);
		V(rmutex);
		V(wpmutex);

		printf("��ǰ��������̺߳�Ϊ%d�Ķ����̡߳�����%d�������߳��ڲ������ݿ⡣\n",i,readercount);

		P(rmutex);
		readercount--;
		if (readercount == 0) V(db);
		V(rmutex);
	}

	return 0;
}

DWORD WINAPI Writer(LPVOID para)
{
	Para *myPara = (Para*)para;
	int i = myPara->i;
	printf("�̺߳�Ϊ%d��д���̵߳��",i);

	//while (TRUE)
	{
		P(wpmutex);
		P(wmutex)
		writercount++;
		if (writercount == 1) P(db);
		V(wmutex);

		printf("��ǰ���̺߳�Ϊ%d��д���߳��ڲ������ݿ⡣\n",i);

		P(wmutex);
		writercount--;
		if (writercount == 0) V(db);
		V(wmutex);
		V(wpmutex);
	}
	return 0;
}

int main()
{
	HANDLE hThread[2 * MAX_NUM];			//�߳̾��
	const int threadcount = 5;					//ʵ�����Ķ����߳���

	//��ʼ���ź���
	rmutex = CreateSemaphore(NULL, 1, 1, (LPCWSTR)"Reader'sMutex");
	wmutex = CreateSemaphore(NULL, 1, 1, (LPCWSTR)"Writer'sMutex");
	wpmutex = CreateSemaphore(NULL, 1, 1, (LPCWSTR)"WriterFirstMark");
	db = CreateSemaphore(NULL, 1, 1, (LPCWSTR)"Database'sMutex");

	printf("��������������������������������ʼ����������������������������\n");

	//�����б�ʵ����
	Para* mypara[threadcount * 2];
	for (int i = 0; i < threadcount; i++)
	{
		mypara[i] = new Para();
		mypara[i]->i = i;
		mypara[i + threadcount] = new Para();
		mypara[i + threadcount]->i = i + threadcount;
	}

	for (int i = 0; i < threadcount; i++)
	{
		hThread[i] = CreateThread(NULL, 0, Reader, mypara[i], NULL, NULL);
		hThread[i + threadcount] = CreateThread(NULL, 0, Writer, mypara[i + threadcount], NULL, NULL);
	}

	for (int i = 0; i < threadcount; i++)
	{
		WaitForSingleObject(hThread[i],INFINITE);
		WaitForSingleObject(hThread[i + threadcount], INFINITE);
	}

	CloseHandle(rmutex);
	CloseHandle(wpmutex);
	CloseHandle(wmutex);

	system("pause");
}