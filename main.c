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
	int heaped;
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
	temp->heaped = 0;
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
		heap[0] = NULL;
		return heap;
	}
	heap[0] = heap[*size - 1];
	heap[*size - 1] = NULL;
	(*size)--;
	int curr = 0;
	while(1){
		if (2 * curr + 1 < *size) {
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
		else 
			break;
	}
	return heap;
}

int factorial(int x) {
	int res = x;
	while (x > 1) 
		res *= --x;
	return res;
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

int* dijkstra(Node* curr, Node* end, int n, int m, int *len) {
	Node** heap = (Node**)malloc(n * m * sizeof(Node*));
	int heap_size = 0;
	curr->distance = 1;

	while (1) {
		curr->visited = 1;
		curr->heaped = 0;
		for (int i = 0; i < 4; i++) {
			if (curr->neighbours[i] && curr->neighbours[i]->visited == 0) {
				//Relax
				if (curr->neighbours[i]->cost + curr->distance < curr->neighbours[i]->distance) {
					curr->neighbours[i]->distance = curr->neighbours[i]->cost + curr->distance;
					curr->neighbours[i]->prev = curr;
				}
				if (curr->neighbours[i]->heaped == 0) {
					curr->neighbours[i]->heaped = 1;
					heap = heap_ins(heap, curr->neighbours[i], &heap_size);
				}
			}
		}
		curr = heap[0];
		//showHeap(heap, heap_size);
		heap = heap_pop(heap, &heap_size);
		if (heap == NULL)
			return NULL;
		if (curr->x == end->x && curr->y == end->y )
			break;
		/*
		printf("\nafter--------------------\n");
		showHeap(heap, heap_size);
		printf("==================\n");*/
	}
	free(heap);
	Node* temp = end;
	*len = 0;
	while (temp) {
		temp = temp->prev;
		(*len)++;
	}
	temp = end;
	int* coordinates_array = (int*)calloc(sizeof(int), 2 * (*len));
	int counter = (*len) *2 - 1;
	while (temp) {
		coordinates_array[counter--] = temp->y;
		coordinates_array[counter--] = temp->x;
		temp = temp->prev;
	}

	return coordinates_array;
}

Node** clear(Node** graph, int len) {
	for (int i = 0; i < len; i++) {
		graph[i]->prev = NULL;
		graph[i]->heaped = NULL;
		graph[i]->visited = 0;
		graph[i]->distance = 10000;
	}
	return graph;
}

int* getPerm(int** pLens, int p_count, int from) {
	int min = 10000, index;
	int* perm = (int*)calloc(p_count + 2, sizeof(int));
	perm[0] = 0;
	for (int j = 1; j <= p_count; j++) {
		for (int i = 1; i < p_count + 1; i++) {
			if (pLens[from][i] < min) {
				min = pLens[from][i];
				index = i;
			}
		}
		perm[j] = index;
		for (int i = 1; i < p_count + 1; i++) {
			pLens[i][index] = 10000;
		}
		from = index;
		min = 10000;
	}
	return perm;	
}

int *zachran_princezne(char **mapa, int n, int m, int t, int *dlzka_cesty) {
	Node** graph = (Node**)malloc(n* m * sizeof(Node*));
	//Stuff popolvar needs to find
	int *goals = (int*)calloc(6, sizeof(int));	//Dragon and princesses
	int p_count = 1;	//Ammount of princesses
	
	//Graph representation
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			graph[i * m + j] = newNode(j, i, isWalkable(mapa[i][j]));
			if (mapa[i][j] == 'D')
				goals[0] = i * m + j;
			else if (mapa[i][j] == 'P')
				goals[p_count++] = i * m + j;
		}
	}
	p_count--;

	//Gets neighbours for all nodes ignoring N tiles
	for (int i = 0; i < n*m; i++)
		graph[i] = getNeighbours(graph[i], graph, n, m);
	
	//Finds dragon
	int len;
	int *path;
	path = dijkstra(graph[0], graph[goals[0]], n, m, &len);
	graph = clear(graph, n*m);
	
	//Finds all possible paths between princesses
	//Allocations
	int **pLens = (int**)calloc(1 + p_count, sizeof(int*));
	int ***pPaths = (int***)calloc(1 + p_count, sizeof(int**));
	for (int i = 0; i < 1 + p_count; i++) {
		pPaths[i] = (int**)calloc(1 + p_count, sizeof(int*));
		pLens[i] = (int*)calloc(1 + p_count, sizeof(int));
	}
	
	//Finding the paths
	for (int i = 0; i < p_count + 1; i++) {
		for (int j = 0; j < p_count + 1; j++) {
			if (i == j) {
				pLens[i][j] = 10000;
				continue;
			}
			pPaths[i][j] = dijkstra(graph[goals[i]], graph[goals[j]], n, m, &pLens[i][j]);
			graph = clear(graph, n*m);
		}
	}

	//Find an acceptable path
	int** dummy_pLens = (int**)calloc(1 + p_count, sizeof(int*));
	for (int i = 0; i < 1 + p_count; i++) {
		dummy_pLens[i] = (int*)calloc(1 + p_count, sizeof(int));
		for (int j = 0; j < 1 + p_count; j++) {
			dummy_pLens[i][j] = pLens[i][j];
		}
	}
	int *perm = getPerm(dummy_pLens, p_count, 0);
	
	for (int i = 0; i < 2 * len; i += 2) {
		printf("%d %d\n", path[i], path[i + 1]);
	}
	for (int i = 0; i < p_count; i++) {
		for (int j = 2; j < pLens[perm[i]][perm[i + 1]] * 2; j += 2) {
			printf("%d %d\n", pPaths[perm[i]][perm[i + 1]][j], pPaths[perm[i]][perm[i + 1]][j + 1]);
		}	
	}
	
	//Free pPaths
	
	for (int i = 0; i < 1 + p_count; i++) {
		for (int j = 0; j < 1 + p_count; j++) {
			free(pPaths[i][j]);
		}
		free(pPaths[i]);
	}
	free(pPaths);
	
	//Free pLens
	for (int i = 0; i < 1 + p_count; i++) {
		free(pLens[i]);
	}
	free(pLens);
	//Free the rest
	for (int i = 0; i < n*m; i++)
		free(graph[i]);
	free(graph);
	free(goals);

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
			f = fopen("C:\\Users\\user\\source\\repos\\Popolvar\\text2.txt", "r");
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