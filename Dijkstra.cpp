#include "stdafx.h"
#include "CGraph.h"


// =============================================================================
// Dijkstra ====================================================================
// =============================================================================



void CGraph::Dijkstra(CVertex *pStart)
{
	int distEuclidiana(CVertex *vertex1,CVertex *vertex2) {
		

	}
	for(CVertex &v:m_Vertices) {
		v.m_DijkstraDistance= numeric_limits<double>::max();
		v.m_pDijkstraPrevious = NULL;

	}
	m_Vertices.begin.m_DijkstraDistance = 0;
	CVertex *current;
	CVertex *auxiliar;
	CVertex *neighbor;
	double distance;
	int min_distance= numeric_limits<double>::max();
	current = m_Vertices.begin;
	while (&current != m_Vertices.end) {
		if (!current->m_DijkstraVisit) {
			current->m_DijkstraVisit = 1;
			min_distance= numeric_limits<double>::max();
			list<CVertex*>::iterator iter;

			iter = current->m_Neighbords.begin();

			while (iter != current->m_Neighbords.end())
			{

				distance =iter.m_pDijkstraPrevious->m_Point.m_X + apuntador->m_previous->m_distance;
				if (distance < min_distance) {

				}
				if (distance < apuntador->m - dijstkra dstance)
					apuntador->m_dijsktra_distance = distance;
		
			}
			iter++;
			

			
		}
	}

	
	/*
	PSEUDOCODE:
	
	
	*/
}

// =============================================================================
// DijkstraQueue ===============================================================
// =============================================================================

void CGraph::DijkstraQueue(CVertex *pStart)
{
	/*
	PSEUDOCODE:

	*/
}
