#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct graf {
	int x;
	int y;
	int cost;

	//Variables needed for dijkstra
	int distance;
	int visited;
	struct graf** neighbours;
	struct graf* prev;

}Node;

int getVal(Node* node) {
	if (!node)
		return 10000; //Infinity
	return node->distance;
}

Node* newNode(int x, int y, int cost) {
	Node* temp = (Node*)malloc(sizeof(Node));
	temp->x = x;
	temp->y = y;
	temp->cost = cost;

	temp->distance = 10000; //Infinity
	temp->visited = 0;
	temp->neighbours = (Node**)calloc(4, sizeof(Node));
	temp->prev = NULL;
	return temp;
}

Node** heap_ins(Node** heap, Node* toAdd, int *size) {
	(*size)++;
	heap[*size - 1] = toAdd;
	int curr = *size - 1;
	while (curr != 0 && getVal(heap[curr / 2]) > getVal(heap[curr])) {
		Node* temp = heap[curr];
		heap[curr] = heap[curr / 2];
		heap[curr / 2] = temp;
		curr /= 2;
	}

	return heap;
}

Node** heap_pop(Node** heap, int *size) {
	if (*size  == 0)
		return NULL;
	else if (*size == 1) {
		*size = 0;
		free(heap[0]);
		return NULL;
	}
	free(heap[0]);
	heap[0] = heap[*size - 1];
	heap[*size - 1] = NULL;
	(*size)--;
	int curr = 0;
	while(1){
		if (getVal(heap[2 * curr]) <= getVal(heap[2 * curr + 1]) && getVal(heap[curr]) > getVal(heap[2 * curr])) {
			Node* temp = heap[curr];
			heap[curr] = heap[2 * curr];
			heap[2 * curr] = temp;
			curr *= 2;
		}
		else if (getVal(heap[2 * curr]) > getVal(heap[2 * curr + 1]) && getVal(heap[curr]) > getVal(heap[2 * curr + 1])) {
			Node* temp = heap[curr];
			heap[curr] = heap[2 * curr + 1];
			heap[2 * curr + 1] = temp;
			curr *= 2;
			curr++;
		}
		else
			break;
	}
	return heap;
}

int isWalkable(char x) {
	if (x == 'C' || x == 'P' || x == 'D')
		return 1;
	else if (x == 'H')
		return 2;
	else
		return 0;
}

//Finds node in graph by its coordinates
Node* find(int x, int y, Node** graph, int m) {
	return graph[y*m + x];
}

//////////////////////////////////
//								//
//			   [2]				//
//				|				//
//				|				//
//   [0] ----- node -----[1]	//
//				|				//
//				|				//
//  		   [3]				//
//								//
//////////////////////////////////
Node* getNeighbours(Node* node, Node** graph, int n, int m) {
	if (node->cost) {
		if (node->x > 0 && find(node->x - 1, node->y, graph, m)->cost) 
			node->neighbours[0] = find(node->x - 1, node->y, graph, m);
		if (node->x < m - 1 && find(node->x + 1, node->y, graph, m)->cost) 
			node->neighbours[1] = find(node->x + 1, node->y, graph, m);
		if (node->y > 0 && find(node->x, node->y - 1, graph, m)->cost) 
			node->neighbours[2] = find(node->x, node->y - 1, graph, m);
		if (node->y < n - 1 && find(node->x, node->y + 1, graph, m)->cost) 
			node->neighbours[3] = find(node->x, node->y + 1, graph, m);	
	}
	return node;
}

Node* dijkstra(Node* curr, Node* prev, Node* goal, Node*** heap, int *heap_size) {
	curr->visited = 1;
	for (int i = 0; i < 4; i++) {
		if (curr->neighbours[i] && !curr->neighbours[i]->visited) {
			heap_ins(*heap, curr->neighbours[i], heap_size);
			if (curr->distance + curr->neighbours[i]->cost < curr->neighbours[i]->distance) {
				curr->neighbours[i]->distance = curr->distance + curr->neighbours[i]->cost;
				curr->neighbours[i]->prev = curr;
			}
		}

	}
	return curr;
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty) {
	Node** heap = (Node**)malloc(n * m * sizeof(Node*));
	int heap_size = 0;
	Node** graph = (Node**)malloc(n* m * sizeof(Node*));
	//Stuff popolvar needs to find
	int dragon;
	int princ[5];
	int p_count = 0;
	
	//Graph representation
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			graph[i * m + j] = newNode(j, i, isWalkable(mapa[i][j]));
			if (mapa[i][j] == 'D')
				dragon = i * m + j;
			else if (mapa[i][j] == 'P')
				princ[p_count++] = i * m + j;
		}
	}
	graph[0]->distance = 0;

	//Gets neighbours for all nodes ignoring N tiles
	for (int i = 0; i < n*m; i++)
		graph[i] = getNeighbours(graph[i], graph, n, m);
	

	//Debuging
	graph[0] = dijkstra(graph[0], NULL, NULL, &heap, &heap_size);
	printf("DEBUG %d",graph[5]->distance);


	return NULL;
}

int main()
{
	char **mapa;
	int i, test, dlzka_cesty, cas, *cesta;
	int n = 0, m = 0, t = 0;
	FILE* f;
	while (1) {
		printf("Zadajte cislo testu (0 ukonci program):\n");
		scanf("%d", &test);
		dlzka_cesty = 0;
		n = m = t = 0;
		switch (test) {
		case 0://ukonci program
			return 0;
		case 1://nacitanie mapy zo suboru
			f = fopen("C:\\Users\\user\\source\\repos\\Popolvar\\text.txt", "r");
			if (f)
				fscanf(f, "%d %d %d", &n, &m, &t);
			else
				continue;
			mapa = (char**)malloc(n * sizeof(char*));
			for (i = 0; i < n; i++) {
				mapa[i] = (char*)malloc(m * sizeof(char));
				for (int j = 0; j < m; j++) {
					char policko = fgetc(f);
					if (policko == '\n') policko = fgetc(f);
					mapa[i][j] = policko;
				}
			}
			fclose(f);
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 2://nacitanie preddefinovanej mapy
			n = 10;
			m = 10;
			t = 12;
			mapa = (char**)malloc(n * sizeof(char*));
			for (int i = 0; i < n; i++)
				mapa[i] = (char*)malloc(m * sizeof(char));
			mapa[0] = strdup("CCHCNHCCHN");
			mapa[1] = strdup("NNCCCHHCCC");
			mapa[2] = strdup("DNCCNNHHHC");
			mapa[3] = strdup("CHHHCCCCCC");
			mapa[4] = strdup("CCCCCNHHHH");
			mapa[5] = strdup("PCHCCCNNNN");
			mapa[6] = strdup("NNNNNHCCCC");
			mapa[7] = strdup("CCCCCPCCCC");
			mapa[8] = strdup("CCCNNHHHHH");
			mapa[9] = strdup("HHHPCCCCCC");
			cesta = zachran_princezne(mapa, n, m, t, &dlzka_cesty);
			break;
		case 3: //pridajte vlastne testovacie vzorky
		default:
			continue;
		}
		cas = 0;
		for (i = 0; i < dlzka_cesty; i++) {
			printf("%d %d\n", cesta[i * 2], cesta[i * 2 + 1]);
			if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'H')
				cas += 2;
			else
				cas += 1;
			if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'D' && cas > t)
				printf("Nestihol si zabit draka!\n");
			if (mapa[cesta[i * 2 + 1]][cesta[i * 2]] == 'N')
				printf("Prechod cez nepriechodnu prekazku!\n");
			if (i > 0 && abs(cesta[i * 2 + 1] - cesta[(i - 1) * 2 + 1]) + abs(cesta[i * 2] - cesta[(i - 1) * 2]) > 1)
				printf("Neplatny posun Popolvara!\n");
		}
		printf("%d\n", cas);
		if(cesta)
			free(cesta);
		for (i = 0; i < n; i++) {
			free(mapa[i]);
		}
		free(mapa);
	}
	return 0;
}