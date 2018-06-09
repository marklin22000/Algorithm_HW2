#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <unistd.h>
#include <sys/resource.h>
#include <math.h>
#include <stdio.h>
#include <cstdlib>
using namespace std;

long S_node = -1;
long node_num=-1;
long edge_num=-1;

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
	
	S_node = atoi(argv[2]);

	/* testing */
	printf("%d\n",S_node);

	/* File check */
	ofstream ofs(argv[3]);
	if (!ofs.is_open()){
		cout << "Output file: " << argv[3] << " cannot be opened." << endl;
		return -1;
	}
	
	/* input node number & edge number*/
	char* temp;
	ifs >> temp;
	cout >>temp>>endl;

	ifs >> edge_num;
	long *node_ID1;
	node_ID1= new long [edge_num];
	long *node_ID2;
	node_ID2= new long [edge_num];
	long *weight;
	weight= new long [edge_num];

	int i=0;
	for(i=0; i<edge_num; i++)
	{
		weight[i] = -9999;
	}

	i=0;
	printf("%d %d\n",node_num,edge_num);
	
	/* input the element in the file*/
	while (!ifs.eof()){
		long tmp_num;

		ifs >> tmp_num;
		node_ID1[i] = tmp_num;

		ifs >> tmp_num;
		node_ID2[i] = tmp_num;

		ifs >> tmp_num;
		weight[i] = tmp_num;

		i++;
	}
	ifs.close();
	printf("GGGGG\n");

	printf("%d %d\n",node_num,edge_num);
	for(i=0; i<edge_num; i++)
	{
		printf("%d %d %d\n",node_ID1[i],node_ID2[i],weight[i]);
	}
	
	/* Function */

	//sort(num,a);

	double run_time = (double)(clock() - start_time) / CLOCKS_PER_SEC;

	struct rusage r_usage;
	getrusage(RUSAGE_SELF,&r_usage);
	printf("Memory usage: %ld k-bytes\n",r_usage.ru_maxrss);
	
	cout << "=====  Answer Report  =====" << endl;
	cout << "CPU Run time: " << run_time << endl;
	cout << "Mem Usage:    " << r_usage.ru_maxrss << endl;
	
	ofs << "=====  Answer Report  =====" << endl;
	ofs << "CPU Run time: " << run_time << endl;
	ofs << "Mem Usage:    " << r_usage.ru_maxrss << endl;
	ofs << "ans: "<<endl;
	/*for (int i = 0; i < num; i++)		
		ofs << a[i] <<" ";
	*/	
	ofs.close();
	
	return 0;
}