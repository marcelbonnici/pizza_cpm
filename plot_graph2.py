# Courtesy of https://github.com/suman95/Critical-path-management
from graphviz import Digraph

def plot_layout():
	plot = Digraph("Critical path")
	plot.body.extend(['rankdir=LR', 'size="8,5"'])
	plot.attr('node', shape='box')
	number_of_tasks = int(input())
	edge_list = []
	node_list = []
	return plot, number_of_tasks, edge_list, node_list

def make_nodes (plot, number_of_tasks, node_list):
	for i in range(number_of_tasks+2):
		temp = []
		[a,b] = str(input()).split(' ') # a=task name, b=duration
		temp.append(a)
		plot.node(str(a),str(a)) # name & label, and I've made both the same
		temp.append(b)
		node_list.append(temp) # [[a0,b0],[a1,b1],...]
	return plot, node_list

def make_edges (edge_list):
	while True:
		temp = []
		k = str(input())
		if(k=='quit'):
			break
		[a,b] = k.split(' ') # a=tail, b=head of arrow
		temp.append(a)
		temp.append(b)
		edge_list.append(temp)
	return edge_list

def highlight_critical_path (plot, node_list):
	temp = str(input()).strip() # This is critical path
	temp = temp.split(' ')
	comp = []
	print(temp)
	for x in range(1,len(temp)):
		# Make red edge from Start to Stov_On, Stov_On to next critical node & so on
		plot.edge(node_list[int(temp[x-1])][0],node_list[int(temp[x])][0], color='red')
		te2 = []
		te2.append(temp[x-1]) # Copy temp into te2
		te2.append(temp[x])
		comp.append(te2)
	return plot, comp, node_list

def make_remaining_edges(plot, edge_list, comp, node_list):
	for x in edge_list:
		if x not in comp: # If edge not part of critical path, make it black edge
			[a,b] = x
			plot.edge(node_list[int(a)][0],node_list[int(b)][0])
	plot.view()

def main():
	plot, number_of_tasks, edge_list, node_list = plot_layout()
	plot, node_list = make_nodes(plot, number_of_tasks, node_list)
	edge_list = make_edges (edge_list)
	plot, comp, node_list = highlight_critical_path (plot, node_list)
	make_remaining_edges(plot, edge_list, comp, node_list)
if __name__ == '__main__':
	main()
