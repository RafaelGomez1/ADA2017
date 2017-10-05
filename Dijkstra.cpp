#include "stdafx.h"
#include "CGraph.h"


// =============================================================================
// Dijkstra ====================================================================
// =============================================================================


//Primer punto equivale al vertice anterior(m_dijkstraPrevious)
//Retorna la distancia total hasta el nuevo vertice, equivalente a sumar la distancia
//en el vertice anterior y ademas la nueva distancia euclidea hasta el nuevo vertice.
int calc_distance(CVertex *vertex1,CVertex *vertex2 ) {
	int sum = 0;
	sum += vertex1->m_DijkstraDistance;
	sum += sqrt(pow(vertex1->m_Point.m_X- vertex2->m_Point.m_X,2) + pow(vertex1->m_Point.m_Y - vertex2->m_Point.m_Y,2));
	return sum;
}
bool sorting(CVertex *a, CVertex *b) {
	return a->m_DijkstraDistance < b->m_DijkstraDistance;
}

void CGraph::Dijkstra(CVertex *pStart)
{
	int distancia = 0;
	for (CVertex &v : m_Vertices) {
		v.m_DijkstraDistance = numeric_limits <double>::max();
		v.m_pDijkstraPrevious = NULL;
	}
	m_Vertices.begin.m_DijkstraDistance = 0;
	for (CVertex &v :m_Vertices){
		v.m_DijkstraVisit = 1;
		for (CVertex *y : v.m_Neighbords) {
			if (distancia=calc_distance(&v, y) < y->m_DijkstraDistance) {
				y->m_DijkstraDistance = distancia;

			}
		}
	}

}

// =============================================================================
// DijkstraQueue ===============================================================
// =============================================================================




void CGraph::DijkstraQueue(CVertex *pStart)
{
	for (CVertex &v : m_Vertices) {
		v.m_DijkstraDistance = numeric_limits<double>::max();
		v.m_pDijkstraPrevious = NULL;

	}
	m_Vertices.begin.m_DijkstraDistance = 0;
	CVertex *current;
	
	list <CVertex *> temporal;
	temporal.push_front(m_Vertices.begin);
	double distance;
	int falta_visitar = m_Vertices.size();
	double min_distance = numeric_limits<double>::max();
	current = m_Vertices.begin;
	while (falta_visitar>0) {
		min_distance = numeric_limits<double>::max();
		if (!current->m_DijkstraVisit) {
			current->m_DijkstraVisit = 1;
			falta_visitar -= 1;
			temporal.pop_front();

			for (CVertex *iter : current->m_Neighbords)
			{
				distance = calc_distance(iter->m_pDijkstraPrevious, iter);
				iter->m_DijkstraDistance = distance;
				if (distance < min_distance) {
					min_distance = distance;
					iter->m_DijkstraDistance = distance;

				}
				temporal.push_back(iter);
			}
			temporal.sort(sorting);



		}
		current = temporal.front();

	}

	
}
