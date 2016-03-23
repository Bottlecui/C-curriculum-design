//#include"bankt.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>
#include<time.h>

FILE *fAcc, *fALog, *fUser, *fSLog;
time_t timep;
struct tm *tim;//ϵͳʱ����ñر�
char* filename; 
int flag=1;

//�˻��ṹ��
struct Account//�˻��ṹ��
{
	char ID[11];//�˻����
	char pw[30];//�˻�����
	char name[12];//�˻�����
	double saving;//������
	char identity[25];//�˻����֤��***
	char phone[15];//�˻�Ԥ����ϵ�绰***
	struct Account *next;
	FILE *ThisLog;//���˻���־
};

struct Account *head;//�����˻�������ָ�� 

//��ȡ����ʱ��
struct tm*getime(FILE* log)//��ȡ����ʱ�� 
{
	time(&timep);
	tim = localtime(&timep);
	fprintf(log,"%4d/%02d/%02d\n", (1900+tim->tm_year), (1+tim->tm_mon), tim->tm_mday);
	return tim;
 } 

//��ȡ����ʱ�� �޲�����(��������) 
struct tm*getime()//��ȡ����ʱ�� �޲�����(��������) 
{
	time(&timep);
	tim = localtime(&timep);
	return tim;
} 
 
//�ѿͻ���Ϣд���ڴ� 
struct Account* Loadin()//�ѿͻ���Ϣд���ڴ� ......�����p�Ŀռ�Ҫfree����
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
//	fseek(fAcc,3,1);//��ָ��ӵ�ǰλ������Ų�����ַ� 
//	fscanf(fAcc,"%c",&temp);
//	printf("y%cy\n",temp);

	while(feof(fAcc)==0)//�ļ�δ��β 
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
	//	fseek(fAcc,10,1);//��ָ��ӵ�ǰλ������Ų�����ַ� 
		p->next=q;
		temp=p;
		p=q;
	//	system("pause");
	}
	
//	free(q);
//	temp->next=NULL;
//	system("pause");
	rewind(fAcc);//��ָ��ָ���ļ��� 
	return head;
}

//������һ��ID��ר�� 
struct Account* add(struct Account*p)//������һ��ID��ר�� 
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

//����˻���ź����룬ȷ�Ϻ󷵻��˻�����ָ�� 
struct Account* check()//����˻���ź����룬ȷ�Ϻ󷵻��˻�����ָ�� ��������������һ�£�flag�������ⲿ������������ˣ���ô�Ļ�ȥ��
{
	char *nID, *npw;
	struct Account *p=head;
	if( !(nID=(char*)malloc(11*sizeof(char)))  || !(npw = (char*)malloc(30 * sizeof(char))) )  //******�����Ҹĳ��߼������********
	{
		printf("�ڴ治��\n");
		return NULL;
	}
	
	printf("****�˻���֤****\n�� q �˳��˻���֤\n****\n");
	printf("�������˻���ţ�");
	fflush(stdin);
	gets(nID);
	if(*nID=='q')
	{
		flag=0;
		return NULL;
	}
	
	printf("���룺");
	fflush(stdin);
	gets(npw);
	do   //����������֤�˻����������Ƿ��Ǻ�
	{
		if((strcmp(p->ID,nID)==0)&&(strcmp(p->pw,npw)==0))
		{
			puts("��֤�ɹ���");
			tim=getime(p->ThisLog);
			fprintf(p->ThisLog,"%2d:%02d:%02d  ��¼�ɹ�\n",  tim->tm_hour, tim->tm_min, tim->tm_sec);
			tim=getime();
			fprintf(fSLog,"%2d:%02d:%02d  ��¼�û��ʻ� %s �ɹ�\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,nID);
			return p;
		}
		else
		{
			p=p->next;
		}
		
	}while(p!=NULL);
	printf("�˻���Ż��������\n");
	return NULL;
}

//�������� 
void Search()//�������� 
{
	system("cls");
	printf("��ѯģʽ\n");
	printf("��������Ҫ���ҵ�����\n");//��tm��Ҫ��ʲô��ֱ�����ͻ���Ϣ�𡭡� 
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

//�¿��� 
void NewAcc()//�¿��� 
{
	struct Account *p = head, *q = NULL;
	char *name = NULL, *pw = NULL, *pw2 = NULL;//name�������� 
	char *iden = NULL, *phone = NULL;
	while (p->next)  p = p->next;// ָ������β�Ժ����� 

	if (!(q = (struct Account*)malloc(sizeof(struct Account))) || !(name = (char*)malloc(12 * sizeof(char))) || !(pw = (char*)malloc(30 * sizeof(char))) ||
		!(pw2 = (char*)malloc(30 * sizeof(char))) || !(iden = (char*)malloc(25 * sizeof(char))) || !(phone = (char*)malloc(15 * sizeof(char))))
	{
		printf("�ڴ治��\n");
		system("pause");
		system("cls");
		return;
	}

	//���������Ľ�������
	while (1)            
	{
		system("cls");                        //���涥������ʾ
		printf("******�¿��˻�******\n");
		printf("****�� q �˳�****\n\n");
		printf("�������˻���������");
		fflush(stdin);
		gets(name);
		if (*name == 'q')    //�˳���
		{
			system("pause");
			system("cls");
			free(q); free(name); free(pw); free(pw2); free(iden); free(phone); //*****������ٵĿռ�******
			return;
		}
		else if (*name == NULL || sizeof(*name) > 12)   //�ж����������Ƿ�淶
		{
			printf("����������������Ϲ淶�����������룡");
			system("pause");
			continue;
		}
		else
		{
			printf("����������"); break;
		}
	}

	//��������
	while (1)
	{
		printf("\n�������˻����루6��15���ַ�����");    //�����Ҿ���������һ��������ַ�����
		fflush(stdin);
		gets(pw);
	/*	if (*pw == 'q')   //�˳���
		{
			system("pause");
			system("cls");
			free(q); free(name); free(pw); free(pw2); free(iden); free(phone); //*****������ٵĿռ�******
			return;
		}
		else */
		if (strlen(pw) < 6 || strlen(pw) > 15)    //���벻���Ϲ淶��Ҫ��������
		{
			printf("���벻���Ϲ淶��\n");
			system("pause");
			system("cls");
			continue;
		}

		printf("\n���ٴ������˻����룺");
		fflush(stdin);
		gets(pw2);
	/*	if (*pw2 == 'q')   //�˳���
		{
			system("pause");
			system("cls");
			free(q); free(name); free(pw); free(pw2); free(iden); free(phone); //*****������ٵĿռ�******
			return;
		}
		else
		*/
		 if (!strcmp(pw, pw2))   //ǰ������һ��
		{
			printf("����һ��!\n");break;
		}
		else
		{
			printf("�������벻һ�£�����������\n");
			system("pause");
			system("cls");
			continue;
		}
	}

	//�������֤
	while (1)
	{
		printf("\n�������˻������֤���룺");
		fflush(stdin);
		gets(iden);
		if (*iden == 'q')    //�˳���
		{
			system("pause");
			system("cls");
			free(q); free(name); free(pw); free(pw2); free(iden); free(phone); //*****������ٵĿռ�******
			return;
		}
		else if (*iden == NULL || sizeof(*iden) > 25)   //�ж����������Ƿ�淶
		{
			printf("\n����������֤�Ų����Ϲ淶�����������룡");
			system("pause");
			system("cls");
			continue;
		}
		else
		{
			printf("\n���֤�����룡");break;
		}
	}

	//����Ԥ���ֻ�
	while (1)
	{
		printf("\n�������˻���Ԥ���ֻ����룺");
		fflush(stdin);
		gets(phone);
		if (*phone == 'q')    //�˳���
		{
			system("pause");
			system("cls");
			free(q); free(name); free(pw); free(pw2); free(iden); free(phone); //*****������ٵĿռ�******
			return;
		}
		else if (*phone == NULL || sizeof(*phone) > 15)   //�ж����������Ƿ�淶
		{
			printf("\n�������Ԥ���ֻ����벻���Ϲ淶�����������룡");
			system("pause");
			system("cls");
			continue;
		}
		else
		{
			printf("\nԤ���ֻ����������룡");break;
		}
	}

	printf("���ڴ��������Ե�......\n");
	strcpy(q->ID, p->ID);//�Ȱ�ID�ӹ����ټ� 
//	printf("now ID\n");
//	puts(q->ID);
	q = add(q);
	strcpy(filename, q->ID);
	strcat(filename, ".txt");
//	puts(filename);

	fALog = fopen(filename, "a+");//�����Ҫ���û���������־�������Ҫ�ı���ļ��Ķ�д��ʽ 
	q->ThisLog = fALog;//����־�ļ���ָ������� 
	fALog = NULL;

	strcpy(q->name, name);
	strcpy(q->pw, pw);
	strcpy(q->identity, iden);
	strcpy(q->phone, phone);

	fseek(fAcc, 0, 2);//��fAccŲ���ļ�ĩβ����ʼ���ļ���д������
	fprintf(fAcc, "\n%s\n%s\n%s\n0.0\n%s\n%s", q->ID, q->pw, q->name, q->identity, q->phone);//д���ʽ���Ȼ��� 
	rewind(fAcc);//ָ���ļ��� 
	p->next = q;
	q->next = NULL;

	tim = getime(q->ThisLog);
	fprintf(q->ThisLog, "%2d:%02d:%02d  �˻��½��ɹ�\n", tim->tm_hour, tim->tm_min, tim->tm_sec);
	tim = getime();
	fprintf(fSLog, "%2d:%02d:%02d  �½��˻� %s �ɹ�\n", tim->tm_hour, tim->tm_min, tim->tm_sec, q->ID);

	system("cls");   //*****����������ģ�����һ�½���*****
	printf("******\n�½��˻����\n******\n�˻���ţ�%s\n�˻�������%s\n�˻����֤���룺%s\n�˻�Ԥ���ֻ����룺%s", q->ID, q->name, q->identity, q->phone);
	printf("\n******���μ������˻���ź�����******\n");
	system("pause");
	system("cls");
	///����Ӧ�ð��½����˻���Ϣ�����������ļ���һ����������һ�� 
}

//ɾ���˻� 
void DelAcc()//ɾ���˻� 
{
	struct Account* nID=NULL, *former=head;//formerΪҪɾ��������ǰһ���� 
	double saving;
	FILE* del;//�½�һ���ļ�����Դ�ļ���Ϣ������д 
	char ch;
	while(!nID&&flag)
	nID=check();//����˻�����
	if(!flag)//��Ҫ�˳�
	{
		system("pause");
		flag=1;
		system("cls");
		return;
	 }  
	saving=nID->saving;
	printf("ȷʵҪɾ�����˻�����������Ϣ���ò������ɳ���\n");
	printf("********\ny����Ȼɾ��\nn����ɾ��\n����Ӧ��ĸִ�в���\n");
	fflush(stdin);
	ch=getchar();
	if(ch=='y')//ɾ������ 
	{
		printf("����ɾ��......\n");
		del=fopen("temp.txt","w");
		struct Account temp;
		fscanf(fAcc,"%s\n",temp.ID);
		while(!feof(fAcc)) //�ļ���������д���ļ� 
		{    //ԭ�ļ�δ��β 
			fscanf(fAcc,"%s\n%s\n%lf",temp.pw,temp.name,&temp.saving);
			if(strcmp(temp.ID,nID->ID))//�жϳ��������Ҫɾ���˻�
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
		//	fseek(fAcc,2,1);//�ӵ�ǰλ������Ų�����ַ� �������ļ�ĩβʱ�����˳�ѭ�� 
		}
		fclose(fAcc);
		fclose(del);//�ȹص������ļ�
		fclose(nID->ThisLog);
		remove("account.txt");
		rename("temp.txt","account.txt");//��è��̫��
		fAcc=fopen("account.txt","r+");//��fAccָ���ٸ��û��� 
		memset(filename,0,256);
		strcpy(filename,nID->ID);
		strcat(filename,".txt");
		tim=getime();
		fprintf(fSLog,"%2d:%02d:%02d  �˻� %s ��ע��\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,nID->ID);
		if(!strcmp(former->ID,nID->ID))//Ҫɾ�����˻�Ϊ����ͷ
		{
			fclose(head->ThisLog);
			head=head->next;
			free(former);//��ԭ����ͷ�ͷŵ� 
		} 
		else//Ҫɾ���˻�������ͷ 
		{ 
			fclose(nID->ThisLog);
			while(strcmp((former->next)->ID,nID->ID))//former��Ϊ��ɾ���˻���ǰһ���˻� 
			former=former->next;
			former->next=nID->next;//��������
			free(nID);//�ͷ�ԭ���ڿռ� 
		} 
		remove(filename); //ɾ�����˻���־����Ҫɾ�𣿻�������дһ����˻���ɾ���� 
		
		printf("ɾ���ɹ���\nӦ�˻��ֽ�%.2f Ԫ\n",saving);
		system("pause");
		system("cls");
		return;
		//�Ȱ��ļ����ɾ�ˣ���ɾ���� 
		//�ǵð�saving�����Ǯ�˳��� 
	}
	else 
	{
		printf("δɾ��\n");
		tim=getime(nID->ThisLog);
		fprintf(nID->ThisLog,"%2d:%02d:%02d  ɾ���˻�����ȡ��",  tim->tm_hour, tim->tm_min, tim->tm_sec);
		tim=getime();
		fprintf(fSLog,"%2d:%02d:%02d  ɾ���˻�����ȡ��",  tim->tm_hour, tim->tm_min, tim->tm_sec); 
		system("pause");
		system("cls");
		return;
	}
}

//��� 
void Save()//��� 
{
	struct Account* nID=NULL, temp;
	double saving;
	FILE *change;
	change=fopen("temp.txt","w");//�¿�һ���ļ�׼����д 
	while(!nID&&flag)
	nID=check();//����˻�����
	if(!flag)//��Ҫ�˳�
	{
		system("pause");
		flag=1;
		system("cls");
		return;
	 } 
	printf("���������");
	scanf("%lf",&saving);
	nID->saving+=saving;
	
	while(!feof(fAcc))
	{
		fscanf(fAcc,"%s\n%s\n%s\n%lf",temp.ID,temp.pw,temp.name,&temp.saving);
		if(!strcmp(nID->ID,temp.ID))//temp��ʱΪҪ�Ķ����˻�
		{
			temp.saving=nID->saving;
		} 
		fprintf(change,"%s\n%s\n%s\n%.2lf",temp.ID,temp.pw,temp.name,temp.saving);//��ӡʱ�Ѿ�������λ���
		if(!feof(fAcc))
		{
			fprintf(change,"\n");
		}
		//fseek(fAcc,2,1);//�ӵ�ǰλ������Ų�����ַ� �������ļ�ĩβʱ�����˳�ѭ��
	}
	rewind(fAcc);//ָ���ļ���
	fclose(fAcc);
	fclose(change);
	remove("account.txt");
	rename("temp.txt","account.txt"); //��è��̫�� 
	fAcc=fopen("account.txt","r+");//��fAccָ���ٸ��û���
	//����Ϊ�ļ��ڴ��䶯
	tim = getime(nID->ThisLog);
	fprintf(nID->ThisLog,"%2d:%02d:%02d  ���� %.2f Ԫ\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,saving);
	tim=getime();
	fprintf(fSLog,"%2d:%02d:%02d  �˻� %s ���� %.2f Ԫ\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,nID->ID,saving);
	printf("�ɹ�\n");
	printf("\n�Ѵ����%.2f\t���˻��ִ�%.2f\n",saving,nID->saving);
	system("pause");
	system("cls");
	return;
}

//ȡ�� 
void Withdraw()//ȡ�� 
{
	struct Account* nID=NULL, temp;
	double withdraws;
	FILE* change;
	change=fopen("temp.txt","w");//�����ļ���д���۲��� 
	while(!nID&&flag)
		nID=check();//����˻�����
	if(!flag)//��Ҫ�˳�
	{
		system("pause");
		flag=1;
		system("cls");
		return;
	 } 
	while(1)
	{
		printf("������ȡ���0-%.2f)��",nID->saving);
		scanf("%lf",&withdraws);
		if( withdraws > nID->saving )
		{
			printf("������\n");
		} 
		else
		{
			printf("ȡ��%.2fԪ����ȷ�Ͻ��\n",withdraws);
			system("pause");
			nID->saving -= withdraws;
			while(!feof(fAcc))
			{
				fscanf(fAcc,"%s\n%s\n%s\n%lf",temp.ID,temp.pw,temp.name,&temp.saving);
				if(strcmp(nID->ID,temp.ID)==0)//temp��ʱΪҪ�Ķ����˻�
				{
					temp.saving=nID->saving;
				} 
				fprintf(change,"%s\n%s\n%s\n%.2f",temp.ID,temp.pw,temp.name,temp.saving);//��ӡʱ�Ѿ�������λ���
				if(!feof(fAcc))
				{
					fprintf(change,"\n");
				}
			//	fseek(fAcc,2,1);//�ӵ�ǰλ������Ų�����ַ� �������ļ�ĩβʱ�����˳�ѭ��
			}
			tim=getime(nID->ThisLog);
			fprintf(nID->ThisLog,"%2d:%02d:%02d  ȡ�� %.2fԪ\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,withdraws);
			tim=getime();
			fprintf(fSLog,"%2d:%02d:%02d  �˻� %s ȡ�� %.2fԪ\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,nID->ID,withdraws);
			fclose(fAcc);
			fclose(change);
			remove("account.txt");
			rename("temp.txt","account.txt"); //��è��̫�� 
			fAcc=fopen("account.txt","r+");//��fAccָ���ٸ��û���
			//�ļ��ڴ��䶯 
			printf("ȡ��ɹ���\nȡ�� %.2fԪ\n�˻������ %.2fԪ\n",withdraws,nID->saving);
			system("pause");
			system("cls");
			return;
		}
	}
}

//ת��
void Transfer()
{
	struct Account* nID=NULL, *target=head,temp;
	char tID[11],ch;
	double amount;
	FILE *change;
	change=fopen("temp.txt","w");//������ʱ�ļ���д 
	while(!nID&&flag)
	nID=check();//����˻�����
	if(!flag)//��Ҫ�˳�
	{
		system("pause");
		flag=1;
		system("cls");
		return;
	 } 
	while(1)
	{
		printf("������ת���˻���ţ�10λ����");
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
		printf("���˻������ڣ���˶Ժ��������룡\n");
		else
		break;
	}
	
	while(1)
	{
		printf("��������Ҫת�˵Ľ�0-%.2f����",nID->saving);
		scanf("%lf",&amount);
		if(amount>nID->saving)
		{
			printf("������\n");
		}
		else
		{
			printf("�����������˻� %s ת�� %.2fԪ ���˻� %s Ŀ���˻�������%s \n�˶�������밴y,�����������������룺",nID->ID,amount,target->ID,target->name);
			fflush(stdin);
			ch=getchar();
			if(ch=='y')
			{
				printf("ִ����......\n");
				nID->saving-=amount;
				target->saving+=amount;
				while(!feof(fAcc)) 
				{
					fscanf(fAcc,"%s\n%s\n%s\n%lf",temp.ID,temp.pw,temp.name,&temp.saving);
					if(strcmp(temp.ID,nID->ID)==0)//����ת���˻�
					{
						temp.saving=nID->saving;
					}
					else if(strcmp(temp.ID,target->ID)==0)//����ת���˻�
					{
						temp.saving=target->saving;
					}
					fprintf(change,"%s\n%s\n%s\n%.2lf",temp.ID,temp.pw,temp.name,temp.saving);//��ӡʱ�Ѿ�������λ���
					if(!feof(fAcc))
					{
						fprintf(change,"\n");
					}
				}
				tim=getime(nID->ThisLog);
				fprintf(nID->ThisLog,"%2d:%02d:%02d  ת�˸� %s ���� %s %.2fԪ\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,target->ID,target->name,amount);
				tim=getime(target->ThisLog);
				fprintf(target->ThisLog,"%2d:%02d:%02d  �յ� %s ���� %s ת�� %.2fԪ\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,nID->ID,nID->name,amount);
				tim=getime();
				fprintf(fSLog,"%2d:%02d:%02d  ���˻� %s ת���˻� %s %.2fԪ\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,nID->ID,target->ID,amount);
				fclose(fAcc);
				fclose(change);
				remove("account.txt");
				rename("temp.txt","account.txt"); //��è��̫�� 
				fAcc=fopen("account.txt","r+");//��fAccָ���ٸ��û���
				//�ļ�����
				printf("******���\n�����ʻ���ǰ��%.2f\n",nID->saving);
				system("pause");
				system("cls");
				return; 
			}
			
		}
	}
}

//�޸��������һ�����Ĺ�ͬ���֣���������־�޸��ϣ��һ�������޸�������һ���ģ���û��û�����֣�
struct Account* ChangePwPart(struct Account *p)
{
	struct Account* nID = p, temp;
	FILE* change;
	change = fopen("temp.txt", "w");//�����ļ���д
	char *pw = NULL, *pw2 = NULL;//����������
	if (!(pw = (char*)malloc(30 * sizeof(char))) || !(pw2 = (char*)malloc(30 * sizeof(char))))//�ǵ�Ҫfree��
	{
		printf("�ڴ治��\n");
		system("pause");
		system("cls");
		return NULL;
	}

	while (1)//����������
	{
		system("cls");
		printf("*******�޸�����*******\n*******��q�˳�*******\n");
		printf("�˻�ID��%s\t�˻�����%s\n", nID->ID, nID->name);

		printf("\n�����������˻����루6��15���ַ�����");    //�����Ҿ���������һ��������ַ�����
		fflush(stdin);
		gets(pw);
		if (*pw == 'q')   //�˳���
		{
			system("pause");
			system("cls");
			free(pw); free(pw2); //*****������ٵĿռ�******
			return NULL;
		}
		else if (strlen(pw) < 6 || strlen(pw) > 15)    //���벻���Ϲ淶��Ҫ��������
		{
			printf("���벻���Ϲ淶��\n");
			system("pause");
			continue;
		}

		printf("\n���ٴ������������룺");             //ȷ������
		fflush(stdin);
		gets(pw2);
		if (*pw2 == 'q')   //�˳���
		{
			system("pause");
			system("cls");
			free(pw); free(pw2); //*****������ٵĿռ�******
			return NULL;
		}
		else if (!strcmp(pw, pw2))   //ǰ������һ��
		{
			printf("����һ�£�"); break;
		}
		else
		{
			printf("�������벻һ�£�����������\n");
			system("pause");
			continue;
		}
	}//����������
	printf("���Եȣ������޸�...");
	strcpy(nID->pw, pw);//����֮ǰ������

						//��־�ϵ��޸�
	while (!feof(fAcc))
	{
		fscanf(fAcc, "%s\n%s\n%s\n%s\n%s\n%lf", temp.ID, temp.pw, temp.name, temp.identity, temp.phone, &temp.saving);
		if (strcmp(nID->ID, temp.ID) == 0)//temp��ʱΪҪ�Ķ����˻�
		{
			strcpy(temp.pw, nID->pw);//temp.pw = nID->pw;�ñ�����һ��ʼд����ġ�����
		}
		fprintf(change, "%s\n%s\n%s\n%s\n%s\n%.2f", temp.ID, temp.pw, temp.name, temp.identity, temp.phone, temp.saving);//��ӡʱ�Ѿ�������λ���
		if (!feof(fAcc))
		{
			fprintf(change, "\n");
		}
		//	fseek(fAcc,2,1);//�ӵ�ǰλ������Ų�����ַ� �������ļ�ĩβʱ�����˳�ѭ��
	}
	tim = getime(nID->ThisLog);
	fprintf(nID->ThisLog, "%2d:%02d:%02d  �޸�����--�����룺%s\n", tim->tm_hour, tim->tm_min, tim->tm_sec, nID->pw);
	tim = getime();
	fprintf(fSLog, "%2d:%02d:%02d  �˻� %s �޸�����--�����룺%s\n", tim->tm_hour, tim->tm_min, tim->tm_sec, nID->ID, nID->pw);
	fclose(fAcc);
	fclose(change);
	remove("account.txt");
	rename("temp.txt", "account.txt"); //��è��̫�� 
	fAcc = fopen("account.txt", "r+");//��fAccָ���ٸ��û���
									  //�ļ�������䶯 
	free(pw); free(pw2);//���տռ�
	printf("\n�����޸ĳɹ���\n��������˳�...");
	system("pause");
	system("cls");
	return NULL;

}

//�޸�����(ֻҪ�þ�����)
void ChangePw()//�޸�����
{
	struct Account* nID = NULL;
	while (!nID&&flag)
		nID = check();//����˻�����
	if (!flag)//��Ҫ�˳�
	{
		system("pause");
		flag = 1;
		system("cls");
		return;
	}
	if (ChangePwPart(nID) == NULL)   //�������빫������
	{
		return;
	}
}

//�һ����루�ñ�š����������֤�š�Ԥ���ֻ���������֤��ֻ�������ü򵥵ķ��������������ˣ�
void FindPw()//��ʵ�һ���ʵ�ֿ�����;��������Ĺ��ܣ����ǡ������㶮��
{
	char *nID, *name, *identity, *phone;//��֤����Ŀǰ��ֻ�����˺ţ����������֤�š��ֻ��ţ�ʲô������֤��Щ���Ⱥ��԰�
	struct Account *p = head;
	if (!(nID = (char*)malloc(11 * sizeof(char))) || !(identity = (char*)malloc(25 * sizeof(char))) ||
		!(phone = (char*)malloc(15 * sizeof(char))) || !(name = (char*)malloc(12 * sizeof(char))))  //******�����Ҹĳ��߼������********
	{
		printf("�ڴ治��\n");
		system("pause");
		return ;
	}

	system("cls");
	printf("*******�һ�����******");
	printf("****�˻���֤****\n�� q �˳��˻���֤\n****\n");

	{//�����ĸ�����
		printf("�������˻���ţ�");
		fflush(stdin);
		gets(nID);
		if ( *nID == 'q')
		{
			return;
		}
		printf("�������˻���������");
		fflush(stdin);
		gets(name);
		if ( *name == 'q' )
		{
			return;
		}
		printf("�������˻������֤���룺");
		fflush(stdin);
		gets(identity);
		if ( *identity == 'q')
		{
			return;
		}
		printf("�������˻���Ԥ���ֻ����룺");
		fflush(stdin);
		gets(phone);
		if (*phone == 'q' )
		{
			return;
		}
	}
	do   //����������֤�ĸ������Ƿ��Ǻ�
	{
		if ((strcmp(p->ID, nID) == 0) && (strcmp(p->name, name) == 0) &&                      //��֤�ɹ�������ʵ�����޸��������һ����빦���ص����ֵĴ�����ȡ
			(strcmp(p->identity, identity) == 0) && (strcmp(p->phone, phone) == 0))        
		{
			printf("��֤�ɹ������������ʼ����������...");
			system("pause");

			free(identity); free(nID); free(name); free(phone);  //���տռ�
			if (ChangePwPart(p) == NULL)     //�����޸�������һ�����Ĺ�ͬ����
			{
				return;
			}
		}
		else
		{
			p = p->next;
		}

	} while (p != NULL);
	printf("�������\n��������˳�");
	free(identity); free(nID); free(name); free(phone);//���տռ�
	system("pause");
	return;
}
 


//������
main()
{
	char name[8],pswd[6];
	fAcc=fopen("account.txt","r+");
	fUser=fopen("user.txt","r+");
	fSLog=fopen("syslog.txt","a");//ϵͳ��־������Ϊֻ���ļ�ĩβ׷�ӵ����� 
	printf("��ȡ�У����Ժ򡭡�\n");

	if(!(filename=(char*)malloc(256*sizeof(char))))
	{
		printf("�ڴ治��\n");
		return 0;
	}
	tim=getime();
//	fprintf(fSLog,"%4d/%02d/%02d\n", (1900+tim->tm_year), (1+tim->tm_mon), tim->tm_mday);
//	fprintf(fSLog,"%2d:%02d:%02d  �򿪳��򣬶�ȡ�û�����\n",  tim->tm_hour, tim->tm_min, tim->tm_sec);
	head=Loadin();//���Ѵ���Ϣд����������ͷhead 
//	system("pause");
	tim=getime();
//	fprintf(fSLog,"%2d:%02d:%02d  ��ȡ���ݳɹ�\n",  tim->tm_hour, tim->tm_min, tim->tm_sec);
	
	system("cls");//�����Ļ 
	while(1)
	{
		printf("�������û��������룺\n");
		printf("�û�����");
		scanf("%s",name);
		printf("���룺");
		scanf("%s",pswd);
		tim=getime();
		fprintf(fSLog,"%4d/%02d/%02d\n", (1900+tim->tm_year), (1+tim->tm_mon), tim->tm_mday);
		fprintf(fSLog,"%2d:%02d:%02d  ���������֤\n",  tim->tm_hour, tim->tm_min, tim->tm_sec);
		int flag=0;
		char namec[8],pswdc[6];
		while(!feof(fUser))//���ҹ����Ա����Ϣ����ʵ�û��������룬��ʵ�ɹ����˳�while
		{
			fscanf(fUser,"%s%s",namec,pswdc);
			if((!strcmp(name,namec))&&(!strcmp(pswd,pswdc)))
			{
				printf("Success!\n");system("pause");flag=1;break;
			}
		}
		if(!flag)
		{
			rewind(fUser);//��fUser���õ��ļ��� 
			printf("�û������������\n");
			tim=getime();
			fprintf(fSLog,"%2d:%02d:%02d  �����֤ʧ�ܣ�������֤\n",  tim->tm_hour, tim->tm_min, tim->tm_sec);
		}
		else
		{
			tim=getime();
			fprintf(fSLog,"%2d:%02d:%02d  �����֤�ɹ�\n          ��¼�ߣ�%s\n",  tim->tm_hour, tim->tm_min, tim->tm_sec,name);
			break;
		}
	}
	system("cls");
	printf("------��ӭʹ�ñ�ϵͳ------\n");
	char ch;
	if(*name=='1')		//����Ա�˵� 
	{
		while(1)
		{
			printf("��ǰ�û���%s\t��ݣ�����Ա\n",name);
			printf("�˵�\n");
			printf("1:��ѯ\n2���޸�����\n3���һ�����\n4:���ù���Ա\n5:���ò���Ա\nq:�˳�����");  //*****�޸�*****
			printf("�밴��Ӧ����ѡ���Ӧ���ܣ�");
			fflush(stdin);
			ch=getchar();
			switch(ch)
			{
				case '1':Search();break;
				case '2':ChangePw(); break;
				case '3':FindPw(); break;
				case '4':
				case '5':printf("***�Բ��𣬹�����δ����***\n�����������..."); system("pause"); break;
				case 'q':tim = getime(); fprintf(fSLog, "%2d:%02d:%02d  �˳�����\n", tim->tm_hour, tim->tm_min, tim->tm_sec);
					printf("\n****�ټ�****\n****\n"); system("pause"); return 0;
				default:printf("������������������\n"); system("pause"); system("cls");
			}
		}

	} 
	else          // ����Ա�Ľ���
	{
		while(1)
		{
			printf("��ǰ�û���%s\t��ݣ�����Ա\n",name);
			printf("�˵�\n");
			printf("1����ѯ\n2������\n3������\n4�����\n5��ȡ��\n6��ת��\nq���˳�����\n");
			printf("�밴��Ӧ����ѡ���Ӧ���ܣ�");
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
				case 'q':tim=getime();fprintf(fSLog,"%2d:%02d:%02d  �˳�����\n",  tim->tm_hour, tim->tm_min, tim->tm_sec);
					     printf("\n****�ټ�****\n****\n");system("pause");return 0;
				default:printf("������������������\n");system("pause");system("cls");
			}
		}
	}
}

//�Ҿ�����������־�ϵ�д�����Ǻܺ��������ʱ���ڸĸİ�
//�ҷ����޸�����������û����תҳ�档��������ô��
