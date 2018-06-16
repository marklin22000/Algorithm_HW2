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
#include <set>
using namespace std;

//struct edge of graph
typedef struct {
	int u, v;
	double w;
} Edge;

//struct graph composed of edges
typedef struct {
	int V;	// total node
	int E;	// total edge
	Edge *edge;	//array of edges
} graph;

void  SSSP(double *ans[], graph *g, int S_node)
{
	int i, j, u, v;
	double w;
	int tV = g->V;
	int tE = g->E;
	
	/* distance array */
	double *d;
	d= new double [tV];
	
	/* initilaize d[i] */
	for (i = 0; i < tV; i++)
		d[i] = INT_MAX;

	/* source node d[i] */
	d[S_node] = 0;
	
	/* relax edges |V| - 1 times */
	for (i = 1; i <= tV-1; i++) 
	{
		for (j = 0; j < tE; j++) 
		{
			// retrieve edge data
			u = g->edge[j].u;
			v = g->edge[j].v;
			w = g->edge[j].w;
			// relax d[v] if d[u]+w is smaller than current d[v]
			if (d[u]!= INT_MAX && d[v]>d[u]+w)
				d[v] = d[u] + w;
		}
	}
	
	/* detect negative cycle */
	for (i = 0; i < tE; i++) 
	{
		u = g->edge[i].u;
		v = g->edge[i].v;
		w = g->edge[i].w;
		if (d[u]!= INT_MAX && d[v]>d[u]+w) 
		{
			printf("[Error] A negative-weight cycle detected!\n");
			return ;
		}
	}

	/* if can't find the path, replaced distance by -9999 */
	for(i=0;i<tV;i++)
	{
		if( d[i] == INT_MAX )
			d[i] = -9999;
	}

	*ans = d;
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

	//allocate space for edge
	g->edge = (Edge*)malloc(g->E * sizeof(Edge));

	/* using set can automatically sort and detect duplicated node when input data */
	set<int> re_list;
	set<int>::iterator it;

	int *node_ID1;
	node_ID1= new int [g->E];
	int *node_ID2;
	node_ID2= new int [g->E];

	/* input the element in the file*/
	int i=0;
	while (!ifs.eof()){

		ifs >> node_ID1[i];
		re_list.insert(node_ID1[i]);

		ifs >> node_ID2[i];
		re_list.insert(node_ID2[i]);

		ifs >> g->edge[i].w;
		
		i++;
	}
	ifs.close();

	/* there had an weird bug, I always got a "0" at the very beginning of the whole set, */
	/* so I decide to remove it here. */
	re_list.erase(re_list.begin());

	/* get the index of rearranged nodes */
	for(i=0; i<g->E; i++)
	{
		it = re_list.find(node_ID1[i]);
		g->edge[i].u = distance(re_list.begin(),it);

		it = re_list.find(node_ID2[i]);
		g->edge[i].v = distance(re_list.begin(),it);
	}

	/* Function */
	double *ans;
	ans = new double [g->V];
	
	it = re_list.find(S_node);
	/* bad input */
	if(it == re_list.end())
	{
		printf("The node index %d is not found in %s!\n",S_node,argv[1]);
		return 0;
	}
	else
		SSSP(&ans,g,distance(re_list.begin(),it));

	double run_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;

	struct rusage r_usage;
	getrusage(RUSAGE_SELF,&r_usage);
	
	cout << "=====  Answer Report  =====" << endl;
	cout << "Nodes: " << g->V << endl;
	cout << "CPU Run time: " << run_time << endl;
	cout << "Mem Usage:    " << r_usage.ru_maxrss << "kB" << endl;
	for(it=re_list.begin(),i=0;it!=re_list.end();it++,i++)
		printf("%8d    %-6.1lf\n",*it,ans[i]);
	
	ofs << "=====  Answer Report  =====" << endl;
	ofs << "Nodes: " << g->V << endl;
	ofs << "CPU Run time: " << run_time << endl;
	ofs << "Mem Usage:    " << r_usage.ru_maxrss << "kB" << endl;
	for(it=re_list.begin(),i=0;it!=re_list.end();it++,i++)
		ofs << *it << "  " << ans[i] << endl;
	ofs.close();
	
	return 0;
}