#include<stdio.h>
#include<string.h>
//网络编程头文件
#include<WinSock2.h>
//引入静态库
#pragma comment(lib,"ws2_32.lib")

void sendhtml(SOCKET s,char* filename);

int merror(int redata,int error, char* showinfo) 
{
    if(redata == error)
    {
        perror(showinfo);
        getchar();
        return -1;
    }
    return 0;
}


int main()
{
    printf("欢迎使用webservice\n");
    WSADATA wsdata;
    //确定socket版本信息
    int isok=WSAStartup(MAKEWORD(2,2),&wsdata);
    //验证失败
    merror(isok,WSAEINVAL,(char*)"申请socket失败");
    //申请ipv4地址
    SOCKET service=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    //验证失败
    merror(service,INVALID_SOCKET,(char*)"创建socket失败");

     struct sockaddr_in seraddr;

    seraddr.sin_family=AF_INET;
    seraddr.sin_port=htons(80);
    seraddr.sin_addr.s_addr=INADDR_ANY;

    int length=sizeof(struct sockaddr_in);
    isok=bind(service,(struct sockaddr *)&seraddr,length);
    merror(isok,SOCKET_ERROR,(char*)"绑定失败");

    isok=listen(service,5);
    merror(isok,SOCKET_ERROR,(char*)"监听失败");

    struct sockaddr_in claddr;
    int cllen=sizeof(claddr);
    while (1)
    {
        /* code */
         SOCKET client=accept(service,(struct sockaddr *)&claddr,&cllen);
         merror(client,INVALID_SOCKET,(char*)"连接失败");

         char revdata[1024]="";
         recv(client,revdata,1024,0);
         printf("%s 共接收到%d个字节数据\n",revdata,strlen(revdata));


        //char sendata[1024]="<h1 style=\" color:blue;\">吴忠辉 </h1>";
        //send(client,sendata,strlen(sendata),0);
        char* filename=(char*)"./index.html";
        sendhtml(client,filename);
        closesocket(client);

    }
    closesocket(service);
    WSACleanup();
    
    getchar();
    return 0;
}

void sendhtml(SOCKET s,char* filename)
{
    FILE* pfile=fopen(filename,"r");
    if(pfile==NULL)
    {
        printf("打开文件失败\n");
        return;
    }
    char temp[1024]="";
    do{
    fgets(temp,1024,pfile);
    send(s,temp,strlen(temp),0);
    }while (!feof(pfile));
}