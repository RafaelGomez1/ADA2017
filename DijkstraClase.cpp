#include "stdafx.h"
#include "CGraph.h"


// =============================================================================
// Dijkstra ====================================================================
// =============================================================================

void CGraph::Dijkstra(CVertex *pStart)
{
	for(CVertex &v:m_Vertices) {
		v.m_DijkstraDistance= numeric_limits<double>::max();
		v.m_pDijkstraPrevious = NULL;

	}
	m_Vertices.begin.m_DijkstraDistance = 0;
	CVertex *current;
	double distance;
	int min_distance= numeric_limits<double>::max();
	current = m_Vertices.begin;
	while (current=primer elemento con un false de la lista) {
		if (!current->m_DijkstraVisit) {
			current->m_DijkstraVisit = 1;
			min_distance= numeric_limits<double>::max();
			for each(apuntador=current.neighbor) {
				distance = Euclidiana(apuntador.apuntador->previo) + apuntador->m_previous->m_distance;
				if (distance<min_distance)
				if (distance < apuntador->m - dijstkra dstance)
					apuntador->m_dijsktra_distance = distance;
		
			}
			

			
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
