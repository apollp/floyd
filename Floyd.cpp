#include<iostream>
#include<stdio.h>
using namespace std;
#define MaxVertexNum 100    /* ��󶥵�����Ϊ100 */
#define INFINITY 65535        /* �����ֵ65535*/
typedef int Vertex;         /* �ö����±��ʾ����,Ϊ���� */
typedef int WeightType;        /* �ߵ�Ȩֵ��Ϊ���� */
typedef struct ENode* Elementype;
typedef struct ENode* Edge;
typedef struct ENode {
    Vertex V1, V2;      /* �����<V1, V2> */
    WeightType Weight;  /* Ȩ�� */
}*Edge;
/*����ṹ�嶨��*/
typedef struct LNode {
    Elementype Data;
    struct LNode *Next;
}*List;
/* �ߵĶ��� */
/* ͼ���Ķ��� */
typedef struct GNode {
    int Nv;  /* ������ */
    int Ne;  /* ����   */
    WeightType G[MaxVertexNum][MaxVertexNum]; /* �ڽӾ��� */
}*MGraph;
/* ���ڽӾ���洢��ͼ���� */
MGraph CreateGraph(int VertexNum)
{ 
    /* ��ʼ��һ����VertexNum�����㵫û�бߵ�ͼ */
    Vertex V, W;
    MGraph Graph;
    /* ����ͼ */
    Graph = (MGraph)malloc(sizeof(struct GNode)); 
    if (Graph == NULL)
       return NULL;
    Graph->Nv = VertexNum;
    Graph->Ne = 0;
    /* ��ʼ���ڽӾ��� */
    for (V = 0; V < Graph->Nv; V++)
        for (W = 0; W < Graph->Nv; W++)
            Graph->G[V][W] = INFINITY;

    return Graph;
}

void InsertEdge(MGraph Graph, Edge E)
{
    /* ����� <V1, V2> */
    Graph->G[E->V1][E->V2] = E->Weight;
    /* �����<V2, V1> */
    Graph->G[E->V2][E->V1] = E->Weight;
} 
//β�巨��������
void Insert(List L, Elementype E)  
{
    List tmp, pre;
    for (pre = L; pre->Next != NULL; pre = pre->Next);
    tmp = (List)malloc(sizeof(struct LNode));
    if (tmp == NULL)
        return;
    // ������㲢���� 
    tmp->Data = E;
    tmp->Next = NULL;
    pre->Next = tmp;
    return;
}
//�Ƚ��������Ƿ����
bool Compare(Edge data, Edge E)
{
    if (data == NULL)
        return 0;
    if (data->V1 == E->V1 && data->V2 == E->V2)
        return 1;
    else if (data->V2 == E->V1 && data->V1 == E->V2)
        return 1;
    else
        return 0;
}
//����Ƿ����ظ���������
bool view(List L, Elementype E)
{
    int a = 1;
    List tmp;
    for (tmp = L; tmp != NULL; tmp = tmp->Next)
        if (Compare(tmp->Data, E))
            a = 0;
    return a;
}
MGraph BuildGraph()
{
    Edge E[1000];
    MGraph Graph;
    int Nv, i;
    cout << "������ͼ�Ķ�����:";
    cin >> Nv;
    //���붥����� 
    cout << endl;
    if (Nv == 0)
        return NULL;
//Ϊ�û������ṩ�ݴ���
    for (int i = 0; i < 3;)
    {
        if (Nv < 0)
        {
            cout << "��������ȷ�Ķ�������";
            cin >> Nv;
            cout << endl;
            i++;
            if (i == 3)
                return NULL;
        }
        else
            i++;
    }
    Graph = CreateGraph(Nv); /* ��ʼ����Nv�����㵫û�бߵ�ͼ */
    cout << "�����������";
    cin >> Graph->Ne;
    cout << endl;
    //������� 
    for (int i = 0; i < 3;)//Ϊ�û������ṩ�ݴ���
    {
        if (Graph->Ne<Nv - 1 || Graph->Ne>Nv * (Nv - 1) / 2)
        {
            cout << "��������ȷ�ı�����";
            cin >> Graph->Ne;
            cout << endl;
            i++;
            if (i == 3)
                return NULL;
        }
        else
            i++;
    }
    //��������
    List L;
    L = (List)malloc(sizeof(struct LNode));
    L->Data = NULL;
    L->Next = NULL;
    //�����߽�� 
        for (i = 0; i < Graph->Ne; i++)
        {
            cout << "�밴 ��� �յ� Ȩ�صĸ�ʽ����ߣ����磺0 2 3" << endl;   //����ߣ���ʽΪ"��� �յ� Ȩ��"�������ڽӾ���
            E[i] = (Edge)malloc(sizeof(struct ENode));
            cin >> E[i]->V1 >> E[i]->V2 >> E[i]->Weight;
            cout << endl;
            if (view(L, E[i]) && E[i]->V1 >= 0 && E[i]->V1 < Graph->Nv && E[i]->V2 >= 0 && E[i]->V2 < Graph->Nv)      
                //�жϱ��Ƿ������,���Ƿ�Ϸ�
            {
                Insert(L, E[i]);
                InsertEdge(Graph, E[i]);
            }
            else
            {
                cout << "����Ƿ��ߣ�����������ߣ���ʽΪ ��� �յ� Ȩ�أ����磺0 2 3" << endl;
                cin >> E[i]->V1 >> E[i]->V2 >> E[i]->Weight;
                cout << endl;
                if (view(L, E[i]) && E[i]->V1 >= 0 && E[i]->V1 < Graph->Nv && E[i]->V2 >= 0 && E[i]->V2 < Graph->Nv)       
                    //�жϱ��Ƿ������,���Ƿ�Ϸ�
                {
                    Insert(L, E[i]);
                    InsertEdge(Graph, E[i]);
                }
                else
                {
                    return NULL;
                }
            }
        }
    return Graph;
}
int D[MaxVertexNum][MaxVertexNum];//�洢���·����С
int path[MaxVertexNum][MaxVertexNum];//�洢·��
bool Floyd(MGraph Graph)
{
    Vertex i, j, k;
    /* ��ʼ�� */
    for (i = 0; i < Graph->Nv; i++)
        for (j = 0; j < Graph->Nv; j++) 
        {
            D[i][j] = Graph->G[i][j];
            path[i][j] = -1;
        }
    //kΪ�м�� 
    for (k = 0; k < Graph->Nv; k++)
        for (i = 0; i < Graph->Nv; i++) //iΪ��� 
            for (j = 0; j < Graph->Nv; j++)//jΪ�յ�
                if (D[i][k] + D[k][j] < D[i][j]) 
                {
                    D[i][j] = D[i][k] + D[k][j];//������С·��
                    if (i == j && D[i][j] < 0) /* �����ָ�ֵȦ */
                        return false; /* ������ȷ��������ش����� */
                    path[i][j] = k;//������С·���м䶥��
                }
    return true; /* �㷨ִ����ϣ�������ȷ��� */
}   
void showPath( int Nv1, int Nv2)
{
    if (path[Nv1][Nv2] != -1)
    {
        cout << "->" << path[Nv1][Nv2];
        int mid = path[Nv1][Nv2];
        showPath(mid, Nv2);
    }
    else

        cout << "->" << Nv2;
    
}
bool chack(MGraph Graph,int Nv1, int Nv2)
{
    if (Nv1 == Nv2)
        return false;
    else if (Nv1 > Graph->Nv || Nv2 > Graph->Nv)
        return false;
    else if (Nv1 < 0 || Nv2 < 0)
        return false;
    else
        return true;
}
int main()
{
 
    MGraph Graph;
    int Nv1,Nv2,k,i;
    Graph = BuildGraph();//����ͼ
    if (Graph == NULL)
    {
        cout << "����ͼʧ��";
    }
    else
    {
        if (Floyd(Graph))
        {
            cout << "���������·������ʼ����ֹ���㣬���磺0 2";
            cout << endl;
            cin >> Nv1 >> Nv2;
            if (!chack(Graph, Nv1, Nv2))
            {
                cout << "����Ƿ��������������붥��" << endl;
                cin >> Nv1 >> Nv2;
                if (!chack(Graph, Nv1, Nv2))
                {
                    cout << "����Ƿ����㽨��ͼʧ��";
                    return 0;
                }
            }
            else
            {
                if (D[Nv1][Nv2] < INFINITY)
                {
                    cout << Nv1 << "��" << Nv2 << "�����·������Ϊ��" << D[Nv1][Nv2] << endl;
                    cout << "�Ƿ��ӡ���·������������1����������0��������";
                    cin >> i;
                    if (i == 1)
                    {
                        cout << Nv1;
                        showPath(Nv1, Nv2);
                    }
                }
                else
                {
                    cout << "���ڵ�֮�䲻��ͨ";
                    exit(1);
                }
            }
        }
        else
            cout << "���ڸ�ֵȦ";
    }
    return 0;
}