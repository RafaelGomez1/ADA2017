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

// SalesmanTrackBacktrackingGreedy ============================================
void BacktrackingGreedy(list<int> &best, list<int> &visited, list<int> &toVisit, vector<double>& matrix,vector<double>& min_cost_matrix,double &best_cost, double current_cost, int &length) {
	

	double h = 0;
	for (int i : toVisit) {
		h += min_cost_matrix[i];
	}
	h += min_cost_matrix[length - 1];


	if (current_cost + h > best_cost)
		return;

	if (toVisit.size() == 0) {
		int d = matrix[visited.back()*length + length - 1];
		if (current_cost + d < best_cost) {
			best = visited;
			best_cost = current_cost + d;
		}
	}
	else {
		int current = visited.back();
		int distance;
		list<int> temp = toVisit;
		for (int i : temp) {
			distance = matrix[current*length + i];
			if (current_cost + distance < best_cost) {
				toVisit.remove(i);
				visited.push_back(i);
				BacktrackingGreedy(best, visited, toVisit, matrix, min_cost_matrix,best_cost,current_cost + distance, length);
				visited.pop_back();
				toVisit.push_front(i);			
			}
		}
	}

}

CTrack CGraph::SalesmanTrackBacktrackingGreedy(CVisits &visits)
{

	CTrack bestTrack(this);
	CTrack partialTrack(this);
	list<int> visited;
	list<int> best;
	list<int> toVisit;
	double best_cost = numeric_limits<double>::max();
	int length = visits.m_Vertices.size();

	vector<double> matrix;
	matrix.resize(length*length);


	vector<CVertex*> v{ begin(visits.m_Vertices), end(visits.m_Vertices) };

	for (int i = 0; i < length; i++) {
		CGraph::Dijkstra(v[i]);
		for (int j = 0; j < length; j++) {
			matrix[i*length + i] = (v[i]->m_DijkstraDistance);
		}
	}

	vector<double> min_dist_matrix;
	min_dist_matrix.resize(length);
	double min;
	for (int i = 0; i < length; i++) {
		min = matrix[0 + i];
		for (int j = 0; j < length; j++) {
			if (matrix[j*length + i] < min && j != i) {
				min = matrix[j*length + i];
			}
			min_dist_matrix[i] = min;
		}
	}


	visited.push_front(0);

	CVertex *first = visits.m_Vertices.front();
	CVertex *last = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();
	visits.m_Vertices.pop_front();

	int count = 1;
	for (CVertex *pCount : visits.m_Vertices) {
		toVisit.push_back(count);
		count++;
	}

	BacktrackingGreedy(best, visited, toVisit, matrix, min_dist_matrix,best_cost,0, length);

	best.push_back(length - 1);

	int origin_index = best.front();
	CVertex *origin = v[origin_index];
	int dedstination_index;
	CVertex *destination;
	best.pop_front();
	bestTrack.AddFirst(first);
	for (int n : best) {
		partialTrack.Clear();
		dedstination_index = n;
		destination = v[dedstination_index];
		CGraph::Dijkstra(v[origin_index]);
		while (origin != destination) {
			partialTrack.AddFirst(destination);
			destination = destination->m_pDijkstraPrevious;
		}
		origin_index = dedstination_index;
		origin = v[origin_index];
		bestTrack.Append(partialTrack);

	}
	return bestTrack;
}
