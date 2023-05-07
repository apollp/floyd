#include<iostream>
#include<stdio.h>
using namespace std;
#define MaxVertexNum 100    /* 最大顶点数设为100 */
#define INFINITY 65535        /* 设最大值65535*/
typedef int Vertex;         /* 用顶点下标表示顶点,为整型 */
typedef int WeightType;        /* 边的权值设为整型 */
typedef struct ENode* Elementype;
typedef struct ENode* Edge;
typedef struct ENode {
    Vertex V1, V2;      /* 有向边<V1, V2> */
    WeightType Weight;  /* 权重 */
}*Edge;
/*链表结构体定义*/
typedef struct LNode {
    Elementype Data;
    struct LNode *Next;
}*List;
/* 边的定义 */
/* 图结点的定义 */
typedef struct GNode {
    int Nv;  /* 顶点数 */
    int Ne;  /* 边数   */
    WeightType G[MaxVertexNum][MaxVertexNum]; /* 邻接矩阵 */
}*MGraph;
/* 以邻接矩阵存储的图类型 */
MGraph CreateGraph(int VertexNum)
{ 
    /* 初始化一个有VertexNum个顶点但没有边的图 */
    Vertex V, W;
    MGraph Graph;
    /* 建立图 */
    Graph = (MGraph)malloc(sizeof(struct GNode)); 
    if (Graph == NULL)
       return NULL;
    Graph->Nv = VertexNum;
    Graph->Ne = 0;
    /* 初始化邻接矩阵 */
    for (V = 0; V < Graph->Nv; V++)
        for (W = 0; W < Graph->Nv; W++)
            Graph->G[V][W] = INFINITY;

    return Graph;
}

void InsertEdge(MGraph Graph, Edge E)
{
    /* 插入边 <V1, V2> */
    Graph->G[E->V1][E->V2] = E->Weight;
    /* 插入边<V2, V1> */
    Graph->G[E->V2][E->V1] = E->Weight;
} 
//尾插法插入链表
void Insert(List L, Elementype E)  
{
    List tmp, pre;
    for (pre = L; pre->Next != NULL; pre = pre->Next);
    tmp = (List)malloc(sizeof(struct LNode));
    if (tmp == NULL)
        return;
    // 创建结点并插入 
    tmp->Data = E;
    tmp->Next = NULL;
    pre->Next = tmp;
    return;
}
//比较两数据是否相等
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
//检查是否有重复数据输入
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
    cout << "请输入图的顶点数:";
    cin >> Nv;
    //读入顶点个数 
    cout << endl;
    if (Nv == 0)
        return NULL;
//为用户输入提供容错性
    for (int i = 0; i < 3;)
    {
        if (Nv < 0)
        {
            cout << "请输入正确的顶点数：";
            cin >> Nv;
            cout << endl;
            i++;
            if (i == 3)
                return NULL;
        }
        else
            i++;
    }
    Graph = CreateGraph(Nv); /* 初始化有Nv个顶点但没有边的图 */
    cout << "请输入边数：";
    cin >> Graph->Ne;
    cout << endl;
    //读入边数 
    for (int i = 0; i < 3;)//为用户输入提供容错性
    {
        if (Graph->Ne<Nv - 1 || Graph->Ne>Nv * (Nv - 1) / 2)
        {
            cout << "请输入正确的边数：";
            cin >> Graph->Ne;
            cout << endl;
            i++;
            if (i == 3)
                return NULL;
        }
        else
            i++;
    }
    //建立链表
    List L;
    L = (List)malloc(sizeof(struct LNode));
    L->Data = NULL;
    L->Next = NULL;
    //建立边结点 
        for (i = 0; i < Graph->Ne; i++)
        {
            cout << "请按 起点 终点 权重的格式输入边，例如：0 2 3" << endl;   //读入边，格式为"起点 终点 权重"，插入邻接矩阵
            E[i] = (Edge)malloc(sizeof(struct ENode));
            cin >> E[i]->V1 >> E[i]->V2 >> E[i]->Weight;
            cout << endl;
            if (view(L, E[i]) && E[i]->V1 >= 0 && E[i]->V1 < Graph->Nv && E[i]->V2 >= 0 && E[i]->V2 < Graph->Nv)      
                //判断边是否输入过,边是否合法
            {
                Insert(L, E[i]);
                InsertEdge(Graph, E[i]);
            }
            else
            {
                cout << "输入非法边，请重新输入边，格式为 起点 终点 权重，例如：0 2 3" << endl;
                cin >> E[i]->V1 >> E[i]->V2 >> E[i]->Weight;
                cout << endl;
                if (view(L, E[i]) && E[i]->V1 >= 0 && E[i]->V1 < Graph->Nv && E[i]->V2 >= 0 && E[i]->V2 < Graph->Nv)       
                    //判断边是否输入过,边是否合法
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
int D[MaxVertexNum][MaxVertexNum];//存储最短路径大小
int path[MaxVertexNum][MaxVertexNum];//存储路径
bool Floyd(MGraph Graph)
{
    Vertex i, j, k;
    /* 初始化 */
    for (i = 0; i < Graph->Nv; i++)
        for (j = 0; j < Graph->Nv; j++) 
        {
            D[i][j] = Graph->G[i][j];
            path[i][j] = -1;
        }
    //k为中间点 
    for (k = 0; k < Graph->Nv; k++)
        for (i = 0; i < Graph->Nv; i++) //i为起点 
            for (j = 0; j < Graph->Nv; j++)//j为终点
                if (D[i][k] + D[k][j] < D[i][j]) 
                {
                    D[i][j] = D[i][k] + D[k][j];//更新最小路径
                    if (i == j && D[i][j] < 0) /* 若发现负值圈 */
                        return false; /* 不能正确解决，返回错误标记 */
                    path[i][j] = k;//更新最小路径中间顶点
                }
    return true; /* 算法执行完毕，返回正确标记 */
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
    Graph = BuildGraph();//创建图
    if (Graph == NULL)
    {
        cout << "创建图失败";
    }
    else
    {
        if (Floyd(Graph))
        {
            cout << "请输入最短路径的起始和终止顶点，例如：0 2";
            cout << endl;
            cin >> Nv1 >> Nv2;
            if (!chack(Graph, Nv1, Nv2))
            {
                cout << "输入非法顶点请重新输入顶点" << endl;
                cin >> Nv1 >> Nv2;
                if (!chack(Graph, Nv1, Nv2))
                {
                    cout << "输入非法顶点建立图失败";
                    return 0;
                }
            }
            else
            {
                if (D[Nv1][Nv2] < INFINITY)
                {
                    cout << Nv1 << "到" << Nv2 << "的最短路径长度为：" << D[Nv1][Nv2] << endl;
                    cout << "是否打印最短路径，是请输入1，否请输入0或其它：";
                    cin >> i;
                    if (i == 1)
                    {
                        cout << Nv1;
                        showPath(Nv1, Nv2);
                    }
                }
                else
                {
                    cout << "两节点之间不连通";
                    exit(1);
                }
            }
        }
        else
            cout << "存在负值圈";
    }
    return 0;
}