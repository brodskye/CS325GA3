// C / C++ program for Prim's MST for adjacency list representation of graph

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <vector>
#include <algorithm>

// A structure to represent a node in adjacency list
struct AdjListNode
{
	int dest;
	int weight;
	struct AdjListNode* next;
};

// A structure to represent an adjacency liat
struct AdjList
{
	struct AdjListNode *head;  // pointer to head node of list
};

// A structure to represent a graph. A graph is an array of adjacency lists.
// Size of array will be V (number of vertices in graph)
struct Graph
{
	int V;
	struct AdjList* array;
};

// A utility function to create a new adjacency list node
struct AdjListNode* newAdjListNode(int dest, int weight)
{
	struct AdjListNode* newNode =
		(struct AdjListNode*) malloc(sizeof(struct AdjListNode));
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;
	return newNode;
}

// A utility function that creates a graph of V vertices
struct Graph* createGraph(int V)
{
	struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
	graph->V = V;

	// Create an array of adjacency lists.  Size of array will be V
	graph->array = (struct AdjList*) malloc(V * sizeof(struct AdjList));

	// Initialize each adjacency list as empty by making head as NULL
	for (int i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

// Adds an edge to an undirected graph
void addEdge(struct Graph* graph, int src, int dest, int weight)
{
	// Add an edge from src to dest.  A new node is added to the adjacency
	// list of src.  The node is added at the begining
	struct AdjListNode* newNode = newAdjListNode(dest, weight);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;

	// Since graph is undirected, add an edge from dest to src also
	newNode = newAdjListNode(src, weight);
	newNode->next = graph->array[dest].head;
	graph->array[dest].head = newNode;
}

// Structure to represent a min heap node
struct MinHeapNode
{
	int  v;
	int key;
};

// Structure to represent a min heap
struct MinHeap
{
	int size;      // Number of heap nodes present currently
	int capacity;  // Capacity of min heap
	int *pos;     // This is needed for decreaseKey()
	struct MinHeapNode **array;
};

// A utility function to create a new Min Heap Node
struct MinHeapNode* newMinHeapNode(int v, int key)
{
	struct MinHeapNode* minHeapNode =
		(struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
	minHeapNode->v = v;
	minHeapNode->key = key;
	return minHeapNode;
}

// A utilit function to create a Min Heap
struct MinHeap* createMinHeap(int capacity)
{
	struct MinHeap* minHeap =
		(struct MinHeap*) malloc(sizeof(struct MinHeap));
	minHeap->pos = (int *)malloc(capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array =
		(struct MinHeapNode**) malloc(capacity * sizeof(struct MinHeapNode*));
	return minHeap;
}

// A utility function to swap two nodes of min heap. Needed for min heapify
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
	struct MinHeapNode* t = *a;
	*a = *b;
	*b = t;
}

// A standard function to heapify at given idx
// This function also updates position of nodes when they are swapped.
// Position is needed for decreaseKey()
void minHeapify(struct MinHeap* minHeap, int idx)
{
	int smallest, left, right;
	smallest = idx;
	left = 2 * idx + 1;
	right = 2 * idx + 2;

	if (left < minHeap->size &&
		minHeap->array[left]->key < minHeap->array[smallest]->key)
		smallest = left;

	if (right < minHeap->size &&
		minHeap->array[right]->key < minHeap->array[smallest]->key)
		smallest = right;

	if (smallest != idx)
	{
		// The nodes to be swapped in min heap
		MinHeapNode *smallestNode = minHeap->array[smallest];
		MinHeapNode *idxNode = minHeap->array[idx];

		// Swap positions
		minHeap->pos[smallestNode->v] = idx;
		minHeap->pos[idxNode->v] = smallest;

		// Swap nodes
		swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

		minHeapify(minHeap, smallest);
	}
}

// A utility function to check if the given minHeap is ampty or not
int isEmpty(struct MinHeap* minHeap)
{
	return minHeap->size == 0;
}

// Standard function to extract minimum node from heap
struct MinHeapNode* extractMin(struct MinHeap* minHeap)
{
	if (isEmpty(minHeap))
		return NULL;

	// Store the root node
	struct MinHeapNode* root = minHeap->array[0];

	// Replace root node with last node
	struct MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	// Update position of last node
	minHeap->pos[root->v] = minHeap->size - 1;
	minHeap->pos[lastNode->v] = 0;

	// Reduce heap size and heapify root
	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}

// Function to decreasy key value of a given vertex v. This function
// uses pos[] of min heap to get the current index of node in min heap
void decreaseKey(struct MinHeap* minHeap, int v, int key)
{
	// Get the index of v in  heap array
	int i = minHeap->pos[v];

	// Get the node and update its key value
	minHeap->array[i]->key = key;

	// Travel up while the complete tree is not hepified.
	// This is a O(Logn) loop
	while (i && minHeap->array[i]->key < minHeap->array[(i - 1) / 2]->key)
	{
		// Swap this node with its parent
		minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
		minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
		swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

		// move to parent index
		i = (i - 1) / 2;
	}
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
bool isInMinHeap(struct MinHeap *minHeap, int v)
{
	if (minHeap->pos[v] < minHeap->size)
		return true;
	return false;
}

struct Pair {
	int v1;
	int v2;
};

std::vector<Pair> pairs;

struct MST {
	int weight;
	std::vector<Pair> pairs;
};

// A utility function used to print the constructed MST
void printArr(int arr[], int n)
{
	pairs.clear();
	for (int i = 1; i < n; ++i) {
		printf("%d - %d\n", arr[i], i);
		Pair newPair;
		newPair.v1 = i;
		newPair.v2 = arr[i];
		pairs.push_back(newPair);
	}
}

// The main function that constructs Minimum Spanning Tree (MST)
// using Prim's algorithm
void PrimMST(struct Graph* graph)
{
	int V = graph->V;// Get the number of vertices in graph
	int parent[V];   // Array to store constructed MST
	int key[V];      // Key values used to pick minimum weight edge in cut

					 // minHeap represents set E
	struct MinHeap* minHeap = createMinHeap(V);

	// Initialize min heap with all vertices. Key value of
	// all vertices (except 0th vertex) is initially infinite
	for (int v = 1; v < V; ++v)
	{
		parent[v] = -1;
		key[v] = INT_MAX;
		minHeap->array[v] = newMinHeapNode(v, key[v]);
		minHeap->pos[v] = v;
	}

	// Make key value of 0th vertex as 0 so that it
	// is extracted first
	key[0] = 0;
	minHeap->array[0] = newMinHeapNode(0, key[0]);
	minHeap->pos[0] = 0;

	// Initially size of min heap is equal to V
	minHeap->size = V;

	// In the followin loop, min heap contains all nodes
	// not yet added to MST.
	while (!isEmpty(minHeap))
	{
		// Extract the vertex with minimum key value
		struct MinHeapNode* minHeapNode = extractMin(minHeap);
		int u = minHeapNode->v; // Store the extracted vertex number

								// Traverse through all adjacent vertices of u (the extracted
								// vertex) and update their key values
		struct AdjListNode* pCrawl = graph->array[u].head;
		while (pCrawl != NULL)
		{
			int v = pCrawl->dest;

			// If v is not yet included in MST and weight of u-v is
			// less than key value of v, then update key value and
			// parent of v
			if (isInMinHeap(minHeap, v) && pCrawl->weight < key[v])
			{
				key[v] = pCrawl->weight;
				parent[v] = u;
				decreaseKey(minHeap, v, key[v]);
			}
			pCrawl = pCrawl->next;
		}
	}

	// print edges of MST
	printArr(parent, V);
}

// Driver program to test above functions
int main()
{
	int n;
	int k;
	int test;
	FILE* fp;
	fp = fopen("input.txt", "rb");
	fscanf(fp, "%d", &n);
	//printf("num = %d\n", n);
	int graph[n][n];

	for (int i = 0;; i++) {
	for (int j = 0;; j++) {
	if (fscanf(fp, "%d", &test) == -1)
	goto end;
	graph[i][j] = test;
	//printf("grid[%d][%d] = %d\n", i, j, grid[i][j]);
	if ((k = fgetc(fp)) == '\n')
	break;
	}
	}
	end:
	printf("All Done\n");
	printf("%d\n", n);
	/*int n = 5;
	int graph[5][5] = { { 0,1,9,9,1 },{ 1,0,1,9,9 },{ 9,1,0,1,9 },{ 9,9,1,0,1 },{ 1,9,9,1,0 } };
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << graph[i][j] << " ";
			if (j == n - 1)
				std::cout << std::endl;
		}
	}*/

	/////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////

	int V = n;
	struct Graph* ggraph = createGraph(V); //GENERATE FIRST GRAPH WITH EDGES
	for (int i = 0; i < V; i++) {
		for (int j = 0; j < i; j++) {
			if (graph[i][j] != 0) {
				addEdge(ggraph, i, j, graph[i][j]);
			}
		}
	}
	PrimMST(ggraph); //CALL PRIM ALG TO GET MST
	int total = 0; //LOOP THROUGH pairs WHICH IS SET TO THE EDGE STARTING AND EDGING POINTS AS A VECTOR. ADD TO TOTAL THE WEIGHT OF THE EDGES
	for (std::vector<Pair>::const_iterator i = pairs.begin(); i != pairs.end(); ++i) {
		//std::cout << graph[i->v1][i->v2] << ' ';
		total += graph[i->v2][i->v1];
		//std::cout << "\n";
	}

	struct MST mst1;
	mst1.weight = total;
	mst1.pairs = pairs;
	std::cout << "first MST: " << mst1.weight << std::endl; //PRINT FIRST MST

	pairs.clear();

	/*
	END FIRST MST CYCLE
	*/
	std::vector<int> mstWeights;
	mstWeights.push_back(mst1.weight);
	std::vector<MST> mstVec;
	//create MST graph with a different edge removed every time and calculate MST' on MST
	for (std::vector<Pair>::const_iterator iter = mst1.pairs.begin(); iter != mst1.pairs.end(); ++iter) {
		ggraph = createGraph(V); //create a new graph without one of the edges in MST1
		std::cout << "Excluding " << iter->v1 << " and " << iter->v2 << std::endl;
		for (int i = 0; i < V; i++) {
			for (int j = 0; j <= i; j++) {
				if (graph[i][j] != 0 && !((i == iter->v1 && j == iter->v2) || (i == iter->v2 && j == iter->v1))) {
					std::cout << "graph[" << i << "][" << j << "]: " << graph[i][j] << std::endl;
					addEdge(ggraph, i, j, graph[i][j]);
				}
			}
		}
		//PROBLEM IS THE CONNECTIONS DON'T CHANGE FOR EACH NEW ITERATION IN CALCULATING MST2
		PrimMST(ggraph); //CALL PRIM ALG TO GET MST //changes pairs to be for MST'
		total = 0;
		for (std::vector<Pair>::const_iterator k = pairs.begin(); k != pairs.end(); ++k) {
			total += graph[k->v1][k->v2];
			std::cout << "Total " << total << std::endl;
		}
		struct MST mst;
		mst.weight = total;
		mst.pairs = pairs;
		mstVec.push_back(mst);
		std::cout << total << std::endl;
		//pairs.clear();

	}

	//ALL OF THE MSTs should be in an array called mstVEC at this point.

	struct MST smallest = mstVec.at(0);
	int c = 0;
	//smallest.weight = INT_MAX;
	/*for (int l = 0; l < mstVec.size(); l++) {
		std::cout << "mstVec at " << l << ": " << mstVec.at(l).weight << std::endl;
		if (mstVec.at(l).weight < smallest.weight && mstVec.at(l).weight != mst1.weight) {
			smallest = mstVec.at(l);
			c++;
		}
			//smallest = mstVec.at(l);
		if (c == 0)
			smallest = mst1;
	}*/

	for (int l = 0; l < mstVec.size(); l++) {
		std::cout << "mstVec at " << l << ": " << mstVec.at(l).weight << std::endl;
		if (mstVec.at(l).weight < smallest.weight) {
			if (std::find(mstWeights.begin(), mstWeights.end(), mstVec.at(l).weight) == mstWeights.end()) {
				smallest = mstVec.at(l);
				std::cout << "smallest weight: " << smallest.weight << std::endl;
			}
		}
	}

	struct MST mst2 = smallest;
	std::cout << "Second MST: " << mst2.weight << std::endl;
	mstWeights.push_back(mst2.weight);
	/*
	END SECOND MST CYCLE
	*/


	mstVec.clear();
	for (std::vector<Pair>::const_iterator iter = mst2.pairs.begin(); iter != mst2.pairs.end(); ++iter) {
		ggraph = createGraph(V); //create a new graph without one of the edges in MST2
		std::cout << iter->v1 << " and " << iter->v2 << std::endl;
		for (int i = 0; i < V; i++) {
			for (int j = 0; j <= i; j++) {
				if (graph[i][j] != 0 && !((i == iter->v1 && j == iter->v2) || (i == iter->v2 && j == iter->v1))) {
					std::cout << "graph[" << i << "][" << j << "]: " << graph[i][j] << std::endl;
					addEdge(ggraph, i, j, graph[i][j]);
				}
			}
		}

		PrimMST(ggraph); //CALL PRIM ALG TO GET MST //changes pairs to be for MST'
		total = 0;
		for (std::vector<Pair>::const_iterator k = pairs.begin(); k != pairs.end(); ++k) {
			total += graph[k->v1][k->v2];
			std::cout << "Total " << total << std::endl;
		}
		struct MST mst;
		mst.weight = total;
		mst.pairs = pairs;
		pairs.clear();
		mstVec.push_back(mst);
		std::cout << total << std::endl;
	}
	smallest = mstVec.at(0);
	smallest.weight = INT_MAX;

	std::cout << "mst2 weight: " << mst2.weight << std::endl;
	std::cout << "smallest weight: " << smallest.weight << std::endl;
	for (int l = 0; l < mstVec.size(); l++) {
		std::cout << "mstVec at " << l << ": " << mstVec.at(l).weight << std::endl;
		if (mstVec.at(l).weight < smallest.weight) {
			if (std::find(mstWeights.begin(), mstWeights.end(), mstVec.at(l).weight) == mstWeights.end()) {
				smallest = mstVec.at(l);
				std::cout << "smallest weight: " << smallest.weight << std::endl;
			}
		}
	}
	struct MST mst3 = smallest;
	std::cout << "Third MST: " << mst3.weight << std::endl;
	mstWeights.push_back(mst3.weight);

	for (std::vector<int>::const_iterator i = mstWeights.begin(); i != mstWeights.end(); ++i) {
		std::cout << *i << std::endl;
	}
	/*
	Use Prim's to find MST.
	For each edge in MST:
	Remove edge from graph
	Calculate MST' on MST
	Keep track of smallest MST
	Add edge back to graph
	Return the smallest MST.*/

	return 0;
}
