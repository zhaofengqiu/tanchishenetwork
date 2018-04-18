#include<stdio.h>
#include<string.h>
#include <winsock2.h>  
#include<windows.h>
#include<time.h>
#include<conio.h> 
#include<stdlib.h>
#include<pthread.h> 
#pragma comment(lib,"ws2_32.lib")  
#define up 'w'
#define down 's'
#define left 'a'
#define right 'd'
#define stop 'p'
#define fup 'i'
#define fdown 'k'
#define fleft 'j'
#define fright 'l'


char cmd;

void welcome();
void Finish(); 
void creatgraph(); 
void* getorder(void* args);
void gotoxy(int x, int y);//�ƶ���� 
void gotoprint(int x, int y);
void gotodelete(int x, int y);
void clearSnake();
void drawSnake();
void creatfood();
int ClickControl();
int Judge();
int judgeSame();
void SmovingBody(char c);  
void Fmovingbody(char c); 
void Eating();  
void Bool();             
void ChangeBody(int a,int b); 
void color(unsigned short c) ;
void* playgame(void* args);
void * sendorder(void* args);
void textOut(short y,short x,unsigned short c,const char s[]);
/*ȫ�ֱ��� + Ԥ����*/
typedef struct Snakes
{
	int x;
	int y;
	struct Snakes *next;
}snake;


snake *head,*tail;


struct Food
{
	int x;
	int y;
}food;
char name[20];
HANDLE handle;
int score = 0;
char fclick = 0;
char sclick = 0;
int speed;


/************************************************************/
int main()
{
	pthread_t p1;
	
	pthread_t p3;
	handle=GetStdHandle(STD_OUTPUT_HANDLE);
	pthread_create(&p1,NULL,playgame,NULL); 
	
	pthread_create(&p3,NULL,sendorder,NULL);
	pthread_join(p1,NULL);
	
	pthread_join(p3,NULL);
	
}
void * playgame(void* args)
{
	
	/****************��ʼ��******************/
	//welcome(); 
	creatgraph();
	creatfood();
	
	/*****************************************/
	ClickControl();
	
}


/**********************************************************/
void welcome()
{
	gotoxy(15,10);
	printf("/**********************************************/");
	gotoxy(15, 20);
	printf("/**********************************************/");
	gotoxy(20, 13);
	printf("WELCOME TO THE GAME OF RETRO SNAKE");
	gotoxy(14, 16);
	printf("����Ӣ�����뷨�в�����ijkl����ʳ���ƶ���wasd������p��ͣ");
	gotoxy(20, 18);
	printf("GOOD LUCKY\n");
	Sleep(1000);
	//scanf_s("%s", &name,10);
	system("cls");
}
/**********************************************************/
void creatgraph()//��ӡ��ͷ���ֵı߿����             
{
	int i;
	for (i = 0; i<58; i += 2)//��ӡ���±߿�
	{
		gotoprint(i, 0);
		gotoprint(i, 30);
	}
	for (i = 1; i < 30; i++)
	{
		gotoprint(0, i);
		gotoprint(56, i);
	}
	textOut(63, 10,10,"hello Welcome To Play");
	gotoxy(63, 15);
	printf("Your Score Is:%d    =���أ�= ",score);
	gotoxy(63, 20);
	printf("This Game Is Created By qipaqiu");
	head = (snake*)malloc(sizeof(snake));
	head->x = 16;
	head->y = 15; //��һ�ڵ�x,y���� 
	//gotoprint(head->x, head->y);
	tail = (snake*)malloc(sizeof(snake));
	snake *p = (snake*)malloc(sizeof(snake));
	snake *q = (snake*)malloc(sizeof(snake));
	p->x = 16;
	p->y = 16;//�ڶ��ڵ�x,y���� 
	q->x = 16;
	q->y = 17;// �����ڵ�x,y���� 
	/*������������������*/ 
	head->next = p;
	p->next = q;
	q->next = tail;
	
	//gotoprint(p->x, p->y);
	//gotoprint(q->x, q->y);
	tail->next = NULL;
	tail->x = 4;
	tail->y = 2;
	
}
/**********************************************************/
void gotoxy(int x, int y)//���ʵ�ֻ�ô��ں���Ӧ��꣬�� �����ʾ����Ӧλ�á� 
{
	COORD pos;//һ���ַ��ڿ���̨��Ļ�ϵ����� 
	HANDLE hOutput;//��� 
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//GetStdHandle��һ��Windows API�����������ڴ�һ���ض��ı�׼�豸����׼���롢��׼������׼������ȡ��һ�������������ʶ��ͬ�豸����ֵ��������Ƕ��ʹ�á�
	SetConsoleCursorPosition(hOutput, pos);//��pos��ʾ�ڸþ������ 
}
/**********************************************************/
void gotoprint(int x, int y)
{
	gotoxy(x, y);
	//printf("��") ;
	printf("��");
}
/**********************************************************/
void gotodelete(int x, int y)
{
	gotoxy(x, y);
	printf("  ");
}
/**********************************************************/
void creatfood()
{
	srand((int)time(NULL));
	lable:
	food.y = rand() % (25 - 1 + 1) + 1;
	food.x = rand() % (54 - 2 + 1) + 2;
	if (food.x % 2 != 0)
	{
		food.x = food.x+1;
	}
	snake * judge = head;
	while (1)  //�����ų������ظ�
	{
		if (judge->next == NULL) break;
		if (food.x == judge->x&&food.y == judge->y)
		{
			goto lable;
		}
		judge = judge->next;
	}
	gotoxy(food.x, food.y);
	color(0x04);
	printf("��");
}
/**********************************************************/
int ClickControl()
{
	char dest[]="w,a,s,d,p,i,j,k,l";
	char sdest[]="w,a,s,d,p";
	char fdest[]="i,j,k,l";
	char temp='w';
	while (1)
	{
		if (Judge()==0) return 0;//��������������ͽ��� 
		if (_kbhit()!=0)//��������ȡ�����ַ�������Ҫ����һ������ 
		{
			temp=_getch();
			//temp=_getch(); 
			if (strchr(dest,temp)!=0){
				cmd=temp;
				if (strchr(sdest,temp)!=0){
					sclick=temp;
					
				}
				if (strchr(fdest,temp)!=0){
					fclick=temp;
			
					Fmovingbody(fclick);
					cmd=0; 
				}
				
			}
			
		}
		SmovingBody(sclick);
		gotoxy(0, 31);
		Bool();
		Eating();
	}
	return 1;
}
/**********************************************************/
void SmovingBody(char c)
{
	int count = 0;
	int a = head->x, b = head->y;//a�洢��header��x,y���ꡣ 
	 
	//ͨ������պ��ӡʵ�ֶ���Ч��
	clearSnake();
	switch (c)
	{
		case up://ͷ�����������һ 
			head->y -= 1;
			ChangeBody(a,b);
			break;
		case down://ͷ�������������� 
			head->y += 1;
			ChangeBody(a,b);
			break;
		case left: 
			head->x -= 2;
			ChangeBody(a,b);
			break;
		case right:
			head->x += 2;
			ChangeBody(a,b);
			break;
		
		case stop:
			_getch();
			break;
	}
	
	/*********************************************/ 
	
	drawSnake();
	gotoxy(0, 30);
	 
	if (count <= 10) speed = 150;
	else if (count > 10 && count <= 20) speed = 100;
	else if (count > 20 && count <= 40) speed = 50;
	else speed = 10;
	Sleep(speed);
}
void Bool()
{
	snake * judge = head->next;
	while (1)  //�����ų������ظ�
	{
		if (judge->next == NULL) break;
		if (food.x == judge->x&&food.y == judge->y)
		{
			score+=10;
			creatfood();
		} 
		judge = judge->next;
	}
	
}
void clearSnake()
{
	
	snake *p = head; 
	//ͨ������պ��ӡʵ�ֶ���Ч��
	while (1)//������� 
	{
		if (p->next == NULL) break;
		gotodelete(p->x, p->y);
		
		p = p->next;
	}
}
void drawSnake()
{
	snake *p = head;
	/***************��ӡ*************************/ 
	 while (1)
	{
		if (p->next == NULL) break;
		
		color(0x05);
		gotoprint(p->x, p->y);
		p = p->next;
	}
	
}
BOOL SetConsoleTextAttribute(HANDLE hConsoleOutput,WORD wAttributes);
void color(unsigned short c)
{
	SetConsoleTextAttribute(handle,c) ;
}
void textOut(short x,short y,unsigned short c,const char s[])
{
	gotoxy(x,y);
	color(c);
	printf("%s",s);
}
void Fmovingbody(char c)
{
	
	int count = 0;
	int a = head->x, b = head->y;//a�洢��header��x,y���ꡣ 
	snake *p = head; 
	//ͨ������պ��ӡʵ�ֶ���Ч��
	clearSnake();
	gotodelete(food.x, food.y);
	switch (c)
	{
		case fup://ͷ�����������һ 
			food.y -= 1;
			ChangeBody(a,b);
			break;
		case fdown://ͷ�������������� 
			food.y += 1;
			ChangeBody(a,b);
			break;
		case fleft: 
			food.x -= 2;
			ChangeBody(a,b);
			break;
		case fright:
			food.x += 2;
			
			ChangeBody(a,b);
			break;
		
		case stop:
			_getch();
			break;
	}
	gotoxy(food.x, food.y);
	printf("��");
	drawSnake();
	gotoxy(0, 31);
	
	
	/***************��ӡ*************************/ 
	 
	/*********************************************/ 
	
}
/**********************************************************/
void Eating()
{
	
	if (head->x == food.x && head->y == food.y)
	{
		creatfood();
		snake *_new = (snake*)malloc(sizeof(snake));
		snake *p;
		p = head;
		/***********��չsneak*******************/ 
		while (1)
		{
			if (p->next->next == NULL) break;
			p = p->next;
		}
		p->next = _new;
		_new->next = tail;
		
		score += 10;
		gotoxy(77, 15);
		printf("%d", score);
	}
}
/**********************************************************/
void ChangeBody(int a,int b)
{
	snake *p = head->next;
	int mid1, mid2,_mid1,_mid2;
	mid1 = p->x;
	mid2 = p->y;
	while (1)
	{
		if (p->next->next == NULL) break;
		_mid1 = p->next->x;
		_mid2 = p->next->y;
		p->next->x = mid1;
		p->next->y = mid2;
		mid1 = _mid1;
		mid2 = _mid2; 
		p = p->next;
	}
	p = head->next; 
	/*��Ϊǰ�棬û������ȥ*/
	p->x = a;
	p->y = b;

}
/**********************************************************/
int Judge()//�ж�̰�����Ƿ�����ǽ�ڻ��������Լ� 
{
	if (head->x == 0|| head->x == 56 || head->y == 0 || head->y == 30)
	{
		Finish();
		return 0;
	} 
	if (food.x == 0|| food.x == 56 || food.y == 0 || food.y == 30)
	{
		Finish();
		return 0;
	} 
	snake *p = head->next;
	while (1)
	{
		if (p->next == NULL) break;/*
		if (head->x == p->x&&head->y == p->y)
		{
			Finish();
			return 0;
		}*/
		p = p->next;
	}
	return 1;
}
/**********************************************************/
void Finish()
{
	system("cls");
	gotoxy(15, 10);
	printf("/**********************************************/");
	gotoxy(15, 20);
	printf("/**********************************************/");
	gotoxy(18, 14);
	printf("GAME   OVER      �R�بQ");
	gotoxy(20, 16);
	printf("Your Score is %d    hiahiahia", score);
	gotoxy(18, 18);
	printf("������Ŷ��     ����Ŭ��( > c < )");
	gotoxy(0, 27);
	system("pause"); 

}

void * getorder(void* args) 
{  
    //��ʼ��wsa  
    WORD sockVision = MAKEWORD(2,2);  
    WSADATA wsadata;  
    //��������  
    SOCKET slisten;  
    SOCKET sClient;  
    struct sockaddr_in remoteAddr;  
    struct sockaddr_in sin;  
    int ret = -1;  
    int nAddrlen = sizeof(remoteAddr);  
    char revdata[255];  
    char senddata[255];  
  
    if( WSAStartup(sockVision,&wsadata) != 0 )  
    {  
        printf("WSA��ʼ��ʧ��\n");  
        //return 0;  
    }  
  
    //�����׽���  
    slisten = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
    if(slisten == INVALID_SOCKET)  
    {  
        printf("socket�����ߴ���ʧ��\n");  
        
    }  
  
    //��IP�Ͷ˿�  
    sin.sin_family = AF_INET;  
    sin.sin_port = htons(9000);  
    sin.sin_addr.S_un.S_addr = INADDR_ANY;  
    if( bind(slisten,(LPSOCKADDR)&sin,sizeof(sin)) == SOCKET_ERROR )  
    {  
        printf("��IP�Ͷ˿�\n");  
        //  
    }  
  
    //����  
    if(listen(slisten,5) == SOCKET_ERROR)  
    {  
        printf("����ʧ��\n");  
        //return 0;  
    }  
  
    //ѭ����������  
    while(1)  
    {  
        printf("�ȴ�����.........\n");  
        sClient = accept(slisten,(SOCKADDR *)&remoteAddr,&nAddrlen);  
        if(sClient == INVALID_SOCKET)  
        {  
            printf("���ܿͻ���ʧ�ܣ���������.........\n");  
            continue;  
        }  
        printf("���ܿͻ��˳ɹ���%s\n",inet_ntoa(remoteAddr.sin_addr));  
  
        //��������  
        while(1)  
        {  
            ret = recv(sClient,revdata,255,0); 
			food.x=revdata[0]; 
				  
	            revdata[ret] = 0x00;  
	            
	            
	            recv(sClient,revdata,255,0); 
	            food.y=revdata[0]; 
			   
        }  
        closesocket(sClient);  
    }  
  
    closesocket(sClient);  
    WSACleanup();  
}
void * sendorder(void* args)
{
	
	WORD sockVision = MAKEWORD(2,2);  
    WSADATA wsadata;  
    SOCKET sclient ;  
    struct sockaddr_in serAddr;  
    char senddata[255];  
    char revdata[255];  
    int ret = -1;  
    if(WSAStartup(sockVision,&wsadata) != 0)  
    {  
        printf("WSA��ʼ��ʧ��\n");  
        return 0;  
    }  
  
    sclient = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
    if(sclient == INVALID_SOCKET)  
    {  
        printf("socket�ͻ��˴���ʧ��\n");  
        return 0;  
    }  
  
    serAddr.sin_family = AF_INET;  
    serAddr.sin_port = htons(9000);  //
    serAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");  
    if( connect(sclient,(SOCKADDR *)&serAddr,sizeof(serAddr)) == SOCKET_ERROR )  
    {  
        printf("socket�ͻ�������ʧ��\n");  
        return 0;  
    }  
    while(1)  
    {   
		
		
       	senddata[0]=cmd;
        send(sclient,senddata,strlen(senddata),0);
		senddata[0]=food.x;  
        send(sclient,senddata,strlen(senddata),0);
        senddata[0]=food.y+60;
        send(sclient,senddata,strlen(senddata),0);
        senddata[0]=score/10-50;
        send(sclient,senddata,strlen(senddata),0);
        if(ret > 0)  
        {  
            revdata[ret] = 0x00;  
            
			//food.x=revdata[0] ;
              
            //recv(sclient,revdata,255,0); 
            
			//food.y=revdata[0] ;
        }    
    }  
    closesocket(sclient);  
    WSACleanup();  
	
}  
