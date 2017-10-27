#pragma once


// =============================================================================
// CGPoint =====================================================================
// =============================================================================

class CGPoint
{

public:
	double m_X, m_Y;

	CGPoint() {}
	CGPoint(const double x, const double y = 0) : m_X(x), m_Y(y) {}
	CGPoint(const CGPoint&p) : m_X(p.m_X), m_Y(p.m_Y) {}
	CGPoint(const CPoint &p) : m_X(p.x), m_Y(p.y) {}
	operator CPoint() { return CPoint(int(m_X), int(m_Y)); }
	CGPoint& operator=(const CGPoint &p) {
		m_X = p.m_X;
		m_Y = p.m_Y;
		return *this;
	}
	CGPoint operator+(const CGPoint &p2)  const {
		return CGPoint(m_X + p2.m_X, m_Y + p2.m_Y);
	}
	CGPoint operator+=(const CGPoint &p2) {
		m_X += p2.m_X;
		m_Y += p2.m_Y;
		return *this;
	}
	CGPoint operator-(const CGPoint &p2)  const {
		return CGPoint(m_X - p2.m_X, m_Y - p2.m_Y);
	}
	CGPoint operator-=(const CGPoint &p2) {
		m_X -= p2.m_X;
		m_Y -= p2.m_Y;
		return *this;
	}
	CGPoint operator*=(const double n) {
		m_X *= n;
		m_Y *= n;
		return *this;
	}
	CGPoint operator/=(const double n) {
		m_X /= n;
		m_Y /= n;
		return *this;
	}
	CGPoint operator-()  const {
		return CGPoint(-m_X, -m_Y);
	}
	double operator*(const CGPoint &p2)  const {
		return m_X*p2.m_X + m_Y*p2.m_Y;
	}
	bool operator==(const CGPoint& p)  const {
		return m_X == p.m_X && m_Y == p.m_Y;
	}
	bool operator!=(const CGPoint& p)  const {
		return m_X != p.m_X || m_Y != p.m_Y;
	}
	double Module() { return sqrt(m_X*m_X + m_Y*m_Y); }
	double Distance(CGPoint &p2) { return sqrt((m_X-p2.m_X)*(m_X - p2.m_X)+ (m_Y - p2.m_Y)*(m_Y - p2.m_Y)); }
};

inline CGPoint operator*(double v, const CGPoint& p) {
	return CGPoint(v*p.m_X, v*p.m_Y);
}

inline CGPoint operator*(const CGPoint& p, double v) {
	return CGPoint(v*p.m_X, v*p.m_Y);
}

inline CGPoint operator/(const CGPoint& p, double v) {
	return CGPoint(p.m_X / v, p.m_Y / v);
}

inline ostream& operator<< (ostream& s, const CGPoint& p) {
	s << "(" << p.m_X << ", " << p.m_Y << ")";
	return s;
}

inline CGPoint Min(const CGPoint& p1, const CGPoint& p2) {
	return CGPoint(min(p1.m_X, p2.m_X), min(p1.m_Y, p2.m_Y));
}

inline CGPoint Max(const CGPoint& p1, const CGPoint& p2) {
	return CGPoint(max(p1.m_X, p2.m_X), max(p1.m_Y, p2.m_Y));
}


// =============================================================================
// CVertex =====================================================================
// =============================================================================

class CVertex {
public:
	CGPoint m_Point;
	int class_id;
	CVertex* parent;
	int key;
	list<CVertex*> m_Neighbords;
	bool m_Saved;

public:
	CVertex(double x, double y) : m_Point(x, y) {}
	boolean operator==(CVertex compare) { return m_Point != compare.m_Point; }
	CVertex() {}
	bool NeighbordP(CVertex *pVertex);
	
};

// =============================================================================
// CGraph ======================================================================
// =============================================================================

class CSpanningTree;

class CGraph
{
public:
	list<CVertex> m_Vertices; 
public:
	CGraph();
	~CGraph();
	void Clear();
	CVertex* Find(CVertex* children);
	boolean notInList(list <CVertex> all_vertex, CVertex* containThisVertex);
	CVertex mininumValueByKey(list <CVertex> list);
	CVertex* FindVertex(double x, double y);
	CVertex* FindVertex(const CGPoint &p) { return FindVertex(p.m_X, p.m_Y); }
	CVertex* GetVertex(double x, double y);
	CVertex* GetVertex(const CGPoint &p) { return GetVertex(p.m_X, p.m_Y);  }
	CVertex* GetVertex(int indice);
	int GetNVertex();
	bool MemberP(CVertex *pVertex);
	void AddEdge(double x1, double y1, double x2, double y2);
	void Write(const char* filename);
	void Read(const char* filename);
	void CrearAleatorio(int nVertices, int nEdges);
	CSpanningTree SpanningTreePrim();
	CSpanningTree SpanningTreeKruskal();
};

// =============================================================================
// CEdge =======================================================================
// =============================================================================

class CEdge {
public:
	CVertex *m_pV1;
	CVertex *m_pV2;
	double m_Length;
public:
	CEdge(CVertex *pV1, CVertex *pV2) {
		m_pV1 = pV1;
		m_pV2 = pV2;
		m_Length = pV1->m_Point.Distance(pV2->m_Point);
	}
	CEdge(const CEdge &e) {
		m_pV1 = e.m_pV1;
		m_pV2 = e.m_pV2;
		m_Length = e.m_Length;
	}
};

// =============================================================================
// CSpanningTree ===============================================================
// =============================================================================

class CSpanningTree {
public:
	list<CEdge> m_Edges;
	CGraph *m_pGraph;

	CSpanningTree() { m_pGraph = NULL; }
	CSpanningTree(CGraph *pGraph) {	m_pGraph = pGraph;	}
	void SetGraph(CGraph *pGraph) {	Clear(); m_pGraph = pGraph;	}
	void Clear() { m_Edges.clear(); }
	void Write(const char* filename);
	void Read(const char* filename);
};