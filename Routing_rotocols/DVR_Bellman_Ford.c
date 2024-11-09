//LSRT_Bellman_Ford.c
#include <stdio.h>

#define INF 9999  // Representation for no direct link (infinite distance)
#define NODES 4  // Number of nodes

// Function to print distance vector tables
void printRoutingTables(int cost[NODES][NODES]) {
    for (int i = 0; i < NODES; i++) {
        printf("Routing table for node %c:\n", 'A' + i);
        for (int j = 0; j < NODES; j++) {
            if (cost[i][j] == INF)
                printf("%c -> %c : INF\n", 'A' + i, 'A' + j);
            else
                printf("%c -> %c : %d\n", 'A' + i, 'A' + j, cost[i][j]);
        }
        printf("\n");
    }
}

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
			printf("%d ",RT[i][j]);
		}
		printf("\n");
	}
}

// Bellman-Ford algorithm for Distance Vector Routing
void distanceVectorRouting(int cost[NODES][NODES]) {
    int distance[NODES][NODES];
    int update;  // flag to check if distance vectors are updated

    // Initialize the distance vectors with initial costs
    for (int i = 0; i < NODES; i++)
        for (int j = 0; j < NODES; j++)
            distance[i][j] = cost[i][j];

    do {
        update = 0;  // Reset update flag

        // Perform updates based on neighbors' distances
        for (int i = 0; i < NODES; i++) {
            for (int j = 0; j < NODES; j++) {
                if (i != j) {
                    for (int k = 0; k < NODES; k++) {
                        if (distance[i][k] + distance[k][j] < distance[i][j]) {
                            distance[i][j] = distance[i][k] + distance[k][j];
                            update = 1;  // Set flag if an update occurs
                        }
                    }
                }
            }
        }

        // Print updated routing tables after each round
        printf("Updated routing tables:\n");
        printRoutingTables(distance);
        display_overall_RT(distance);

    } while (update);  // Repeat until no updates occur (convergence)
}


int main() {
    // Define the initial topology matrix
    int cost[NODES][NODES] = {
        {0, 1, 4, INF},
        {1, 0, 2, 6},
        {4, 2, 0, 3},
        {INF, 6, 3, 0}
    };

    printf("Initial routing tables:\n");
    printRoutingTables(cost);

    // Perform Distance Vector Routing
    distanceVectorRouting(cost);

    return 0;
}
