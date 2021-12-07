#include <iostream>
#include <fstream>
#include <memory>
#include <queue>
#include <stack>
#include <tuple>
#define DBG 1   // 1 for debugging, 0 for normal run
using namespace std;
#include "cpm_pizza.h"

void CPM_Class::ObjectManager::exec(const char* cmd) {
  /*
  Transition from C++ process in the program to the Python process, for plotting
  cmd - Input plot file into plot maker via python
        "python3 plot_graph.py < plot_graph.plt"
  */
  char buffer[128];
  string result = "";
  shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
  while (!feof(pipe.get())) {
      if (fgets(buffer, 128, pipe.get()) != NULL)
          result=result+buffer;
  }
}

void CPM_Class::ObjectManager::topologicalSort(int node_number,
          vector<bool> &visited,  stack<int> &Stack, vector< vector<int> > &succ)
{
    /*
    Linear ordering of vertices such that for every directed edge,
    the node at the tail comes before node at the head in the ordering

    node_number - Node that we're visiting, to inspect its predecessors
    visited - vector representing T/F for nodes that have(n't) been visited
    Stack - Keeps track of visited nodes
    succ - 2D vector containing node n's neighbors at n index
    */
    visited[node_number] = true; //Step 1: Make the visited node now true
		CPM_Class::ObjectManager tsu;
    vector<int>::iterator i; //Points at memory address of STL container, often with numbers.
		//Spares time & complexity.
    for (i = succ[node_number].begin(); i != succ[node_number].end(); ++i)
        if (!visited[*i]) //if contents are still false | *i=CONTENTS
            tsu.topologicalSort(*i, visited, Stack, succ); // *i is contents at element 1, and up
    Stack.push(node_number);
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::take_users_order(ofstream &f){
  /*
  Allow users to input the items they want and quantity
  f - Plot file that nodes data is sent to

  Returns:
  users_order.food_step_name - Names of steps to fulfill order (ex: Add_Cheese)
  users_order.food_step_duration - Duration of each task, in an int vector
  users_order.number_of_tasks - Tallies the number of tasks
  */
	vector<string> food_step_name;
	vector<int> food_step_duration;
	int number_of_tasks=0, items = 0;

	CPM_Class::ObjectManager food_step_details;
	f.open("plot_graph.plt");
	while (items<1){
		cout<<"How many items will you be ordering today? ";
		cin>>items;
	}

	food_step_name.push_back("Deliverer");
	food_step_duration.push_back(6);
	food_step_name.push_back("Turn_Off");
	food_step_duration.push_back(5);
	number_of_tasks=2;

  int tweak = 0;
  CPM_Class::ObjectManager fn;
  food_step_details=fn.user_enters_food_numbers(items, food_step_details, tweak, food_step_name, food_step_duration, number_of_tasks, fn);
  return fn.take_users_order_cont(food_step_details.tweak, food_step_details.number_of_tasks, f, food_step_details.food_step_name, food_step_details.food_step_duration);
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::user_enters_food_numbers(
  int items, CPM_Class::ObjectManager food_step_details, int tweak,
  vector<string> food_step_name, vector<int> food_step_duration,
  int number_of_tasks, CPM_Class::ObjectManager fn)
{
  /*
  Loop through asking which item they want, based on established quantity

  items - Quantity of items user says they want to order
  food_step_details -
      To feature number_of_tasks, food_step_name, food_step_duration
  food_step_name - Names of steps to fulfill order (ex: Add_Cheese)
  tweak - A number used to handle cases where an order has 2+ unique items
  food_step_duration - Duration of each task, in an int vector
  number_of_tasks - Tally of steps needed to fulfill order
  fn - Constructor to initialize the function

  Returns:
  food_step_details.food_step_name - Names of steps to fulfill order (updated)
  food_step_details.food_step_duration - Duration of each task, in an int vector (updated)
  food_step_details.number_of_tasks - Tally of steps needed to fulfill order (updated)
  */
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

  food_step_details.food_step_name.push_back("Stove_On");
  food_step_details.food_step_duration.push_back(7);
  return food_step_details;
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::take_users_order_cont
          (int tweak, int number_of_tasks, ofstream &f,
          vector<string> food_step_name, vector<int> food_step_duration)
{
  /*
  Continuation of 'take_users_order' to make function less lengthy

  tweak - A number used to handle cases where an order has 2+ unique items
  number_of_tasks - Tally of steps needed to fulfill order
  f - Plot file that nodes data is sent to
  food_step_duration - Duration of each task, in an int vector

  Returns:
  users_order.food_step_name - Names of steps to fulfill order (updated)
  users_order.food_step_duration - Duration of each task, in an int vector (updated)
  users_order.number_of_tasks - Tally of steps needed to fulfill order (updated)
  */
  number_of_tasks++;
  if (tweak==3) { //tweak of 3 means someone ordered a pizza & grilled cheese, which is handled differently
    f<<number_of_tasks-1<<endl; //number_of_tasks is not 1 less, but it is 1 less in f
  }
  else {
    f<<number_of_tasks<<endl;
  }
  f<<"Start 0"<<endl;

  CPM_Class::ObjectManager users_order;
  users_order.food_step_name = food_step_name;
  users_order.food_step_duration = food_step_duration;
  users_order.number_of_tasks = number_of_tasks;
  return users_order;
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::foodnodes(
  CPM_Class::ObjectManager food_step_details, string foodnumber, int tweak,
  vector<string> food_step_name, vector<int> food_step_duration,
  int number_of_tasks){
    /*
    Add nodes that are exclusive to selected food item

    food_step_details -
        To feature number_of_tasks, food_step_name, food_step_duration
    foodnumber - Item that user ordered (0/Pizza, 1/Calzone, 2/Grilled Cheese)
    tweak - A number used to handle cases where an order has 2+ unique items
    food_step_name - Names of steps to fulfill order
    food_step_duration - Duration of each task, in an int vector
    number_of_tasks - Tally of steps needed to fulfill order
    */
	if (foodnumber == "0"){
		tweak++;
		food_step_name.push_back("Remove_Pizza"); food_step_duration.push_back(2);
		food_step_name.push_back("Add_Cheese"); food_step_duration.push_back(4);
		food_step_name.push_back("Apply_Dough"); food_step_duration.push_back(1);
		number_of_tasks=number_of_tasks+3;
	}
	else if (foodnumber=="1"){
		food_step_name.push_back("Remove_Calzone"); food_step_duration.push_back(2);
		food_step_name.push_back("Apply_Calzone"); food_step_duration.push_back(3);
		number_of_tasks=number_of_tasks+2;
	}
	else if (foodnumber=="2"){
		if (tweak==1){
			food_step_name.pop_back(); food_step_duration.pop_back();
			food_step_name.pop_back(); food_step_duration.pop_back();
			food_step_name.pop_back(); food_step_duration.pop_back();
		}
		tweak=tweak+2;
		food_step_name.push_back("Remove_Sandwich"); food_step_duration.push_back(2);
		food_step_name.push_back("Apply_Bread"); food_step_duration.push_back(8);
		food_step_name.push_back("Add_Cheese"); food_step_duration.push_back(4);
		food_step_name.push_back("Apply_Bread"); food_step_duration.push_back(8);
		number_of_tasks=number_of_tasks+4;
		if (tweak==3){
			food_step_name.push_back("Remove_Pizza"); food_step_duration.push_back(2);
			food_step_name.push_back("Add_Cheese"); food_step_duration.push_back(4);
			food_step_name.push_back("Apply_Dough"); food_step_duration.push_back(1);
		}
	}

	food_step_details.food_step_name = food_step_name; food_step_details.food_step_duration = food_step_duration;
	food_step_details.tweak = tweak;
	food_step_details.number_of_tasks = number_of_tasks;
	return food_step_details;
}

CPM_Class::ObjectManager* CPM_Class::ObjectManager::append_start_end_nodes(
  int number_of_tasks, CPM_Class::ObjectManager *nodes) {
  /*
  Sandwich physical steps between nodes that simply begin and end

  number_of_tasks - # of steps to make order, which is pizza & grilled cheese @ 10
  nodes - Object manager to match names with durations

  Returns:
  nodes - Updated nodes
  */
  nodes[0].name = "Start   ";
	nodes[0].duration = 0;
	if (number_of_tasks==10){
		nodes[number_of_tasks].name = "Completed";
		nodes[number_of_tasks].duration = 0;
		}
	else{
		nodes[number_of_tasks+1].name = "Completed";
		nodes[number_of_tasks+1].duration = 0;
	}
	return nodes;
}

vector<int> CPM_Class::ObjectManager::easy_case(int i, vector<int> ReadNumbers,
  CPM_Class::ObjectManager *nodes){
  /*
  When there's only 1 unique food item, the nodes would form an asynchronous
  plot. No need to rearrange nodes from how they're documented in nodes.

  i - Iterator from other function, representing task number
  ReadNumbers - vector to hold successors to a node
  nodes -  Object manager to match names with durations

  Returns:
  ReadNumbers - vector THAT NOW hold successors to a node
  */
  if ((nodes[2].name=="Apply_Dough" && nodes[3].name=="Add_Cheese" && nodes[4].name=="Remove_Pizza" && nodes[5].name=="Turn_Off")
  || (nodes[2].name=="Apply_Calzone" && nodes[3].name=="Remove_Calzone" && nodes[4].name=="Turn_Off")
  || (nodes[2].name=="Apply_Bread" && nodes[3].name=="Add_Cheese" && nodes[4].name=="Apply_Bread" && nodes[5].name=="Remove_Sandwich" && nodes[6].name=="Turn_Off")
  || (nodes[2].name=="Apply_Dough" && nodes[3].name=="Add_Cheese" && nodes[4].name=="Remove_Pizza" && nodes[5].name=="Apply_Dough" && nodes[6].name=="Add_Cheese" && nodes[7].name=="Remove_Pizza" && nodes[8].name=="Turn_Off")
  || (nodes[2].name=="Apply_Calzone" && nodes[3].name=="Remove_Calzone" && nodes[4].name=="Apply_Calzone" && nodes[5].name=="Remove_Calzone" && nodes[6].name=="Turn_Off")
  || (nodes[2].name=="Apply_Bread" && nodes[3].name=="Add_Cheese" && nodes[4].name=="Apply_Bread" && nodes[5].name=="Remove_Sandwich" && nodes[6].name=="Apply_Bread" && nodes[7].name=="Add_Cheese" && nodes[8].name=="Apply_Bread" && nodes[9].name=="Remove_Sandwich" && nodes[10].name=="Turn_Off")){
    ReadNumbers.push_back(i+1);//0, 1, 2, 00, 11, 22. +1 per non-physical start node
  }
  return ReadNumbers;
}

vector<int> CPM_Class::ObjectManager::other_cases(int i,
  vector<int> ReadNumbers, CPM_Class::ObjectManager* nodes,
  int number_of_tasks) {
  /*
  When there's 2+ unique food items, the nodes form a complex plot, that cannot
  simply dump 'nodes' into 'ReadNumbers'. This handles the conversion.

  i - Iterator from other function, representing task number
  ReadNumbers - vector of task numbers/nodes that has nodes in sequential order
  nodes -  Object manager with nodes' names

  Returns:
  ReadNumbers - Now updated, with newly-handled node, into correct order
  */
  CPM_Class::ObjectManager CPM_obj;
  if((nodes[2].name=="Apply_Calzone" && nodes[3].name=="Remove_Calzone" && nodes[4].name=="Apply_Dough" && nodes[5].name=="Add_Cheese" && nodes[6].name=="Remove_Pizza" && nodes[7].name=="Turn_Off")
        || (nodes[2].name=="Apply_Calzone" && nodes[3].name=="Remove_Calzone" && nodes[4].name=="Apply_Bread" && nodes[5].name=="Add_Cheese" && nodes[6].name=="Apply_Bread" && nodes[7].name=="Remove_Sandwich" && nodes[8].name=="Turn_Off")){
    if (i!=3)
      ReadNumbers.push_back(i+1);//01 & 21
    else
      ReadNumbers.push_back(number_of_tasks-1);
  }
  else if(nodes[2].name=="Apply_Bread" && nodes[3].name=="Apply_Bread" && nodes[4].name=="Remove_Sandwich" && nodes[5].name=="Apply_Dough" && nodes[6].name=="Add_Cheese" && nodes[7].name=="Remove_Pizza" && nodes[8].name=="Turn_Off"){
    ReadNumbers = CPM_obj.make_02_edges(i, ReadNumbers, number_of_tasks);
  }
  else if(nodes[2].name=="Apply_Dough" && nodes[3].name=="Add_Cheese" && nodes[4].name=="Remove_Pizza" && nodes[5].name=="Apply_Calzone" && nodes[6].name=="Remove_Calzone" && nodes[7].name=="Turn_Off"){
    if (i!=4)
      ReadNumbers.push_back(i+1);//10
    else
      ReadNumbers.push_back(number_of_tasks-1);
  }
  else if(nodes[2].name=="Apply_Bread" && nodes[3].name=="Add_Cheese" && nodes[4].name=="Apply_Bread" && nodes[5].name=="Remove_Sandwich" && nodes[6].name=="Apply_Calzone" && nodes[7].name=="Remove_Calzone" && nodes[8].name=="Turn_Off"){
    if (i!=5)
      ReadNumbers.push_back(i+1);//12: Type 1, then 2. Conditional looks more like the opposite (21), as do all the conditonals under this "else"
    else
      ReadNumbers.push_back(number_of_tasks-1);
  }
  else if(nodes[2].name=="Apply_Dough" && nodes[3].name=="Remove_Pizza" && nodes[4].name=="Apply_Bread" && nodes[5].name=="Add_Cheese" && nodes[6].name=="Apply_Bread" && nodes[7].name=="Remove_Sandwich" && nodes[8].name=="Turn_Off"){
    ReadNumbers = CPM_obj.make_20_edges(i, ReadNumbers, number_of_tasks);
  }
  return ReadNumbers;
}

CPM_Class::ObjectManager* CPM_Class::ObjectManager::tasks_to_unique_nodes(
  int number_of_tasks, vector<string> food_step_name,
  vector<int> food_step_duration, CPM_Class::ObjectManager *nodes) {
  /*
  Take tasks generated upon user's order, and put all but 'Add_Cheese' in nodes
  object manager one-by-one. Otherwise, we would dispense cheese twice, when we
  only need it once when bread and dough are ready.

  number_of_tasks - # of steps to make order
  food_step_name - Names of steps to make order (no Start or Completed)
  food_step_duration - Duration of each task, in an int vector
  nodes - Converted food_step names & durations; just Start & Complete atm

  Returns:
  nodes - uniquely named nodes, associated with their durations
  */
	bool cheese_passed=false;
	for (int i=1; i<=number_of_tasks; i++){
			if (number_of_tasks==10){
				if ((i==3 || i==4) && food_step_name.back()=="Add_Cheese"){
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

vector<int> CPM_Class::ObjectManager::make_stov_on_edges (int i,
  int number_of_tasks, CPM_Class::ObjectManager* nodes, vector<int>ReadNumbers){
  /*
    Identify successor(s) of 'Stove_On' node.

    i - Iterator from other function, representing task number
    number_of_tasks - # of steps to make order
    nodes - Has task names
    ReadNumbers - Stores successor(s)

    Returns:
    ReadNumbers - Updated to store successor(s)
  */
  for (int j=2; j<=number_of_tasks; j++){
    if (nodes[j].name.find("Apply_") != string::npos){ //is "Apply_" in the name?
      if(ReadNumbers.size()>0){
        if (ReadNumbers.size()==2){
          if (nodes[ReadNumbers[0]].name!=nodes[j].name &&
              nodes[ReadNumbers[1]].name!=nodes[j].name){
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

int CPM_Class::ObjectManager::send_nodes_to_plot_file(ofstream &f,
  CPM_Class::ObjectManager *nodes, int number_of_tasks){
  /*
  Now that nodes object manager is neatly ordered, output each index of it to
  plot file.

  f - Plot file that nodes data is sent to
  nodes - Has task names & durations
  number_of_tasks - # of steps to make order

  Returns:
  number_of_tasks - Decremented is pizza & grilled cheese was chosen
  */
	for (int i=1; i<=number_of_tasks; i++){
		f<<nodes[i].name<<" "<<nodes[i].duration<<endl;
	}

	if(number_of_tasks!=10) {
		f<<"Completed 0"<<endl;
	}
	else{
		number_of_tasks--;
	}

	return number_of_tasks;
}

void CPM_Class::ObjectManager::print_tasks(CPM_Class::ObjectManager *nodes,
                                          int number_of_tasks){
  /*
  Print node names and durations, line-by-line, for debugging purposes.

  nodes - Has task names & durations
  number_of_tasks - # of steps to make order
  */
  if (DBG){ //Courtesy of https://github.com/suman95/Critical-path-management
		cout<<"\n\n\t\tTasks entered :\n";
		for(int i = 0 ; i <= number_of_tasks+1; i++) { //upper limit could be i < number_of_tasks+2, no?
			cout<<"\t\t"<<i<<". "<<nodes[i].name<<" "<<nodes[i].duration<<endl;
		}
	}
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::make_succ_pred_vectors(
                                                    int number_of_tasks){
  /*
  Make 2D vectors, with [0].size() established to have indexes for neighbors
  ready.

  number_of_tasks - # of steps to make order

  Returns:
  succpred - successor and predecessor vectors
  */
	vector<vector<int>> succ(number_of_tasks+1);  // succ represents sucessor list
	vector<vector<int>> pred(number_of_tasks+1); // pred reperesents predecessor list
	CPM_Class::ObjectManager succpred; succpred.succ=succ; succpred.pred=pred;
	return succpred;
}

vector<int> CPM_Class::ObjectManager::make_02_edges(int i,
  vector<int> ReadNumbers, int number_of_tasks){
  /*
  Runs when someone orders pizza, then grilled cheese. Hard codes dough and
  bread nodes to converge to and diverge from 'Add_Cheese'.

  ReadNumbers - Stores successor(s)
  number_of_tasks - # of steps to make order

  Returns:
  ReadNumbers -  Stores more successor(s)
  */
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
  return ReadNumbers;
}

vector<int> CPM_Class::ObjectManager::make_20_edges(int i,
  vector<int> ReadNumbers, int number_of_tasks){
  /*
  Runs when someone orders grilled cheese, then pizza. Hard codes dough and
  bread nodes to converge to and diverge from 'Add_Cheese'.

  ReadNumbers - Stores successor(s)
  number_of_tasks - # of steps to make order

  Returns:
  ReadNumbers -  Stores more successor(s)
  */
  if(i==2) //apply_dough -> add_cheese
    ReadNumbers.push_back(i+3);
  else if (i==3) //remove_pizza -> Turn_Off
    ReadNumbers.push_back(number_of_tasks-1);
  else if (i==5){ //cheese -> add_bread #2 and remove_pizza
    ReadNumbers.push_back(i-2);
    ReadNumbers.push_back(i+1);
  }
  else
    ReadNumbers.push_back(i+1);//20: type 2, then 0
  return ReadNumbers;
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::fill_succ_pred(
  vector<int> ReadNumbers, int number_of_tasks, int i, vector<vector<int>> succ,
  vector<vector<int>> pred, ofstream &f){
	//Courtesy of https://github.com/suman95/Critical-path-management
  /*
  Fill successor and predecessor vectors with appropriate nodes/task numbers

  ReadNumbers - Only used to see how many successors there are
  number_of_tasks - # of steps to make order
  i - Iterator from other function, representing task number
  succ - successors vector
  pred - predecessors vector
  f - Plot file that nodes data is sent to

  Returns:
  fsuccpred - The successor and predecessor vectors, but filled completely
  */
	if(ReadNumbers.size()==0){
		succ[i].push_back(number_of_tasks);
		pred[number_of_tasks].push_back(i);
		f<<i<<" "<<number_of_tasks<<endl;
	}

	if(ReadNumbers.size()!=0) {
		for(int j=0; j<ReadNumbers.size(); j++)
			succ[i].push_back(ReadNumbers[j]);
		for(int j=0;j < ReadNumbers.size(); j++){ //for the qty of successors in this iteration
			if(ReadNumbers[j]==ReadNumbers.size()){ //if this node's number equals the # of successors in this iteration
					vector<int> neighbors;
					succ.push_back(neighbors);
					pred.push_back(neighbors);
			}
			pred[ReadNumbers[j]].push_back(i);
		}
		for(int j=0;j<ReadNumbers.size();j++){
			f<<i<<" "<<ReadNumbers[j]<<endl;
		}
	}
	CPM_Class::ObjectManager fsuccpred; fsuccpred.succ=succ; fsuccpred.pred=pred;
	return fsuccpred;
}

void CPM_Class::ObjectManager::debug_matrices(int number_of_tasks,
  vector<vector<int>> succ, vector< vector<int> > pred){
    /*
    Prints predecessor and successors of each node.

    number_of_tasks - # of steps to make order
    succ - successors vector
    pred - predessors vector
    */
	if(DBG) {
		cout<<"\nSuccessor matrix :\n";
		for(int i = 0 ; i < number_of_tasks+1; i++) {
			cout<<i<<" : ";
			for(int j = 0 ; j < succ[i].size(); j++) {
				cout<<succ[i][j]<<", ";
			}
			cout<<endl;
		}
		cout<<"Predecessor matrix :\n";
		for(int i = 1 ; i < number_of_tasks+2; i++) {
			cout<<i<<" : ";
			for(int j = 0 ; j < pred[i].size(); j++) {
				cout<<pred[i][j]<<", ";
			}
			cout<<endl;
		}
	}
}

CPM_Class::ObjectManager CPM_Class::ObjectManager::make_edges(
  int number_of_tasks, CPM_Class::ObjectManager *nodes, vector<vector<int>> succ,
  vector<vector<int>> pred, ofstream &f) {
    /*
    Stores a node's successors in 'ReadNumbers', then transfers that info to
    fill 2D vectors holding succ[essor] and pred[ecessor] nodes.

    number_of_tasks - # of steps to make order
    nodes - Has task names & durations
    succ - successors vector
    pred - predecessors vector
    f - Plot file that nodes data is sent to

    Returns:
    succpred - The successor and predecessor vectors, filled completely
    */
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
    CPM_Class::ObjectManager fsuccpred = CPM_obj.fill_succ_pred(ReadNumbers, number_of_tasks, i, succ, pred, f);
    succ=fsuccpred.succ; pred=fsuccpred.pred;
  }

  f<<"quit"<<endl;
  CPM_Class::ObjectManager succpred;
  succpred.succ=succ; succpred.pred=pred;
  return succpred;
}

CPM_Class::ObjectManager* CPM_Class::ObjectManager::calculate_es_ef(
  int number_of_tasks, vector<vector<int>> succ,
  CPM_Class::ObjectManager* nodes, vector<vector<int>> pred){
	//Courtesy of https://github.com/suman95/Critical-path-management
  /*
  Calculates early start and finish times, which are also printed in DBG is ON

  number_of_tasks - # of steps to make order
  succ - Successors vector
  nodes - Has es & ef
  pred - Predecessors vector

  Returns:
  nodes.es - Contains all early start times, w.r.t. the other variables
  nodes.ef - Contains all early finish times, w.r.t. the other variables
  */
	stack<int> Stack;
	vector<bool> visit(number_of_tasks+2, false);
	CPM_Class::ObjectManager tsu;
	tsu.topologicalSort(0, visit, Stack, succ); //Topologically sorted in stack
	nodes[0].es = 0;
	nodes[0].ef = 0;
	Stack.pop(); //because all but one node have preds & sucs, we have one extra stack element
	while(!Stack.empty()) {
		int top = Stack.top();
		int max_f = INT8_MIN; //what?
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

CPM_Class::ObjectManager CPM_Class::ObjectManager::calculate_ls_lf(
  int number_of_tasks, vector<vector<int>> succ, vector<vector<int>> pred,
  CPM_Class::ObjectManager *nodes){
    /*
    Calculates late start and finish times, which are also printed in DBG is ON

    nodes - Has ls & lf
    pred - Predecessors vector

    Returns:
    nodes.ls - Contains all late start times, w.r.t. the other variables
    nodes.lf - Contains all late finish times, w.r.t. the other variables
    */
	stack<int> Stack2;
	vector<bool> visit2(number_of_tasks+2, false);
	CPM_Class::ObjectManager tsu;
	tsu.topologicalSort(number_of_tasks+1, visit2, Stack2, pred);
	//pred starts from the back
	nodes[number_of_tasks+1].ls = nodes[number_of_tasks+1].es;
	nodes[number_of_tasks+1].lf = nodes[number_of_tasks+1].ef;
	Stack2.pop();
	while(!Stack2.empty()) {
		int top = Stack2.top();
		int min_s = INT8_MAX;
		for(int i = 0; i < succ[top].size(); i++) {
			if(min_s > nodes[succ[top][i]].ls)
				min_s = nodes[succ[top][i]].ls;
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
	CPM_Class::ObjectManager nodesandsucc;
	nodesandsucc.nodes=nodes; nodesandsucc.number_of_tasks=number_of_tasks; nodesandsucc.succ=succ;
	return nodesandsucc;
}

void CPM_Class::ObjectManager::results_table(int number_of_tasks,
  CPM_Class::ObjectManager *nodes){
    /*
    Reveals tasks and their durations/ES/EF/LS/LF/Slack Time in 1 table.

    number_of_tasks - # of steps to make order
    nodes - Has all the details the table needs
    */
	cout<<"RESULTS : \n\n";
	cout<<"\t#\tTask\t\t\tDur.\tES\tEF\tLS\tLF\tST\n\n";
	for(int i = 0 ; i < number_of_tasks+2 ; i++) {
		nodes[i].st = nodes[i].ls - nodes[i].es;
		cout<<"\t"<<i<<"\t"<<nodes[i].name<<"\t\t"<<nodes[i].duration<<"\t"<<nodes[i].es<<"\t"<<nodes[i].ef<<"\t"<<nodes[i].ls<<"\t"<<nodes[i].lf<<"\t"<<nodes[i].st<<"\n\n";
	}
}

void CPM_Class::ObjectManager::the_critical_path(int number_of_tasks,
  CPM_Class::ObjectManager *nodes, vector<vector<int>> succ, ofstream &f){
	//Courtesy of https://github.com/suman95/Critical-path-management
  /*
  Reveals the path that has no slack times (ls-es=0) from start to finish.

  number_of_tasks - # of steps to make order
  nodes - Has all the details the table needs
  succ - Successors vector
  f - Plot file that nodes data is sent to
  */
	queue<int> q;
	vector<int> critical_path(number_of_tasks+2,0);
	q.push(0);
	critical_path[0] = 1;

	while(!q.empty()) {
		int top = q.front();
		q.pop();
		if(nodes[top].es == nodes[top].ls) {
			critical_path[top] = 1;
		}
		for(int i = 0 ; i < succ[top].size(); i++) {
				q.push(succ[top][i]);
		}
  }/*
  for (int i=0; i<number_of_tasks+2; ++i) {
    if(nodes[i].es == nodes[i].ls) {
			critical_path[i] = 1;
		}
  }*/ //Could just be this commented for loop instead of the above, but haven't tested all choices yet
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
	execut.exec("python3 plot_graph.py < plot_graph.plt");
	delete[] nodes;
}

int main() {
	ofstream f; //means by which we can open external files
	CPM_Class::ObjectManager CPM_obj; //necessary object
	CPM_Class::ObjectManager users_order = CPM_obj.take_users_order(f);

	CPM_Class::ObjectManager *nodes = new CPM_Class::ObjectManager [users_order.number_of_tasks+2];
	nodes = CPM_obj.append_start_end_nodes(users_order.number_of_tasks, nodes);
	nodes = CPM_obj.tasks_to_unique_nodes(users_order.number_of_tasks, users_order.food_step_name, users_order.food_step_duration, nodes);
	int number_of_tasks=CPM_obj.send_nodes_to_plot_file(f, nodes, users_order.number_of_tasks);
	CPM_obj.print_tasks(nodes, number_of_tasks);

	CPM_Class::ObjectManager succpred=CPM_obj.make_succ_pred_vectors(number_of_tasks);
	succpred = CPM_obj.make_edges(number_of_tasks, nodes, succpred.succ, succpred.pred, f);

	CPM_obj.debug_matrices(number_of_tasks, succpred.succ, succpred.pred);
	nodes = CPM_obj.calculate_es_ef(number_of_tasks, succpred.succ, nodes, succpred.pred);
	CPM_Class::ObjectManager lslf = CPM_obj.calculate_ls_lf(number_of_tasks, succpred.succ, succpred.pred, nodes);

	CPM_obj.results_table(lslf.number_of_tasks, lslf.nodes);
	CPM_obj.the_critical_path(lslf.number_of_tasks, lslf.nodes, lslf.succ, f);
	return 0;
}
