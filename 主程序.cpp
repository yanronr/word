#include <bits/stdc++.h>//���ڲ�ͬ���Ե�Ӣ�ĵ��ʵĴ�Ƶͳ�ƺͼ���ϵͳ 
#include <windows.h>//���ڸ߾���ʱ����� 
using namespace std;
#define OK 1
#define ERROR 0
#define WordMaxLength 30
#define WordMaxNum 5000
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
typedef struct{//���� 
	char *field;//�ֶ�����ָ�� 
	int num;//��Ƶ	
}Word;
typedef struct{//˳��� 
	Word *elem;
	int length;//��ǰ���� 
	int listsize;//��󳤶� 
}SqList;
typedef struct LNode{//���� 
	Word data;
	struct LNode *next;
}LNode,*LinkList;
typedef struct BiTNode{//������ 
	Word data;
	struct BiTNode *lchild,*rchild;
}BiTNode,*BiTree;
typedef struct{//���ڿ��ŵ�ַ���Ĺ�ϣ�� 
	Word *elem;//���� 
	int number;//ʹ�ø��� �˴�δ��space�ռ��Сֵ������Ϊʹ���㷨��������ռ䲻������
}Table;   
void InitSqList(SqList &S){//��ʼ��˳��� 
	S.elem=(Word *)malloc(LIST_INIT_SIZE*sizeof(Word));
	if(!S.elem)exit(OVERFLOW);
	S.length=0;
	S.listsize=LIST_INIT_SIZE;
}
void InitLinkList(LinkList &L){ //��ʼ��������
	L=(LinkList)malloc(sizeof(LNode));
	if(!L)exit(OVERFLOW);
	L->next=NULL; 
}
void BiTreeInsert(BiTree &T,Word *Array,int base,int top){//��������������в���Ԫ�� 
	if(top>=base){
		T=(BiTree)malloc(sizeof(BiTNode));
		int median=(base+top)/2;
		T->data.field=(char *)malloc((strlen(Array[median].field)+1)*sizeof(char));
		strcpy(T->data.field,Array[median].field);
		T->data.num=Array[median].num;
		T->lchild=T->rchild=NULL;
		BiTreeInsert(T->lchild,Array,base,median-1);//������֧�� 
		BiTreeInsert(T->rchild,Array,median+1,top); //������֧�� 
	}
}
void InitBiTree(SqList &S,BiTree &T){//����������������,����Word���� 
	Word Array[S.length];
	for(int i=0;i<S.length;i++){
		Array[i].field=S.elem[i].field;
		Array[i].num=S.elem[i].num;
	}
	BiTreeInsert(T,Array,0,S.length-1); 
}
int Hash(Table &B,char *wordarray){//��ϣ���� 
	int hashvalue=0;
	for(int i=0;wordarray[i];i++)hashvalue+=wordarray[i];
	hashvalue%=B.number;
	return hashvalue;
}
void TableInsert(Table &B,char *wordarray,int num){//���ϣ���в��뵥�� 
	int hashvalue=Hash(B,wordarray);
	if(!B.elem[hashvalue].field){//δ������ͻ 
		B.elem[hashvalue].field=(char *)malloc((strlen(wordarray)+1)*sizeof(char));
		strcpy(B.elem[hashvalue].field,wordarray);
		B.elem[hashvalue].num=num;
	}else{
		
	}
}
void InitTable(SqList &S,Table &B){////����Word��������һ���պ�Ϊ���������Ĺ�ϣ���ռ�պ����� 
	B.elem=(Word *)malloc(S.length*sizeof(Word));
	if(!B.elem)exit(OVERFLOW);
	B.number=S.length;//�������� 
	for(int i=0;i<B.number;i++)B.elem[i].field=NULL;//��ʼ�������ڼ���Ƿ�����ͻ 
}
void SqListInsert(SqList &S,char *wordarray,int length){//˳������Ԫ�� 
	Word *p;
	if(S.length>=S.listsize){
		Word *newbase;
		newbase=(Word *)realloc(S.elem,(S.listsize+LISTINCREMENT)*sizeof(Word));
		if(!newbase)
			exit(OVERFLOW);
		S.elem=newbase;
		S.listsize +=LISTINCREMENT;
	}
	for(int i=0;i<S.length;i++)//�����ж��Ƿ������ͬ�ĵ��� 
		if(!strcmp(S.elem[i].field,wordarray)){
			S.elem[i].num++;
			return;
		}
	S.elem[S.length].field=(char *)malloc((length+1)*sizeof(char));
	strcpy(S.elem[S.length].field,wordarray);
	S.elem[S.length].num=1;
	S.length++;
}
void BiTNodeDisp(BiTree &T){//��������������ڵ㣬����������˳�������
	if(T){ 
		BiTNodeDisp(T->lchild);//��֧�� 
		FILE *Outfile;
		Outfile=fopen("Outfile4.txt","a");//����׷�ӵķ�ʽ 
		fprintf(Outfile,"%s  %d\n",T->data.field,T->data.num);
		fclose(Outfile);
		BiTNodeDisp(T->rchild);//��֧�� 
	}
}
int BiTNodeCount(BiTree &T){//��������������Ľڵ����������ڴ�Ƶͳ�� 
	if(T)return BiTNodeCount(T->lchild)+BiTNodeCount(T->rchild)+1;
		else return 0;
}
void DispFileBiTree(BiTree &T){//��������� 
	FILE *Outfile;
	Outfile=fopen("Outfile4.txt","w");
	fprintf(Outfile,"%d\n",BiTNodeCount(T));//д�ļ�ͷ 
	fclose(Outfile);
	BiTNodeDisp(T);//���д�ļ��� 
}
int BiTNodeFind(BiTree &T,char *wordarray){//�������������ڽ��Ĳ���
	if(T){
		int result=strcmp(T->data.field,wordarray);
		if(result==0)return T->data.num;
		else if(result>0)return BiTNodeFind(T->lchild,wordarray);//T.data->field<wordarray����֧�� 
			else return BiTNodeFind(T->rchild,wordarray); 
	}else return 0; 
} 
int BiTreeASL(BiTree &T,int depth){
	if(T){
		return BiTreeASL(T->lchild,depth+1)+BiTreeASL(T->rchild,depth+1)+depth;
	}else return 0;
} 
void FindSuccess(int num,double time,double asl){
	printf("�˵��ʵ���ƵΪ��%d\n",num);
	printf("���Ҹõ���������ʱ�䣺%.2lf\n",time);
	printf("ƽ�����ҳ��ȣ�%.1lf\n",asl); 
}
void FindWordBiTree(BiTree &T){//�������������� 
	char wordarray[WordMaxLength];
	fflush(stdin);
	printf("����������ҵĵ��ʣ�"); 
	gets(wordarray);
	_LARGE_INTEGER time_start,time_end;//��ʼʱ�������ʱ�� 
	double dqFreq;LARGE_INTEGER f;//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ
	int num;
	num=BiTNodeFind(T,wordarray);
	QueryPerformanceCounter(&time_end);//��ʱ����
	if(num){
		FindSuccess(num,1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq,(double)BiTreeASL(T,1)/BiTNodeCount(T));
	}else{
		printf("����ʧ��\n");
	}
}
void DestroySqList(SqList &S){//�ͷ�˳��� 
	for(int i=0;i<S.length;i++)
		free(S.elem[i].field);
	free(S.elem);
	S.elem=NULL;
	S.length=0;
	S.listsize=0;
}
void DestroyLinkList(LinkList &L){//�ͷ����� 
	LinkList p=L,q=p->next;
	while(q){
		if(p->data.field)free(p->data.field);
		free(p);
		p=NULL;
		p=q;
		q=p->next;
	}
	L=NULL;
}
void DestoryBiTree(BiTree &T){//�ͷŶ��������� 
	if(T){
		DestoryBiTree(T->lchild);
		DestoryBiTree(T->rchild);//�����ͷ� 
		free(T->data.field);
		free(T);
		T=NULL; 
	} 
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
void SqListSort(SqList &S){//�������� 
	int low=0,high=S.length-1;
	Word *array[S.length];
	for(int i=0;i<S.length;i++){
		array[i]=&S.elem[i];
	}
	QSort(array,low,high);	//���������㷨
}
void DispFileSqList(SqList S){//����ļ�����Ӧ���ļ���  
	FILE *Outfile;
	Outfile=fopen("Outfile1.txt","w");
	fprintf(Outfile,"%d\n",S.length);
	char word;
	for(int i=0;i<S.length;i++)fprintf(Outfile,"%s  %d\n",S.elem[i].field,S.elem[i].num);
	fclose(Outfile);
}
void DispFileLinkList(LinkList &L){//����ļ�����Ӧ���ļ���  
	FILE *Outfile;
	Outfile=fopen("Outfile2.txt","w");
	LinkList q=L->next;
	int length;
	for(length=0;q;q=q->next)length++;
	fprintf(Outfile,"%d\n",length);
	for(LinkList q=L->next;q;q=q->next)fprintf(Outfile,"%c  %d\n",q->data.field,q->data.num);
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
void FindWordHalf(SqList &S){	//�۰���� 
	char wordarray[WordMaxLength];
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
	if(num){
		FindSuccess(num,1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq,0);
	}else{
		printf("����ʧ��\n");
	}
}
int LinkListCount(LinkList &L){
	LinkList q=L;
	int n=0;
	while(q=q->next)n++;
		return n;
}
void FindWordLinkList(LinkList &L){//����˳����� 
	char wordarray[WordMaxLength];
	fflush(stdin);
	printf("����������ҵĵ��ʣ�"); 
	gets(wordarray);
	_LARGE_INTEGER time_start,time_end;//��ʼʱ�������ʱ�� 
	double dqFreq;LARGE_INTEGER f;//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ
	LinkList p;
	for(p=L->next;p;p=p->next){
		if(!strcmp(p->data.field,wordarray))
			break;
	}
	QueryPerformanceCounter(&time_end);//��ʱ����
	if(p){
		FindSuccess(p->data.num,1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq,(double)(LinkListCount(L)+1)/2);
	}else{
		printf("����ʧ��\n");
	}
}
void FindWordSqList(SqList &S){	//˳���˳����� 
	char wordarray[WordMaxLength];
	fflush(stdin);
	printf("����������ҵĵ��ʣ�"); 
	gets(wordarray);
	_LARGE_INTEGER time_start,time_end;//��ʼʱ�������ʱ�� 
	double dqFreq;LARGE_INTEGER f;//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ
	int i;
	for(i=0;i<S.length;i++){
		if(!strcmp(S.elem[i].field,wordarray))
			break;
	}
	QueryPerformanceCounter(&time_end);//��ʱ����
	if(i<S.length){
		FindSuccess(S.elem[i].num,1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq,(double)(S.length+1)/2);
	}else{
		printf("����ʧ��\n");
	}
}
void LinkListInsert(LinkList &L,char *wordarray,int length){//�������в���Ԫ��,��ͷ�ڵ㣬������в��� 
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
}
void ReadFile(SqList &S,LinkList &L){//��ȡ�ļ������ֽ�ɵ��� ,���뵽��ͬ�����ݴ洢�ṹ�� 
	FILE *Infile;
	if((Infile=fopen("Infile.txt","r"))==NULL){
		printf("Failed to read file!");
		exit(1);
	}
	char wordarray[WordMaxLength],word; 
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
		SqListInsert(S,wordarray,length);//���뵽˳����� 
		LinkListInsert(L,wordarray,length);//���뵽������ 
	}//��ReadFileʱ�����õ��Ƕ�����ȡ�ķ�ʽ��ԭ�е�wordarray�к�������������ĸ�������� filed����ʱ�Ǳ䳤��,������Ҫ����length������ʵ�ֱ䳤���� 
	SqListSort(S);//˳�������
	SimpleSort(L);//�������� 
	fclose(Infile);
}
void Menu(char *name,char *option1,char *option2){
	printf("    /****���ڲ�ͬ���Եĵ���ͳ�ƺͼ���ϵͳ****/\n\n");
	printf("   ------%s------\n\n",name);
	printf("1.%s\n",option1);
	printf("2.%s\n",option2);
	printf("3.������һ��\n");
	printf("�밴��Ӧ���ּ�����ѡ�� ");
} 
void Linearlist(SqList &S,LinkList &L){//�������Ա�Ĳ��Ҳ˵� 
	int x; 
	while(1){
		Menu("�������Ա�Ĳ���","˳�����","�۰����"); 
		scanf("%d",&x);
		if(x==3){
			system("cls");
			break;
		}
		else switch(x){
			case 1:system("cls");
				while(1){
					Menu("����˳���Ĳ���","����˳����˳�����","��������Ĳ���"); 
					scanf("%d",&x);
					if(x==3){
						system("cls");
						break;
					}
					else switch(x){
						case 1:system("cls");
							while(1){ 
								Menu("����˳����˳�����","��Ƶͳ��","���ʲ���");
								scanf("%d",&x);
								if(x==3){
									system("cls");
									break;
								}	
								else switch(x){
									case 1:
										DispFileSqList(S);//��� 
										printf("��Ƶͳ�Ƴɹ����ѵ�����Outfile1.txt�ļ��У�\n"); 
										fflush(stdin);
										if(getchar())system("cls");
										break;
									case 2:
										FindWordSqList(S);//���� 
										fflush(stdin);
										if(getchar())system("cls");
										break;
								}
								
							}
							break;
						case 2:system("cls");
							while(1){ 
								Menu("���������˳�����","��Ƶͳ��","���ʲ���");
								scanf("%d",&x);
								if(x==3){
									system("cls");
									break;
								}	
								else switch(x){
									case 1:
										DispFileLinkList(L);//��� 
										printf("��Ƶͳ�Ƴɹ����ѵ�����Outfile2.txt�ļ��У�\n"); 
										fflush(stdin);
										if(getchar())system("cls");
										break;
									case 2:
										FindWordLinkList(L);//���� 
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
					Menu("�۰����","��Ƶͳ��","���ʲ���");
					scanf("%d",&x);
					if(x==3){
						system("cls");
						break;
					}
					else switch(x){
						case 1:
							DispFileSqList(S);//��� 
							printf("��Ƶͳ�Ƴɹ����ѵ�����Outfile3.txt�ļ��У�\n"); 
							fflush(stdin);
							if(getchar())system("cls");
								break;
						case 2:
							FindWordHalf(S);//���� 
							fflush(stdin);
							if(getchar())system("cls");
								break;
							}		
				}
		} 
	}
}
void BinarySortTree(BiTree &T){
	int x;
	while(1){ 
		Menu("���ڶ����������Ĳ���","��Ƶͳ��","���ʲ���");
		scanf("%d",&x);
		if(x==3){
			system("cls");
			break;
		}	
		else switch(x){
			case 1:
				DispFileBiTree(T);//��� 
				printf("��Ƶͳ�Ƴɹ����ѵ�����Outfile4.txt�ļ��У�\n"); 
				fflush(stdin);
				if(getchar())system("cls");
					break;
			case 2:
				FindWordBiTree(T);//���� 
				fflush(stdin);
				if(getchar())system("cls");
				break;
		}							
	}
}
void Hashtable(){
	int x;
	while(1){ 
		Menu("���ڹ�ϣ��Ĳ���","���ڿ��ŵ�ַ���Ĺ�ϣ����","��������ַ���Ĺ�ϣ����");
		scanf("%d",&x);
		if(x==3){
			system("cls");
			break;
		}	
		else switch(x){
			case 1:
				while(1){ 
					Menu("���ڿ��ŵ�ַ���Ĺ�ϣ����","��Ƶͳ��","���ʲ���");
					scanf("%d",&x);
					if(x==3){
						system("cls");
						break;
					}	
					else switch(x){
						case 1:
								//��� 
							printf("��Ƶͳ�Ƴɹ����ѵ�����Outfile5.txt�ļ��У�\n"); 
							fflush(stdin);
							if(getchar())system("cls");
						break;
						case 2:
								//���� 
							fflush(stdin);
							if(getchar())system("cls");
						break;
					}							
				}
				break;
			case 2:
				while(1){ 
					Menu("��������ַ���Ĺ�ϣ����","��Ƶͳ��","���ʲ���");
					scanf("%d",&x);
					if(x==3){
						system("cls");
						break;
					}	
					else switch(x){
						case 1:
								//��� 
							printf("��Ƶͳ�Ƴɹ����ѵ�����Outfile6.txt�ļ��У�\n"); 
							fflush(stdin);
							if(getchar())system("cls");
						break;
						case 2:
								//���� 
							fflush(stdin);
							if(getchar())system("cls");
						break;
					}							
				}
				break;
		}							
	}
}
int main(){
	int x;
	SqList S;InitSqList(S);
	LinkList L;InitLinkList(L);
	ReadFile(S,L);
	BiTree T;InitBiTree(S,T);

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
			case 1:system("cls");Linearlist(S,L);break;//���Ա� 
			case 2:system("cls");BinarySortTree(T);break;//���������� 
			case 3:system("cls");break;//��ϣ�� 
		} 	
	}
	DestroySqList(S);
	DestroyLinkList(L);
	DestoryBiTree(T); 
} 
