#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <sys/resource.h>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
using namespace std;

//long node_num=-1;
//long edge_num=-1;
long *ans_cost;

//struct edge of graph
typedef struct {
	int u, v, w;
} Edge;

//struct graph composed of edges
typedef struct {
	int V;	// total node
	int E;	// total edge
	Edge *edge;	//array of edges
} graph;

void displayArray(int arr[], int size) {
	int i;
	for (i = 0; i < size; i ++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void SSSP(graph *g, int S_node)
{
	int i, j, u, v, w;
	int tV = g->V;
	int tE = g->E;
	
	//distance array
	//int d[tV];
	int *d;
	d= new long [tV];
	//predecessor array
	//int p[tV];
	int *p;
	p= new long [tV];

	//step 1: initilaize graph: d[i] and p[i]
	for (i = 0; i < tV; i++) {
		d[i] = INT_MAX;
		p[i] = 0;
	}
	//special case: source
	d[S_node] = 0;
	
	//step 2: relax edges |V| - 1 times
	for (i = 1; i <= tV-1; i++) {
		for (j = 0; j < tE; j++) {
			// retrieve edge data
			u = g->edge[j].u;
			v = g->edge[j].v;
			w = g->edge[j].w;
			// relax d[v] and p[v] if d[u]+w is smaller than current d[v]
			if (d[u]!=INT_MAX && d[v]>d[u]+w) {
				d[v] = d[u] + w;
				p[v] = u;
			}
		}
	}
	
	//step 3: detect negative cycle
	for (i = 0; i < tE; i++) {
		u = g->edge[i].u;
		v = g->edge[i].v;
		w = g->edge[i].w;
		if (d[u]!=INT_MAX && d[v]>d[u]+w) {
			printf("[Error] A negative-weight cycle detected!\n");
			return -1;
		}
	}
	
	//No negative weight cycle found!
	//print the distance and predecessor array
	printf("final distances: ");
	displayArray(d, tV);
	printf("final parents: ");
	displayArray(p, tV);
}

int main(int argc, char* argv[]){

	/* meansure time */
	const clock_t start_time = clock();
	/* Input check */
	if (argc < 4){
		cout << "Usage: " << argv[0] << " <input_file> <source node> <output_file>" << endl;
		return -1;
	}
	
	/* File check */
	ifstream ifs(argv[1]);
	if (!ifs.is_open()){
		cout << "Input file: " << argv[1] << " cannot be opened." << endl;
		return -1;
	}
	
	/* source node */
	int S_node = atoi(argv[2]);

	/* File check */
	ofstream ofs(argv[3]);
	if (!ofs.is_open()){
		cout << "Output file: " << argv[3] << " cannot be opened." << endl;
		return -1;
	}
	//create graph
	graph *g = (graph*)malloc(sizeof(graph));

	/* input node number & edge number*/
	string dummy;
	ifs >> dummy;
	ifs >> g->V;	//node number
	ifs >> dummy;
	ifs >> g->E;	//edge number

	g->edge = (Edge*)malloc(g->E * sizeof(Edge));
	/*
	long *node_ID1;
	node_ID1= new long [edge_num];
	long *node_ID2;
	node_ID2= new long [edge_num];
	double *weight;
	weight= new double [edge_num];
	*/
	int i=0;
	/* input the element in the file*/
	while (!ifs.eof()){
		/*
		ifs >> node_ID1[i];
		ifs >> node_ID2[i];
		ifs >> weight[i];
		*/
		ifs >> g->edge[i].u;
		ifs >> g->edge[i].v;
		ifs >> g->edge[i].w;
		i++;
	}
	ifs.close();
	
	/* initial all ans to -9999 */
	ans_cost = new long [node_num];
	for(i=0; i<node_num; i++)
	{
		ans_cost[i] = -9999;
	}
	/* Function */
	SSSP(g,S_node);

	double run_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;

	struct rusage r_usage;
	getrusage(RUSAGE_SELF,&r_usage);
	//printf("Memory usage: %ld k-bytes\n",r_usage.ru_maxrss);
	
	cout << "=====  Answer Report  =====" << endl;
	ofs << "Nodes: " << node_num << endl;
	cout << "CPU Run time: " << run_time << endl;
	cout << "Mem Usage:    " << r_usage.ru_maxrss << "kB" << endl;
	
	ofs << "=====  Answer Report  =====" << endl;
	ofs << "Nodes: " << node_num << endl;
	ofs << "CPU Run time: " << run_time << endl;
	ofs << "Mem Usage:    " << r_usage.ru_maxrss << "kB" << endl;
	//ofs << "ans: "<<endl;
	/*for (int i = 0; i < num; i++)		
		ofs << a[i] <<" ";
	*/	
	ofs.close();
	
	return 0;
}