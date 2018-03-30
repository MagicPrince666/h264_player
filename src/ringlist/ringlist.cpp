/*
单循环链表
*/

#include <iostream>

using namespace std;

const int MAX_N = 100;
//定义节点
class Node
{
public:
	//数据域
	double data;
	//指针域
	class Node *next;
};

class List
{
public:
	//构建循环链表（尾插法）
	List(double a[], int n){
		Node *p, *s;
		int i;
		L = (Node *)malloc(sizeof(Node));
		p = L;//使p始终指向尾节点
		for (i = 0; i < n; i++){
			s = (Node *)malloc(sizeof(Node));
			//读入数据
			s->data = a[i];
			//链接
			p->next = s;
			//p指向新的尾节点
			p = s;
		}
		//将最后的尾节点的指针域赋值成空
		p->next = L;
	}
	//是否为空
	bool isEmpty(){
		return (L->next == NULL);
	}
	//析构
	~List(){
		//pre和p起始位置没有限制
		Node *pre = L->next, *p = pre->next;
		Node *s = L->next;//第一个数据节点
		while (p->next!=s){
			free(pre);
			pre = p;
			p = p->next;
		}
		free(pre);
	}
	//输出
	void DisplayList(){
		//讨论不同情况
		if (isEmpty()){
			cout << "Empty." << endl;
		}
		else{
			//指向头第一个数据结点
			Node *p = L->next, *s = L->next;
			//自身地址（非遍历指针）不等于自身地址（遍历指针）
			while (p->next != s){
				cout<< " " << p->data << ends;
				p = p->next;
			}
			cout << endl;
		}
	}
	//插入
	bool NodeInsert(int i, double elem){
		//讨论两种情况
		if (isEmpty()){
			return false;
		}
		else{
			i--;//找到前一节点
			Node *p = L, *s = L, *pn;
			int j = 0;
			//找到位置
			while (i != j&&p->next != s){
				j++;
				p = p->next;
			}
			if (i != j&&p->next == s){
				return false;
			}
			//插入
			else{
				pn = (Node *)malloc(sizeof(Node));
				pn->data = elem;
				pn->next = p->next;
				p->next = pn;
				return true;
			}
		}
	}
	//按序号查找并返回值
	bool GetElem(int i, double &elem){
		Node *p = L, *s = L;
		int j = 0;
		if (isEmpty()){
			return false;
		}
		else{
			//找到位置
			while (i!=j&&p->next != s){
				j++;
				p = p->next;
			}
			if (i != j&&p->next == s){
				return false;
			}
			else{
				//赋值结果
				elem = p->data;
				return true;
			}
		}
	}
	//按值查找并返回序号
	bool LocatElem(int &i, double elem){
		if (isEmpty()){
			return false;
		}
		else{
			Node *p = L, *s = L;
			int j = 0;
			//遍历搜索
			while (elem!=p->data&&p->next != s){
				j++;
				p = p->next;
			}
			if (elem != p->data&&p->next == s){
				return false;
			}
			else{
				//找到位置并传出值
				i = j;
				return true;
			}
		}
	}
	//删除节点
	bool DeleteNode(int i){
		if (isEmpty()){
			return false;
		}
		else{
			i--;
			Node *p, *s = L, *pn = L;
			int j = 0;
			//遍历搜索
			while (i!=j&&pn->next != s){
				j++;
				pn = pn->next;
			}
			if (i!=j&&pn->next == s){
				return false;
			}
			else{
				//释放空间
				p = pn->next;
				pn->next = pn->next->next;
				free(p);
				return true;
			}
		}
	}
private:
	Node *L;
};

int main()
{
	double a[MAX_N], res = 0;
	int n,i = 0;
	cout << "Input n:";
	cin >> n;
	for (i = 0; i < n; i++){
		cin >> a[i];
	}
	List m_list(a,n);
	m_list.DisplayList();
	cout << "Input i,element:";
	cin >> i >> res;
	m_list.NodeInsert(i,res);
	m_list.DisplayList();
	cout << "Input i:";
	cin >> i;
	if (m_list.GetElem(i, res)){
		cout << "Location " << i << " , element: ";
		cout << res << endl;
	}
	else{
		cout << "error." << endl;
	}
	cout << "Input element:";
	cin >> res;
	if (m_list.LocatElem(i, res)){
		cout << "element: " << res << " , location: ";
		cout << i << endl;
	}
	else{
		cout << "error." << endl;
	}
	cout << "Input Location i for delete:";
	cin >> i;
	if (m_list.DeleteNode(i)){
		cout << "Location " << i << " is deleted." << endl;
		m_list.DisplayList();
	}
	else{
		cout << "error." << endl;
	}
	return 0;
}