#ifndef Snap_Cliques
#define Snap_Cliques

#include "Snap.h"

/////////////////////////////////////////////////
// Clique Percolation Method for Overlapping community detection
class TCliqueOverlap {
private: //Recursion variables only
	PUNGraph m_G;
	TIntV m_Q;
	TVec<TIntV>* m_maxCliques;
	TVec<TIntV>* m_Cliques;
	int m_minMaxCliqueSize;
	int m_CliqueSize;
private:
	void GetNbrs(int NId, THashSet<TInt>& Nbrs) const;
	int GetNodeIdWithMaxDeg(const THashSet<TInt>& Set) const;
	int MaxNbrsInCANDNodeId(const THashSet<TInt>& SUBG, const THashSet<TInt>& CAND) const;
private:
	// maximal = true, 计算maximal clique， maximal = false 计算指定size大小的clique
	// void Expand(const THashSet<TInt>& SUBG, THashSet<TInt>& CAND, bool maximal = true, int leval = 0);
	void Expand(TIntV last_R, const THashSet<TInt>& SUBG, THashSet<TInt>& CAND, bool maximal = true, int leval = 0);
public:
	static void GetRelativeComplement(const THashSet<TInt>& A, const THashSet<TInt>& B, THashSet<TInt>& Complement);
	static void GetIntersection(const THashSet<TInt>& A, const THashSet<TInt>& B, THashSet<TInt>& C);
	static int Intersection(const THashSet<TInt>& A, const THashSet<TInt>& B);
	static void CalculateOverlapMtx(const TVec<TIntV>& MaxCliques, int MinNodeOverlap, TVec<TIntV>& OverlapMtx);
	static PUNGraph CalculateOverlapMtx(const TVec<TIntV>& MaxCliques, int MinNodeOverlap);
	static void GetOverlapCliques(const TVec<TIntV>& OverlapMtx, int MinNodeOverlap, TVec<TIntV>& CliqueIdVV);
	static void GetOverlapCliques(const TVec<TIntV>& OverlapMtx, const TVec<TIntV>& MaxCliques, double MinOverlapFrac, TVec<TIntV>& CliqueIdVV);
public:
	TCliqueOverlap() : m_G(), m_Q(), m_maxCliques(NULL), m_minMaxCliqueSize(3), m_Cliques(NULL), m_CliqueSize(3) { }
	//Bron-Kerbosch 算法 https://oi-wiki.org/graph/max-clique/ https://www.jianshu.com/p/437bd6936dad
	void GetMaximalCliques(const PUNGraph& G, int MinMaxCliqueSize, TVec<TIntV>& MaxCliques);

	//void GetMaximalCliquesWithCand(const PUNGraph& G, int MinMaxCliqueSize, TVec<TIntV>& MaxCliques, TIntH CandMaintain);

	void GetCliques(const PUNGraph& G, int CliqueSize, TVec<TIntV>& Cliques);
	/// Enumerate maximal cliques of the network on more than MinMaxCliqueSize nodes
	static void GetMaxCliques(const PUNGraph& G, int MinMaxCliqueSize, TVec<TIntV>& MaxCliques);
	/// Enumerate maximal cliques of the network on more than MinMaxCliqueSize nodes and label which node is seleted in CandMaintain
	//static void GetMaxCliquesWithCand(const PUNGraph& G, int MinMaxCliqueSize, TVec<TIntV>& MaxCliques, TIntH CandMaintain);
	/// Clique Percolation method communities
	static void GetCPMCommunities(const PUNGraph& G, int MinMaxCliqueSize, TVec<TIntV>& Communities);
	/// Enumerate  cliques of the network which size of CliqueSize nodes
	static void GetCliquesSizeof(const PUNGraph& G, int CliqueSize, TVec<TIntV>& Cliques);
};

#endif
