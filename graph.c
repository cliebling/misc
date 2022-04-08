#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAX_DEPTH 5
#define NUM_NODES 5

void print_path(int32_t *path)
{
	int32_t i;
	for(i = 0; i < MAX_DEPTH; i++)
	{
		if(path[i] < 0)
		{
			break;
		}
		printf("%d ", path[i]);
	}
	printf("\n");
}

int32_t traverse(int32_t current_node, int32_t end_node, int32_t *path, int32_t adjacency[NUM_NODES][NUM_NODES], int32_t depth)
{
	int32_t i;
	printf("Current node: %d\n", current_node);
	if(depth >= MAX_DEPTH)
	{
		return 0;
	}
	if(current_node == end_node)
	{
		printf("Arrived at destination\n");
		print_path(path);
		return 0;
	}
	for(i = 0; i < NUM_NODES; i++)
	{
		if(adjacency[current_node][i] != 0)
		{
			printf("Trying path to node %d\n", i);
			path[depth] = current_node;
			traverse(i, end_node, path, adjacency, depth + 1);
			memset((void*)&path[i], 0xFF, sizeof(int32_t) * (MAX_DEPTH - i));
		}
	}
	printf("Finished at depth %d\n", depth);
	return 0;
}

int32_t adj[NUM_NODES][NUM_NODES] =
	{{0, 1, 0, 0, 0},
	 {0, 0, 0, 1, 0},
	 {1, 0, 0, 0, 0},
	 {0, 0, 0, 0, 1},
	 {0, 0, 1, 0, 0}};

int main(void)
{	
	int32_t path[MAX_DEPTH];
	traverse(0, 4, path, adj, 0); 
	return 0;
}
