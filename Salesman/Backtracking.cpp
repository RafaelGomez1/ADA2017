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
void SalesmanMatrix(list<int> &best, list<int> &visited, list<int> &toVisit, vector<double> &matrix,double &bestCost, double partialCost, int &nVertex) {
	
	if (partialCost > bestCost)
		return;

	if (toVisit.size() == 0) {
		int d = matrix[visited.back()*nVertex + nVertex - 1];
		if (partialCost + d < bestCost) {
			best = visited;
			bestCost = partialCost + d;
		}
	}
	else {
		int actual = visited.back();
		int dist;
		list<int> temp = toVisit;
		for (int i : temp) {
			dist = matrix[actual*nVertex + i];
			if (partialCost + dist < bestCost) {
				toVisit.remove(i);
				visited.push_back(i);
				SalesmanMatrix(best, visited, toVisit, matrix,bestCost,partialCost + dist, nVertex);
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
	double bestCost = numeric_limits<double>::max();
	double min;
	int nVertex = visits.m_Vertices.size();

	vector<double> matrix;
	matrix.resize(nVertex*nVertex);

	vector<CVertex*> v{ begin(visits.m_Vertices), end(visits.m_Vertices) };

	int desti;
	for (int origen = 0; origen < nVertex; origen++) {
		CGraph::Dijkstra(v[origen]);
		for (desti = 0; desti < nVertex; desti++) {
			matrix[origen*nVertex + desti] = (v[desti]->m_DijkstraDistance);
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

	SalesmanMatrix(best, visited, toVisit, matrix, bestCost,0, nVertex);

	best.push_back(nVertex - 1);

	int pOrigen = best.front();
	CVertex *vOrigen = v[pOrigen];
	int pDesti;
	CVertex *vDesti;
	best.pop_front();
	bestTrack.AddFirst(first);
	for (int n : best) {
		partialTrack.Clear();
		pDesti = n;
		vDesti = v[pDesti];
		CGraph::Dijkstra(v[pOrigen]);
		while (vOrigen != vDesti) {
			partialTrack.AddFirst(vDesti);
			vDesti = vDesti->m_pDijkstraPrevious;
		}
		pOrigen = pDesti;
		vOrigen = v[pOrigen];
		bestTrack.Append(partialTrack);

	}
	return bestTrack;
}
