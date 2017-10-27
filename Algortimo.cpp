#include "stdafx.h"
#include "CGraph.h"


// =============================================================================
// SpanningTreePrim ============================================================
// =============================================================================

CVertex CGraph::mininumValueByKey(list <CVertex> list) {
	CVertex minimumValueByKey;
	int min_key = numeric_limits<int>::max();
	for (CVertex current : list) {
		if (current.key < min_key) {
			min_key = current.key;
			minimumValueByKey = current;
		}


	}
	return minimumValueByKey;
}
boolean CGraph::notInList(list <CVertex> list, CVertex* containsThisValue) {
	for (CVertex current: list) {
		if (&current == containsThisValue) {
			return true;
		}
			
	}
	return false;
}


CSpanningTree CGraph::SpanningTreePrim()
{
	
	CSpanningTree edges_prim(this);
	int count = 0;
	for (CVertex &each_vertex :m_Vertices) {
		each_vertex.m_Saved = false;
		each_vertex.key = 9999999;
		each_vertex.parent = NULL;
		count++;
	}
	double calc_distance;
	CVertex* most_near;
	m_Vertices.front().key = 0;
	for (int i = 0; i < GetNVertex(); i++) {
		double min_key = 999999999;
		for (CVertex &current : m_Vertices) {
			if (current.m_Saved == false && current.key < min_key) {
				min_key = current.key;
				most_near = &current;
			}
		}
		most_near->m_Saved = true;
		if (most_near->parent != NULL) {
			CEdge new_edge(most_near, most_near->parent);
			edges_prim.m_Edges.push_back(new_edge);
		}if(most_near->m_Neighbords.size()>0){
		for (CVertex *neighbor : most_near->m_Neighbords) {
			if (neighbor->m_Saved == false && neighbor->m_Point.Distance(most_near->m_Point) < neighbor->key) {
				neighbor->parent = most_near;
				neighbor->key = neighbor->m_Point.Distance(most_near->m_Point);

			}
		}
		}
	

	}
	return edges_prim;
}

// =============================================================================
// SpanningTreeKruskal =========================================================
// =============================================================================
CVertex* CGraph::Find(CVertex *children) {
	if (children->parent == children)
		return children->parent;
	else {
		return Find(children->parent);
	}
}

CSpanningTree CGraph::SpanningTreeKruskal()
{
	auto compare = [](CEdge a, CEdge b) { return a.m_Length > b.m_Length; };
	priority_queue <CEdge, vector<CEdge>, decltype(compare)> all_edges(compare);
	CSpanningTree kruskal_tree(this);

	int counter = 0;
	for (CVertex &init_each_vertex : m_Vertices) {
		init_each_vertex.class_id = counter;
		counter++;
		init_each_vertex.parent = &init_each_vertex;
	}

	for (CVertex &each_vertex : m_Vertices) {
		for (CVertex *neighbord : each_vertex.m_Neighbords) {
			CEdge new_edge(&each_vertex, neighbord);
			all_edges.push(new_edge);
		}
	}

	
	
	all_edges.pop();
	while (!all_edges.empty()) {
		CEdge each_edge(all_edges.top());
		CVertex *first_extreme = Find(each_edge.m_pV1);
		CVertex *second_extreme = Find(each_edge.m_pV2);

		if (first_extreme->class_id != second_extreme->class_id) {
			kruskal_tree.m_Edges.push_back(each_edge);
			if (first_extreme->class_id > second_extreme->class_id) {
				second_extreme->parent = first_extreme;
			}
			else if (second_extreme->class_id > first_extreme->class_id) {
					first_extreme->parent = second_extreme;
			}
		else {
			first_extreme->parent = second_extreme;
			second_extreme->class_id++;

		}

		}
		all_edges.pop();
}
	
	return kruskal_tree;
} 
