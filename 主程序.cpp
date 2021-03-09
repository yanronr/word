#include <bits/stdc++.h>//���ڲ�ͬ���Ե�Ӣ�ĵ��ʵĴ�Ƶͳ�ƺͼ���ϵͳ 
#include <windows.h>//���ڸ߾���ʱ����� 
using namespace std;
#define OK 1
#define ERROR 0
#define WordMax 30
typedef int Status;
typedef struct{
	char *field;//�ֶ�����ָ�� 
	int num;//��Ƶ	
}Word;
//���Ա�Ԫ 
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
//˳��� 
typedef struct{//˳��� 
	Word *elem;
	int length;//��ǰ���� 
	int listsize;//��󳤶� 
}SqList;
typedef struct node{//���� 
	Word data;
	struct node *next;
}LNode,*LinkList;
typedef struct BiTNode{//������ 
	Word data;
	struct BiTNode *lchild,*rchild;
}BiTNode,*BiTree;
//������ģ��
Status InitList(SqList &S){//��ʼ��˳��� 
	S.elem=(Word *)malloc(LIST_INIT_SIZE*sizeof(Word));
	if(!S.elem)exit(OVERFLOW);
	S.length=0;
	S.listsize=LIST_INIT_SIZE;
	return OK;
}
Status InitLinkList(LinkList &L){ //��ʼ��������
	L=(LinkList)malloc(sizeof(LNode));
	if(!L)return ERROR;
	L->next=NULL; 
	return OK;
}
Status BiTreeInsert(BiTree &T,Word *Array,int base,int top){//��������������в���Ԫ�� 
	if(top>=base){
		T=(BiTree)malloc(sizeof(BiTNode));
		int median=(base+top)/2,length=0;
		for(length=0;Array[median].field[length];length++);//���㵥�ʳ���
		T->data.field=(char *)malloc((length+1)*sizeof(char));
		strcpy(T->data.field,Array[median].field);
		T->lchild=NULL;T->rchild=NULL;
		BiTreeInsert(T->lchild,Array,base,median-1);//������֧�� 
		BiTreeInsert(T->rchild,Array,median+1,top); //������֧�� 
	}
}
Status CreateBiTree(BiTree &T){//����������������,����Word���� 
	
}
Status ListInsert(SqList &S,char *wordarray,int length){//˳������Ԫ�� 
	Word *p;
	if(S.length>=S.listsize){
		Word *newbase;
		newbase=(Word *)realloc(S.elem,(S.listsize+LISTINCREMENT)*sizeof(Word));
		if(!newbase)
			return ERROR;
		S.elem=newbase;
		S.listsize +=LISTINCREMENT;
	}
	for(int i=0;i<S.length;i++)//�����ж��Ƿ������ͬ�ĵ��� 
		if(!strcmp(S.elem[i].field,wordarray)){
			S.elem[i].num++;
			return OK;
		}
	S.elem[S.length].field=(char *)malloc((length+1)*sizeof(char));
	strcpy(S.elem[S.length].field,wordarray);
	S.elem[S.length].num=1;
	S.length++;
	return OK;
}
Status DestroyList(SqList &S){//�ͷ�˳��� 
	for(int i=0;i<S.length;i++)
		free(S.elem[i].field);
	free(S.elem);
	S.elem=NULL;
	S.length=0;
	S.listsize=0;
	return OK;
}
Status DestroyLinkList(LinkList &L){//�ͷ����� 
	LinkList p=L,q=p->next;
	while(q){
		if(p->data.field)free(p->data.field);
		free(p);
		p=NULL;
		p=q;
		q=p->next;
	}
	L=NULL;
	return OK;
}
//������Ԫ 
int Partition(Word *(*array),int low,int high)//�����������λ�� 
{	Word pivotkey;
	pivotkey.field=array[low]->field;pivotkey.num=array[low]->num;
	while(low<high)
	{	while(low<high && strcmp(array[high]->field,pivotkey.field)>0)
			--high;
		array[low]->field=array[high]->field;array[low]->num=array[high]->num;
		while(low<high && strcmp(array[low]->field,pivotkey.field)<0)
			++low;
		array[high]->field=array[low]->field;array[high]->num=array[low]->num;
	} 
	array[low]->field=pivotkey.field;array[low]->num=pivotkey.num;
	return low;
}
void QSort(Word *(*array),int low,int high)//˳�����������ȸ�������ĸ��ASCLL��С�������� ���ٽ���ͬ����ĸ�Ľ��ж�������
{	if(low<high)
	{	int pivotloc=Partition(array,low,high);
		QSort(array,low,pivotloc-1);
		QSort(array,pivotloc+1,high);
	}
}
void SimpleSort(LinkList &L){//�����ѡ���������ֵ�ʮ�������㷨��,��ѡ�������㷨���ֵ������� 
	LinkList p,q,min;
	Word temp;
	for(p=L->next;p;p=p->next){//pΪ��׼ָ�룬qΪѡ����Сֵָ�� 
		for(q=p->next,min=p;q;q=q->next)
			if(strcmp(q->data.field,min->data.field)<0)min=q;
		if(min!=p){
			temp.field=p->data.field;temp.num=p->data.num;//����temp�ֱ𽻻��ֶε�ַ����Ƶ 
			p->data.field=min->data.field;p->data.num=min->data.num;
			min->data.field=temp.field;min->data.num=temp.num;
		}		
	} 
} 
void ListSort(SqList &S){//�������� 
	int low=0,high=S.length-1;
	Word *array[S.length];
	for(int i=0;i<S.length;i++){
		array[i]=&S.elem[i];
	}
	QSort(array,low,high);	//���������㷨
}
void ListDispFile(SqList S){//����ļ�����Ӧ���ļ���  
	FILE *Outfile;
	Outfile=fopen("Outfile1.txt","w");
	fprintf(Outfile,"%d\n",S.length);
	char word;
	for(int i=0;i<S.length;i++){
		for(int j=0;word=S.elem[i].field[j];j++)
			fprintf(Outfile,"%c",word);
		fprintf(Outfile,"  ");
		fprintf(Outfile,"%d",S.elem[i].num);
		fprintf(Outfile,"\n");
	}
	fclose(Outfile);
}
void LinkListDispFile(LinkList &L){//����ļ�����Ӧ���ļ���  
	FILE *Outfile;
	Outfile=fopen("Outfile2.txt","w");
	LinkList q=L->next;
	int length;
	for(length=0;q;q=q->next)length++;
	fprintf(Outfile,"%d\n",length);
	char word;
	for(LinkList q=L->next;q;q=q->next){
		for(int i=0;word=q->data.field[i];i++)
			fprintf(Outfile,"%c",word);
		fprintf(Outfile,"  ");
		fprintf(Outfile,"%d",q->data.num);
		fprintf(Outfile,"\n");
	}
	fclose(Outfile);
}
int HalfSearch(SqList &S,char *wordarray,int base,int top){//�۰���ҹ����� 
	if(top<base)return 0;
	else{
		int Standard=(base+top)/2;//������λ�� 
		int Result=strcmp(S.elem[Standard].field,wordarray);
		if(Result==0)return S.elem[Standard].num;
		else if(Result<0)return HalfSearch(S,wordarray,Standard+1,top);//���С��0����˵��StandardС��wordarray������۰���� 
			else return HalfSearch(S,wordarray,base,Standard-1);//�������0����˵��Standard����wordarray����ǰ�۰���� 
	}
}
void HalfFindWord(SqList &S){	//�۰���� 
	char wordarray[WordMax];
	fflush(stdin);
	printf("����������ҵĵ��ʣ�"); 
	gets(wordarray);
	_LARGE_INTEGER time_start,time_end;//��ʼʱ�������ʱ�� 
	double dqFreq;LARGE_INTEGER f;//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ
	int num;
	num=HalfSearch(S,wordarray,0,S.length-1);
	QueryPerformanceCounter(&time_end);//��ʱ����
	if(num>0){
		printf("�˵��ʵ���ƵΪ��%d\n",num);
		printf("���Ҹõ���������ʱ�䣺%lf\n",1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq);
		printf("ƽ�����ҳ��ȣ�%d\n",0); 
	}else{
		printf("����ʧ��\n");
	}
}
void LinkListFindWord(LinkList &L){//����˳����� 
	char wordarray[WordMax];
	fflush(stdin);
	printf("����������ҵĵ��ʣ�"); 
	gets(wordarray);
	_LARGE_INTEGER time_start,time_end;//��ʼʱ�������ʱ�� 
	double dqFreq;LARGE_INTEGER f;//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ
	int n=0;
	LinkList p;
	for(p=L->next;p;p=p->next){
		n++;
		if(!strcmp(p->data.field,wordarray))
			break;
	}
	QueryPerformanceCounter(&time_end);//��ʱ����
	if(p){
		printf("�˵��ʵ���ƵΪ��%d\n",p->data.num);
		printf("���Ҹõ���������ʱ�䣺%lf\n",1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq);
		printf("ƽ�����ҳ��ȣ�%d\n",(n+1)/2); 
	}else{
		printf("����ʧ��\n");
	}
}
void ListFindWord(SqList &S){	//˳���˳����� 
	char wordarray[WordMax];
	fflush(stdin);
	printf("����������ҵĵ��ʣ�"); 
	gets(wordarray);
	_LARGE_INTEGER time_start,time_end;//��ʼʱ�������ʱ�� 
	double dqFreq;LARGE_INTEGER f;//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ
	int n=0,i;
	for(i=0;i<S.length;i++){
		n++;
		if(!strcmp(S.elem[i].field,wordarray))
			break;
	}
	QueryPerformanceCounter(&time_end);//��ʱ����
	if(i<S.length){
		printf("�˵��ʵ���ƵΪ��%d\n",S.elem[i].num);
		printf("���Ҹõ���������ʱ�䣺%lf\n",1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq);
		printf("ƽ�����ҳ��ȣ�%d\n",(n+1)/2); 
	}else{
		printf("����ʧ��\n");
	}
}
Status LinkListInsert(LinkList &L,char *wordarray,int length){//�������в���Ԫ��,��ͷ�ڵ㣬������в��� 
	LinkList p=L->next,q=L;
	while(p){
		if(!strcmp(p->data.field,wordarray)){
			p->data.num++;
			break;
		}
		q=p;
		p=p->next; 
	}
	if(!p){
		p=(LinkList)malloc(sizeof(LNode));
		p->data.field=(char *)malloc((length+1)*sizeof(char));
		strcpy(p->data.field,wordarray);
		p->data.num=1;
		q->next=p;
		p->next=NULL;
	}
	return OK;
}
void ReadFile(SqList &S,LinkList &L){//��ȡ�ļ������ֽ�ɵ��� ,���뵽��ͬ�����ݴ洢�ṹ�� 
	FILE *Infile;
	if((Infile=fopen("Infile.txt","r"))==NULL){
		printf("Failed to read file!");
		exit(1);
	}
	char wordarray[WordMax],word; 
	int length;
	while((word=fgetc(Infile))!=EOF){//��Ӣ�ĵ��ʴ��Ķ��в�� 
		for(length=0;word!=EOF;word=fgetc(Infile)){	
			if(!(('a'<=word&&word<='z')||('A'<=word&&word<='Z'))&&(length==0)){
				continue;
			}
			else if(!(('a'<=word&&word<='z')||('A'<=word&&word<='Z'))&&(length>0)){
				break;
			}
			if('A'<=word&&word<='Z')wordarray[length++]=word+32;
				else wordarray[length++]=word;
		}
		wordarray[length]='\0';
		ListInsert(S,wordarray,length);//���뵽˳����� 
		LinkListInsert(L,wordarray,length);//���뵽������ 
	}//��ReadFileʱ�����õ��Ƕ�����ȡ�ķ�ʽ��ԭ�е�wordarray�к�������������ĸ�������� filed����ʱ�Ǳ䳤��,������Ҫ����length������ʵ�ֱ䳤���� 
	ListSort(S);//˳�������
	SimpleSort(L);//�������� 
	fclose(Infile);
}
void Linearlist(SqList &S,LinkList &L){//�������Ա�Ĳ��Ҳ˵� 
	int x; 
	while(1){
		printf("    /****���ڲ�ͬ���Եĵ���ͳ�ƺͼ���ϵͳ****/\n\n");
		printf("   ------�������Ա�Ĳ���------\n\n");
		printf("1.˳�����\n");
		printf("2.�۰����\n");
		printf("3.������һ��\n");
		printf("�밴��Ӧ���ּ�����ѡ�� ");
		scanf("%d",&x);
		if(x==3){
			system("cls");
			break;
		}
		else switch(x){
			case 1:system("cls");
				while(1){
					printf("    /****���ڲ�ͬ���Եĵ���ͳ�ƺͼ���ϵͳ****/\n\n");
					printf("   ------˳�����------\n\n");
					printf("1.����˳����˳�����\n");
					printf("2.��������Ĳ���\n");
					printf("3.������һ��\n");
					printf("�밴��Ӧ���ּ�����ѡ�� ");
					scanf("%d",&x);
					if(x==3){
						system("cls");
						break;
					}
					else switch(x){
						case 1:system("cls");
							while(1){ 
								printf("    /****���ڲ�ͬ���Եĵ���ͳ�ƺͼ���ϵͳ****/\n\n");
								printf("   ------����˳����˳�����------\n\n");
								printf("1.��Ƶͳ��\n");
								printf("2.���ʲ���\n");
								printf("3.������һ��\n");
								printf("�밴��Ӧ���ּ�����ѡ�� ");
								scanf("%d",&x);
								if(x==3){
									system("cls");
									break;
								}	
								else switch(x){
									case 1:
										ListDispFile(S);//��� 
										printf("��Ƶͳ�Ƴɹ����ѵ�����Outfile1.txt�ļ��У�\n"); 
										fflush(stdin);
										if(getchar())system("cls");
										break;
									case 2:
										ListFindWord(S);//���� 
										fflush(stdin);
										if(getchar())system("cls");
										break;
								}
								
							}
							break;
						case 2:system("cls");
							while(1){ 
								printf("    /****���ڲ�ͬ���Եĵ���ͳ�ƺͼ���ϵͳ****/\n\n");
								printf("   ------���������˳�����------\n\n");
								printf("1.��Ƶͳ��\n");
								printf("2.���ʲ���\n");
								printf("3.������һ��\n");
								printf("�밴��Ӧ���ּ�����ѡ�� ");
								scanf("%d",&x);
								if(x==3){
									system("cls");
									break;
								}	
								else switch(x){
									case 1:
										LinkListDispFile(L);//��� 
										printf("��Ƶͳ�Ƴɹ����ѵ�����Outfile2.txt�ļ��У�\n"); 
										fflush(stdin);
										if(getchar())system("cls");
										break;
									case 2:
										LinkListFindWord(L);//���� 
										fflush(stdin);
										if(getchar())system("cls");
										break;
								}
								
							}
					} 
				}
				break;
			case 2:system("cls");
				while(1){
					printf("    /****���ڲ�ͬ���Եĵ���ͳ�ƺͼ���ϵͳ****/\n\n");
					printf("   ------�۰����------\n\n");
					printf("1.��Ƶͳ��\n");
					printf("2.���ʲ���\n");
					printf("3.������һ��\n");
					printf("�밴��Ӧ���ּ�����ѡ�� ");
					scanf("%d",&x);
					if(x==3){
						system("cls");
						break;
					}
					else switch(x){
						case 1:system("cls");
							ListDispFile(S);//��� 
							printf("��Ƶͳ�Ƴɹ����ѵ�����Outfile3.txt�ļ��У�\n"); 
							fflush(stdin);
							if(getchar())system("cls");
								break;
						case 2:
							HalfFindWord(S);//���� 
							fflush(stdin);
							if(getchar())system("cls");
								break;
							}		
				}
		} 
	}
}
int main(){
	int x;
	SqList S;InitList(S);
	LinkList L;InitLinkList(L);
	ReadFile(S,L);
	while(1){
		printf("    /****���ڲ�ͬ���Եĵ���ͳ�ƺͼ���ϵͳ****/\n\n\n");
		printf("1.�������Ա�Ĳ���\n");
		printf("2.���ڶ����������Ĳ���\n");
		printf("3.���ڹ�ϣ��Ĳ���\n");
		printf("4.�˳�\n");
		printf("�밴��Ӧ���ּ�����ѡ�� ");
		scanf("%d",&x);
		if(x==4)break;
		else switch(x){
			case 1:system("cls");Linearlist(S,L);break;
			case 2:system("cls");break;
			case 3:system("cls");break;
		} 	
	}
	DestroyList(S);
	DestroyLinkList(L);
} 
