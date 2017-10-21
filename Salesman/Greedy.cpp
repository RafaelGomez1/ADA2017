
#include "stdafx.h"
#include "CGraph.h"

// SalesmanTrackGreedy =========================================================

CTrack CGraph::SalesmanTrackGreedy(CVisits &visits)
{
	//Some pointers
	CVertex* current = visits.m_Vertices.front();
	CVertex* last_one = visits.m_Vertices.back();
	CVertex* aux;
	CVertex* next;

	//Auxiliar for the minimum distance of the visits elements at each iteration.
	double min_dist;

	//Needed Tracks structures
	CTrack partial_track(this);
	CTrack track(this);

	//Deletion of the last element
	visits.m_Vertices.pop_back();
	visits.m_Vertices.pop_front();


	// Addition of the first element to main track
	track.AddFirst(current);
	
	//Main loop
	CGraph::Dijkstra(current);
	while (!visits.m_Vertices.empty()) {
		aux = current;
		min_dist = numeric_limits<double>::max();
		
		//Calculation of closest vertex
		for (CVertex* c : visits.m_Vertices) {

			if (min_dist > c->m_DijkstraDistance) {
				min_dist = c->m_DijkstraDistance;
				current = c;
			}

		}
		
		visits.m_Vertices.remove(current);
		next = current;
		//Generation of partial track between new vertex and the previous one.
		while (current->m_pDijkstraPrevious != aux) {

			partial_track.AddFirst(current);
			current = current->m_pDijkstraPrevious;
		}

		partial_track.AddFirst(current);
		track.Append(partial_track);
		partial_track.Clear();
		CGraph::Dijkstra(next);
		current = next;

	}

	//append the last element of the visits list.
	while (last_one->m_pDijkstraPrevious != current) {

		partial_track.AddFirst(last_one);
		last_one = last_one->m_pDijkstraPrevious;
	}
	partial_track.AddFirst(last_one);
	track.Append(partial_track);

	return track;

}

