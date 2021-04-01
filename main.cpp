#include <stdio.h>//���ڲ�ͬ���Ե�Ӣ�ĵ��ʵĴ�Ƶͳ�ƺͼ���ϵͳ��ͼ�λ���
#include <graphics.h>//����EasyXͼ�ο�ͷ�ļ�
#include <string>//�ַ��������ͷ�ļ�
#include <math.h>//������ѧ�����ͷ�ļ�
#include <windows.h>//���ڸ߾���ʱ����� 
using namespace std;
#define OK 1
#define ERROR 0
#define WordMaxLength 30
#define WordMaxNum 5000
#define LIST_INIT_SIZE 100
#define LISTINCREMENT 10
typedef struct {//���� 
	char* field;//�ֶ�����ָ�� 
	int num;//��Ƶ	
}Word;
typedef struct {//˳��� 
	Word* elem;
	int length;//��ǰ���� 
	int listsize;//��󳤶� 
}SqList;
typedef struct LNode {//���� 
	Word data;
	struct LNode* next;
}LNode, * LinkList;
typedef struct BiTNode {//������ 
	Word data;
	struct BiTNode* lchild, * rchild;
}BiTNode, * BiTree;
typedef struct {//���ڿ��ŵ�ַ���Ĺ�ϣ�� 
	Word* elem;//���� 
	int number;//ʹ�ø��� �˴�δ��space�ռ��Сֵ������Ϊʹ���㷨��������ռ䲻������
}Table;
typedef struct WNode {//��������ַ���Ĺ�ϣ��ר������ 
	char* field;//�ֶ�����ָ�� 
	int num;//��Ƶ
	struct WNode* next;//���� 
}WNode, * WLink;
typedef struct {//��������ַ���Ĺ�ϣ�� 
	WLink elem;//���� 
	int number;//��ǰ���� 
}Chain;
int TableASL, ChainASL;//����ȫ�ֱ������ڼ����ϣ��ASLֵ 
char* wchar_to_char(const wchar_t* wchar)
{
	char* m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}
wchar_t* char_to_wchar(const char* cchar)
{
	wchar_t* m_wchar;
	int len = MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}
void InitSqList(SqList& S) {//��ʼ��˳��� 
	S.elem = (Word*)malloc(LIST_INIT_SIZE * sizeof(Word));
	if (!S.elem)exit(OVERFLOW);
	S.length = 0;
	S.listsize = LIST_INIT_SIZE;
}
void InitLinkList(LinkList& L) { //��ʼ��������
	L = (LinkList)malloc(sizeof(LNode));
	if (!L)exit(OVERFLOW);
	L->next = NULL;
}
void BiTreeInsert(BiTree& T, Word* Array, int base, int top) {//��������������в���Ԫ�� 
	if (top >= base) {
		T = (BiTree)malloc(sizeof(BiTNode));
		int median = (base + top) / 2;
		T->data.field = (char*)malloc((strlen(Array[median].field) + 1) * sizeof(char));
		strcpy_s(T->data.field, strlen(Array[median].field) + 1, Array[median].field);
		T->data.num = Array[median].num;
		T->lchild = T->rchild = NULL;
		BiTreeInsert(T->lchild, Array, base, median - 1);//������֧�� 
		BiTreeInsert(T->rchild, Array, median + 1, top); //������֧�� 
	}
}
void InitBiTree(SqList& S, BiTree& T) {//����������������,����Word���� 
	Word* Array = new Word[S.length];
	for (int i = 0; i < S.length; i++) {
		Array[i].field = S.elem[i].field;
		Array[i].num = S.elem[i].num;
	}
	BiTreeInsert(T, Array, 0, S.length - 1);
	delete []Array;
}
int Hash(int number, char* wordarray) {//��ϣ����:��ϣֵλ��0��number 
	int hashvalue = 0;
	for (int i = 0; wordarray[i]; i++)hashvalue += wordarray[i];
	hashvalue %= number;
	return hashvalue;
}
void TableInsert(Table& B, Word& array) {//���ϣ���в��뵥�� 
	int hashvalue = Hash(B.number, array.field);//���ϣֵ 
	if (!B.elem[hashvalue].field) {//δ������ͻ 
		B.elem[hashvalue].field = (char*)malloc((strlen(array.field) + 1) * sizeof(char));//����ռ� 
		strcpy_s(B.elem[hashvalue].field, strlen(array.field) + 1, array.field);
		B.elem[hashvalue].num = array.num;
		TableASL++;
	}
	else {
		int i;
		for (i = 1; B.elem[(hashvalue + i) % B.number].field != NULL; i++)TableASL++;//̽���ַ 
		B.elem[(hashvalue + i) % B.number].field = (char*)malloc((strlen(array.field) + 1) * sizeof(char));//����ռ� 
		strcpy_s(B.elem[(hashvalue + i) % B.number].field, strlen(array.field) + 1, array.field);//���� 
		B.elem[(hashvalue + i) % B.number].num = array.num;
	}
}
void InitTable(SqList& S, Table& B) {//����Word��������һ���պ�Ϊ���������Ĺ�ϣ���ռ�պ����� 
	B.elem = (Word*)malloc(S.length * sizeof(Word));
	if (!B.elem)exit(OVERFLOW);
	B.number = S.length;//�������� 
	for (int i = 0; i < B.number; i++)B.elem[i].field = NULL;//��ʼ�������ڼ���Ƿ�����ͻ
	for (int j = 0; j < S.length; j++)TableInsert(B, S.elem[j]);
}
void SqListInsert(SqList& S, char* wordarray, int length) {//˳������Ԫ�� 
	Word* p;
	if (S.length >= S.listsize) {
		Word* newbase;
		newbase = (Word*)realloc(S.elem, (S.listsize + LISTINCREMENT) * sizeof(Word));
		if (!newbase)
			exit(OVERFLOW);
		S.elem = newbase;
		S.listsize += LISTINCREMENT;
	}
	for (int i = 0; i < S.length; i++)//�����ж��Ƿ������ͬ�ĵ��� 
		if (!strcmp(S.elem[i].field, wordarray)) {
			S.elem[i].num++;
			return;
		}
	S.elem[S.length].field = (char*)malloc((length + 1) * sizeof(char));
	strcpy_s(S.elem[S.length].field, strlen(wordarray) + 1, wordarray);
	S.elem[S.length].num = 1;
	S.length++;
}
void BiTNodeDisp(BiTree& T) {//��������������ڵ㣬����������˳�������
	if (T) {
		BiTNodeDisp(T->lchild);//��֧�� 
		FILE* Outfile;
		fopen_s(&Outfile, "Outfile4.txt", "a");//����׷�ӵķ�ʽ 
		fprintf(Outfile, "%s  %d\n", T->data.field, T->data.num);
		fclose(Outfile);
		BiTNodeDisp(T->rchild);//��֧�� 
	}
}
int BiTNodeCount(BiTree& T) {//��������������Ľڵ����������ڴ�Ƶͳ�� 
	if (T)return BiTNodeCount(T->lchild) + BiTNodeCount(T->rchild) + 1;
	else return 0;
}
void DispFileSuccess(int tag) {//����ɹ�
	HWND hwnd;
	wchar_t s[200];
	hwnd = GetHWnd(); // ��ȡ��ǰ���ھ��
	_stprintf_s(s, _T("��Ƶͳ�Ƴɹ����ѵ�����Outfile%d.txt�ļ��У�\n"), tag);
	MessageBox(hwnd, s, _T("��Ƶ����ϵͳ���"), MB_ICONINFORMATION);
}
void DispFileBiTree(BiTree& T) {//��������� 
	FILE* Outfile;
	fopen_s(&Outfile, "Outfile4.txt", "w");
	fprintf(Outfile, "%d\n", BiTNodeCount(T));//д�ļ�ͷ 
	fclose(Outfile);
	BiTNodeDisp(T);//���д�ļ��� 
	DispFileSuccess(4);
}
int BiTNodeFind(BiTree& T, char* wordarray) {//�������������ڽ��Ĳ���
	if (T) {
		int result = strcmp(T->data.field, wordarray);
		if (result == 0)return T->data.num;
		else if (result > 0)return BiTNodeFind(T->lchild, wordarray);//T.data->field<wordarray����֧�� 
		else return BiTNodeFind(T->rchild, wordarray);
	}
	else return 0;
}
void FindSuccess(int num, double time, double asl) {
	HWND hwnd = GetHWnd();// ��ȡ��ǰ���ھ��
	wchar_t s[200];
	_stprintf_s(s, _T("�˵��ʵ���ƵΪ��%d\n���Ҹõ���������ʱ�䣺%.2lf\nƽ�����ҳ��ȣ�%.1lf\n"), num,time,asl);
	MessageBox(hwnd, s, _T("���ʲ���ϵͳ���"),MB_SETFOREGROUND);
}
void FindFail() {
	HWND hwnd;
	hwnd = GetHWnd(); // ��ȡ��ǰ���ھ��
	MessageBox(hwnd, _T("����ʧ��,�Ķ���û�д˵���"), _T("���ʲ���ϵͳ���"), MB_SETFOREGROUND| MB_ICONHAND);
}
void FindWordBiTree(BiTree& T,char *wordarray) {//�������������� 
	_LARGE_INTEGER time_start, time_end;//��ʼʱ�������ʱ�� 
	double dqFreq; LARGE_INTEGER f;//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ
	int num;
	num = BiTNodeFind(T, wordarray);
	QueryPerformanceCounter(&time_end);//��ʱ����
	if (num) {
		FindSuccess(num, 1000000 * (time_end.QuadPart - time_start.QuadPart) / dqFreq, ((BiTNodeCount(T) + 1) / BiTNodeCount(T)) * log2(BiTNodeCount(T) + 1) - 1);
	}
	else {
		FindFail();
	}
}
void DestroySqList(SqList& S) {//�ͷ�˳��� 
	for (int i = 0; i < S.length; i++)
		free(S.elem[i].field);
	free(S.elem);
	S.elem = NULL;
	S.length = 0;
	S.listsize = 0;
}
void DestoryChain(Chain& C) {//�ͷŻ�������ַ���Ĺ�ϣ�� 
	WLink p, q;
	for (int j = 0; j < C.number; j++) {
		if (C.elem[j].next == NULL)continue;
		p = C.elem[j].next;
		while (p) {
			q = p->next;
			free(p);
			p = q;
		}
		free(C.elem);
		C.number = 0;
	}
}
void DestroyLinkList(LinkList& L) {//�ͷ����� 
	LinkList p = L->next, q = p->next;
	while (p) {
		q = p->next;
		free(p->data.field);
		free(p);
		p = NULL;
	}
	L = NULL;
}
void DestoryTable(Table& B) {//�ͷſ��ŵ�ַ�Ĺ�ϣ�� 
	for (int i = 0; i < B.number; i++)
		free(B.elem[i].field);
	free(B.elem);
	B.elem = NULL;
	B.number = 0;
}
void DestoryBiTree(BiTree& T) {//�ͷŶ��������� 
	if (T) {
		DestoryBiTree(T->lchild);
		DestoryBiTree(T->rchild);//�����ͷ� 
		free(T->data.field);
		free(T);
		T = NULL;
	}
}
void ChainInsert(Chain& C, Word& array) {//������ַ���Ĺ�ϣ���в���Ԫ�� 
	int hashvalue = Hash(C.number, array.field);
	WLink q = &C.elem[hashvalue], p = q->next;
	while (p) {
		q = p;
		p = p->next;
		ChainASL++;//ͳ��ASLֵ 
	}//�ҵ����һ���ڵ㣬��������������� 
	p = (WNode*)malloc(sizeof(WNode));
	q->next = p;//ǰָ������ 
	p->next = NULL;//��ָ������ 
	p->field = (char*)malloc((strlen(array.field) + 1) * sizeof(char));
	strcpy_s(p->field, strlen(array.field) + 1, array.field);
	p->num = array.num;
}
void InitChain(SqList& S, Chain& C) {//��������ַ���Ĺ�ϣ�� 
	C.elem = (WNode*)malloc(S.length * sizeof(WNode));
	if (!C.elem)exit(OVERFLOW);
	C.number = S.length;
	for (int i = 0; i < C.number; i++)C.elem[i].next = NULL;//��ʼ�� 
	for (int i = 0; i < S.length; i++)ChainInsert(C, S.elem[i]);//����Ԫ�� 
}
int Partition(Word* array, int low, int high)//�����������λ�� 
{
	Word pivotkey;
	pivotkey.field = array[low].field; pivotkey.num = array[low].num;
	while (low < high)
	{
		while (low < high && strcmp(array[high].field, pivotkey.field)>0)
			--high;
		array[low].field = array[high].field; array[low].num = array[high].num;
		while (low < high && strcmp(array[low].field, pivotkey.field) < 0)
			++low;
		array[high].field = array[low].field; array[high].num = array[low].num;
	}
	array[low].field = pivotkey.field; array[low].num = pivotkey.num;
	return low;
}
void QSort(Word* array, int low, int high)//˳�����������ȸ�������ĸ��ASCLL��С�������� ���ٽ���ͬ����ĸ�Ľ��ж�������
{
	if (low < high)
	{
		int pivotloc = Partition(array, low, high);
		QSort(array, low, pivotloc - 1);
		QSort(array, pivotloc + 1, high);
	}
}
void SimpleSort(LinkList& L) {//�����ѡ���������ֵ�ʮ�������㷨��,��ѡ�������㷨���ֵ������� 
	LinkList p, q, min;
	Word temp;
	for (p = L->next; p; p = p->next) {//pΪ��׼ָ�룬qΪѡ����Сֵָ�� 
		for (q = p->next, min = p; q; q = q->next)
			if (strcmp(q->data.field, min->data.field) < 0)min = q;
		if (min != p) {
			temp.field = p->data.field; temp.num = p->data.num;//����temp�ֱ𽻻��ֶε�ַ����Ƶ 
			p->data.field = min->data.field; p->data.num = min->data.num;
			min->data.field = temp.field; min->data.num = temp.num;
		}
	}
}
void DispFileTable(Table& B) {
	Word* array = new Word[B.number];
	for (int i = 0; i < B.number; i++) {
		array[i].field = B.elem[i].field;
		array[i].num = B.elem[i].num;
	}
	QSort(array, 0, B.number - 1);//��ֹ�޸�ԭ�� 
	FILE* Outfile;
	fopen_s(&Outfile, "Outfile5.txt", "w");
	fprintf(Outfile, "%d\n", B.number);
	for (int j = 0; j < B.number; j++)fprintf(Outfile, "%s  %d\n", array[j].field, array[j].num);
	fclose(Outfile);
	DispFileSuccess(5);
	delete []array;
}
void DispFileSqList(SqList S, int flag) {//����ļ�����Ӧ���ļ���  
	FILE* Outfile;
	if (!flag)fopen_s(&Outfile, "Outfile1.txt", "w");
	else fopen_s(&Outfile, "Outfile3.txt", "w");
	fprintf(Outfile, "%d\n", S.length);
	for (int i = 0; i < S.length; i++)fprintf(Outfile, "%s  %d\n", S.elem[i].field, S.elem[i].num);
	fclose(Outfile);
	if (!flag)DispFileSuccess(1);
	else DispFileSuccess(3);
}
void DispFileLinkList(LinkList& L) {//����ļ�����Ӧ���ļ���  
	FILE* Outfile=NULL;
	fopen_s(&Outfile, "Outfile2.txt", "w");
	LinkList q = L->next;
	int length;
	for (length = 0; q; q = q->next)length++;
	fprintf(Outfile, "%d\n", length);
	for (LinkList q = L->next; q; q = q->next)fprintf(Outfile, "%s  %d\n", q->data.field, q->data.num);
	fclose(Outfile);
	DispFileSuccess(2);
}
int HalfSearch(SqList& S, char* wordarray, int base, int top) {//�۰���ҹ����� 
	if (top < base)return 0;
	else {
		int Standard = (base + top) / 2;//������λ�� 
		int Result = strcmp(S.elem[Standard].field, wordarray);
		if (Result == 0)return S.elem[Standard].num;
		else if (Result < 0)return HalfSearch(S, wordarray, Standard + 1, top);//���С��0����˵��StandardС��wordarray������۰���� 
		else return HalfSearch(S, wordarray, base, Standard - 1);//�������0����˵��Standard����wordarray����ǰ�۰���� 
	}
}
void FindWordHalf(SqList& S,char *wordarray) {	//�۰���� 
	_LARGE_INTEGER time_start, time_end;//��ʼʱ�������ʱ�� 
	double dqFreq; LARGE_INTEGER f;//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ
	int num;
	num = HalfSearch(S, wordarray, 0, S.length - 1);
	QueryPerformanceCounter(&time_end);//��ʱ����
	if (num) {
		FindSuccess(num, 1000000 * (time_end.QuadPart - time_start.QuadPart) / dqFreq, log(S.length) / log(2));
	}
	else {
		FindFail();
	}
}
int LinkListCount(LinkList& L) {
	LinkList q = L;
	int n = 0;
	while (q = q->next)n++;
	return n;
}
void FindWordLinkList(LinkList& L,char *wordarray) {//����˳����� 
	_LARGE_INTEGER time_start, time_end;//��ʼʱ�������ʱ�� 
	double dqFreq; LARGE_INTEGER f;//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ
	LinkList p;
	for (p = L->next; p; p = p->next) {
		if (!strcmp(p->data.field, wordarray))
			break;
	}
	QueryPerformanceCounter(&time_end);//��ʱ����
	if (p) {
		FindSuccess(p->data.num, 1000000 * (time_end.QuadPart - time_start.QuadPart) / dqFreq, (double)(LinkListCount(L) + 1) / 2);
	}
	else {
		FindFail();
	}
}
void FindWordSqList(SqList& S,char *wordarray) {	//˳���˳����� 
	_LARGE_INTEGER time_start, time_end;//��ʼʱ�������ʱ�� 
	double dqFreq; LARGE_INTEGER f;//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ
	int i;
	for (i = 0; i < S.length; i++) {
		if (!strcmp(S.elem[i].field, wordarray))
			break;
	}
	QueryPerformanceCounter(&time_end);//��ʱ����
	if (i < S.length) {
		FindSuccess(S.elem[i].num, 1000000 * (time_end.QuadPart - time_start.QuadPart) / dqFreq, (double)(S.length + 1) / 2);
	}
	else {
		FindFail();
	}
}
void FindWordTable(Table& B,char *wordarray) {
	_LARGE_INTEGER time_start, time_end;//��ʼʱ�������ʱ�� 
	double dqFreq; LARGE_INTEGER f;//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ
	int hashvalue = Hash(B.number, wordarray);
	int i, num = 0;
	for (i = 0; i < B.number; i++)
		if (!strcmp(B.elem[(hashvalue + i) % B.number].field, wordarray)) {
			num = B.elem[(hashvalue + i) % B.number].num;
			break;
		}
	QueryPerformanceCounter(&time_end);//��ʱ����
	if (num) {
		FindSuccess(num, 1000000 * (time_end.QuadPart - time_start.QuadPart) / dqFreq, (double)TableASL / B.number);
	}
	else {
		FindFail();
	}
}
void FindWordChain(Chain& C,char *wordarray) {
	_LARGE_INTEGER time_start, time_end;//��ʼʱ�������ʱ�� 
	double dqFreq; LARGE_INTEGER f;//��ʱ��Ƶ��
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	QueryPerformanceCounter(&time_start);//��ʱ��ʼ
	int hashvalue = Hash(C.number, wordarray);
	int num = 0;
	WLink p = C.elem[hashvalue].next;
	while (p) {
		if (!strcmp(p->field, wordarray))num = p->num;
		p = p->next;
	}
	QueryPerformanceCounter(&time_end);//��ʱ����
	if (num) {
		FindSuccess(num, 1000000 * (time_end.QuadPart - time_start.QuadPart) / dqFreq, (double)ChainASL / C.number);
	}
	else {
		FindFail();
	}
}
void LinkListInsert(LinkList& L, char* wordarray, int length) {//�������в���Ԫ��,��ͷ�ڵ㣬������в��� 
	LinkList p = L->next, q = L;
	while (p) {
		if (!strcmp(p->data.field, wordarray)) {
			p->data.num++;
			break;
		}
		q = p;
		p = p->next;
	}
	if (!p) {
		p = (LinkList)malloc(sizeof(LNode));
		p->data.field = (char*)malloc((length + 1) * sizeof(char));
		strcpy_s(p->data.field, strlen(wordarray) + 1, wordarray);
		p->data.num = 1;
		q->next = p;
		p->next = NULL;
	}
}
void ReadFile(SqList& S, LinkList& L) {//��ȡ�ļ������ֽ�ɵ��� ,���뵽��ͬ�����ݴ洢�ṹ�� 
	FILE* Infile=NULL;
	if ((fopen_s(&Infile, "Infile.txt", "r")) !=0) {
		exit(1);
	}
	char wordarray[WordMaxLength], word;
	int length;
	while (1) {//��Ӣ�ĵ��ʴ��Ķ��в��  
		length = 0;
		while ((word = fgetc(Infile)) != EOF) {
			if (!(('a' <= word && word <= 'z') || ('A' <= word && word <= 'Z')) && (length == 0)) {
				continue;
			}
			else if (!(('a' <= word && word <= 'z') || ('A' <= word && word <= 'Z')) && (length > 0)) {
				break;
			}
			if ('A' <= word && word <= 'Z')wordarray[length++] = word + 32;
			else wordarray[length++] = word;
		}
		if (word != EOF) {
			wordarray[length] = '\0';
			SqListInsert(S, wordarray, length);//���뵽˳����� 
			LinkListInsert(L, wordarray, length);//���뵽������
		}
		else break;
	}//��ReadFileʱ�����õ��Ƕ�����ȡ�ķ�ʽ��ԭ�е�wordarray�к�������������ĸ�������� filed����ʱ�Ǳ䳤��,������Ҫ����length������ʵ�ֱ䳤���� 
	QSort(S.elem, 0, S.length - 1);//˳�������
	SimpleSort(L);//�������� 
	fclose(Infile);
}
void DispFileChain(Chain& C) {//��������ַ���Ĺ�ϣ������ 
	Word* array = new Word[C.number];//����һ��Word���������������� 
	WLink p;
	for (int i = 0, j = 0; j < C.number; j++) {
		if (C.elem[j].next == NULL)continue;//����ǿ���ֱ���Թ� 
		p = C.elem[j].next;
		while (p) {
			array[i].field = p->field;
			array[i++].num = p->num;
			p = p->next;
		}
	}
	QSort(array, 0, C.number - 1);
	FILE* Outfile;
	fopen_s(&Outfile, "Outfile6.txt", "w");
	fprintf(Outfile, "%d\n", C.number);
	for (int i = 0; i < C.number; i++)fprintf(Outfile, "%s  %d\n", array[i].field, array[i].num);
	fclose(Outfile);
	DispFileSuccess(6);
}
void Disp(SqList& S, LinkList& L, BiTree& T, Table& B, Chain& C, int tag) {//��Ƶ��������
	if (tag == 1)DispFileSqList(S, 0);
	if (tag == 2)DispFileLinkList(L);
	if (tag == 3)DispFileSqList(S, 1);
	if (tag == 4)DispFileBiTree(T);
	if (tag == 5)DispFileTable(B);
	if (tag == 6)DispFileChain(C);
}
void Find(SqList& S, LinkList& L, BiTree& T, Table& B, Chain& C, int tag) {//���ʲ�������
	wchar_t wordarray[100];
	int i = InputBox(wordarray, 100, _T("��������Ҫ���ҵĵ���"), _T("���ڲ�ͬ���Ե�Ӣ�ĵ��ʲ���ϵͳ"), NULL, 0, 0, false);
	if (i == false)return;
	if (tag == 1)FindWordSqList(S, wchar_to_char(wordarray));
	if (tag == 2)FindWordLinkList(L, wchar_to_char(wordarray));
	if (tag == 3)FindWordHalf(S, wchar_to_char(wordarray));
	if (tag == 4)FindWordBiTree(T, wchar_to_char(wordarray));
	if (tag == 5)FindWordTable(B, wchar_to_char(wordarray));
	if (tag == 6)FindWordChain(C, wchar_to_char(wordarray));
}
void Switch(int &value,int tag) {
	value = tag;
	HWND hwnd=GetHWnd(); // ��ȡ��ǰ���ھ��
	wchar_t s[200];
	if (tag == 1)_stprintf_s(s, _T("��ǰ�������л�Ϊ˳���˳����ң�\n"));
	if (tag == 2)_stprintf_s(s, _T("��ǰ�������л�Ϊ�����˳����ң�\n"));
	if (tag == 3)_stprintf_s(s, _T("��ǰ�������л�Ϊ˳����۰���ң�\n"));
	if (tag == 4)_stprintf_s(s, _T("��ǰ�������л�Ϊ������������\n"));
	if (tag == 5)_stprintf_s(s, _T("��ǰ�������л�Ϊ���ڿ��Ŷ�ַ���Ĺ�ϣ��\n"));
	if (tag == 6)_stprintf_s(s, _T("��ǰ�������л�Ϊ��������ַ���Ĺ�ϣ��\n"));
	MessageBox(hwnd,s, _T("����ģʽ�л����"), MB_ICONINFORMATION);
}
IMAGE Interface;//�������ͼƬ
void Menu(SqList& S, LinkList& L, BiTree& T, Table& B, Chain& C) {
	loadimage(&Interface, L"Word.jpg");
	putimage(0, 0, &Interface);//��ʾ�˵�����
	int tag = 1;
	MOUSEMSG msg;//������������������Ϣ
	FlushMouseMsgBuffer();// ��������Ϣ��������������Ч�����Ϣ���뵽��ʽ�ж���
	while (true) {
		while (MouseHit())	//���������Ϣ;���������Ϣ��ʱ��ִ��,�ɼ�������������Ϣ
		{
			msg = GetMouseMsg();//��ȡ�����Ϣ
			if (WM_LBUTTONDOWN == msg.uMsg)//�ж������Ϣ;����������
			{
				if (msg.x > 39 && msg.x < 265 && msg.y > 115 && msg.y < 153) {//�˳�
					HWND hwnd = GetHWnd();
					if (IDYES == MessageBox(hwnd, _T("�Ƿ�ȷ���˳���ϵͳ��"), _T("���ڲ�ͬ���Ե�Ӣ�ĵ��ʵĴ�Ƶ�����͵��ʲ���ϵͳ"), MB_YESNO| MB_ICONQUESTION))
						exit(0);
				}
				if (msg.x > 39 && msg.x < 265 && msg.y > 264 && msg.y < 300)Switch(tag, 1);//˳���
				if (msg.x > 39 && msg.x < 265 && msg.y > 314 && msg.y < 351)Switch(tag, 2);//����
				if (msg.x > 39 && msg.x < 265 && msg.y > 364 && msg.y < 402)Switch(tag, 3);//�۰����
				if (msg.x > 39 && msg.x < 265 && msg.y > 414 && msg.y < 452)Switch(tag, 4);//����������
				if (msg.x > 39 && msg.x < 265 && msg.y > 515 && msg.y < 553)Switch(tag, 5);//���Ŷ�ַ��
				if (msg.x > 39 && msg.x < 265 && msg.y > 566 && msg.y < 603)Switch(tag, 6);//����ַ��
				if (msg.x > 1181 && msg.x < 1543 && msg.y > 160 && msg.y < 224)Disp(S, L, T, B, C, tag);//��Ƶ����
				if (msg.x > 1181 && msg.x < 1543 && msg.y > 252 && msg.y < 312)Find(S, L, T, B, C, tag);//���ʲ���
			}
		}
	}
}
int main() {
	int x;
	SqList S; InitSqList(S);//˳��� 
	LinkList L; InitLinkList(L);//���� 
	ReadFile(S, L);//��ȡ�ļ� 
	BiTree T; InitBiTree(S, T);//������ 
	Table B; InitTable(S, B);//���ڿ��ŵ�ַ���Ĺ�ϣ�� 
	Chain C; InitChain(S, C);//��������ַ���Ĺ�ϣ�� 
	initgraph(1600, 900);//���ô��ڴ�С
	HWND Win=GetHWnd();//��ȡ���ھ��
	SetWindowText(Win, _T("���ڲ�ͬ���Ե�Ӣ�ĵ��ʵĴ�Ƶͳ�ƺͼ���ϵͳ"));
	Menu(S,L,T,B,C);//�������˵�
	DestroySqList(S);//�ͷ�˳��� 
	DestroyLinkList(L);//�ͷ����� 
	DestoryBiTree(T);//�ͷŶ����� 
	DestoryTable(B);//�ͷŻ��ڿ��ŵ�ַ���Ĺ�ϣ��
	DestoryChain(C);//�ͷ�����ַ���Ĺ�ϣ�� 
	return 0;
}