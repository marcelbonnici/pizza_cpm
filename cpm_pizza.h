namespace CPMBegin
{
    class ObjectManager
    {
    public:
			vector<string> name1;
			vector<int> duration1;
			int n_tasks;
			int tweak;
		  CPMBegin::ObjectManager cpm_input(ofstream &f);
			CPMBegin::ObjectManager foodnodes(CPMBegin::ObjectManager super, string foodnumber, int tweak, vector<string> name1, vector<int> duration1, int n_tasks);
			string name;
			int duration;
			int es, ef, ls, lf, st;
			CPMBegin::ObjectManager* start_end_nodes(int n_tasks, CPMBegin::ObjectManager *nodes);
			// This function vvv courtesy of https://github.com/suman95/Critical-path-management
			void topologicalSortUtil(int v, vector<bool> &visited,  stack<int> &Stack, vector< vector<int> > &adj);
			string exec(const char* cmd);
			CPMBegin::ObjectManager* task_nodes(int n_tasks, vector<string> name1, vector<int> duration1, CPMBegin::ObjectManager *nodes);
			int nodes_to_plot(ofstream &f, CPMBegin::ObjectManager *nodes, int n_tasks);
			void tasks_print(CPMBegin::ObjectManager *nodes, int n_tasks);
			vector< vector<int> > adj;
			vector< vector<int> > pred;
			CPMBegin::ObjectManager make_adj_pred(int n_tasks);
			CPMBegin::ObjectManager fill_adj_pred(vector<int> ReadNumbers, int n_tasks, int i, vector<vector<int>> adj, vector<vector<int>> pred, ofstream &f);
			void debug_matrices(int n_tasks, vector<vector<int>> adj, vector< vector<int> > pred);
      CPMBegin::ObjectManager make_edges(int n_tasks, CPMBegin::ObjectManager *nodes, vector<vector<int>> adj, vector<vector<int>> pred, ofstream &f);
			void results_table(int n_tasks, CPMBegin::ObjectManager *nodes);
			void critical_path1(int n_tasks, CPMBegin::ObjectManager *nodes, vector<vector<int>> adj, ofstream &f);
			CPMBegin::ObjectManager *nodes;
			CPMBegin::ObjectManager* esef(int n_tasks, vector<vector<int>> adj, CPMBegin::ObjectManager* nodes, vector<vector<int>> pred);
			CPMBegin::ObjectManager lslf(int n_tasks, vector<vector<int>> adj, vector<vector<int>> pred, CPMBegin::ObjectManager *nodes);
      vector<int> ReadNumbers;
      vector<int> make_02_edges(int i, vector<int> ReadNumbers, int n_tasks);
      vector<int> make_20_edges(int i, vector<int> ReadNumbers, int n_tasks);
      vector<int> make_stov_on_edges (int i, int n_tasks, CPMBegin::ObjectManager* nodes, vector<int> ReadNumbers);
      vector<int> easy_case(int i, vector<int> ReadNumbers, CPMBegin::ObjectManager *nodes);
      vector<int> other_cases(int i, vector<int> ReadNumbers, CPMBegin::ObjectManager* nodes, int n_tasks);
		};
}
