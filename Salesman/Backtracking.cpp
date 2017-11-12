#include "stdafx.h"
#include "CGraph.h"
#include <set>

void Bakctracking(CVisits &visits, CTrack track, set<CVertex*>subtrack, CTrack &bestTrack, double &bestDist, CVertex* last) {

	if (track.Length() > bestDist)
		return;

	CVisits visits_temp = visits;
	set<CVertex*>subtrack_temp = subtrack;
	CTrack track_temp = track;

	if (visits.m_Vertices.empty()) {
			bestTrack = track;
			bestDist = bestTrack.Length();
			return;
		}

	else {
		CVertex* current = track.m_Vertices.back();
		bool visit = false;

		for (CVertex* n : current->m_Neighbords) {
			if (!subtrack.count(n)) {
				for (CVertex* temp : visits.m_Vertices) {
					if (temp == n)
						visit = true;
				}
				track.AddLast(n);
				subtrack.insert(n);
				if (track.Length() < bestDist && visit) {

					subtrack.clear();
					subtrack.insert(n);
					visits.m_Vertices.remove(n);
					Bakctracking(visits, track, subtrack, bestTrack, bestDist,last);
				}
				else 
					Bakctracking(visits, track, subtrack, bestTrack, bestDist,last);
						
				visits = visits_temp;
				track = track_temp;
				subtrack = subtrack_temp;
			}		
		}
		return;
	}
}

CTrack CGraph::SalesmanTrackBacktracking(CVisits &visits)
{
	CTrack track(this);
	 set<CVertex*> subtrack;

	double bestdist = numeric_limits<double>::max();
	CTrack bestTrack(this);

	CVertex* first = visits.m_Vertices.front();
	CVertex* last = visits.m_Vertices.back();
	visits.m_Vertices.pop_front();
	visits.m_Vertices.pop_back();
	track.AddFirst(first);
	subtrack.insert(first);

	Bakctracking(visits,track, subtrack,bestTrack, bestdist,last);

	CVertex *second_to_last = bestTrack.m_Vertices.back();
	CGraph::Dijkstra(second_to_last);
	CVertex* path = last;
	track.Clear();
	while (second_to_last != path) {
		track.AddFirst(path);
		path = path->m_pDijkstraPrevious;
	}
	bestTrack.Append(track);



	return bestTrack;
}

// SalesmanTrackBacktrackingGreedy =============================================

void BacktrackingGreedy(list<int> &best_path, list<int> &visited, set<int> &to_visit, vector<double> &matrix, double &bestCost, double partialCost, int &nVertex, vector<double> &min_dist_matrix) {

	if (to_visit.size() == 0) {
		int d = matrix[visited.back()*nVertex + nVertex - 1];
		if (partialCost + d < bestCost) {
			best_path = visited;
			bestCost = partialCost + d;
		}
	}
	else {
		double h = 0;
		for (int i : to_visit) {
			h += min_dist_matrix[i];
		}
		h += min_dist_matrix[nVertex - 1];
		if (partialCost + h - 1 <= bestCost) {
			int actual = visited.back();
			int distance;
			set<int> tempToVisit = to_visit;
			for (int i : tempToVisit) {
				distance = matrix[actual*nVertex + i];
				if (partialCost + distance < bestCost) {
					to_visit.erase(i);
					visited.push_back(i);
					BacktrackingGreedy(best_path, visited, to_visit, matrix, bestCost, partialCost + distance, nVertex, min_dist_matrix);
					visited.remove(i);
					to_visit.insert(i);
				}
			}
		}
	}
}

CTrack CGraph::SalesmanTrackBacktrackingGreedy(CVisits &visits)
{
	CTrack bestTrack(this);
	CTrack partialTrack(this);
	list<int> visited;
	list<int> best_path;
	set<int> to_visit;
	double min;
	double best_cost = numeric_limits<double>::max();
	int count = 1;
	int length = visits.m_Vertices.size();

	vector<double> matrix;
	matrix.resize(length*length);
	vector<CVertex*> v{ begin(visits.m_Vertices), end(visits.m_Vertices) };

	for (int i = 0; i < length; i++) {
		CGraph::Dijkstra(v[i]);
		for (int j = 0; j < length; j++) {
			matrix[i*length + j] = (v[j]->m_DijkstraDistance);
		}
	}

	vector<double> min_dist_matrix;
	min_dist_matrix.resize(length);
	for (int j = 0; j < length; j++) {
		min = matrix[0 + j];
		for (int i = 0; i < length; i++) {
			if (matrix[i*length + j] < min && i != j) {
				min = matrix[i*length + j];
			}
			min_dist_matrix[j] = min;
		}
	}

	visited.push_front(0);

	CVertex *first = visits.m_Vertices.front();
	CVertex *last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();
	visits.m_Vertices.pop_front();

	
	for (CVertex *pCount : visits.m_Vertices) {
		to_visit.insert(count);
		count++;
	}
	BacktrackingGreedy(best_path, visited, to_visit, matrix, best_cost, 0, length, min_dist_matrix);

	best_path.push_back(length - 1);

	int origin_index = best_path.front();
	CVertex *origin = v[origin_index];
	int destination_index;
	CVertex *destination;

	best_path.pop_front();
	bestTrack.AddFirst(first);

	for (int n : best_path) {
		partialTrack.Clear();
		destination_index = n;
		destination = v[destination_index];
		CGraph::Dijkstra(v[origin_index]);

		while (origin != destination) {
			partialTrack.AddFirst(destination);
			destination = destination->m_pDijkstraPrevious;
		}
		origin_index = destination_index;
		origin = v[origin_index];
		bestTrack.Append(partialTrack);

	}
	return bestTrack;
}