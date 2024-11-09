//LSRT_Dijkstra.c
#include <stdio.h>
#define NODES 4
#define INF 9999


void display_overall_RT(int RT[NODES][NODES])
{
	printf("Final Routing Table:\n ");
	for(int i=0;i<NODES;i++)
		printf(" %c",'A'+i);
	printf("\n");
	for(int i=0;i<NODES;i++)
	{
		printf("%c ",'A'+i);
		for(int j=0;j<NODES;j++)
		{
			if(RT[i][j]==9999)
				printf("INF ");
			else printf("%d ",RT[i][j]);
		}
		printf("\n");
	}
}
// Function to find the node with the minimum distance
int mindistance(int dist[], int visited[]) {
    int min = INF, min_index;
    for (int v = 0; v < NODES; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}
void dijkstra(int RT[NODES][NODES], int src)
{
	//use dijkstra's algorithm:
	int distance[NODES];
	int visited[NODES];
	int parent[NODES];

	for(int i=0;i<NODES;i++)
	{
		distance[i]=INF;//set distances to INF initially
		visited[i]=0;//no nodes are visited initially
		parent[i]=-1;//no parent initially
	}
	distance[src]=0;
	for (int count = 0; count < NODES - 1; count++) {
    // Step 1: Pick the minimum distance node that hasn't been visited
    int u = mindistance(distance, visited);
    visited[u] = 1;

    // Step 2: Update distance[] of the adjacent nodes of the picked node
    for (int v = 0; v < NODES; v++) {
        if (!visited[v] && RT[u][v] != INF && distance[u] != INF && distance[u] + RT[u][v] < distance[v]) {
            distance[v] = distance[u] + RT[u][v];
            parent[v] = u;
        }
    }
}
	// Display the results
    printf("From Node %c:\n", src + 'A');
    printf("Destination\tDistance\tPath\n");
    for (int i = 0; i < NODES; i++) {
        if (i != src) {
            printf("%c\t\t%d\t\t", i + 'A', distance[i]);
            int j = i;
            printf("%c", i + 'A');
            while (parent[j] != -1) {
                printf(" <- %c", parent[j] + 'A');
                j = parent[j];
            }
            printf("\n");
        }
    }
    printf("\n");

}

void LinkStateRouting(int RT[NODES][NODES])
{
	// Run Dijkstra's algorithm from each node as the source
    for (int i = 0; i < NODES; i++) {
        dijkstra(RT, i);
    }
}

int main()
{
	// Define the initial topology matrix
    int cost[NODES][NODES] = {
        {0, 1, 4, INF},
        {1, 0, 2, 6},
        {4, 2, 0, 3},
        {INF, 6, 3, 0}
    };

    printf("Initial routing tables:\n");
    display_overall_RT(cost);

    // Perform Link-State Routing
    LinkStateRouting(cost);

	return 0;
}
