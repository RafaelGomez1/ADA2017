#include "stdafx.h"
#include "CGraph.h"


// =============================================================================
// Dijkstra ====================================================================
// =============================================================================

void CGraph::Dijkstra(CVertex *pStart)
{
	for (CVertex &v : m_Vertices) {

		v.m_DijkstraDistance = numeric_limits<double>::max();
		v.m_DijkstraVisit = false;
	}

	CVertex* current = pStart;
	current->m_DijkstraDistance = 0;

	do
	{
		for (CVertex* v : current->m_Neighbords) {
			double dist = current->m_DijkstraDistance + (v->m_Point - current->m_Point).Module();
			if (dist < v->m_DijkstraDistance) {
				v->m_DijkstraDistance = dist;
				v->m_pDijkstraPrevious = current;

			}
		}

		current->m_DijkstraVisit = true;
		current = NULL;
		for (CVertex &v : m_Vertices) {

				if (!v.m_DijkstraVisit && (current == NULL || v.m_DijkstraDistance < current->m_DijkstraDistance))
					current = &v;
		}
	} while (current != NULL);

}


// =============================================================================
// DijkstraQueue ===============================================================
// =============================================================================

void CGraph::DijkstraQueue(CVertex *pStart)
{

	auto compare = [](CVertex a, CVertex b) { return a.m_DijkstraDistance > b.m_DijkstraDistance; };
	priority_queue <CVertex, vector<CVertex>, decltype(compare)> pq(compare);

	for (CVertex &v : m_Vertices) {

		v.m_DijkstraDistance = numeric_limits<double>::max();
		v.m_DijkstraVisit = false;
		if (pStart->m_DijkstraDistance != 0)
			pStart->m_DijkstraDistance = 0;
		pq.push(v);
	}


	while (!pq.empty()) {

		CVertex current = pq.top();
		pq.pop();

		for (CVertex* v : current.m_Neighbords) {
			if (!v->m_DijkstraVisit) {
				double dist = current.m_DijkstraDistance + (v->m_Point - current.m_Point).Module();
				if (dist < v->m_DijkstraDistance) {
					v->m_DijkstraDistance = dist;
					pq.push(*v);
				}
			}
		}

	}
}
