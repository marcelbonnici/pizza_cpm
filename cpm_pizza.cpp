#include <iostream>
#include <fstream>
#include <memory>
#include <queue>
#include <stack>
#include <tuple>
#define DBG 1   // 1 for debugging, 0 for normal run

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
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result=result+buffer;
    }
    return result;
}

void topologicalSortUtil(int v, vector<bool> &visited,  stack<int> &Stack, vector< vector<int> > &adj);

struct cpmStart {
    vector<string> name1;
		vector<int> duration1;
		int n_tasks;
		int tweak;
};

typedef struct cpmStart Struct;
Struct cpm_input(ofstream &f);
Struct foodnodes(Struct super, string foodnumber, int tweak, vector<string> name1, vector<int> duration1, int n_tasks);

int main() {
	bool cheese_passed=false;
	int top=0;
	ofstream f; //means by which we can open external files

	Struct superduper;
	superduper = cpm_input(f);
	vector<string> name1 = superduper.name1;
	vector<int> duration1 = superduper.duration1;
	int n_tasks = superduper.n_tasks;

	struct activity nodes[n_tasks+3];
	nodes[0].name = "Start";
	nodes[0].duration = 0;

	if (n_tasks==10){
		nodes[n_tasks].name = "Deliver";
		nodes[n_tasks].duration = 0;
		}
	else{
		nodes[n_tasks+1].name = "Deliver";
		nodes[n_tasks+1].duration = 0;
	}

	for (int i=1; i<=n_tasks; i++){
			if (n_tasks==10){
				if ((i==3 || i==4)&&name1.back()=="Cheese"){
					cheese_passed=true;
				}
				else if(cheese_passed==true){
					nodes[i-1].name=name1.back();
					nodes[i-1].duration=duration1.back();
				}
				else{
					nodes[i].name=name1.back();
					nodes[i].duration=duration1.back();
				}
			}
			else{
				nodes[i].name=name1.back();
				nodes[i].duration=duration1.back();
			}
		name1.pop_back();
		duration1.pop_back();

	}

	for (int i=1; i<=n_tasks; i++){
		f<<nodes[i].name<<" "<<nodes[i].duration<<endl;
	}

	if(n_tasks!=10) {
		f<<"Deliver 0"<<endl;
	}
	else{
		n_tasks--;
	}

	cout<<"\n\n\t\tTasks entered :\n";
	for(int i = 0 ; i <= n_tasks+1; i++) { //upper limit could be i < n_tasks+2, no?
		cout<<"\t\t"<<i<<". "<<nodes[i].name<<" "<<nodes[i].duration<<endl;
	}

	vector< vector<int> > adj;  // adj represents sucessor list
	vector< vector<int> > pred; // pred reperesents predecessor list


	// initialization of both lists with empty vectors
	for(int i = 0 ; i <= n_tasks; i++) {
		vector<int> temp;
		adj.push_back(temp);
		pred.push_back(temp);
	}

	// initialization of successor list based on user input
	// NOTE : User need to input all the tasks with no predecessors as the successor of "Start"
	cout<<"\n\nNOTE : User need to input all the tasks with no predecessors as the successor of \"Start\"";
	for(int i = 0 ; i <= n_tasks; i++) {
		vector<int> ReadNumbers;
		cout<<"\n\nEnter successors for task "<<nodes[i].name<<" : ";
		if (i==1){
			//all Stov_On's successors have "Apply_...", so get 'em!
			for (int j=2; j<=n_tasks; j++){
				if (nodes[j].name.find("Apply_") != string::npos){
					if(ReadNumbers.size()>0){
						if (ReadNumbers.size()==2){
							if (nodes[ReadNumbers[0]].name!=nodes[j].name && nodes[ReadNumbers[1]].name!=nodes[j].name){
								ReadNumbers.push_back(j);
							}
						}
						else{
							if (nodes[ReadNumbers[0]].name!=nodes[j].name){
								ReadNumbers.push_back(j);
							}
						}
					}
					else{
						ReadNumbers.push_back(j);
					}
				}
			}
		}
		else if (i==0 || i==n_tasks || i==n_tasks-1 || i==n_tasks-2){
			ReadNumbers.push_back(i+1);
		}
		else{
			if (nodes[2].name=="Apply_D" && nodes[3].name=="Cheese" && nodes[4].name=="RemoveD" && nodes[5].name=="Stov_Of"){
				ReadNumbers.push_back(i+1);//0
			}
			else if (nodes[2].name=="Apply_Z" && nodes[3].name=="RemoveZ" && nodes[4].name=="Stov_Of"){
				ReadNumbers.push_back(i+1);//1
			}
			else if (nodes[2].name=="Apply_B" && nodes[3].name=="Cheese" && nodes[4].name=="Apply_B" && nodes[5].name=="RemoveB" && nodes[6].name=="Stov_Of"){
				ReadNumbers.push_back(i+1);//2
			}
			else if(nodes[2].name=="Apply_D" && nodes[3].name=="Cheese" && nodes[4].name=="RemoveD" && nodes[5].name=="Apply_D" && nodes[6].name=="Cheese" && nodes[7].name=="RemoveD" && nodes[8].name=="Stov_Of"){
				ReadNumbers.push_back(i+1);//00
			}
			else if(nodes[2].name=="Apply_Z" && nodes[3].name=="RemoveZ" && nodes[4].name=="Apply_D" && nodes[5].name=="Cheese" && nodes[6].name=="RemoveD" && nodes[7].name=="Stov_Of"){
				if (i!=3){
					ReadNumbers.push_back(i+1);//01
				}
				else{
					ReadNumbers.push_back(n_tasks-1);
				}
			}
			else if(nodes[2].name=="Apply_B" && nodes[3].name=="Apply_B" && nodes[4].name=="RemoveB" && nodes[5].name=="Apply_D" && nodes[6].name=="Cheese" && nodes[7].name=="RemoveD" && nodes[8].name=="Stov_Of"){
				if(i==2){
					ReadNumbers.push_back(i+4);
				}
				else if (i==4){
					ReadNumbers.push_back(n_tasks-1);
				}
				else if (i==6){
					ReadNumbers.push_back(i-3);
					ReadNumbers.push_back(i+1);
				}
				else{
					ReadNumbers.push_back(i+1);//02: type 0, then 2
					//This one causes the critical path algo to mess up somehow, so I made it act like type 2,0 through earlier code
				}
			}
			else if(nodes[2].name=="Apply_D" && nodes[3].name=="Cheese" && nodes[4].name=="RemoveD" && nodes[5].name=="Apply_Z" && nodes[6].name=="RemoveZ" && nodes[7].name=="Stov_Of"){
				if (i!=4){
					ReadNumbers.push_back(i+1);//10
				}
				else{
					ReadNumbers.push_back(n_tasks-1);
				}
			}
			else if(nodes[2].name=="Apply_Z" && nodes[3].name=="RemoveZ" && nodes[4].name=="Apply_Z" && nodes[5].name=="RemoveZ" && nodes[6].name=="Stov_Of"){
				ReadNumbers.push_back(i+1);//11
			}
			else if(nodes[2].name=="Apply_B" && nodes[3].name=="Cheese" && nodes[4].name=="Apply_B" && nodes[5].name=="RemoveB" && nodes[6].name=="Apply_Z" && nodes[7].name=="RemoveZ" && nodes[8].name=="Stov_Of"){
				if (i!=5){
					ReadNumbers.push_back(i+1);//12: Type 1, then 2. Conditional looks more like the opposite (21), as do all the conditonals under this "else"
				}
				else{
					ReadNumbers.push_back(n_tasks-1);
				}
			}
			else if(nodes[2].name=="Apply_D" && nodes[3].name=="RemoveD" && nodes[4].name=="Apply_B" && nodes[5].name=="Cheese" && nodes[6].name=="Apply_B" && nodes[7].name=="RemoveB" && nodes[8].name=="Stov_Of"){
				if(i==2){
					ReadNumbers.push_back(i+3);
				}
				else if (i==3){
					ReadNumbers.push_back(n_tasks-1);
				}
				else if (i==5){
					ReadNumbers.push_back(i-2);
					ReadNumbers.push_back(i+1);
				}
				else{
					ReadNumbers.push_back(i+1);//20: type 2, then 0
				}
			}
			else if(nodes[2].name=="Apply_Z" && nodes[3].name=="RemoveZ" && nodes[4].name=="Apply_B" && nodes[5].name=="Cheese" && nodes[6].name=="Apply_B" && nodes[7].name=="RemoveB" && nodes[8].name=="Stov_Of"){
				if (i!=3){
					ReadNumbers.push_back(i+1);//21: Type 2, then 1. Conditional looks more like the opposite (12), as do all the conditonals under this "else"
				}
				else{
					ReadNumbers.push_back(n_tasks-1);
				}
			}
			else if(nodes[2].name=="Apply_B" && nodes[3].name=="Cheese" && nodes[4].name=="Apply_B" && nodes[5].name=="RemoveB" && nodes[6].name=="Apply_B" && nodes[7].name=="Cheese" && nodes[8].name=="Apply_B" && nodes[9].name=="RemoveB" && nodes[10].name=="Stov_Of"){
				ReadNumbers.push_back(i+1);//22: Type 2, then 1. Conditional looks more like the opposite (12), as do all the conditonals under this "else"
			}
		}
		vector<int> temp = ReadNumbers;
		if(temp.size()==0){
			adj[i].push_back(n_tasks);
			pred[n_tasks].push_back(i);
			f<<i<<" "<<n_tasks<<endl;
		}

		if(temp.size()!=0) {
			for(int j=0; j<temp.size(); j++)
				adj[i].push_back(temp[j]);
			for(int j=0;j < temp.size(); j++){
				if(temp[j]==temp.size()){
						vector<int> yikes;
						adj.push_back(yikes);
						pred.push_back(yikes);
				}
				pred[temp[j]].push_back(i);
			}
			for(int j=0;j<temp.size();j++){
				f<<i<<" "<<temp[j]<<endl;
			}
		}
	}
	f<<"quit"<<endl;
	if(DBG) {
		//debugging
		cout<<"\nSuccessor matrix :\n";
		for(int i = 0 ; i < n_tasks+2; i++) {
			cout<<i<<" : ";
			for(int j = 0 ; j < adj[i].size(); j++) {
				cout<<adj[i][j]<<"->";
			}
			cout<<endl;
		}

		cout<<"Predecessor matrix :\n";
		for(int i = 0 ; i < n_tasks+2; i++) {
			cout<<i<<" : ";
			for(int j = 0 ; j < pred[i].size(); j++) {
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
		int max_f = -1;
		for(int i = 0; i < pred[top].size(); i++) {
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
		for(int i = 0 ; i < n_tasks+2; i++) {
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
		for(int i = 0; i < adj[top].size(); i++) {
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
		for(int i = 0 ; i < n_tasks+2; i++) {
			cout<<i<<" "<<nodes[i].name<<" "<<nodes[i].ls<<" "<<nodes[i].lf<<endl;
		}
	}

	cout<<"RESULTS : \n\n";
	cout<<"\t#\tTask\tDur.\tEs\tEf\tLs\tLf\tST\n\n";
	for(int i = 0 ; i < n_tasks+2 ; i++) {
		nodes[i].st = nodes[i].ls - nodes[i].es;
		cout<<"\t"<<i<<"\t"<<nodes[i].name<<"\t"<<nodes[i].duration<<"\t"<<nodes[i].es<<"\t"<<nodes[i].ef<<"\t"<<nodes[i].ls<<"\t"<<nodes[i].lf<<"\t"<<nodes[i].st<<"\n\n";
	}


	// finding the critical path
	// simple BFS can be done to find critical path
	queue<int> q3;
	vector<int> critical_path(n_tasks+2,0);
	q3.push(0);
	critical_path[0] = 1;
	while(!q3.empty()) {
		top = q3.front();
		q3.pop();
		if(nodes[top].es == nodes[top].ls) {
			critical_path[top] = 1;
		}
		for(int i = 0 ; i < adj[top].size(); i++) {
				q3.push(adj[top][i]);
		}
	}

	cout<<"Critical Path : ";
	for(int i = 0 ; i < critical_path.size(); i++) {
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

Struct cpm_input(ofstream &f){
	vector<string> name1;
	vector<int> duration1;
	int n_tasks=0, items = 0, tweak = 0;
	Struct super, fantastico;
	f.open("plot_graph.plt");
	cout<<"############## Critical Path management ################\n\n";
	while (items<1){
		cout<<"How many items will you be ordering today? ";
		cin>>items;
	}

	name1.push_back("Courier");
	duration1.push_back(6);
	name1.push_back("Stov_Of");
	duration1.push_back(5);
	n_tasks=2;

	// input of all the tasks
	for(int i = 1 ; i <= items; i++) {
		cout<<"\n\nDish #"<<i<<" - Enter (0) for Pizza, (1) for Calzone, (2) for Grilled Cheese : ";
		std::string foodnumber;
		std::cin >> foodnumber;

		fantastico = foodnodes(fantastico, foodnumber, tweak, name1, duration1, n_tasks); //need to introduce int tweak
		name1 = fantastico.name1; duration1 = fantastico.duration1;
		n_tasks = fantastico.n_tasks;
		tweak = fantastico.tweak;
	}

	name1.push_back("Stov_On");
	duration1.push_back(7);
	n_tasks++;

	if (tweak==3){
		f<<n_tasks-1<<endl;
	}
	else{
		f<<n_tasks<<endl;
	}
	f<<"Start 0"<<endl;

	super.name1 = name1;
	super.duration1 = duration1;
	super.n_tasks = n_tasks;
	return super;
}

Struct foodnodes(Struct fantastico, string foodnumber, int tweak, vector<string> name1, vector<int> duration1, int n_tasks){
	if (foodnumber == "0"){
		tweak++;
		name1.push_back("RemoveD"); duration1.push_back(2);
		name1.push_back("Cheese"); duration1.push_back(4);
		name1.push_back("Apply_D"); duration1.push_back(1);
		n_tasks=n_tasks+3;
	}
	else if (foodnumber=="1"){
		name1.push_back("RemoveZ"); duration1.push_back(2);
		name1.push_back("Apply_Z"); duration1.push_back(3);
		n_tasks=n_tasks+2;
	}
	else if (foodnumber=="2"){
		if (tweak==1){
			name1.pop_back(); duration1.pop_back();
			name1.pop_back(); duration1.pop_back();
			name1.pop_back(); duration1.pop_back();
		}
		tweak=tweak+2;
		name1.push_back("RemoveB"); duration1.push_back(2);
		name1.push_back("Apply_B"); duration1.push_back(8);
		name1.push_back("Cheese"); duration1.push_back(4);
		name1.push_back("Apply_B"); duration1.push_back(8);
		n_tasks=n_tasks+4;
		if (tweak==3){
			name1.push_back("RemoveD"); duration1.push_back(2);
			name1.push_back("Cheese"); duration1.push_back(4);
			name1.push_back("Apply_D"); duration1.push_back(1);
		}
	}
	fantastico.name1 = name1; fantastico.duration1 = duration1;
	fantastico.tweak = tweak;
	fantastico.n_tasks = n_tasks;
	return fantastico;
}
