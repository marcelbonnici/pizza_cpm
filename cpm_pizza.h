namespace CPM_Class
{
    class ObjectManager
    {
    public:
			vector<string> food_step_name;
			vector<int> food_step_duration;
			int number_of_tasks;
			int tweak;
		  CPM_Class::ObjectManager take_users_order(ofstream &f);
			CPM_Class::ObjectManager foodnodes(CPM_Class::ObjectManager super, string foodnumber, int tweak, vector<string> food_step_name, vector<int> food_step_duration, int number_of_tasks);
      CPM_Class::ObjectManager user_enters_food_numbers(int items, CPM_Class::ObjectManager food_step_details, int tweak, vector<string> food_step_name, vector<int> food_step_duration, int number_of_tasks, CPM_Class::ObjectManager fn);
      CPM_Class::ObjectManager take_users_order_cont(int tweak, int number_of_tasks, ofstream &f, vector<string> food_step_name, vector<int> food_step_duration);
			string name;
			int duration;
			int es, ef, ls, lf, st;
			CPM_Class::ObjectManager* append_start_end_nodes(int number_of_tasks, CPM_Class::ObjectManager *nodes);
			// This function vvv courtesy of https://github.com/suman95/Critical-path-management
			void topologicalSort(int v, vector<bool> &visited,  stack<int> &Stack, vector< vector<int> > &succ);
			void exec(const char* cmd);
			CPM_Class::ObjectManager* tasks_to_unique_nodes(int number_of_tasks, vector<string> food_step_name, vector<int> food_step_duration, CPM_Class::ObjectManager *nodes);
			int send_nodes_to_plot_file(ofstream &f, CPM_Class::ObjectManager *nodes, int number_of_tasks);
			void print_tasks(CPM_Class::ObjectManager *nodes, int number_of_tasks);
			vector< vector<int> > succ;
			vector< vector<int> > pred;
			CPM_Class::ObjectManager make_succ_pred_vectors(int number_of_tasks);
			CPM_Class::ObjectManager fill_succ_pred(vector<int> ReadNumbers, int number_of_tasks, int i, vector<vector<int>> succ, vector<vector<int>> pred, ofstream &f);
			void debug_matrices(int number_of_tasks, vector<vector<int>> succ, vector< vector<int> > pred);
      CPM_Class::ObjectManager make_edges(int number_of_tasks, CPM_Class::ObjectManager *nodes, vector<vector<int>> succ, vector<vector<int>> pred, ofstream &f);
			void results_table(int number_of_tasks, CPM_Class::ObjectManager *nodes);
			void the_critical_path(int number_of_tasks, CPM_Class::ObjectManager *nodes, vector<vector<int>> succ, ofstream &f);
			CPM_Class::ObjectManager *nodes;
			CPM_Class::ObjectManager* calculate_es_ef(int number_of_tasks, vector<vector<int>> succ, CPM_Class::ObjectManager* nodes, vector<vector<int>> pred);
			CPM_Class::ObjectManager calculate_ls_lf(int number_of_tasks, vector<vector<int>> succ, vector<vector<int>> pred, CPM_Class::ObjectManager *nodes);
      vector<int> ReadNumbers;
      vector<int> make_02_edges(int i, vector<int> ReadNumbers, int number_of_tasks);
      vector<int> make_20_edges(int i, vector<int> ReadNumbers, int number_of_tasks);
      vector<int> make_stov_on_edges (int i, int number_of_tasks, CPM_Class::ObjectManager* nodes, vector<int> ReadNumbers);
      vector<int> easy_case(int i, vector<int> ReadNumbers, CPM_Class::ObjectManager *nodes);
      vector<int> other_cases(int i, vector<int> ReadNumbers, CPM_Class::ObjectManager* nodes, int number_of_tasks);
		};
}
