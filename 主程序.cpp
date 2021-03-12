#include <bits/stdc++.h>//基于不同策略的英文单词的词频统计和检索系统 
#include <windows.h>//用于高精度时间计算 
using namespace std;
#define OK 1
#define ERROR 0
#define WordMaxLength 30
#define WordMaxNum 5000
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
typedef struct{//单词 
	char *field;//字段数组指针 
	int num;//字频	
}Word;
typedef struct{//顺序表 
	Word *elem;
	int length;//当前长度 
	int listsize;//最大长度 
}SqList;
typedef struct LNode{//链表 
	Word data;
	struct LNode *next;
}LNode,*LinkList;
typedef struct BiTNode{//二叉树 
	Word data;
	struct BiTNode *lchild,*rchild;
}BiTNode,*BiTree;
typedef struct{//基于开放地址法的哈希表 
	Word *elem;//基表 
	int number;//使用个数 此处未设space空间大小值，是因为使用算法不会产生空间不足的情况
}Table;   
void InitSqList(SqList &S){//初始化顺序表 
	S.elem=(Word *)malloc(LIST_INIT_SIZE*sizeof(Word));
	if(!S.elem)exit(OVERFLOW);
	S.length=0;
	S.listsize=LIST_INIT_SIZE;
}
void InitLinkList(LinkList &L){ //初始化单链表
	L=(LinkList)malloc(sizeof(LNode));
	if(!L)exit(OVERFLOW);
	L->next=NULL; 
}
void BiTreeInsert(BiTree &T,Word *Array,int base,int top){//向二叉树排序树中插入元素 
	if(top>=base){
		T=(BiTree)malloc(sizeof(BiTNode));
		int median=(base+top)/2;
		T->data.field=(char *)malloc((strlen(Array[median].field)+1)*sizeof(char));
		strcpy(T->data.field,Array[median].field);
		T->data.num=Array[median].num;
		T->lchild=T->rchild=NULL;
		BiTreeInsert(T->lchild,Array,base,median-1);//创建左支树 
		BiTreeInsert(T->rchild,Array,median+1,top); //创建右支树 
	}
}
void InitBiTree(SqList &S,BiTree &T){//创建二叉树排序树,创建Word数组 
	Word Array[S.length];
	for(int i=0;i<S.length;i++){
		Array[i].field=S.elem[i].field;
		Array[i].num=S.elem[i].num;
	}
	BiTreeInsert(T,Array,0,S.length-1); 
}
int Hash(Table &B,char *wordarray){//哈希函数 
	int hashvalue=0;
	for(int i=0;wordarray[i];i++)hashvalue+=wordarray[i];
	hashvalue%=B.number;
	return hashvalue;
}
void TableInsert(Table &B,char *wordarray,int num){//向哈希表中插入单词 
	int hashvalue=Hash(B,wordarray);
	if(!B.elem[hashvalue].field){//未发生冲突 
		B.elem[hashvalue].field=(char *)malloc((strlen(wordarray)+1)*sizeof(char));
		strcpy(B.elem[hashvalue].field,wordarray);
		B.elem[hashvalue].num=num;
	}else{
		
	}
}
void InitTable(SqList &S,Table &B){////借助Word数组生成一个刚好为单词数量的哈希表，空间刚好满足 
	B.elem=(Word *)malloc(S.length*sizeof(Word));
	if(!B.elem)exit(OVERFLOW);
	B.number=S.length;//单词数量 
	for(int i=0;i<B.number;i++)B.elem[i].field=NULL;//初始化，便于检测是否发生冲突 
}
void SqListInsert(SqList &S,char *wordarray,int length){//顺序表插入元素 
	Word *p;
	if(S.length>=S.listsize){
		Word *newbase;
		newbase=(Word *)realloc(S.elem,(S.listsize+LISTINCREMENT)*sizeof(Word));
		if(!newbase)
			exit(OVERFLOW);
		S.elem=newbase;
		S.listsize +=LISTINCREMENT;
	}
	for(int i=0;i<S.length;i++)//搜索判断是否存在相同的单词 
		if(!strcmp(S.elem[i].field,wordarray)){
			S.elem[i].num++;
			return;
		}
	S.elem[S.length].field=(char *)malloc((length+1)*sizeof(char));
	strcpy(S.elem[S.length].field,wordarray);
	S.elem[S.length].num=1;
	S.length++;
}
void BiTNodeDisp(BiTree &T){//输出二叉排序树节点，按照中序是顺序输出的
	if(T){ 
		BiTNodeDisp(T->lchild);//左支树 
		FILE *Outfile;
		Outfile=fopen("Outfile4.txt","a");//采用追加的方式 
		fprintf(Outfile,"%s  %d\n",T->data.field,T->data.num);
		fclose(Outfile);
		BiTNodeDisp(T->rchild);//右支树 
	}
}
int BiTNodeCount(BiTree &T){//计算二叉排序树的节点数量，用于词频统计 
	if(T)return BiTNodeCount(T->lchild)+BiTNodeCount(T->rchild)+1;
		else return 0;
}
void DispFileBiTree(BiTree &T){//输出排序树 
	FILE *Outfile;
	Outfile=fopen("Outfile4.txt","w");
	fprintf(Outfile,"%d\n",BiTNodeCount(T));//写文件头 
	fclose(Outfile);
	BiTNodeDisp(T);//输出写文件体 
}
int BiTNodeFind(BiTree &T,char *wordarray){//二叉排序树对于结点的查找
	if(T){
		int result=strcmp(T->data.field,wordarray);
		if(result==0)return T->data.num;
		else if(result>0)return BiTNodeFind(T->lchild,wordarray);//T.data->field<wordarray进左支树 
			else return BiTNodeFind(T->rchild,wordarray); 
	}else return 0; 
} 
int BiTreeASL(BiTree &T,int depth){
	if(T){
		return BiTreeASL(T->lchild,depth+1)+BiTreeASL(T->rchild,depth+1)+depth;
	}else return 0;
} 
void FindSuccess(int num,double time,double asl){
	printf("此单词的字频为：%d\n",num);
	printf("查找该单词所花的时间：%.2lf\n",time);
	printf("平均查找长度：%.1lf\n",asl); 
}
void FindWordBiTree(BiTree &T){//二叉排序树查找 
	char wordarray[WordMaxLength];
	fflush(stdin);
	printf("请输入待查找的单词："); 
	gets(wordarray);
	_LARGE_INTEGER time_start,time_end;//开始时间与结束时间 
	double dqFreq;LARGE_INTEGER f;//计时器频率
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//计时开始
	int num;
	num=BiTNodeFind(T,wordarray);
	QueryPerformanceCounter(&time_end);//计时结束
	if(num){
		FindSuccess(num,1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq,(double)BiTreeASL(T,1)/BiTNodeCount(T));
	}else{
		printf("查找失败\n");
	}
}
void DestroySqList(SqList &S){//释放顺序表 
	for(int i=0;i<S.length;i++)
		free(S.elem[i].field);
	free(S.elem);
	S.elem=NULL;
	S.length=0;
	S.listsize=0;
}
void DestroyLinkList(LinkList &L){//释放链表 
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
void DestoryBiTree(BiTree &T){//释放二叉排序树 
	if(T){
		DestoryBiTree(T->lchild);
		DestoryBiTree(T->rchild);//后序释放 
		free(T->data.field);
		free(T);
		T=NULL; 
	} 
}
//主任务单元 
int Partition(Word *(*array),int low,int high)//快速排序更换位置 
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
void QSort(Word *(*array),int low,int high)//顺序表快速排序，先根据首字母的ASCLL大小进行排序 ，再将相同的字母的进行二次排序
{	if(low<high)
	{	int pivotloc=Partition(array,low,high);
		QSort(array,low,pivotloc-1);
		QSort(array,pivotloc+1,high);
	}
}
void SimpleSort(LinkList &L){//链表简单选择排序，重现第十章排序算法二,将选择排序算法变现到链表中 
	LinkList p,q,min;
	Word temp;
	for(p=L->next;p;p=p->next){//p为基准指针，q为选择最小值指针 
		for(q=p->next,min=p;q;q=q->next)
			if(strcmp(q->data.field,min->data.field)<0)min=q;
		if(min!=p){
			temp.field=p->data.field;temp.num=p->data.num;//借助temp分别交换字段地址与字频 
			p->data.field=min->data.field;p->data.num=min->data.num;
			min->data.field=temp.field;min->data.num=temp.num;
		}		
	} 
} 
void SqListSort(SqList &S){//进行排序 
	int low=0,high=S.length-1;
	Word *array[S.length];
	for(int i=0;i<S.length;i++){
		array[i]=&S.elem[i];
	}
	QSort(array,low,high);	//加入排序算法
}
void DispFileSqList(SqList S){//输出文件到对应的文件中  
	FILE *Outfile;
	Outfile=fopen("Outfile1.txt","w");
	fprintf(Outfile,"%d\n",S.length);
	char word;
	for(int i=0;i<S.length;i++)fprintf(Outfile,"%s  %d\n",S.elem[i].field,S.elem[i].num);
	fclose(Outfile);
}
void DispFileLinkList(LinkList &L){//输出文件到对应的文件中  
	FILE *Outfile;
	Outfile=fopen("Outfile2.txt","w");
	LinkList q=L->next;
	int length;
	for(length=0;q;q=q->next)length++;
	fprintf(Outfile,"%d\n",length);
	for(LinkList q=L->next;q;q=q->next)fprintf(Outfile,"%c  %d\n",q->data.field,q->data.num);
	fclose(Outfile);
}
int HalfSearch(SqList &S,char *wordarray,int base,int top){//折半查找功能区 
	if(top<base)return 0;
	else{
		int Standard=(base+top)/2;//定义中位点 
		int Result=strcmp(S.elem[Standard].field,wordarray);
		if(Result==0)return S.elem[Standard].num;
		else if(Result<0)return HalfSearch(S,wordarray,Standard+1,top);//如果小于0，则说明Standard小于wordarray，向后折半查找 
			else return HalfSearch(S,wordarray,base,Standard-1);//如果大于0，则说明Standard大于wordarray，向前折半查找 
	}
}
void FindWordHalf(SqList &S){	//折半查找 
	char wordarray[WordMaxLength];
	fflush(stdin);
	printf("请输入待查找的单词："); 
	gets(wordarray);
	_LARGE_INTEGER time_start,time_end;//开始时间与结束时间 
	double dqFreq;LARGE_INTEGER f;//计时器频率
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//计时开始
	int num;
	num=HalfSearch(S,wordarray,0,S.length-1);
	QueryPerformanceCounter(&time_end);//计时结束
	if(num){
		FindSuccess(num,1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq,0);
	}else{
		printf("查找失败\n");
	}
}
int LinkListCount(LinkList &L){
	LinkList q=L;
	int n=0;
	while(q=q->next)n++;
		return n;
}
void FindWordLinkList(LinkList &L){//链表顺序查找 
	char wordarray[WordMaxLength];
	fflush(stdin);
	printf("请输入待查找的单词："); 
	gets(wordarray);
	_LARGE_INTEGER time_start,time_end;//开始时间与结束时间 
	double dqFreq;LARGE_INTEGER f;//计时器频率
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//计时开始
	LinkList p;
	for(p=L->next;p;p=p->next){
		if(!strcmp(p->data.field,wordarray))
			break;
	}
	QueryPerformanceCounter(&time_end);//计时结束
	if(p){
		FindSuccess(p->data.num,1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq,(double)(LinkListCount(L)+1)/2);
	}else{
		printf("查找失败\n");
	}
}
void FindWordSqList(SqList &S){	//顺序表顺序查找 
	char wordarray[WordMaxLength];
	fflush(stdin);
	printf("请输入待查找的单词："); 
	gets(wordarray);
	_LARGE_INTEGER time_start,time_end;//开始时间与结束时间 
	double dqFreq;LARGE_INTEGER f;//计时器频率
	QueryPerformanceFrequency(&f);
	dqFreq=(double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//计时开始
	int i;
	for(i=0;i<S.length;i++){
		if(!strcmp(S.elem[i].field,wordarray))
			break;
	}
	QueryPerformanceCounter(&time_end);//计时结束
	if(i<S.length){
		FindSuccess(S.elem[i].num,1000000*(time_end.QuadPart-time_start.QuadPart)/dqFreq,(double)(S.length+1)/2);
	}else{
		printf("查找失败\n");
	}
}
void LinkListInsert(LinkList &L,char *wordarray,int length){//向单链表中插入元素,含头节点，方便进行操作 
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
void ReadFile(SqList &S,LinkList &L){//读取文件，并分解成单词 ,进入到不同的数据存储结构中 
	FILE *Infile;
	if((Infile=fopen("Infile.txt","r"))==NULL){
		printf("Failed to read file!");
		exit(1);
	}
	char wordarray[WordMaxLength],word; 
	int length;
	while((word=fgetc(Infile))!=EOF){//将英文单词从文段中拆出 
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
		SqListInsert(S,wordarray,length);//进入到顺序表中 
		LinkListInsert(L,wordarray,length);//进入到链表中 
	}//在ReadFile时，采用的是定长读取的方式，原有的wordarray中含有其他垃圾字母，在生成 filed数组时是变长的,所以需要输入length变量以实现变长生成 
	SqListSort(S);//顺序表排序
	SimpleSort(L);//链表排序 
	fclose(Infile);
}
void Menu(char *name,char *option1,char *option2){
	printf("    /****基于不同策略的单词统计和检索系统****/\n\n");
	printf("   ------%s------\n\n",name);
	printf("1.%s\n",option1);
	printf("2.%s\n",option2);
	printf("3.返回上一级\n");
	printf("请按相应数字键进行选择 ");
} 
void Linearlist(SqList &S,LinkList &L){//基于线性表的查找菜单 
	int x; 
	while(1){
		Menu("基于线性表的查找","顺序查找","折半查找"); 
		scanf("%d",&x);
		if(x==3){
			system("cls");
			break;
		}
		else switch(x){
			case 1:system("cls");
				while(1){
					Menu("基于顺序表的查找","基于顺序表的顺序查找","基于链表的查找"); 
					scanf("%d",&x);
					if(x==3){
						system("cls");
						break;
					}
					else switch(x){
						case 1:system("cls");
							while(1){ 
								Menu("基于顺序表的顺序查找","词频统计","单词查找");
								scanf("%d",&x);
								if(x==3){
									system("cls");
									break;
								}	
								else switch(x){
									case 1:
										DispFileSqList(S);//输出 
										printf("词频统计成功！已导出至Outfile1.txt文件中！\n"); 
										fflush(stdin);
										if(getchar())system("cls");
										break;
									case 2:
										FindWordSqList(S);//查找 
										fflush(stdin);
										if(getchar())system("cls");
										break;
								}
								
							}
							break;
						case 2:system("cls");
							while(1){ 
								Menu("基于链表的顺序查找","词频统计","单词查找");
								scanf("%d",&x);
								if(x==3){
									system("cls");
									break;
								}	
								else switch(x){
									case 1:
										DispFileLinkList(L);//输出 
										printf("词频统计成功！已导出至Outfile2.txt文件中！\n"); 
										fflush(stdin);
										if(getchar())system("cls");
										break;
									case 2:
										FindWordLinkList(L);//查找 
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
					Menu("折半查找","词频统计","单词查找");
					scanf("%d",&x);
					if(x==3){
						system("cls");
						break;
					}
					else switch(x){
						case 1:
							DispFileSqList(S);//输出 
							printf("词频统计成功！已导出至Outfile3.txt文件中！\n"); 
							fflush(stdin);
							if(getchar())system("cls");
								break;
						case 2:
							FindWordHalf(S);//查找 
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
		Menu("基于二叉排序树的查找","词频统计","单词查找");
		scanf("%d",&x);
		if(x==3){
			system("cls");
			break;
		}	
		else switch(x){
			case 1:
				DispFileBiTree(T);//输出 
				printf("词频统计成功！已导出至Outfile4.txt文件中！\n"); 
				fflush(stdin);
				if(getchar())system("cls");
					break;
			case 2:
				FindWordBiTree(T);//查找 
				fflush(stdin);
				if(getchar())system("cls");
				break;
		}							
	}
}
void Hashtable(){
	int x;
	while(1){ 
		Menu("基于哈希表的查找","基于开放地址法的哈希查找","基于链地址法的哈希查找");
		scanf("%d",&x);
		if(x==3){
			system("cls");
			break;
		}	
		else switch(x){
			case 1:
				while(1){ 
					Menu("基于开放地址法的哈希查找","词频统计","单词查找");
					scanf("%d",&x);
					if(x==3){
						system("cls");
						break;
					}	
					else switch(x){
						case 1:
								//输出 
							printf("词频统计成功！已导出至Outfile5.txt文件中！\n"); 
							fflush(stdin);
							if(getchar())system("cls");
						break;
						case 2:
								//查找 
							fflush(stdin);
							if(getchar())system("cls");
						break;
					}							
				}
				break;
			case 2:
				while(1){ 
					Menu("基于链地址法的哈希查找","词频统计","单词查找");
					scanf("%d",&x);
					if(x==3){
						system("cls");
						break;
					}	
					else switch(x){
						case 1:
								//输出 
							printf("词频统计成功！已导出至Outfile6.txt文件中！\n"); 
							fflush(stdin);
							if(getchar())system("cls");
						break;
						case 2:
								//查找 
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
		printf("    /****基于不同策略的单词统计和检索系统****/\n\n\n");
		printf("1.基于线性表的查找\n");
		printf("2.基于二叉排序树的查找\n");
		printf("3.基于哈希表的查找\n");
		printf("4.退出\n");
		printf("请按相应数字键进行选择 ");
		scanf("%d",&x);
		if(x==4)break;
		else switch(x){
			case 1:system("cls");Linearlist(S,L);break;//线性表 
			case 2:system("cls");BinarySortTree(T);break;//二叉排序树 
			case 3:system("cls");break;//哈希表 
		} 	
	}
	DestroySqList(S);
	DestroyLinkList(L);
	DestoryBiTree(T); 
} 
