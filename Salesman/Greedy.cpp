#include "stdafx.h"
#include "CGraph.h"

// SalesmanTrackGreedy =========================================================

CTrack CGraph::SalesmanTrackGreedy(CVisits &visits)
{
	//Some pointers
	CVertex* current = visits.m_Vertices.front();
	CVertex* last_one = visits.m_Vertices.back();
	CVertex* del;
	CVertex* rec;

	//Auxiliar for the minimum distance of the visits elements at each iteration.
	double min_dist;

	//Needed Tracks structures
	CTrack partial_track(this);
	CTrack track(this);

	//Deletion of the last element
	visits.m_Vertices.pop_back();




	// Addition of the first element to main track
	track.AddFirst(current);
	
	//Main loop
	while (!visits.m_Vertices.empty()) {


		Dijkstra(current);
		
		//Next vertex to delete
		del = current;

		min_dist = numeric_limits<double>::max();
		

		//Calculation of closest vertex
		for (CVertex* c : visits.m_Vertices) {

			if (min_dist > c->m_DijkstraDistance) {

				min_dist = c->m_DijkstraDistance;
				current = c;

			}

		}
		//Auxiliar to keep fixed the current pointer.
		rec = current;
		
		//Generation of partial track between new vertex and the previous one.
		while (rec->m_pDijkstraPrevious != del) {

			partial_track.AddFirst(rec);
			rec = rec->m_pDijkstraPrevious;

		}

		//Its unecessaty to append the last element of the partial track because we had appended at the previous iteration.
		visits.m_Vertices.remove(del);
		track.Append(partial_track);
		partial_track.Clear();

	}

	//append of the last element of the visits list.
	track.AddLast(last_one);

	return track;

}
