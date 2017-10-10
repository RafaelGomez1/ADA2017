#include "stdafx.h"
#include "CGraph.h"

// SalesmanTrackGreedy =========================================================

CTrack CGraph::SalesmanTrackGreedy(CVisits &visits) 
{
	
	CVertex* next = visits.m_Vertices.front();
	double min_dist;
	CVertex* aux;

	CTrack track(this);
	
	aux = visits.m_Vertices.back();
	visits.m_Vertices.pop_back();
	track.AddFirst(visits.m_Vertices.front());
	visits.m_Vertices.pop_front();

	
	while (!visits.m_Vertices.empty()) {

		CGraph::Dijkstra(next);
		min_dist = numeric_limits<double>::max();
		for (CVertex* c : visits.m_Vertices) {
			if (min_dist > c->m_DijkstraDistance) {
				min_dist = c->m_DijkstraDistance;
				next = c;
			}
			
		}
		visits.m_Vertices.remove(next);
		next->m_pDijkstraPrevious = track.m_Vertices.back();
		track.AddLast(next);

	}
	track.AddLast(aux);
	return track;


	

}
