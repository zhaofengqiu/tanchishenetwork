#include<stdio.h>
#include<string.h>
#include <winsock2.h>  
#include<windows.h>
#include<time.h>
#include<conio.h> 

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
void gotoxy(int x, int y);//移动光标 
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
//void color(unsigned short c) ;
void textOut(short y,short x,unsigned short c,const char s[]);           
void ChangeBody(int a,int b); 
void* playgame(void* args);

/*全局变量 + 预处理：*/
typedef struct Snakes
{
	int x;
	int y;
	struct Snakes *next;
}snake;


snake *head,*tail;
HANDLE handle;

struct Food
{
	int x;
	int y;
}food;
char name[20];
int score = 10;
char fclick = 0;
char sclick = 0;
int flag=0;
int speed;

/************************************************************/
int main()
{
	pthread_t p1;
	pthread_t p2;
	int temp1,temp2;
	handle=GetStdHandle(STD_OUTPUT_HANDLE);
	temp1=pthread_create(&p1,NULL,playgame,NULL); 
	temp2=pthread_create(&p2,NULL,getorder,NULL);
	pthread_join(p1,NULL);
	pthread_join(p2,NULL);
	
	
}
void * playgame(void* args)
{
	
	/****************初始化******************/
	//welcome();
	creatgraph();
	
	creatfood();
	gotodelete(food.x,food.y);
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
	printf("观看贪吃蛇");
	gotoxy(20, 18);
	printf("GOOD LUCKY\n");
	Sleep(2000);
	//scanf_s("%s", &name,10);
	system("cls");
}
/**********************************************************/
void creatgraph()//打印开头出现的边框和蛇             
{
	int i;
	for (i = 0; i<58; i += 2)//打印上下边框
	{
		gotoprint(i, 0); 
		gotoprint(i, 30);
	}
	for (i = 1; i < 30; i++)
	{
		gotoprint(0, i);
		gotoprint(56, i);
	}
	gotoxy(63, 10);
	textOut(63, 10,10,"hello Welcome To Play");
	gotoxy(63, 15);
	printf("Your Score Is:%d    =￣ω￣= ",score);
	gotoxy(63, 20);
	printf("This Game Is Created By qipaqiu");
	head = (snake*)malloc(sizeof(snake));
	head->x = 16;
	head->y = 15; //第一节的x,y坐标 
	//gotoprint(head->x, head->y);
	tail = (snake*)malloc(sizeof(snake));
	snake *p = (snake*)malloc(sizeof(snake));
	snake *q = (snake*)malloc(sizeof(snake));
	p->x = 16;
	p->y = 16;//第二节的x,y坐标 
	q->x = 16;
	q->y = 17;// 第三节的x,y坐标 
	/*将三节坐标连接起来*/ 
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
void gotoxy(int x, int y)//如何实现获得窗口和相应光标，将 光标显示在相应位置。 
{
	COORD pos;//一个字符在控制台屏幕上的坐标 
	HANDLE hOutput;//句柄 
	pos.X = x;
	pos.Y = y;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);//GetStdHandle是一个Windows API函数。它用于从一个特定的标准设备（标准输入、标准输出或标准错误）中取得一个句柄（用来标识不同设备的数值）。可以嵌套使用。
	SetConsoleCursorPosition(hOutput, pos);//将pos显示在该句柄里面 
}
/**********************************************************/
void gotoprint(int x, int y)
{
	gotoxy(x, y);
	//printf("●") ;
	printf("■");
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
	food.x= food.x;
	food.y= food.y;
	food.y = rand() % (25 - 1 + 1) + 1;
	food.x = rand() % (54 - 2 + 1) + 2;
	if (food.x % 2 != 0)
	{
		food.x = food.x+1;
	}  
	snake * judge = head;
	while (1)  //遍历排除蛇身重复
	{
		if (judge->next == NULL) break;
		if (food.x == judge->x&&food.y == judge->y)
		{
			goto lable;
		} 
		judge = judge->next;
	}
	
	/*gotoxy(food.x, food.y);
	printf("★");*/
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
		if (Judge()==0) return 0;//如果触碰到东西就结束 
		if (_kbhit()!=0)//这里是收取到的字符，这里要增加一个查找 
		{
			temp=_getch();
			if (strchr(dest,temp)!=0){
				if (strchr(sdest,temp)!=0){
					sclick=temp;
					
				}
				if (strchr(fdest,temp)!=0){
					fclick=temp;
					Sleep(85);
					Fmovingbody(fclick); 
					fclick=0; 
				}
				
			}
			
		}
		SmovingBody(sclick);
		//gotoxy(0,31) ;
		//printf("scmd=%c fcmd=%c",sclick,fclick);
		
		//Sleep(80); 
		if(flag){
			gotoxy(food.x, food.y);
			//color(0x04);
			printf("★");
			
			
		}
		
		Eating();
	}
	return 1;
}
/**********************************************************/
void SmovingBody(char c)
{
	int count = 0;
	int a = head->x, b = head->y;//a存储着header的x,y坐标。 
	 
	//通过先清空后打印实现动画效果
	clearSnake();
	switch (c)
	{
		
		case up://头坐标纵坐标减一 
			head->y -= 1;
			ChangeBody(a,b);
			break;
		case down://头坐标的纵坐标加以 
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
void clearSnake()
{
	int count;
	snake *p = head; 
	//通过先清空后打印实现动画效果
	while (1)//先清除蛇 
	{
		if (p->next == NULL) break;
		gotodelete(p->x, p->y);
		count++;
		p = p->next;
	}
}
void drawSnake()
{
	snake *p = head;
	/***************打印*************************/ 
	 while (1)
	{
		if (p->next == NULL) break;
		//color(0x05);
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
	//color(c);
	printf("%s",s);
}
void Fmovingbody(char c)
{
	
	int count = 0;
	//int a = head->x, b = head->y;//a存储着header的x,y坐标。 
	snake *p = head; 
	//通过先清空后打印实现动画效果
	
	Sleep(10);
	//gotodelete(food.x,food.y) ;
	switch (c)
	{
		case fup://头坐标纵坐标减一 
			food.y -= 1;
			//ChangeBody(a,b);
			break;
		case fdown://头坐标的纵坐标加以 
			food.y += 1;
			//ChangeBody(a,b);
			break;
		case fleft: 
			food.x -= 2;
			//ChangeBody(a,b);
			break;
		case fright:
			food.x += 2;
			
			//ChangeBody(a,b);
			break;
		
		case stop:
			_getch();
			break;
	}
	for (int i = 2; i<56; i += 2){
		for (int j = 1; j < 29; j++){
			gotodelete(i, j);
		}
	}
	int i;
	for (i = 0; i<58; i += 2)//打印上下边框
	{
		gotoprint(i, 0);
		gotoprint(i, 30);
	}
	for (i = 1; i < 30; i++)
	{
		gotoprint(0, i);
		gotoprint(56, i);
	}
	/*gotoxy(food.x, food.y);
	printf("★");*/ 
	drawSnake();
	gotoxy(0, 31);
	
	
	/***************打印*************************/ 
	 
	/*********************************************/ 
	
}
/**********************************************************/
void Eating()
{
	
	if (head->x == food.x && head->y == food.y+1)
	{
		
		creatfood();
		snake *_new = (snake*)malloc(sizeof(snake));
		snake *p;
		p = head;
		/***********扩展sneak*******************/ 
		while (1)
		{
			if (p->next->next == NULL) break;
			p = p->next;
		}
		p->next = _new;
		
		_new->next = tail;
		//score += 10;
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
	/*因为前面，没有连回去*/
	p->x = a;
	p->y = b;

}
/**********************************************************/
int Judge()//判断贪吃蛇是否碰到墙壁或者碰到自己 
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
	printf("GAME   OVER      ╯︿╰﹀");
	gotoxy(20, 16);
	printf("Your Score is %d    hiahiahia", score);
	gotoxy(18, 18);
	printf("还不错哦，     继续努力( > c < )");
	gotoxy(0, 27);
	system("pause"); 

}

void * getorder(void* args) 
{  
    //初始化wsa  
    WORD sockVision = MAKEWORD(2,2);  
    WSADATA wsadata;  
    //其他变量  
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
        printf("WSA初始化失败\n");  
        //return 0;  
    }  
  
    //创建套接字  
    slisten = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);  
    if(slisten == INVALID_SOCKET)  
    {  
        printf("socket监听者创建失败\n");  
        
    }  
  
    //绑定IP和端口  
    sin.sin_family = AF_INET;  
    sin.sin_port = htons(9000);  
    sin.sin_addr.S_un.S_addr = INADDR_ANY;  
    if( bind(slisten,(LPSOCKADDR)&sin,sizeof(sin)) == SOCKET_ERROR )  
    {  
        printf("绑定IP和端口\n");  
        //  
    }  
  
    //监听  
    if(listen(slisten,5) == SOCKET_ERROR)  
    {  
        printf("监听失败\n");  
        //return 0;  
    }  
  
    //循环接收数据  
    while(1)  
    {  
        //printf("等待连接.........\n");  
        sClient = accept(slisten,(SOCKADDR *)&remoteAddr,&nAddrlen);  
        if(sClient == INVALID_SOCKET)  
        {  
            printf("接受客户端失败，正在重试.........\n");  
            continue;  
        }  
        flag=1;
        //printf("接受客户端成功：%s\n",inet_ntoa(remoteAddr.sin_addr));  
        char sdest[]={'w','s','a','d','p'};
		char fdest[]={'i','j','k','l'};
        //接收数据  
        while(1)  
        {  
			ret = recv(sClient,revdata,255,0); 
            if(ret > 0)  
            {  
            
				revdata[ret] = 0x00; 
				if (strchr(sdest,revdata[0])!=0){
					sclick=revdata[0];
					
				}
				if (strchr(fdest,revdata[0])!=0){
					fclick=revdata[0];
					Fmovingbody(fclick);
					cmd=0; 
				}
				else if(revdata[0]<0) {
					score=(revdata[0]+50)*10;
				}
				else if(revdata[0]<60 ){
					food.x= revdata[0];
				} 
				else if(revdata[0]<90  ){
					food.y=revdata[0]-60;
				}   
				           
                
            }  
				
        }  
        closesocket(sClient);  
    }  
  
    closesocket(sClient);  
    WSACleanup();  
} 



