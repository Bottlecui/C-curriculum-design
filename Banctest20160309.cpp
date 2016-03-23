//#include"bankt.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<time.h>

FILE *fAcc, *fALog, *fUser, *fSLog;
time_t timep;
struct tm *tim;//系统时间调用必备
char* filename; 
int flag=1;

//账户结构体
struct Account//账户结构体
{
	char ID[11];//账户编号
	char pw[30];//账户密码
	char name[12];//账户姓名
	double saving;//存款余额
	char identity[25];//账户身份证号***
	char phone[15];//账户预留联系电话***
	struct Account *next;
	FILE *ThisLog;//本账户日志
};

struct Account *head;//所有账户链表首指针 

//读取本机时间
struct tm*getime(FILE* log)//读取本机时间 
{
	time(&timep);
	tim = localtime(&timep);
	fprintf(log,"%4d/%02d/%02d\n", (1900+tim->tm_year), (1+tim->tm_mon), tim->tm_mday);
	return tim;
 } 

//读取本机时间 无参类型(函数重载) 
struct tm*getime()//读取本机时间 无参类型(函数重载) 
{
	time(&timep);
	tim = localtime(&timep);
	return tim;
} 
 
//把客户信息写入内存 
struct Account* Loadin()//把客户信息写入内存 ......这个的p的空间要free掉吗？
{
	struct Account *head, *p, *q,*temp;
//	int temp=0;
	if(!(p=head=(struct Account*)malloc(sizeof(struct Account))))
	{
		printf("No enough memory.\n");
		return NULL;
	}

	fscanf(fAcc,"%s",head->ID);           //puts(head->ID);
	fscanf(fAcc,"%s",head->pw);           //puts(head->pw);
	fscanf(fAcc,"%s",head->name);         //puts(head->name);
	fscanf(fAcc,"%lf",&(head->saving));   //printf("%.2lf\n",head->saving);
	fscanf(fAcc, "%s", head->identity);   //puts(head->identity);
	fscanf(fAcc, "%s", head->phone);      //puts(head->phone);
	strcpy(filename,head->ID);
	strcat(filename,".txt");
	head->ThisLog=fopen(filename,"a+");
	head->next=NULL;
//fscanf(fAcc,"%d",&head->Ptype);
//	fseek(fAcc,3,1);//把指针从当前位置往后挪两个字符 
//	fscanf(fAcc,"%c",&temp);
//	printf("y%cy\n",temp);

	while(feof(fAcc)==0)//文件未结尾 
	{                                               //	printf("************\n");
		if(!(q=(struct Account*)malloc(sizeof(struct Account))))
		{
			printf("No enough memory.\n");
			return NULL;
		}
		q->next=NULL;
		fscanf(fAcc,"%s",q->ID);            //	puts(q->ID);
		fscanf(fAcc,"%s",q->pw);            //	puts(q->pw);
		fscanf(fAcc,"%s",q->name);          //	puts(q->name);
		fscanf(fAcc, "%s", q->identity);    //	puts(q->identity);
		fscanf(fAcc, "%s", q->phone);       //	puts(q->phone);
		fscanf(fAcc,"%lf",&(q->saving));    //	printf("%.2lf\n",q->saving);
		strcpy(filename,q->ID);
		strcat(filename,".txt");
		q->ThisLog=fopen(filename,"a+");
	//	temp='\0';
	//	fscanf(fAcc,"%d",&temp);
	//	printf("1y%dy\n",temp);
	//	fscanf(fAcc,"%d",&temp);
	//	printf("2y%dy\n",temp);
	//	fseek(fAcc,10,1);//把指针从当前位置往后挪两个字符 
		p->next=q;
		temp=p;
		p=q;
	//	system("pause");
	}
	
//	free(q);
//	temp->next=NULL;
//	system("pause");
	rewind(fAcc);//把指针指回文件首 
	return head;
}

//计算下一个ID号专用 
struct Account* add(struct Account*p)//计算下一个ID号专用 
{
	(*(p->ID+9))+=1;           
	for( int i=9;i>=0;i--)
	{
		if((*(p->ID+i))>'9')
		{
			(*(p->ID+i-1))+=1;
			(*(p->ID+i))-=10;
		}
	}
	return p;
}

//检查账户编号和密码，确认后返回账户链表指针 
struct Account* check()//检查账户编号和密码，确认后返回账户链表指针 。。。。我想问一下，flag好像是外部变量，这里改了，怎么改回去？
{
	char *nID, *npw;
	struct Account *p=head;
	if( !(nID=(char*)malloc(11*sizeof(char)))  || !(npw = (char*)malloc(30 * sizeof(char))) )  //******这里我改成逻辑或语句********
	{
		printf("内存不足\n");
		return NULL;
	}
	
	printf("****账户验证****\n按 q 退出账户验证\n****\n");
	printf("请输入账户编号：");
	fflush(stdin);
	gets(nID);
	if(*nID=='q')
	{
		flag=0;
		return NULL;
	}
	
	printf("密码：");
	fflush(stdin);
	gets(npw);
	do   //利用链表验证账户名与密码是否吻合
	{
		if((strcmp(p->ID,nID)==0)&&(strcmp(p->pw,npw)==0))
		{
			puts("验证成功！");
			tim=getime(p->ThisLog);
			fprintf(p->ThisLog,"%2d:%02d:%02d  登录成功\n",  tim->tm_hour, tim->tm_min, tim->tm_sec);
			tim=getime();
			fprintf(fSLog,"%2d:%02d:%02d  登录用户帐户 %s 成功\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,nID);
			return p;
		}
		else
		{
			p=p->next;
		}
		
	}while(p!=NULL);
	printf("账户编号或密码错误！\n");
	return NULL;
}

//搜索内容 
void Search()//搜索内容 
{
	system("cls");
	printf("查询模式\n");
	printf("请输入你要查找的内容\n");//这tm是要查什么？直接拉客户信息吗…… 
	struct Account *p=head;
	do
	{
		puts(p->ID);
		puts(p->name);
		puts(p->pw);
		printf("%.2lf\n",p->saving);
		p=p->next;
		printf("*********\n");
	}while(p);
	system("pause");
	system("cls");
	return;
}

//新开户 
void NewAcc()//新开户 
{
	struct Account *p = head, *q = NULL;
	char *name = NULL, *pw = NULL, *pw2 = NULL;//name户主姓名 
	char *iden = NULL, *phone = NULL;
	while (p->next)  p = p->next;// 指向链表尾稍后连接 

	if (!(q = (struct Account*)malloc(sizeof(struct Account))) || !(name = (char*)malloc(12 * sizeof(char))) || !(pw = (char*)malloc(30 * sizeof(char))) ||
		!(pw2 = (char*)malloc(30 * sizeof(char))) || !(iden = (char*)malloc(25 * sizeof(char))) || !(phone = (char*)malloc(15 * sizeof(char))))
	{
		printf("内存不足\n");
		system("pause");
		system("cls");
		return;
	}

	//输入姓名的界面设置
	while (1)            
	{
		system("cls");                        //界面顶部的显示
		printf("******新开账户******\n");
		printf("****按 q 退出****\n\n");
		printf("请输入账户人姓名：");
		fflush(stdin);
		gets(name);
		if (*name == 'q')    //退出键
		{
			system("pause");
			system("cls");
			free(q); free(name); free(pw); free(pw2); free(iden); free(phone); //*****清除开辟的空间******
			return;
		}
		else if (*name == NULL || sizeof(*name) > 12)   //判断姓名输入是否规范
		{
			printf("你输入的姓名不符合规范，请重新输入！");
			system("pause");
			continue;
		}
		else
		{
			printf("姓名已输入"); break;
		}
	}

	//输入密码
	while (1)
	{
		printf("\n请输入账户密码（6至15个字符）：");    //这里我具体限制了一下密码的字符个数
		fflush(stdin);
		gets(pw);
	/*	if (*pw == 'q')   //退出键
		{
			system("pause");
			system("cls");
			free(q); free(name); free(pw); free(pw2); free(iden); free(phone); //*****清除开辟的空间******
			return;
		}
		else */
		if (strlen(pw) < 6 || strlen(pw) > 15)    //密码不符合规范就要重新来过
		{
			printf("密码不符合规范！\n");
			system("pause");
			system("cls");
			continue;
		}

		printf("\n请再次输入账户密码：");
		fflush(stdin);
		gets(pw2);
	/*	if (*pw2 == 'q')   //退出键
		{
			system("pause");
			system("cls");
			free(q); free(name); free(pw); free(pw2); free(iden); free(phone); //*****清除开辟的空间******
			return;
		}
		else
		*/
		 if (!strcmp(pw, pw2))   //前后密码一致
		{
			printf("密码一致!\n");break;
		}
		else
		{
			printf("两次密码不一致！请重新输入\n");
			system("pause");
			system("cls");
			continue;
		}
	}

	//输入身份证
	while (1)
	{
		printf("\n请输入账户的身份证号码：");
		fflush(stdin);
		gets(iden);
		if (*iden == 'q')    //退出键
		{
			system("pause");
			system("cls");
			free(q); free(name); free(pw); free(pw2); free(iden); free(phone); //*****清除开辟的空间******
			return;
		}
		else if (*iden == NULL || sizeof(*iden) > 25)   //判断姓名输入是否规范
		{
			printf("\n你输入的身份证号不符合规范，请重新输入！");
			system("pause");
			system("cls");
			continue;
		}
		else
		{
			printf("\n身份证已输入！");break;
		}
	}

	//输入预留手机
	while (1)
	{
		printf("\n请输入账户的预留手机号码：");
		fflush(stdin);
		gets(phone);
		if (*phone == 'q')    //退出键
		{
			system("pause");
			system("cls");
			free(q); free(name); free(pw); free(pw2); free(iden); free(phone); //*****清除开辟的空间******
			return;
		}
		else if (*phone == NULL || sizeof(*phone) > 15)   //判断姓名输入是否规范
		{
			printf("\n你输入的预留手机号码不符合规范，请重新输入！");
			system("pause");
			system("cls");
			continue;
		}
		else
		{
			printf("\n预留手机号码已输入！");break;
		}
	}

	printf("正在创建，请稍等......\n");
	strcpy(q->ID, p->ID);//先把ID接过来再加 
//	printf("now ID\n");
//	puts(q->ID);
	q = add(q);
	strcpy(filename, q->ID);
	strcat(filename, ".txt");
//	puts(filename);

	fALog = fopen(filename, "a+");//如果需要把用户以往的日志输出，需要改变此文件的读写方式 
	q->ThisLog = fALog;//把日志文件的指针给过来 
	fALog = NULL;

	strcpy(q->name, name);
	strcpy(q->pw, pw);
	strcpy(q->identity, iden);
	strcpy(q->phone, phone);

	fseek(fAcc, 0, 2);//把fAcc挪到文件末尾，开始在文件内写入数据
	fprintf(fAcc, "\n%s\n%s\n%s\n0.0\n%s\n%s", q->ID, q->pw, q->name, q->identity, q->phone);//写入格式：先换行 
	rewind(fAcc);//指回文件首 
	p->next = q;
	q->next = NULL;

	tim = getime(q->ThisLog);
	fprintf(q->ThisLog, "%2d:%02d:%02d  账户新建成功\n", tim->tm_hour, tim->tm_min, tim->tm_sec);
	tim = getime();
	fprintf(fSLog, "%2d:%02d:%02d  新建账户 %s 成功\n", tim->tm_hour, tim->tm_min, tim->tm_sec, q->ID);

	system("cls");   //*****我添加上来的，整理一下界面*****
	printf("******\n新建账户完成\n******\n账户编号：%s\n账户姓名：%s\n账户身份证号码：%s\n账户预留手机号码：%s", q->ID, q->name, q->identity, q->phone);
	printf("\n******请牢记您的账户编号和密码******\n");
	system("pause");
	system("cls");
	///这里应该把新建的账户信息存起来，在文件存一次在链表添一个 
}

//删除账户 
void DelAcc()//删除账户 
{
	struct Account* nID=NULL, *former=head;//former为要删除的链节前一链节 
	double saving;
	FILE* del;//新建一个文件，把源文件信息进行重写 
	char ch;
	while(!nID&&flag)
	nID=check();//检查账户密码
	if(!flag)//需要退出
	{
		system("pause");
		flag=1;
		system("cls");
		return;
	 }  
	saving=nID->saving;
	printf("确实要删除该账户及其所有信息？该操作不可撤销\n");
	printf("********\ny：仍然删除\nn：不删除\n按相应字母执行操作\n");
	fflush(stdin);
	ch=getchar();
	if(ch=='y')//删除操作 
	{
		printf("正在删除......\n");
		del=fopen("temp.txt","w");
		struct Account temp;
		fscanf(fAcc,"%s\n",temp.ID);
		while(!feof(fAcc)) //文件操作，重写新文件 
		{    //原文件未结尾 
			fscanf(fAcc,"%s\n%s\n%lf",temp.pw,temp.name,&temp.saving);
			if(strcmp(temp.ID,nID->ID))//判断出这个不是要删的账户
			{
				fprintf(del,"%s\n%s\n%s\n%.2f",temp.ID,temp.pw,temp.name,temp.saving);
			} 
			fscanf(fAcc,"%s\n",temp.ID);
			if(feof(fAcc)||!(strcmp(temp.ID,nID->ID)))
			{
				;
			}
			else
			{
				fprintf(del,"\n");
			}
		//	fseek(fAcc,2,1);//从当前位置往后挪两个字符 ，当到文件末尾时正好退出循环 
		}
		fclose(fAcc);
		fclose(del);//先关掉两个文件
		fclose(nID->ThisLog);
		remove("account.txt");
		rename("temp.txt","account.txt");//狸猫换太子
		fAcc=fopen("account.txt","r+");//把fAcc指针再给置回来 
		memset(filename,0,256);
		strcpy(filename,nID->ID);
		strcat(filename,".txt");
		tim=getime();
		fprintf(fSLog,"%2d:%02d:%02d  账户 %s 已注销\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,nID->ID);
		if(!strcmp(former->ID,nID->ID))//要删除的账户为链表头
		{
			fclose(head->ThisLog);
			head=head->next;
			free(former);//把原链表头释放掉 
		} 
		else//要删的账户非链表头 
		{ 
			fclose(nID->ThisLog);
			while(strcmp((former->next)->ID,nID->ID))//former便为待删除账户的前一个账户 
			former=former->next;
			former->next=nID->next;//连接链节
			free(nID);//释放原链节空间 
		} 
		remove(filename); //删除此账户日志？？要删吗？还是往里写一句此账户已删除？ 
		
		printf("删除成功！\n应退回现金%.2f 元\n",saving);
		system("pause");
		system("cls");
		return;
		//先把文件里的删了，再删链表 
		//记得把saving里面的钱退出来 
	}
	else 
	{
		printf("未删除\n");
		tim=getime(nID->ThisLog);
		fprintf(nID->ThisLog,"%2d:%02d:%02d  删除账户操作取消",  tim->tm_hour, tim->tm_min, tim->tm_sec);
		tim=getime();
		fprintf(fSLog,"%2d:%02d:%02d  删除账户操作取消",  tim->tm_hour, tim->tm_min, tim->tm_sec); 
		system("pause");
		system("cls");
		return;
	}
}

//存款 
void Save()//存款 
{
	struct Account* nID=NULL, temp;
	double saving;
	FILE *change;
	change=fopen("temp.txt","w");//新开一个文件准备重写 
	while(!nID&&flag)
	nID=check();//检查账户密码
	if(!flag)//需要退出
	{
		system("pause");
		flag=1;
		system("cls");
		return;
	 } 
	printf("请输入存款金额：");
	scanf("%lf",&saving);
	nID->saving+=saving;
	
	while(!feof(fAcc))
	{
		fscanf(fAcc,"%s\n%s\n%s\n%lf",temp.ID,temp.pw,temp.name,&temp.saving);
		if(!strcmp(nID->ID,temp.ID))//temp此时为要改动的账户
		{
			temp.saving=nID->saving;
		} 
		fprintf(change,"%s\n%s\n%s\n%.2lf",temp.ID,temp.pw,temp.name,temp.saving);//打印时已经打好两次换行
		if(!feof(fAcc))
		{
			fprintf(change,"\n");
		}
		//fseek(fAcc,2,1);//从当前位置往后挪两个字符 ，当到文件末尾时正好退出循环
	}
	rewind(fAcc);//指回文件首
	fclose(fAcc);
	fclose(change);
	remove("account.txt");
	rename("temp.txt","account.txt"); //狸猫换太子 
	fAcc=fopen("account.txt","r+");//把fAcc指针再给置回来
	//以上为文件内存款变动
	tim = getime(nID->ThisLog);
	fprintf(nID->ThisLog,"%2d:%02d:%02d  存入 %.2f 元\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,saving);
	tim=getime();
	fprintf(fSLog,"%2d:%02d:%02d  账户 %s 存入 %.2f 元\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,nID->ID,saving);
	printf("成功\n");
	printf("\n已存入金额：%.2f\t此账户现存款：%.2f\n",saving,nID->saving);
	system("pause");
	system("cls");
	return;
}

//取款 
void Withdraw()//取款 
{
	struct Account* nID=NULL, temp;
	double withdraws;
	FILE* change;
	change=fopen("temp.txt","w");//开新文件重写（累不累 
	while(!nID&&flag)
		nID=check();//检查账户密码
	if(!flag)//需要退出
	{
		system("pause");
		flag=1;
		system("cls");
		return;
	 } 
	while(1)
	{
		printf("请输入取款金额（0-%.2f)：",nID->saving);
		scanf("%lf",&withdraws);
		if( withdraws > nID->saving )
		{
			printf("金额超出余额！\n");
		} 
		else
		{
			printf("取款%.2f元，请确认金额\n",withdraws);
			system("pause");
			nID->saving -= withdraws;
			while(!feof(fAcc))
			{
				fscanf(fAcc,"%s\n%s\n%s\n%lf",temp.ID,temp.pw,temp.name,&temp.saving);
				if(strcmp(nID->ID,temp.ID)==0)//temp此时为要改动的账户
				{
					temp.saving=nID->saving;
				} 
				fprintf(change,"%s\n%s\n%s\n%.2f",temp.ID,temp.pw,temp.name,temp.saving);//打印时已经打好两次换行
				if(!feof(fAcc))
				{
					fprintf(change,"\n");
				}
			//	fseek(fAcc,2,1);//从当前位置往后挪两个字符 ，当到文件末尾时正好退出循环
			}
			tim=getime(nID->ThisLog);
			fprintf(nID->ThisLog,"%2d:%02d:%02d  取款 %.2f元\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,withdraws);
			tim=getime();
			fprintf(fSLog,"%2d:%02d:%02d  账户 %s 取款 %.2f元\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,nID->ID,withdraws);
			fclose(fAcc);
			fclose(change);
			remove("account.txt");
			rename("temp.txt","account.txt"); //狸猫换太子 
			fAcc=fopen("account.txt","r+");//把fAcc指针再给置回来
			//文件内存款变动 
			printf("取款成功！\n取款 %.2f元\n账户现余额 %.2f元\n",withdraws,nID->saving);
			system("pause");
			system("cls");
			return;
		}
	}
}

//转账
void Transfer()
{
	struct Account* nID=NULL, *target=head,temp;
	char tID[11],ch;
	double amount;
	FILE *change;
	change=fopen("temp.txt","w");//创建临时文件重写 
	while(!nID&&flag)
	nID=check();//检查账户密码
	if(!flag)//需要退出
	{
		system("pause");
		flag=1;
		system("cls");
		return;
	 } 
	while(1)
	{
		printf("请输入转入账户编号（10位）：");
		fflush(stdin);
		gets(tID);	
		while(target)
		{
		//	printf("***\n");
			if(strcmp(tID,target->ID)==0)
			{
				printf("success!\n");
				break;
			}
			target=target->next;
		}
		if(target==NULL)
		printf("该账户不存在！请核对后重新输入！\n");
		else
		break;
	}
	
	while(1)
	{
		printf("请输入你要转账的金额（0-%.2f）：",nID->saving);
		scanf("%lf",&amount);
		if(amount>nID->saving)
		{
			printf("金额超出！\n");
		}
		else
		{
			printf("即将从您的账户 %s 转账 %.2f元 到账户 %s 目标账户姓名：%s \n核对无误后请按y,有误按其他键重新输入：",nID->ID,amount,target->ID,target->name);
			fflush(stdin);
			ch=getchar();
			if(ch=='y')
			{
				printf("执行中......\n");
				nID->saving-=amount;
				target->saving+=amount;
				while(!feof(fAcc)) 
				{
					fscanf(fAcc,"%s\n%s\n%s\n%lf",temp.ID,temp.pw,temp.name,&temp.saving);
					if(strcmp(temp.ID,nID->ID)==0)//这是转出账户
					{
						temp.saving=nID->saving;
					}
					else if(strcmp(temp.ID,target->ID)==0)//这是转入账户
					{
						temp.saving=target->saving;
					}
					fprintf(change,"%s\n%s\n%s\n%.2lf",temp.ID,temp.pw,temp.name,temp.saving);//打印时已经打好两次换行
					if(!feof(fAcc))
					{
						fprintf(change,"\n");
					}
				}
				tim=getime(nID->ThisLog);
				fprintf(nID->ThisLog,"%2d:%02d:%02d  转账给 %s 姓名 %s %.2f元\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,target->ID,target->name,amount);
				tim=getime(target->ThisLog);
				fprintf(target->ThisLog,"%2d:%02d:%02d  收到 %s 姓名 %s 转来 %.2f元\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,nID->ID,nID->name,amount);
				tim=getime();
				fprintf(fSLog,"%2d:%02d:%02d  从账户 %s 转到账户 %s %.2f元\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,nID->ID,target->ID,amount);
				fclose(fAcc);
				fclose(change);
				remove("account.txt");
				rename("temp.txt","account.txt"); //狸猫换太子 
				fAcc=fopen("account.txt","r+");//把fAcc指针再给置回来
				//文件操作
				printf("******完成\n您的帐户当前余额：%.2f\n",nID->saving);
				system("pause");
				system("cls");
				return; 
			}
			
		}
	}
}

//修改密码与找回密码的共同部分（但是在日志修改上，找回密码和修改密码是一样的，还没有没有区分）
struct Account* ChangePwPart(struct Account *p)
{
	struct Account* nID = p, temp;
	FILE* change;
	change = fopen("temp.txt", "w");//开新文件重写
	char *pw = NULL, *pw2 = NULL;//更换的密码
	if (!(pw = (char*)malloc(30 * sizeof(char))) || !(pw2 = (char*)malloc(30 * sizeof(char))))//记得要free掉
	{
		printf("内存不足\n");
		system("pause");
		system("cls");
		return NULL;
	}

	while (1)//设置新密码
	{
		system("cls");
		printf("*******修改密码*******\n*******按q退出*******\n");
		printf("账户ID：%s\t账户名：%s\n", nID->ID, nID->name);

		printf("\n请输入您的账户密码（6至15个字符）：");    //这里我具体限制了一下密码的字符个数
		fflush(stdin);
		gets(pw);
		if (*pw == 'q')   //退出键
		{
			system("pause");
			system("cls");
			free(pw); free(pw2); //*****清除开辟的空间******
			return NULL;
		}
		else if (strlen(pw) < 6 || strlen(pw) > 15)    //密码不符合规范就要重新来过
		{
			printf("密码不符合规范！\n");
			system("pause");
			continue;
		}

		printf("\n请再次输入您的密码：");             //确认密码
		fflush(stdin);
		gets(pw2);
		if (*pw2 == 'q')   //退出键
		{
			system("pause");
			system("cls");
			free(pw); free(pw2); //*****清除开辟的空间******
			return NULL;
		}
		else if (!strcmp(pw, pw2))   //前后密码一致
		{
			printf("密码一致！"); break;
		}
		else
		{
			printf("两次密码不一致！请重新输入\n");
			system("pause");
			continue;
		}
	}//设置新密码
	printf("请稍等，正在修改...");
	strcpy(nID->pw, pw);//覆盖之前的密码

						//日志上的修改
	while (!feof(fAcc))
	{
		fscanf(fAcc, "%s\n%s\n%s\n%s\n%s\n%lf", temp.ID, temp.pw, temp.name, temp.identity, temp.phone, &temp.saving);
		if (strcmp(nID->ID, temp.ID) == 0)//temp此时为要改动的账户
		{
			strcpy(temp.pw, nID->pw);//temp.pw = nID->pw;好笨啊！一开始写这个的。。。
		}
		fprintf(change, "%s\n%s\n%s\n%s\n%s\n%.2f", temp.ID, temp.pw, temp.name, temp.identity, temp.phone, temp.saving);//打印时已经打好两次换行
		if (!feof(fAcc))
		{
			fprintf(change, "\n");
		}
		//	fseek(fAcc,2,1);//从当前位置往后挪两个字符 ，当到文件末尾时正好退出循环
	}
	tim = getime(nID->ThisLog);
	fprintf(nID->ThisLog, "%2d:%02d:%02d  修改密码--新密码：%s\n", tim->tm_hour, tim->tm_min, tim->tm_sec, nID->pw);
	tim = getime();
	fprintf(fSLog, "%2d:%02d:%02d  账户 %s 修改密码--新密码：%s\n", tim->tm_hour, tim->tm_min, tim->tm_sec, nID->ID, nID->pw);
	fclose(fAcc);
	fclose(change);
	remove("account.txt");
	rename("temp.txt", "account.txt"); //狸猫换太子 
	fAcc = fopen("account.txt", "r+");//把fAcc指针再给置回来
									  //文件内密码变动 
	free(pw); free(pw2);//回收空间
	printf("\n密码修改成功！\n按任意键退出...");
	system("pause");
	system("cls");
	return NULL;

}

//修改密码(只要用旧密码)
void ChangePw()//修改密码
{
	struct Account* nID = NULL;
	while (!nID&&flag)
		nID = check();//检查账户密码
	if (!flag)//需要退出
	{
		system("pause");
		flag = 1;
		system("cls");
		return;
	}
	if (ChangePwPart(nID) == NULL)   //调用密码公共函数
	{
		return;
	}
}

//找回密码（用编号、姓名、身份证号、预留手机号联合验证，只能用着用简单的方法代替着用着了）
void FindPw()//其实我还想实现可以中途重新输入的功能，可是。。。你懂的
{
	char *nID, *name, *identity, *phone;//验证条件目前就只能用账号，姓名、身份证号、手机号，什么短信验证这些就先忽略吧
	struct Account *p = head;
	if (!(nID = (char*)malloc(11 * sizeof(char))) || !(identity = (char*)malloc(25 * sizeof(char))) ||
		!(phone = (char*)malloc(15 * sizeof(char))) || !(name = (char*)malloc(12 * sizeof(char))))  //******这里我改成逻辑或语句********
	{
		printf("内存不足\n");
		system("pause");
		return ;
	}

	system("cls");
	printf("*******找回密码******");
	printf("****账户验证****\n按 q 退出账户验证\n****\n");

	{//输入四个条件
		printf("请输入账户编号：");
		fflush(stdin);
		gets(nID);
		if ( *nID == 'q')
		{
			return;
		}
		printf("请输入账户人姓名：");
		fflush(stdin);
		gets(name);
		if ( *name == 'q' )
		{
			return;
		}
		printf("请输入账户人身份证号码：");
		fflush(stdin);
		gets(identity);
		if ( *identity == 'q')
		{
			return;
		}
		printf("请输入账户人预留手机号码：");
		fflush(stdin);
		gets(phone);
		if (*phone == 'q' )
		{
			return;
		}
	}
	do   //利用链表验证四个条件是否吻合
	{
		if ((strcmp(p->ID, nID) == 0) && (strcmp(p->name, name) == 0) &&                      //验证成功，终于实现了修改密码与找回密码功能重叠部分的代码提取
			(strcmp(p->identity, identity) == 0) && (strcmp(p->phone, phone) == 0))        
		{
			printf("验证成功！按任意键开始设置新密码...");
			system("pause");

			free(identity); free(nID); free(name); free(phone);  //回收空间
			if (ChangePwPart(p) == NULL)     //调用修改密码和找回密码的共同功能
			{
				return;
			}
		}
		else
		{
			p = p->next;
		}

	} while (p != NULL);
	printf("输入错误！\n按任意键退出");
	free(identity); free(nID); free(name); free(phone);//回收空间
	system("pause");
	return;
}
 


//主函数
main()
{
	char name[8],pswd[6];
	fAcc=fopen("account.txt","r+");
	fUser=fopen("user.txt","r+");
	fSLog=fopen("syslog.txt","a");//系统日志被定义为只在文件末尾追加的类型 
	printf("读取中，请稍候……\n");

	if(!(filename=(char*)malloc(256*sizeof(char))))
	{
		printf("内存不足\n");
		return 0;
	}
	tim=getime();
//	fprintf(fSLog,"%4d/%02d/%02d\n", (1900+tim->tm_year), (1+tim->tm_mon), tim->tm_mday);
//	fprintf(fSLog,"%2d:%02d:%02d  打开程序，读取用户数据\n",  tim->tm_hour, tim->tm_min, tim->tm_sec);
	head=Loadin();//把已存信息写入链表，链表头head 
//	system("pause");
	tim=getime();
//	fprintf(fSLog,"%2d:%02d:%02d  读取数据成功\n",  tim->tm_hour, tim->tm_min, tim->tm_sec);
	
	system("cls");//清除屏幕 
	while(1)
	{
		printf("请输入用户名和密码：\n");
		printf("用户名：");
		scanf("%s",name);
		printf("密码：");
		scanf("%s",pswd);
		tim=getime();
		fprintf(fSLog,"%4d/%02d/%02d\n", (1900+tim->tm_year), (1+tim->tm_mon), tim->tm_mday);
		fprintf(fSLog,"%2d:%02d:%02d  进行身份验证\n",  tim->tm_hour, tim->tm_min, tim->tm_sec);
		int flag=0;
		char namec[8],pswdc[6];
		while(!feof(fUser))//查找管理或员工信息，核实用户名与密码，核实成功后退出while
		{
			fscanf(fUser,"%s%s",namec,pswdc);
			if((!strcmp(name,namec))&&(!strcmp(pswd,pswdc)))
			{
				printf("Success!\n");system("pause");flag=1;break;
			}
		}
		if(!flag)
		{
			rewind(fUser);//把fUser重置到文件首 
			printf("用户名或密码错误！\n");
			tim=getime();
			fprintf(fSLog,"%2d:%02d:%02d  身份验证失败，重新验证\n",  tim->tm_hour, tim->tm_min, tim->tm_sec);
		}
		else
		{
			tim=getime();
			fprintf(fSLog,"%2d:%02d:%02d  身份验证成功\n          登录者：%s\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,name);
			break;
		}
	}
	system("cls");
	printf("------欢迎使用本系统------\n");
	char ch;
	if(*name=='1')		//管理员菜单 
	{
		while(1)
		{
			printf("当前用户：%s\t身份：管理员\n",name);
			printf("菜单\n");
			printf("1:查询\n2：修改密码\n3：找回密码\n4:设置管理员\n5:设置操作员\nq:退出程序");  //*****修改*****
			printf("请按相应数字选择对应功能：");
			fflush(stdin);
			ch=getchar();
			switch(ch)
			{
				case '1':Search();break;
				case '2':ChangePw(); break;
				case '3':FindPw(); break;
				case '4':
				case '5':printf("***对不起，功能尚未完善***\n按任意键返回..."); system("pause"); break;
				case 'q':tim = getime(); fprintf(fSLog, "%2d:%02d:%02d  退出程序\n", tim->tm_hour, tim->tm_min, tim->tm_sec);
					printf("\n****再见****\n****\n"); system("pause"); return 0;
				default:printf("输入有误！请重新输入\n"); system("pause"); system("cls");
			}
		}

	} 
	else          // 操作员的界面
	{
		while(1)
		{
			printf("当前用户：%s\t身份：操作员\n",name);
			printf("菜单\n");
			printf("1：查询\n2：开户\n3：销户\n4：存款\n5：取款\n6：转账\nq：退出程序\n");
			printf("请按相应数字选择对应功能：");
			fflush(stdin);
			ch=getchar();
			switch(ch)
			{
				case '1':Search();break;
				case '2':NewAcc();break;
				case '3':DelAcc();break;
				case '4':Save();break;
				case '5':Withdraw();break;
				case '6':Transfer();break;
				case 'q':tim=getime();fprintf(fSLog,"%2d:%02d:%02d  退出程序\n",  tim->tm_hour, tim->tm_min, tim->tm_sec);
					     printf("\n****再见****\n****\n");system("pause");return 0;
				default:printf("输入有误！请重新输入\n");system("pause");system("cls");
			}
		}
	}
}

//我觉得咱们在日志上的写法不是很合理，如果有时间在改改吧
//我发现修改密码那两项没有跳转页面。。。。肿么办
