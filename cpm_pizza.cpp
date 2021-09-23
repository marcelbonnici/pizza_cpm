#include <cstdio>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <stack>



#define DBG 0   // set DBG 1 for debugging code and 0 for normal run
#define PLOT_GRAPH 1 // set 1 for plotting graph if system meets necessary requirements

using namespace std;

struct activity {
	string name;
	int duration;
	int es, ef, ls, lf, st;  // earliest start time, earliest finish time, latest start time, latest finish time, slack time
};

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}


// returns vector of n numbers for input
std::vector<int> ReadNumbers()
{
    std::vector<int> numbers ;

    do
    {
        int input ;
        if ( std::cin >> input ) //if user input a successor that int (aka a valid input)
            numbers.push_back(input) ; //put at back of vector
    }while ( std::cin && std::cin.peek() != '\n' ); //repeat while input's valid and next character we wanna push isn't 'Enter' key

    return numbers ;
}

// utility for topological sorting of activity graph
void topologicalSortUtil(int v, vector<bool> &visited,  stack<int> &Stack, vector< vector<int> > &adj)
{
    visited[v] = true;

    vector<int>::iterator i; //Points at memory address of STL container, often with numbers.
		//Spares time & complexity.
    for (i = adj[v].begin(); i != adj[v].end(); ++i) //!= instead of <? Why? Because adj #s don't ascend, necessarily? i++ is same as ++i in this code's high level case
        if (!visited[*i]) //if contents are still false | *i=CONTENTS!
            topologicalSortUtil(*i, visited, Stack, adj); // *i is contents at element 1, and up

    Stack.push(v);
}


int main() {

	vector<string> name1;
	vector<int> duration1;

	int i=0, n_tasks=0, top=0, j=0, items = 0;
	ofstream f; //means by which we can open external files
	f.open("plot_graph.plt");

	cout<<"############## Critical Path management ################\n\n";
	cout<<"How many items will you be ordering today? ";
	cin>>items;
	//Ocin>>n_tasks; // n_tasks is the number of tasks

	//thatsucks


	//thatsucks

	//nodes[n_tasks+1].name = "Finish";
	//nodes[n_tasks+1].duration = 0;
	name1.push_back("Courier");
	duration1.push_back(6);
	n_tasks++;

	name1.push_back("Stov_Of");
	duration1.push_back(5);
	n_tasks++;

	// input of all the tasks
	for(i = 1 ; i <= items; i++) {
		cout<<"\n\nDish #"<<i<<" - Enter (0) for Pizza, (1) for Cookie, or (2) for Calzone : ";
		std::string foodnumber;
		std::cin >> foodnumber;

		if (foodnumber == "0"){
			name1.push_back("Cheese");
			duration1.push_back(4);
			name1.push_back("Apply_D");
			duration1.push_back(1);
			n_tasks=n_tasks+2;
		}
		else if (foodnumber=="1"){
			name1.push_back("Apply_C");
			duration1.push_back(2);
			n_tasks++;
		}
		else if (foodnumber=="2"){
			name1.push_back("Apply_Z");
			duration1.push_back(3);
			n_tasks++;
		}
	}

	name1.push_back("Stov_On");
	duration1.push_back(7);
	n_tasks++;

	if(PLOT_GRAPH) {
		f<<n_tasks<<endl;
		f<<"Start 0"<<endl;
	}

	struct activity nodes[n_tasks+3]; // number of activities here 0th activity is the start
									  // and the (n+1)th activity refers to the finish. Both have duration of 0.

	nodes[0].name = "Start";
	nodes[0].duration = 0;
	nodes[n_tasks+1].name = "Delivered";
	nodes[n_tasks+1].duration = 0;

	for (int i=1; i<=n_tasks; i++){
		nodes[i].name=name1.back();
		name1.pop_back();
		nodes[i].duration=duration1.back();
		duration1.pop_back();

		if(PLOT_GRAPH) {
			f<<nodes[i].name<<" "<<nodes[i].duration<<endl;
		}
	}

	if(PLOT_GRAPH) {
		f<<"Deliver 0"<<endl;
	}
		/*
		nodes[i].name=foodnumber;
		//cin>>nodes[i].name;

		cout<<"Enter duration for "<<i<<" : ";
		cin>>nodes[i].duration;
		if(PLOT_GRAPH) {
			f<<nodes[i].name<<" "<<nodes[i].duration<<endl;
		}
	}*/

	cout<<"\n\n\t\tTasks entered :\n";
	for(i = 0 ; i <= n_tasks+1; i++) { //upper limit could be i < n_tasks+2, no?
		cout<<"\t\t"<<i<<". "<<nodes[i].name<<" "<<nodes[i].duration<<endl;
	}

	vector< vector<int> > adj;  // adj represents sucessor list
	vector< vector<int> > pred; // pred reperesents predecessor list


	// initialization of both lists with empty vectors
	for(i = 0 ; i <= n_tasks; i++) {
		vector<int> temp;
		adj.push_back(temp);
		pred.push_back(temp);
	}

	// initialization of successor list based on user input
	// NOTE : User need to input all the tasks with no predecessors as the successor of "Start"
	cout<<"\n\nNOTE : User need to input all the tasks with no predecessors as the successor of \"Start\"";
	for(i = 0 ; i <= n_tasks; i++) {
		vector<int> ReadNumbers;
		cout<<"\n\nEnter successors for task "<<nodes[i].name<<" : ";
		if (i==1){
			//all Start's successors have "Apply_...", so get 'em!
			for (int j=1; j<=n_tasks; j++){
				if (nodes[j].name.find("Apply_") != string::npos){
					ReadNumbers.push_back(j);
				}
			}
		}
		else if (i==0 || nodes[i].name == "Apply_D" || nodes[i].name == "Stov_Of" || nodes[i].name == "Courier" || (nodes[i].name=="Cheese" && nodes[i+1].name=="Apply_D")){//PIZZA
			ReadNumbers.push_back(i+1);
		}
		else{
			ReadNumbers.push_back(n_tasks-1);
		}

		vector<int> temp = ReadNumbers;//();
		if(temp.size()==0){
			adj[i].push_back(n_tasks);
			pred[n_tasks].push_back(i);
			if(PLOT_GRAPH) {
				f<<i<<" "<<n_tasks<<endl;
			}
		}

		if(temp.size()!=0) {
			for(int j=0; j<temp.size(); j++)
				adj[i].push_back(temp[j]);
			for(int j=0;j < temp.size(); j++)
				pred[temp[j]].push_back(i);
			if(PLOT_GRAPH) {
				for(int j=0;j<temp.size();j++){
					f<<i<<" "<<temp[j]<<endl;
				}
			}
		}

	}
	f<<"quit"<<endl;
	if(DBG) {
		//debugging
		cout<<"\nSuccessor matrix :\n";
		for(i = 0 ; i < n_tasks+2; i++) {
			cout<<i<<" : ";
			for(j = 0 ; j < adj[i].size(); j++) {
				cout<<adj[i][j]<<"->";
			}
			cout<<endl;
		}

		cout<<"Predecessor matrix :\n";
		for(i = 0 ; i < n_tasks+2; i++) {
			cout<<i<<" : ";
			for(j = 0 ; j < pred[i].size(); j++) {
				cout<<pred[i][j]<<"->";
			}
			cout<<endl;
		}
	}



	// calculating earliest start and finish times for each task
	// topological sort of task is required here

	stack<int> Stack;
	vector<bool> visit(n_tasks+2, false);
	topologicalSortUtil(0,visit, Stack, adj);

	nodes[0].es = 0;
	nodes[0].ef = 0;
	Stack.pop();

	while(!Stack.empty()) {
		top = Stack.top();
		//cout<<"top "<<top<<endl;
		int max_f = -1;
		for(i = 0; i < pred[top].size(); i++) {
			if(max_f < nodes[pred[top][i]].ef) {
				max_f = nodes[pred[top][i]].ef;
			}
		}
		nodes[top].es = max_f;
		nodes[top].ef = max_f + nodes[top].duration;
		Stack.pop();
	}


	if(DBG) {
		cout<<"Es and Ef : \n";
		for(i = 0 ; i < n_tasks+2; i++) {
			cout<<i<<" "<<nodes[i].name<<" "<<nodes[i].es<<" "<<nodes[i].ef<<endl;
		}
	}

	// calculating latest start and finish time for each task

	stack<int> Stack2;
	vector<bool> visit2(n_tasks+2, false);

	topologicalSortUtil(n_tasks+1, visit2, Stack2, pred);

	nodes[n_tasks+1].ls = nodes[n_tasks+1].es;
	nodes[n_tasks+1].lf = nodes[n_tasks+1].ef;
	Stack2.pop();
	while(!Stack2.empty()) {
		top = Stack2.top();
		//cout<<"top "<<top<<endl;
		int min_s = 99999;
		for(i = 0; i < adj[top].size(); i++) {
			if(min_s > nodes[adj[top][i]].ls) {
				min_s = nodes[adj[top][i]].ls;
			}
		}
		nodes[top].lf = min_s;
		nodes[top].ls = min_s - nodes[top].duration;
		Stack2.pop();
	}

	if(DBG) {
		cout<<"Ls and Lf : \n";
		for(i = 0 ; i < n_tasks+2; i++) {
			cout<<i<<" "<<nodes[i].name<<" "<<nodes[i].ls<<" "<<nodes[i].lf<<endl;
		}
	}


	// display of results

	cout<<"RESULTS : \n\n";
	cout<<"\t#\tTask\tDur.\tEs\tEf\tLs\tLf\tST\n\n";
	for(i = 0 ; i < n_tasks+2 ; i++) {
		nodes[i].st = nodes[i].ls - nodes[i].es;
		cout<<"\t"<<i<<"\t"<<nodes[i].name<<"\t"<<nodes[i].duration<<"\t"<<nodes[i].es<<"\t"<<nodes[i].ef<<"\t"<<nodes[i].ls<<"\t"<<nodes[i].lf<<"\t"<<nodes[i].st<<"\n\n";
	}


	// finding the critical path
	// simple BFS can be done to find critical path
	queue<int> q3;
	//vector<int> visited3(n_tasks+2,0); //redundant
	vector<int> critical_path(n_tasks+2,0);
	q3.push(0);
	critical_path[0] = 1;
	while(!q3.empty()) {
		top = q3.front();
		q3.pop();
		if(nodes[top].es == nodes[top].ls) {
			critical_path[top] = 1;
		}
		for(i = 0 ; i < adj[top].size(); i++) {
			//if(visited3[adj[top][i]] == 0){
				q3.push(adj[top][i]);
			//}
		}
	}

	cout<<"Critical Path : ";
	for(i = 0 ; i < critical_path.size(); i++) {
		if(critical_path[i]==1){
		 cout<<nodes[i].name<<"->";
		 f<<i<<" ";
		}
	}
	cout<<endl;
	f<<endl;
	f.close();
	exec("python3 plot_graph2.py < plot_graph.plt");

	return 0;

}
