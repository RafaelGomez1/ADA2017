#include "stdafx.h"
#include "CGraph.h"


class bNode {
public:
	bNode(vector<int> to_visit, double h, double cost, vector<int> solution,double benchmark);
	vector<int> to_visit;
	double h;
	double cost;
	vector<int> solution;
	double benchmark;



};
bNode::bNode(vector<int> to_visit, double h, double cost, vector<int> solution,double benchmark) {
	this->to_visit = to_visit;
	this->cost = cost;
	this->h = h;
	this->solution = solution;
	this->benchmark = benchmark;
}

struct compare_h2_h1 {
	bool operator()(bNode const *i1, bNode const *i2) {
		return (i1->h < i2->h);
	}
};

struct compare_h3 {
	bool operator()(bNode const *i1, bNode const *i2) {
		return (i1->h > i2->h);
	}
};



// SalesmanTrackBranchAndBound1 ===================================================

CTrack CGraph::SalesmanTrackBranchAndBound1(CVisits &visits)
{
	int n_vertex = visits.m_Vertices.size();
	priority_queue<bNode*, vector<bNode*>, compare_h2_h1> pq;
	double best_cost = numeric_limits<double>::max();
	vector<int> visited;
	vector<int> best_solution;
	vector<int> not_visited;


	vector<double> matrix;
	matrix.resize(n_vertex*n_vertex);
	vector<CVertex*> vec{ begin(visits.m_Vertices), end(visits.m_Vertices) };

	int destination;
	for (int i = 0; i < n_vertex; i++) {
		CGraph::Dijkstra(vec[i]);
		for (destination = 0; destination < n_vertex; destination++) {
			matrix[i*n_vertex + destination] = (vec[destination]->m_DijkstraDistance);
		}
	}


	visited.push_back(0);

	CVertex* first = visits.m_Vertices.front();
	CVertex* last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();
	visits.m_Vertices.pop_front();

	if (n_vertex <= 2) {
		for (int i = 0; i < n_vertex; i++) {
			best_solution.push_back(i);
		}
		goto end1;
	}

	int count = 1;
	for (CVertex *pCount : visits.m_Vertices) {
		not_visited.push_back(count);
		count++;
	}
	bNode *current = new bNode(not_visited, 0, 0, visited,0);
	bNode *new_node;
	pq.push(current);
	int actual;
	int val;
	while (!pq.empty()) {
		current = pq.top();
		pq.pop();
		if (current->h < best_cost) {
			actual = current->solution.back();
			for (int i = 0; i < current->to_visit.size(); i++) {
				val = current->to_visit[i];
				if (current->cost + matrix[actual*n_vertex + val] < best_cost) {
					new_node = new bNode(current->to_visit, 0, current->cost, current->solution,0);
					new_node->cost += matrix[actual*n_vertex + val];
					new_node->h = new_node->cost;
					new_node->to_visit.erase(new_node->to_visit.begin() + i);
					new_node->solution.push_back(val);

					if (new_node->to_visit.empty()) {
						new_node->cost += matrix[(new_node->solution.back()*n_vertex) + n_vertex - 1];
						if (new_node->cost < best_cost) {
							new_node->solution.push_back(n_vertex - 1);
							best_solution = new_node->solution;
							best_cost = new_node->cost;
						}
						else {
							delete new_node;
						}
					}
					else {
						if (new_node->h < best_cost) {
							pq.push(new_node);
						}
						else {
							delete new_node;
						}

					}
				}
			}
		}
		delete current;
	}
end1:
	CTrack bestTrack(this);
	CTrack partialTrack(this);
	int p_origin = best_solution.front();
	CVertex *v_origin = vec[p_origin];
	int p_destination;
	CVertex *v_destination;
	best_solution[0];
	bestTrack.AddFirst(first);
	for (int i : best_solution) {
		partialTrack.Clear();
		p_destination = i;
		v_destination = vec[p_destination];
		Dijkstra(vec[p_origin]);
		while (v_origin != v_destination) {
			partialTrack.AddFirst(v_destination);
			v_destination = v_destination->m_pDijkstraPrevious;
		}
		p_origin = p_destination;
		v_origin = vec[p_origin];
		bestTrack.Append(partialTrack);

	}
	return bestTrack;
}

// SalesmanTrackBranchAndBound2 ===================================================

CTrack CGraph::SalesmanTrackBranchAndBound2(CVisits &visits)
{
	double best_cost = numeric_limits<double>::max();
	double evaluate = best_cost;
	priority_queue<bNode*, vector<bNode*>, compare_h2_h1> pq;
	vector<int> visited;
	vector<int> best_solution;
	vector<int> to_visit;
	int n_vertex = visits.m_Vertices.size();
	vector<double> matrix;
	matrix.resize(n_vertex*n_vertex);
	vector<CVertex*> vec{ begin(visits.m_Vertices), end(visits.m_Vertices) };

	int j;
	int i;
	for ( i = 0; i < n_vertex; i++) {
		CGraph::Dijkstra(vec[i]);
		for (j = 0; j < n_vertex; j++) {
			matrix[i*n_vertex + j] = (vec[j]->m_DijkstraDistance);
		}
	}

	vector<double> min_cost;
	min_cost.resize(n_vertex);
	double min;
	for (j = 0; j < n_vertex; j++) {
		min = matrix[0 + j];
		for (int i = 0; i < n_vertex; i++) {
			if (matrix[i*n_vertex + j] < min && i != j) {
				min = matrix[i*n_vertex + j];
			}
			min_cost[j] = min;
		}
	}


	visited.push_back(0);

	CVertex *first = visits.m_Vertices.front();
	CVertex *last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();
	visits.m_Vertices.pop_front();

	if (n_vertex <= 2) {
		for (int i = 0; i < n_vertex; i++) {
			best_solution.push_back(i);
		}
		goto end2;
	}

	int count = 1;
	for (CVertex *n : visits.m_Vertices) {
		to_visit.push_back(count);
		count++;
	}

	double h = 0;
	for (int i : to_visit) {
		h += min_cost[i];
	}
	h += min_cost[n_vertex - 1];
	int actual;
	int previous = n_vertex - 1;
	double h_actual;
	double updated_cost;
	bNode *current = new bNode(to_visit, h, 0, visited,0);
	bNode *new_node;
	int val;
	pq.push(current);
	while (!pq.empty()) {
		current = pq.top();
		pq.pop();
		h_actual = current->h;
		if (h_actual < best_cost) {
			actual = current->solution.back();
			for (int i = 0; i < current->to_visit.size(); i++) {
				val = current->to_visit[i];
				updated_cost = current->cost + matrix[actual*n_vertex + val];
				h = h_actual - min_cost[val] + matrix[actual*n_vertex + val];
				if (h < best_cost) {
					new_node = new bNode(current->to_visit, h, updated_cost, current->solution,0);
					new_node->to_visit.erase(new_node->to_visit.begin() + i);
					new_node->solution.push_back(val);
					if (new_node->to_visit.empty()) {
						new_node->cost += matrix[(new_node->solution.back()*n_vertex) + previous];
						if (new_node->cost < best_cost) {
							new_node->solution.push_back(previous);
							best_solution = new_node->solution;
							best_cost = new_node->cost;
						}
						else {
							delete new_node;
						}

					}
					else {
						pq.push(new_node);
					}
				}
			}
		}

		delete current;
	}
end2:
	CTrack bestTrack(this);
	CTrack partialTrack(this);
	int p_origin = best_solution.front();
	CVertex *v_origin = vec[p_origin];
	int p_destination;
	CVertex *v_destination;
	best_solution[0];
	bestTrack.AddFirst(first);
	for (int i : best_solution) {
		partialTrack.Clear();
		p_destination = i;
		v_destination = vec[p_destination];
		Dijkstra(vec[p_origin]);
		while (v_origin != v_destination) {
			partialTrack.AddFirst(v_destination);
			v_destination = v_destination->m_pDijkstraPrevious;
		}
		p_origin = p_destination;
		v_origin = vec[p_origin];
		bestTrack.Append(partialTrack);

	}
	return bestTrack;
}

// SalesmanTrackBranchAndBound3 ===================================================

CTrack CGraph::SalesmanTrackBranchAndBound3(CVisits &visits)
{
	double best_cost = numeric_limits<double>::max();
	double update_cost = best_cost;
	priority_queue<bNode*, vector<bNode*>, compare_h3> pq;
	vector<int> visited;
	vector<int> best_solution;
	vector<int> to_visit;
	int n_vertex = visits.m_Vertices.size();
	vector<double> matrix;
	matrix.resize(n_vertex*n_vertex);
	vector<CVertex*> vec{ begin(visits.m_Vertices), end(visits.m_Vertices) };

	int destination;
	for (int i = 0; i < n_vertex; i++) {
		CGraph::Dijkstra(vec[i]);
		for (destination = 0; destination < n_vertex; destination++) {
			matrix[i*n_vertex + destination] = (vec[destination]->m_DijkstraDistance);
		}
	}

	vector<double> min_cost;
	min_cost.resize(n_vertex);
	double min;
	for (destination = 0; destination < n_vertex; destination++) {
		min = matrix[0 + destination];
		for (int i = 0; i < n_vertex; i++) {
			if (matrix[i*n_vertex + destination] < min && i != destination) {
				min = matrix[i*n_vertex + destination];
			}
			min_cost[destination] = min;
		}
	}


	visited.push_back(0);

	CVertex *first = visits.m_Vertices.front();
	CVertex *last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();
	visits.m_Vertices.pop_front();

	if (n_vertex <= 2) {
		for (int i = 0; i < n_vertex; i++) {
			best_solution.push_back(i);
		}
		goto end2;
	}

	int count = 1;
	for (CVertex *pCount : visits.m_Vertices) {
		to_visit.push_back(count);
		count++;
	}

	double h = 0;
	for (int i : to_visit) {
		h += min_cost[i];
	}
	h += min_cost[n_vertex - 1];
	int actual;
	int previous = n_vertex - 1;
	double lastcost = min_cost[previous];
	double h_actual;
	double updated_cost;
	double best = best_cost;
	bNode *current = new bNode(to_visit, h, 0, visited, h);
	bNode *new_node;
	pq.push(current);
	int ival;
	while (!pq.empty()) {
		current = pq.top();
		pq.pop();
		h_actual = current->benchmark;
		if (h_actual < best_cost) {
			actual = current->solution.back();
			for (int i = 0; i < current->to_visit.size(); i++) {
				ival = current->to_visit[i];
				updated_cost = current->cost + matrix[actual*n_vertex + ival];
				h = h_actual - min_cost[ival] + matrix[actual*n_vertex + ival];
				if (h < best_cost) {
					new_node = new bNode(current->to_visit, h, updated_cost, current->solution, h);
					new_node->to_visit.erase(new_node->to_visit.begin() + i);
					new_node->solution.push_back(ival);
					new_node->h = (new_node->h) * (new_node->to_visit.size());
					if (new_node->to_visit.empty()) {
						new_node->cost += matrix[(new_node->solution.back()*n_vertex) + previous];
						if (new_node->cost < best_cost) {
							new_node->solution.push_back(previous);
							best_solution = new_node->solution;
							best_cost = new_node->cost;
						}
						else {
							delete new_node;
						}
					}
					else {
						pq.push(new_node);
					}
				}
			}
		}

		delete current;
	}
end2:
	CTrack bestTrack(this);
	CTrack partialTrack(this);
	int p_origin = best_solution.front();
	CVertex *v_origin = vec[p_origin];
	int p_destination;
	CVertex *v_destination;
	best_solution[0];
	bestTrack.AddFirst(first);
	for (int n : best_solution) {
		partialTrack.Clear();
		p_destination = n;
		v_destination = vec[p_destination];
		Dijkstra(vec[p_origin]);
		while (v_origin != v_destination) {
			partialTrack.AddFirst(v_destination);
			v_destination = v_destination->m_pDijkstraPrevious;
		}
		p_origin = p_destination;
		v_origin = vec[p_origin];
		bestTrack.Append(partialTrack);

	}
	return bestTrack;
}
