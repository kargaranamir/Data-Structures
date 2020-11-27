#include<iostream>
#include <algorithm> // for finding minimum

using namespace std;

struct Queue {
  int front, rear, size;
  unsigned capacity;
  int* array;
};

Queue* createQueue(unsigned capacity)
{
  Queue* queue = (Queue*)malloc(sizeof(Queue));
  queue->capacity = capacity;
  queue->front = 0;
  queue->size = 0;
  queue->rear = capacity - 1;
  queue->array = (int*)malloc(queue->capacity * sizeof(int));
  return queue;
}
int isEmpty(Queue* queue)
{
  return (queue->size == 0);
}
void enqueue(Queue* queue, int item)
{
  queue->rear = (queue->rear + 1) % queue->capacity;
  queue->array[queue->rear] = item;
  queue->size = queue->size + 1;
}
int dequeue(Queue* queue)
{
  int item = queue->array[queue->front];
  queue->front = (queue->front + 1) % queue->capacity;
  queue->size = queue->size - 1;
  return item;
}
///Queue finished

struct node
{
  int vertex;
  node* next;
  node* prev;
  int capacity;
  int flow;
};

node* createNode(int v, int capacity)
{
  node* newNode = (node*)malloc(sizeof(node));
  newNode->vertex = v;
  newNode->flow = 0;
  newNode->capacity = capacity;
  newNode->next = NULL;
  newNode->prev = NULL;
  return newNode;
}

struct Graph
{
  int numVertices;
  bool* visited;
  node** adjLists;
};

Graph* createGraph(int vertices)
{
  Graph* graph = (Graph*)malloc(sizeof(Graph));
  graph->numVertices = vertices;

  graph->adjLists = (node**)malloc(vertices * sizeof(node*));

  graph->visited = (bool*)malloc(vertices * sizeof(bool));

  for (int i = 0; i < vertices; i++) {
    graph->adjLists[i] = NULL;
    graph->visited[i] = 0;
  }
  return graph;
}
void addEdge(Graph* graph, int source, int sink, int weight)
{
  node* newNode = createNode(sink, weight);
  newNode->next = graph->adjLists[source];
  if (graph->adjLists[source] != NULL)
    graph->adjLists[source]->prev = newNode->next;
  graph->adjLists[source] = newNode;
}
int Capac(Graph* rGraph, int u, int v, bool mode)
{
  if (mode == true)
  {
    node* temp = rGraph->adjLists[u];
    while (temp != NULL)
    {
      if (temp->vertex == v)
        if (temp->capacity - temp->flow > 0)
          return (temp->capacity - temp->flow);
      temp = temp->next;
    }
  }
  else
  {
    node* temp = rGraph->adjLists[u];
    while (temp != NULL)
    {
      if (temp->vertex == v)
        return temp->flow;
      temp = temp->next;
    }
  }

  return 0;
}
int* BFS(Graph* rGraph, int source, int sink)
{
  int* path;
  path = (int*)malloc(rGraph->numVertices * sizeof(int));
  memset(path, 0, rGraph->numVertices * sizeof(path));
  memset(rGraph->visited, 0, rGraph->numVertices * sizeof(rGraph->visited));

  Queue *queue = createQueue(rGraph->numVertices);
  enqueue(queue, source);
  rGraph->visited[source] = true;
  path[source] = -1;

  while (!isEmpty(queue))
  {
    int u = dequeue(queue);

    for (int v = 0; v < rGraph->numVertices; v++)
    {
      if (rGraph->visited[v] == false)
      {
        if (Capac(rGraph, u, v, true) != 0)
        {
          enqueue(queue, v);
          path[v] = u;
          rGraph->visited[v] = true;
        }
      }
    }
  }

  if (rGraph->visited[sink] == true)
    return path;
  else
    return NULL;
}
void printGraph(Graph* graph)
{
  int v;
  for (v = 0; v < graph->numVertices; v++)
  {
    node* temp = graph->adjLists[v];
    printf("\n Adjacency list of vertex %d\n ", v);
    while (temp)
    {
      printf("%d  {%d(%d)} -> ", temp->vertex, temp->capacity, temp->flow);
      temp = temp->next;
    }
printf("NULL\n");
  }
}
int pathFlow(Graph* rGraph, int* path, int source, int sink)
{
  if (path == NULL)
    return 0;
  int flow = Capac(rGraph, path[sink], sink, true);
  for (int v = path[sink]; v != source; v = path[v])
    flow = min(flow, Capac(rGraph, path[v], v, true));
  return flow;
}   // maximum flow in path 
void printPath(int* path, int source, int sink, int pathFlow,Graph * graph)
{
  int* StoS = (int*)malloc(graph->numVertices * sizeof(int));
  StoS[0] = sink;
  int i = 0;
  for (int v = sink; v != source; v = path[v])
  {
    StoS[i] = v;
    i++;
  }
  cout << "flow: ";
  printf("%d", pathFlow);
  cout << "   path: ";
  cout << source;
  for (int j = i - 1; j != -1; j--)
  {
    cout << "->" << StoS[j];
  }
  cout << endl;
}
bool updateCapac(Graph* rGraph, int*path, int pathflow, int source, int sink)
{
  if (path == NULL)
    return false;

  for (int v = sink; v != source; v = path[v])
  {
    int u = path[v];
    node* temp1 = rGraph->adjLists[u];
    while (temp1 != NULL)
    {
      if (temp1->vertex == v)
      {
        temp1->flow += pathflow;
        break;
      }
      temp1 = temp1->next;
    }
    node* temp2 = rGraph->adjLists[v];
    while (temp2 != NULL)
    {
      if (temp2->vertex == u)
      {
        temp2->capacity += pathflow;
        break;
      }
      temp2 = temp2->next;
    }
    if (temp2 == NULL)
      addEdge(rGraph, v, u, pathflow);
  }
  return true;
}
int fordFulkerson(Graph* rGraph, int source, int sink, bool mode)
{
  int maxFlow = 0;
  int *path;
  memset(rGraph->visited, 0, rGraph->numVertices * sizeof(rGraph->visited));
  path = (int*)malloc(rGraph->numVertices * sizeof(int));

  if (mode == true)
  {
    path = BFS(rGraph, source, sink);

    while (path != NULL)
    {
      int flow = pathFlow(rGraph, path, source, sink);
      maxFlow += flow;
      updateCapac(rGraph, path, flow, source, sink);
      memset(rGraph->visited, 0, rGraph->numVertices * sizeof(rGraph->visited));
      path = BFS(rGraph, source, sink);
    }
    memset(rGraph->visited, 0, sizeof(rGraph->visited));
    return maxFlow;
  }
  else
  {
    path = BFS(rGraph, source, sink);

    while (path != NULL)
    {
      int flow = pathFlow(rGraph, path, source, sink);
      printPath(path, source, sink, flow,rGraph);
      updateCapac(rGraph, path, flow, source, sink);
      memset(rGraph->visited, 0, sizeof(rGraph->visited));
      path = BFS(rGraph, source, sink);
    }
    return 0;
  }
}

int main()
{
  Graph* graph = createGraph(6);
  Graph* graph_cpy = createGraph(6);
  addEdge(graph, 0, 1, 10);
  addEdge(graph, 0, 2, 10);
  addEdge(graph, 1, 2, 2);
  addEdge(graph, 2, 4, 9);
  addEdge(graph, 1, 4, 8);
  addEdge(graph, 1, 3, 4);
  addEdge(graph, 4, 3, 6);
  addEdge(graph, 3, 5, 10);
  addEdge(graph, 4, 5, 10);

  for (int i = 0; i < graph->numVertices; i++)
  {
    node* temp = graph->adjLists[i];
    while (temp != NULL)
    {
      addEdge(graph_cpy, i, temp->vertex, temp->capacity);
      temp = temp->next;
    }
  }

  fordFulkerson(graph, 0, 5, true);

  int i = 0;
  while (i != graph_cpy->numVertices)
  {
    int j = 0;
    node* temp = graph_cpy->adjLists[i];
    while (temp != NULL)
    {
      temp->capacity = Capac(graph, i, temp->vertex, false) - Capac(graph, temp->vertex, i, false);

      temp->flow = 0;
      j++;
      temp = temp->next;
    }
    i++;
  }
  printGraph(graph_cpy);

  fordFulkerson(graph_cpy, 0, 5, false);


  system("pause");
  return 0;
}