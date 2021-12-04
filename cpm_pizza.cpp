#include <iostream>
#include <fstream>
#include <memory>
#include <queue>
#include <stack>
#include <tuple>
#define DBG 1   // 1 for debugging, 0 for normal run
using namespace std;
#include "cpm_pizza.h"

string CPM_Class::ObjectManager::exec(const char* cmd) {
  /*
  Transition from C++ process in the program to the Python process, for plotting
  */
  char buffer[128];
  string result = "";
  shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  while (!feof(pipe.get())) {
      if (fgets(buffer, 128, pipe.get()) != NULL)
          result=result+buffer;
  }
  return result;
}

void CPM_Class::ObjectManager::topologicalSort(int node_number, vector<bool> &visited,  stack<int> &Stack, vector< vector<int> > &adj)
{
    /*
    Linear ordering of vertices such that for every directed edge,
    the node at the tail comes before node at the head in the ordering
    */
    visited[node_number] = true; //Step 1: Make the visited node now true
		CPM_Class::ObjectManager tsu;
    vector<int>::iterator i; //Points at memory address of STL container, often with numbers.
		//Spares time & complexity.
    for (i = adj[node_number].begin(); i != adj[node_number].end(); ++i)
        if (!visited[*i]) //if contents are still false | *i=CONTENTS
            tsu.topologicalSort(*i, visited, Stack, adj); // *i is contents at element 1, and up
    Stack.push(node_number);
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::take_users_order(ofstream &f){
  /*
  Allow users to input the items they want and quantity
  */
	//CPM_Class::ObjectManager users_order;
	vector<string> food_step_name;
	vector<int> food_step_duration;
	int number_of_tasks=0, items = 0;

	CPM_Class::ObjectManager food_step_details;
	f.open("plot_graph.plt");
	while (items<1){
		cout<<"How many items will you be ordering today? ";
		cin>>items;
	}

	food_step_name.push_back("Courier");
	food_step_duration.push_back(6);
	food_step_name.push_back("TurnOff");
	food_step_duration.push_back(5);
	number_of_tasks=2;

  int tweak = 0;
  // input of all the tasks
  CPM_Class::ObjectManager fn;
  food_step_details=fn.user_enters_food_numbers(items, food_step_details, tweak, food_step_name, food_step_duration, number_of_tasks, fn);
  return fn.take_users_order_cont(food_step_details.tweak, food_step_details.number_of_tasks, f, food_step_details.food_step_name, food_step_details.food_step_duration);
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::user_enters_food_numbers(int items, CPM_Class::ObjectManager food_step_details, int tweak, vector<string> food_step_name, vector<int> food_step_duration, int number_of_tasks, CPM_Class::ObjectManager fn)
{
  for(int i = 1 ; i <= items; i++) {
    cout<<"\nDish #"<<i<<
    " - Enter (0) for Pizza, (1) for Calzone, (2) for Grilled Cheese : ";
    std::string foodnumber; std::cin >> foodnumber;

    food_step_details = fn.foodnodes(food_step_details, foodnumber, tweak, food_step_name, food_step_duration, number_of_tasks);
    food_step_name = food_step_details.food_step_name;
    food_step_duration = food_step_details.food_step_duration;
    number_of_tasks = food_step_details.number_of_tasks;
    tweak = food_step_details.tweak;
  }

  food_step_details.food_step_name.push_back("TurnOn");
  food_step_details.food_step_duration.push_back(7);
  return food_step_details;
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::take_users_order_cont
          (int tweak, int number_of_tasks, ofstream &f,
          vector<string> food_step_name, vector<int> food_step_duration)
{
  CPM_Class::ObjectManager users_order;
  number_of_tasks++;
  if (tweak==3) { //tweak of 3 means someone ordered a pizza, which is handled differently
    f<<number_of_tasks-1<<endl;
  }
  else {
    f<<number_of_tasks<<endl;
  }
  f<<"Start 0"<<endl;

  users_order.food_step_name = food_step_name;
  users_order.food_step_duration = food_step_duration;
  users_order.number_of_tasks = number_of_tasks;
  return users_order;
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::foodnodes(
  CPM_Class::ObjectManager food_step_details, string foodnumber, int tweak,
  vector<string> food_step_name, vector<int> food_step_duration,
  int number_of_tasks){
	if (foodnumber == "0"){
		tweak++;
		food_step_name.push_back("RemoveD"); food_step_duration.push_back(2);
		food_step_name.push_back("Cheese"); food_step_duration.push_back(4);
		food_step_name.push_back("Apply_D"); food_step_duration.push_back(1);
		number_of_tasks=number_of_tasks+3;
	}
	else if (foodnumber=="1"){
		food_step_name.push_back("RemoveZ"); food_step_duration.push_back(2);
		food_step_name.push_back("Apply_Z"); food_step_duration.push_back(3);
		number_of_tasks=number_of_tasks+2;
	}
	else if (foodnumber=="2"){
		if (tweak==1){
			food_step_name.pop_back(); food_step_duration.pop_back();
			food_step_name.pop_back(); food_step_duration.pop_back();
			food_step_name.pop_back(); food_step_duration.pop_back();
		}
		tweak=tweak+2;
		food_step_name.push_back("RemoveB"); food_step_duration.push_back(2);
		food_step_name.push_back("Apply_B"); food_step_duration.push_back(8);
		food_step_name.push_back("Cheese"); food_step_duration.push_back(4);
		food_step_name.push_back("Apply_B"); food_step_duration.push_back(8);
		number_of_tasks=number_of_tasks+4;
		if (tweak==3){
			food_step_name.push_back("RemoveD"); food_step_duration.push_back(2);
			food_step_name.push_back("Cheese"); food_step_duration.push_back(4);
			food_step_name.push_back("Apply_D"); food_step_duration.push_back(1);
		}
	}

	food_step_details.food_step_name = food_step_name; food_step_details.food_step_duration = food_step_duration;
	food_step_details.tweak = tweak;
	food_step_details.number_of_tasks = number_of_tasks;
	return food_step_details;
}

CPM_Class::ObjectManager* CPM_Class::ObjectManager::append_start_end_nodes(int number_of_tasks, CPM_Class::ObjectManager *nodes){
	nodes[0].name = "Start";
	nodes[0].duration = 0;
	if (number_of_tasks==10){
		nodes[number_of_tasks].name = "Deliver";
		nodes[number_of_tasks].duration = 0;
		}
	else{
		nodes[number_of_tasks+1].name = "Deliver";
		nodes[number_of_tasks+1].duration = 0;
	}
	return nodes;
}

vector<int> CPM_Class::ObjectManager::easy_case(int i, vector<int> ReadNumbers, CPM_Class::ObjectManager *nodes){
  if ((nodes[2].name=="Apply_D" && nodes[3].name=="Cheese" && nodes[4].name=="RemoveD" && nodes[5].name=="TurnOff")
  || (nodes[2].name=="Apply_Z" && nodes[3].name=="RemoveZ" && nodes[4].name=="TurnOff")
  || (nodes[2].name=="Apply_B" && nodes[3].name=="Cheese" && nodes[4].name=="Apply_B" && nodes[5].name=="RemoveB" && nodes[6].name=="TurnOff")
  || (nodes[2].name=="Apply_D" && nodes[3].name=="Cheese" && nodes[4].name=="RemoveD" && nodes[5].name=="Apply_D" && nodes[6].name=="Cheese" && nodes[7].name=="RemoveD" && nodes[8].name=="TurnOff")
  || (nodes[2].name=="Apply_Z" && nodes[3].name=="RemoveZ" && nodes[4].name=="Apply_Z" && nodes[5].name=="RemoveZ" && nodes[6].name=="TurnOff")
  || (nodes[2].name=="Apply_B" && nodes[3].name=="Cheese" && nodes[4].name=="Apply_B" && nodes[5].name=="RemoveB" && nodes[6].name=="Apply_B" && nodes[7].name=="Cheese" && nodes[8].name=="Apply_B" && nodes[9].name=="RemoveB" && nodes[10].name=="TurnOff")){
    ReadNumbers.push_back(i+1);//0, 1, 2, 00, 11, 22
  }
  return ReadNumbers;
}

vector<int> CPM_Class::ObjectManager::other_cases(int i, vector<int> ReadNumbers, CPM_Class::ObjectManager* nodes, int number_of_tasks){
  CPM_Class::ObjectManager CPM_obj;
  if((nodes[2].name=="Apply_Z" && nodes[3].name=="RemoveZ" && nodes[4].name=="Apply_D" && nodes[5].name=="Cheese" && nodes[6].name=="RemoveD" && nodes[7].name=="TurnOff")
        || (nodes[2].name=="Apply_Z" && nodes[3].name=="RemoveZ" && nodes[4].name=="Apply_B" && nodes[5].name=="Cheese" && nodes[6].name=="Apply_B" && nodes[7].name=="RemoveB" && nodes[8].name=="TurnOff")){
    if (i!=3)
      ReadNumbers.push_back(i+1);//01 & 21
    else
      ReadNumbers.push_back(number_of_tasks-1);
  }
  else if(nodes[2].name=="Apply_B" && nodes[3].name=="Apply_B" && nodes[4].name=="RemoveB" && nodes[5].name=="Apply_D" && nodes[6].name=="Cheese" && nodes[7].name=="RemoveD" && nodes[8].name=="TurnOff"){
    ReadNumbers = CPM_obj.make_02_edges(i, ReadNumbers, number_of_tasks);
  }
  else if(nodes[2].name=="Apply_D" && nodes[3].name=="Cheese" && nodes[4].name=="RemoveD" && nodes[5].name=="Apply_Z" && nodes[6].name=="RemoveZ" && nodes[7].name=="TurnOff"){
    if (i!=4)
      ReadNumbers.push_back(i+1);//10
    else
      ReadNumbers.push_back(number_of_tasks-1);
  }
  else if(nodes[2].name=="Apply_B" && nodes[3].name=="Cheese" && nodes[4].name=="Apply_B" && nodes[5].name=="RemoveB" && nodes[6].name=="Apply_Z" && nodes[7].name=="RemoveZ" && nodes[8].name=="TurnOff"){
    if (i!=5)
      ReadNumbers.push_back(i+1);//12: Type 1, then 2. Conditional looks more like the opposite (21), as do all the conditonals under this "else"
    else
      ReadNumbers.push_back(number_of_tasks-1);
  }
  else if(nodes[2].name=="Apply_D" && nodes[3].name=="RemoveD" && nodes[4].name=="Apply_B" && nodes[5].name=="Cheese" && nodes[6].name=="Apply_B" && nodes[7].name=="RemoveB" && nodes[8].name=="TurnOff"){
    ReadNumbers = CPM_obj.make_20_edges(i, ReadNumbers, number_of_tasks);
  }
  return ReadNumbers;
}

CPM_Class::ObjectManager* CPM_Class::ObjectManager::align_names_to_durations(int number_of_tasks, vector<string> food_step_name, vector<int> food_step_duration, CPM_Class::ObjectManager *nodes){
	bool cheese_passed=false;
	for (int i=1; i<=number_of_tasks; i++){
			if (number_of_tasks==10){
				if ((i==3 || i==4)&&food_step_name.back()=="Cheese"){
					cheese_passed=true;
				}
				else if(cheese_passed==true){
					nodes[i-1].name=food_step_name.back();
					nodes[i-1].duration=food_step_duration.back();
				}
				else{
					nodes[i].name=food_step_name.back();
					nodes[i].duration=food_step_duration.back();
				}
			}
			else{
				nodes[i].name=food_step_name.back();
				nodes[i].duration=food_step_duration.back();
			}
		food_step_name.pop_back();
		food_step_duration.pop_back();
	}
	return nodes;
}

vector<int> CPM_Class::ObjectManager::make_stov_on_edges (int i, int number_of_tasks, CPM_Class::ObjectManager* nodes, vector<int>ReadNumbers){
  for (int j=2; j<=number_of_tasks; j++){
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
  return ReadNumbers;
}

int CPM_Class::ObjectManager::format_nodes_to_plot(ofstream &f, CPM_Class::ObjectManager *nodes, int number_of_tasks){
	for (int i=1; i<=number_of_tasks; i++){
		f<<nodes[i].name<<" "<<nodes[i].duration<<endl;
	}

	if(number_of_tasks!=10) {
		f<<"Deliver 0"<<endl;
	}
	else{
		number_of_tasks--;
	}
	CPM_Class::ObjectManager fantastico1;
	return number_of_tasks;
}

void CPM_Class::ObjectManager::print_tasks(CPM_Class::ObjectManager *nodes, int number_of_tasks){
	if (DBG){ //Courtesy of https://github.com/suman95/Critical-path-management
		cout<<"\n\n\t\tTasks entered :\n";
		for(int i = 0 ; i <= number_of_tasks+1; i++) { //upper limit could be i < number_of_tasks+2, no?
			cout<<"\t\t"<<i<<". "<<nodes[i].name<<" "<<nodes[i].duration<<endl;
		}
	}
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::make_adj_pred_vectors(int number_of_tasks){
	vector< vector<int> > adj;  // adj represents sucessor list
	vector< vector<int> > pred; // pred reperesents predecessor list
	for(int i = 0 ; i <= number_of_tasks; i++) {
		vector<int> temp; adj.push_back(temp); pred.push_back(temp);
	}
	CPM_Class::ObjectManager adjpred; adjpred.adj=adj; adjpred.pred=pred;
	return adjpred;
}

vector<int> CPM_Class::ObjectManager::make_02_edges(int i, vector<int> ReadNumbers, int number_of_tasks){
  if(i==2)
    ReadNumbers.push_back(i+4);
  else if (i==4)
    ReadNumbers.push_back(number_of_tasks-1);
  else if (i==6){
    ReadNumbers.push_back(i-3);
    ReadNumbers.push_back(i+1);
  }
  else
    ReadNumbers.push_back(i+1);//02: type 0, then 2
    //This one causes the critical path algo to mess up somehow, so I made it act like type 2,0 through earlier code
  return ReadNumbers;
}

vector<int> CPM_Class::ObjectManager::make_20_edges(int i, vector<int> ReadNumbers, int number_of_tasks){
  if(i==2)
    ReadNumbers.push_back(i+3);
  else if (i==3)
    ReadNumbers.push_back(number_of_tasks-1);
  else if (i==5){
    ReadNumbers.push_back(i-2);
    ReadNumbers.push_back(i+1);
  }
  else
    ReadNumbers.push_back(i+1);//20: type 2, then 0
  return ReadNumbers;
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::fill_adj_pred(vector<int> ReadNumbers, int number_of_tasks, int i, vector<vector<int>> adj, vector<vector<int>> pred, ofstream &f){
	//Courtesy of https://github.com/suman95/Critical-path-management
	vector<int> temp = ReadNumbers;
	if(temp.size()==0){
		adj[i].push_back(number_of_tasks);
		pred[number_of_tasks].push_back(i);
		f<<i<<" "<<number_of_tasks<<endl;
	}

	if(temp.size()!=0) {
		for(int j=0; j<temp.size(); j++)
			adj[i].push_back(temp[j]);
		for(int j=0;j < temp.size(); j++){ //for the qty of successors in this iteration
			if(temp[j]==temp.size()){ //if this node's number equals the # of successors in this iteration
					vector<int> yikes; //2, 11, 20, 21, 22, maybe overkill
					adj.push_back(yikes);
					pred.push_back(yikes);
			}
			pred[temp[j]].push_back(i);
		}
		for(int j=0;j<temp.size();j++){
			f<<i<<" "<<temp[j]<<endl;
		}
	}
	CPM_Class::ObjectManager fadjpred; fadjpred.adj=adj; fadjpred.pred=pred;
	return fadjpred;
}

void CPM_Class::ObjectManager::debug_matrices(int number_of_tasks, vector<vector<int>> adj, vector< vector<int> > pred){
	if(DBG) {
		cout<<"\nSuccessor matrix :\n";
		for(int i = 0 ; i < number_of_tasks+2; i++) {
			cout<<i<<" : ";
			for(int j = 0 ; j < adj[i].size(); j++) {
				cout<<adj[i][j]<<"->";
			}
			cout<<endl;
		}
		cout<<"Predecessor matrix :\n";
		for(int i = 0 ; i < number_of_tasks+2; i++) {
			cout<<i<<" : ";
			for(int j = 0 ; j < pred[i].size(); j++) {
				cout<<pred[i][j]<<"->";
			}
			cout<<endl;
		}
	}
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::make_edges(int number_of_tasks, CPM_Class::ObjectManager *nodes, vector<vector<int>> adj, vector<vector<int>> pred, ofstream &f){
  CPM_Class::ObjectManager CPM_obj;
  for(int i = 0 ; i <= number_of_tasks; i++) {
    vector<int> ReadNumbers;
    if (i==1){
      ReadNumbers=CPM_obj.make_stov_on_edges (i, number_of_tasks, nodes, ReadNumbers);
    }
    else if (i==0 || i==number_of_tasks || i==number_of_tasks-1 || i==number_of_tasks-2){
      ReadNumbers.push_back(i+1);
    }
    else{
      ReadNumbers=CPM_obj.easy_case(i, ReadNumbers, nodes);
      ReadNumbers=CPM_obj.other_cases(i, ReadNumbers, nodes, number_of_tasks);
    }
    CPM_Class::ObjectManager fadjpred = CPM_obj.fill_adj_pred(ReadNumbers, number_of_tasks, i, adj, pred, f);
    adj=fadjpred.adj; pred=fadjpred.pred;
  }
  f<<"quit"<<endl;
  CPM_Class::ObjectManager bigret;
  bigret.adj=adj; bigret.pred=pred;
  return bigret;
}

CPM_Class::ObjectManager* CPM_Class::ObjectManager::calculate_es_ef(int number_of_tasks, vector<vector<int>> adj, CPM_Class::ObjectManager* nodes, vector<vector<int>> pred){
	//Courtesy of https://github.com/suman95/Critical-path-management
	stack<int> Stack;
	vector<bool> visit(number_of_tasks+2, false);
	CPM_Class::ObjectManager tsu1;
	tsu1.topologicalSort(0,visit, Stack, adj);
	nodes[0].es = 0;
	nodes[0].ef = 0;
	Stack.pop(); //because all but one node have preds & sucs, we have one extra stack element
	while(!Stack.empty()) {
		int top = Stack.top();
		int max_f = -1; //what?
		for(int i = 0; i < pred[top].size(); i++) {
			if(max_f < nodes[pred[top][i]].ef) { //first p[0][i], then p[1][i], then p[4][i], etc
				max_f = nodes[pred[top][i]].ef;
			}
		}
		nodes[top].es = max_f;
		nodes[top].ef = max_f + nodes[top].duration;
		Stack.pop();
	}

	if(DBG) {
		cout<<"Es and Ef : \n";
		for(int i = 0 ; i < number_of_tasks+2; i++) {
			cout<<i<<" "<<nodes[i].name<<" "<<nodes[i].es<<" "<<nodes[i].ef<<endl;
		}
	}
	return nodes;
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::calculate_ls_lf(int number_of_tasks, vector<vector<int>> adj, vector<vector<int>> pred, CPM_Class::ObjectManager *nodes){
	stack<int> Stack2;
	vector<bool> visit2(number_of_tasks+2, false);
	CPM_Class::ObjectManager tsu2;
	tsu2.topologicalSort(number_of_tasks+1, visit2, Stack2, pred);
	//pred starts from the back
	nodes[number_of_tasks+1].ls = nodes[number_of_tasks+1].es;
	nodes[number_of_tasks+1].lf = nodes[number_of_tasks+1].ef;
	Stack2.pop();
	while(!Stack2.empty()) {
		int top = Stack2.top();
		int min_s = 99999;
		for(int i = 0; i < adj[top].size(); i++) {
			if(min_s > nodes[adj[top][i]].ls)
				min_s = nodes[adj[top][i]].ls;
		}
		nodes[top].lf = min_s;
		nodes[top].ls = min_s - nodes[top].duration;
		Stack2.pop();
	}
	if(DBG) {
		cout<<"Ls and Lf : \n";
		for(int i = 0 ; i < number_of_tasks+2; i++) {
			cout<<i<<" "<<nodes[i].name<<" "<<nodes[i].ls<<" "<<nodes[i].lf<<endl;
		}
	}
	CPM_Class::ObjectManager losreturnes;
	losreturnes.nodes=nodes; losreturnes.number_of_tasks=number_of_tasks; losreturnes.adj=adj;
	return losreturnes;
}

void CPM_Class::ObjectManager::results_table(int number_of_tasks, CPM_Class::ObjectManager *nodes){
	cout<<"RESULTS : \n\n";
	cout<<"\t#\tTask\tDur.\tES\tEF\tLS\tLF\tST\n\n";
	for(int i = 0 ; i < number_of_tasks+2 ; i++) {
		nodes[i].st = nodes[i].ls - nodes[i].es;
		cout<<"\t"<<i<<"\t"<<nodes[i].name<<"\t"<<nodes[i].duration<<"\t"<<nodes[i].es<<"\t"<<nodes[i].ef<<"\t"<<nodes[i].ls<<"\t"<<nodes[i].lf<<"\t"<<nodes[i].st<<"\n\n";
	}
}

void CPM_Class::ObjectManager::the_critical_path(int number_of_tasks, CPM_Class::ObjectManager *nodes, vector<vector<int>> adj, ofstream &f){
	//Courtesy of https://github.com/suman95/Critical-path-management
	queue<int> q3;
	vector<int> critical_path(number_of_tasks+2,0);
	q3.push(0);
	critical_path[0] = 1;
	while(!q3.empty()) {
		int top = q3.front();
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
	CPM_Class::ObjectManager execut;
	execut.exec("python3 plot_graph2.py < plot_graph.plt");
	delete[] nodes;
}

int main() {
	ofstream f; //means by which we can open external files
	CPM_Class::ObjectManager CPM_obj; //necessary object
	CPM_Class::ObjectManager users_order = CPM_obj.take_users_order(f);

	CPM_Class::ObjectManager *nodes = new CPM_Class::ObjectManager [users_order.number_of_tasks+2];
	nodes = CPM_obj.append_start_end_nodes(users_order.number_of_tasks, nodes);
	nodes = CPM_obj.align_names_to_durations(users_order.number_of_tasks, users_order.food_step_name, users_order.food_step_duration, nodes);
	int number_of_tasks=CPM_obj.format_nodes_to_plot(f, nodes, users_order.number_of_tasks);
	CPM_obj.print_tasks(nodes, number_of_tasks);

	CPM_Class::ObjectManager adjpred=CPM_obj.make_adj_pred_vectors(number_of_tasks);
	adjpred = CPM_obj.make_edges(number_of_tasks, nodes, adjpred.adj, adjpred.pred, f);

	CPM_obj.debug_matrices(number_of_tasks, adjpred.adj, adjpred.pred);
	nodes = CPM_obj.calculate_es_ef(number_of_tasks, adjpred.adj, nodes, adjpred.pred);
	CPM_Class::ObjectManager lslf = CPM_obj.calculate_ls_lf(number_of_tasks, adjpred.adj, adjpred.pred, nodes);

	CPM_obj.results_table(lslf.number_of_tasks, lslf.nodes);
	CPM_obj.the_critical_path(lslf.number_of_tasks, lslf.nodes, lslf.adj, f);
	return 0;
}
