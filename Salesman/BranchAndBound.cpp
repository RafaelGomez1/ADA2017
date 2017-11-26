#include "stdafx.h"
#include "CGraph.h"

class Aux_node {
public:
	Aux_node(list<int> to_visit, double h, double cost, list<int> sol);
	list<int> to_visit;
	double h;
	double cost;
	list<int> sol;

};
Aux_node::Aux_node(list<int> to_visit, double h, double cost, list<int> sol) {
	this->to_visit = to_visit;
	this->cost = cost;
	this->h = h;
	this->sol = sol;
}


struct CompareDistance {
	bool operator()(Aux_node const *i1, Aux_node const *i2) {
		return i1->h < i2->h;
	}
};

// SalesmanTrackBranchAndBound1 ===================================================
/*
Siempre completar el camino parcial más corto de los que se hayan creado. En el caso
de haber encontrado un camino completo, se podarán los caminos parciales más largos
que el mejor camino completo que se haya encontrado hasta el momento.
*/
CTrack CGraph::SalesmanTrackBranchAndBound1(CVisits &visits)
{
	int n_vertex = visits.m_Vertices.size();
	priority_queue<Aux_node*, vector<Aux_node*>, CompareDistance> pq;
	double best_cost = numeric_limits<double>::max();
	list<int> visited;
	list<int> best_solution;
	list<int> not_visited;

	vector<double> matrix;
	matrix.resize(n_vertex*n_vertex);
	vector<CVertex*> vec{ begin(visits.m_Vertices), end(visits.m_Vertices) };

	int destination;
	int origin;
	for (origin = 0; origin < n_vertex; origin++) {
		CGraph::Dijkstra(vec[origin]);
		for (destination = 0; destination < n_vertex; destination++) {
			matrix[origin*n_vertex + destination] = (vec[destination]->m_DijkstraDistance);
		}
	}

	visited.push_front(0);

	CVertex *first = visits.m_Vertices.front();
	CVertex *last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();
	visits.m_Vertices.pop_front();

	if (n_vertex <= 2) {
		for (int nv = 0; nv < n_vertex; nv++) {
			best_solution.push_back(nv);
		}
		goto end1;
	}

	int count = 1;
	for (CVertex *pCount : visits.m_Vertices) {
		not_visited.push_back(count);
		count++;
	}
	Aux_node *current_node = new Aux_node(not_visited, 0, 0, visited);
	Aux_node *new_node;
	pq.push(current_node);

	int actual;
	while (!pq.empty()) {
		current_node = pq.top();
		pq.pop();
		if (current_node->h < best_cost) {
			actual = current_node->sol.back();
			for (int i : current_node->to_visit) {
				if (current_node->cost + matrix[actual*n_vertex + i] < best_cost) {
					new_node = new Aux_node(current_node->to_visit, 0, current_node->cost, current_node->sol);
					new_node->cost += matrix[actual*n_vertex + i];
					new_node->h = new_node->cost;
					new_node->to_visit.remove(i);
					new_node->sol.push_back(i);

					if (new_node->to_visit.empty()) {
						new_node->cost += matrix[(new_node->sol.back()*n_vertex) + n_vertex - 1];
						if (new_node->cost < best_cost) {
							new_node->sol.push_back(n_vertex - 1);
							best_solution = new_node->sol;
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
		delete current_node;
	}
end1:
	CTrack bestTrack(this);
	CTrack partialTrack(this);
	int origin_p = best_solution.front();
	CVertex *origin_v = vec[origin_p];
	int destination_p;
	CVertex *destination_v;
	best_solution.pop_front();
	bestTrack.AddFirst(first);
	for (int sol : best_solution) {
		partialTrack.Clear();
		destination_p = sol;
		destination_v = vec[destination_p];
		Dijkstra(vec[origin_p]);
		while (origin_v != destination_v) {
			partialTrack.AddFirst(destination_v);
			destination_v = destination_v->m_pDijkstraPrevious;
		}
		origin_p = destination_p;
		origin_v = vec[origin_p];
		bestTrack.Append(partialTrack);

	}
	return bestTrack;
}

// SalesmanTrackBranchAndBound2 ===================================================

CTrack CGraph::SalesmanTrackBranchAndBound2(CVisits &visits)
{
	double best_cost = numeric_limits<double>::max();
	double cost = best_cost;
	priority_queue<Aux_node*, vector<Aux_node*>, CompareDistance> pq;
	list<int> visited;
	list<int> solution;
	list<int> to_visit;
	int n_vertex = visits.m_Vertices.size();
	vector<double> matrix;
	matrix.resize(n_vertex*n_vertex);
	vector<CVertex*> vec{ begin(visits.m_Vertices), end(visits.m_Vertices) };

	int destination;
	int origin;
	for (origin = 0; origin < n_vertex; origin++) {
		CGraph::Dijkstra(vec[origin]);
		for (destination = 0; destination < n_vertex; destination++) {
			matrix[origin*n_vertex + destination] = (vec[destination]->m_DijkstraDistance);
		}
	}

	vector<double> min_cost;
	min_cost.resize(n_vertex);
	double min;
	for (destination = 0; destination < n_vertex; destination++) {
		min = matrix[0 + destination];
		for (origin = 0; origin < n_vertex; origin++) {
			if (matrix[origin*n_vertex + destination] < min && origin != destination) {
				min = matrix[origin*n_vertex + destination];
			}
			min_cost[destination] = min;
		}
	}

	visited.push_front(0);

	CVertex *first = visits.m_Vertices.front();
	CVertex *last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();
	visits.m_Vertices.pop_front();

	if (n_vertex <= 2) {
		for (int i = 0; i < n_vertex; i++) {
			solution.push_back(i);
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
	int lastVertex = n_vertex - 1;
	double h_actual;
	double new_cost;

	Aux_node *current_node = new Aux_node(to_visit, h, 0, visited);
	Aux_node *new_node;
	pq.push(current_node);
	while (!pq.empty()) {
		current_node = pq.top();
		pq.pop();
		h_actual = current_node->h;
		if (h_actual < best_cost) {
			actual = current_node->sol.back();
			for (int i : current_node->to_visit) {
				new_cost = current_node->cost + matrix[actual*n_vertex + i];
				h = h_actual - min_cost[i] + matrix[actual*n_vertex + i];
				if (h < best_cost) {
					new_node = new Aux_node(current_node->to_visit, h, new_cost, current_node->sol);
					new_node->to_visit.remove(i);
					new_node->sol.push_back(i);
					if (new_node->to_visit.empty()) {

						new_node->cost += matrix[(new_node->sol.back()*n_vertex) + lastVertex];
						if (new_node->cost < best_cost) {
							new_node->sol.push_back(lastVertex);
							solution = new_node->sol;
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
		else {
			goto end2;
		}
		delete current_node;
	}
end2:
	CTrack bestTrack(this);
	CTrack partialTrack(this);
	int p_origin = solution.front();
	CVertex *v_origin = vec[p_origin];
	int p_destination;
	CVertex *v_destination;
	solution.pop_front();
	bestTrack.AddFirst(first);
	for (int i : solution) {
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
	double cost = best_cost;
	priority_queue<Aux_node*, vector<Aux_node*>, CompareDistance> pq;
	list<int> visited;
	list<int> best_solution;
	list<int> to_visit;
	int n_vertex = visits.m_Vertices.size();

	vector<double> matrix;
	matrix.resize(n_vertex*n_vertex);
	vector<CVertex*> vec{ begin(visits.m_Vertices), end(visits.m_Vertices) };

	int destination;
	int origin;
	for (origin = 0; origin < n_vertex; origin++) {
		CGraph::Dijkstra(vec[origin]);
		for (destination = 0; destination < n_vertex; destination++) {
			matrix[origin*n_vertex + destination] = (vec[destination]->m_DijkstraDistance);
		}
	}

	vector<double> min_cost;
	min_cost.resize(n_vertex);
	double min;
	for (destination = 0; destination < n_vertex; destination++) {
		min = matrix[0 + destination];
		for (origin = 0; origin < n_vertex; origin++) {
			if (matrix[origin*n_vertex + destination] < min && origin != destination) {
				min = matrix[origin*n_vertex + destination];
			}
			min_cost[destination] = min;
		}
	}


	visited.push_front(0);

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
	for (int he : to_visit) {
		h += min_cost[he];
	}
	h += min_cost[n_vertex - 1];
	int actual;
	int lastVertex = n_vertex - 1;
	double h_actual;
	double new_cost;

	Aux_node *current_node = new Aux_node(to_visit, h, 0, visited);
	Aux_node *new_node;
	pq.push(current_node);
	while (!pq.empty()) {
		current_node = pq.top();
		pq.pop();
		h_actual = current_node->h;
		if (h_actual < best_cost) {
			actual = current_node->sol.back();
			for (int f : current_node->to_visit) {

				new_cost = current_node->cost + matrix[actual*n_vertex + f];
				h = h_actual - min_cost[f] + matrix[actual*n_vertex + f];
				if (h < best_cost) {
					new_node = new Aux_node(current_node->to_visit, h, new_cost, current_node->sol);
					new_node->to_visit.remove(f);
					new_node->sol.push_back(f);
					if (new_node->to_visit.empty()) {
						new_node->cost += matrix[(new_node->sol.back()*n_vertex) + lastVertex];
						if (new_node->cost < best_cost) {
							new_node->sol.push_back(lastVertex);

							best_solution = new_node->sol;
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
		else {
			goto end2;
		}
		delete current_node;
	}
end2:
	CTrack bestTrack(this);
	CTrack partialTrack(this);
	int p_origin = best_solution.front();
	CVertex *v_origin = vec[p_origin];
	int p_desti;
	CVertex *v_desti;
	best_solution.pop_front();
	bestTrack.AddFirst(first);
	for (int i : best_solution) {
		partialTrack.Clear();
		p_desti = i;
		v_desti = vec[p_desti];
		Dijkstra(vec[p_origin]);
		while (v_origin != v_desti) {
			partialTrack.AddFirst(v_desti);
			v_desti = v_desti->m_pDijkstraPrevious;
		}
		p_origin = p_desti;
		v_origin = vec[p_origin];
		bestTrack.Append(partialTrack);

	}
	return bestTrack;
}
