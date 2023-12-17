#include "stdafx.h"
#include "cliques.h"
#include <iostream>

/////////////////////////////////////////////////
// TCommunity implementation
void TCliqueOverlap::GetRelativeComplement(const THashSet<TInt>& A, const THashSet<TInt>& B, THashSet<TInt>& Complement) {
	for (THashSet<TInt>::TIter it = A.BegI(); it < A.EndI(); it++) {
		const int nId = it.GetKey();
		if (!B.IsKey(nId)) Complement.AddKey(nId);
	}
}

void TCliqueOverlap::GetIntersection(const THashSet<TInt>& A, const THashSet<TInt>& B, THashSet<TInt>& C) {
	if (A.Len() < B.Len()) {
		for (THashSetKeyI<TInt> it = A.BegI(); it < A.EndI(); it++)
			if (B.IsKey(it.GetKey())) C.AddKey(it.GetKey());
	}
	else {
		for (THashSetKeyI<TInt> it = B.BegI(); it < B.EndI(); it++)
			if (A.IsKey(it.GetKey())) C.AddKey(it.GetKey());
	}
}

int TCliqueOverlap::Intersection(const THashSet<TInt>& A, const THashSet<TInt>& B) {
	int n = 0;
	if (A.Len() < B.Len()) {
		for (THashSetKeyI<TInt> it = A.BegI(); it < A.EndI(); it++)
			if (B.IsKey(it.GetKey())) n++;
	}
	else {
		for (THashSetKeyI<TInt> it = B.BegI(); it < B.EndI(); it++)
			if (A.IsKey(it.GetKey())) n++;
	}
	return n;
}

void TCliqueOverlap::GetNbrs(int NId, THashSet<TInt>& Nbrs) const {
	TUNGraph::TNodeI node = m_G->GetNI(NId);
	int deg = node.GetDeg();
	for (int i = 0; i < deg; i++) Nbrs.AddKey(node.GetNbrNId(i));
}

int TCliqueOverlap::GetNodeIdWithMaxDeg(const THashSet<TInt>& Set) const {
	int id = -1;
	int maxDeg = -1;
	//
	for (THashSetKeyI<TInt> it = Set.BegI(); it < Set.EndI(); it++) {
		int nId = it.GetKey();
		int deg = m_G->GetNI(nId).GetDeg();
		if (maxDeg < deg) { maxDeg = deg; id = nId; }
	}
	return id;
}

int TCliqueOverlap::MaxNbrsInCANDNodeId(const THashSet<TInt>& SUBG, const THashSet<TInt>& CAND) const {
	int id = -1;
	int maxIntersection = -1;
	//
	for (THashSetKeyI<TInt> it = SUBG.BegI(); it < SUBG.EndI(); it++) {
		int nId = it.GetKey();
		TUNGraph::TNodeI nIt = m_G->GetNI(nId);
		int deg = nIt.GetDeg();
		//
		int curIntersection = 0;
		for (int i = 0; i < deg; i++) {
			int nbrId = nIt.GetNbrNId(i);
			if (CAND.IsKey(nbrId)) curIntersection++;
		}
		//
		if (maxIntersection < curIntersection) { maxIntersection = curIntersection; id = nId; }
	}
	return id;
}

void TCliqueOverlap::Expand(TIntV last_R, const THashSet<TInt>& SUBG, THashSet<TInt>& CAND, bool maximal, int level) {

	/*THashSet<TInt> CAND = CAND;*/
	if (maximal == false) {
		//printf("m_Q.Len : %d\n", m_Q.Len());
		if (last_R.Len() == m_CliqueSize) {
			//printf("%d", 1);
			 m_Cliques->Add(last_R);
			return;
		}

	}

	if (SUBG.Len() == 0) {
		
		if (maximal == true) {
			if (last_R.Len() >= m_minMaxCliqueSize) {
				#pragma omp critical
				{
					 m_maxCliques->Add(last_R);
				}
				
				/*for (TIntV::TIter TI = m_Q.BegI(); TI < m_Q.EndI(); TI++)
				{
					std::cout << *TI << " ";
				}
				std::cout << std::endl;*/
			}
		}
		return;
	}


	if (CAND.Len() == 0) return;
	//Get u that maximaze CAND intersection with neighbours of vertex u
	int u = MaxNbrsInCANDNodeId(SUBG, CAND);
	//Get neighbours of node u
	THashSet<TInt> nbrsU;
	GetNbrs(u, nbrsU);
	//Get relative complement of nbrsU in CAND
	THashSet<TInt> EXT;
	GetRelativeComplement(CAND, nbrsU, EXT);

#pragma omp parallel for 
	for (int i = 0; i < EXT.Len(); i++) {

	/*}
	for (THashSet<TInt>::TIter TI = EXT.BegI(); TI < EXT.EndI(); TI++) {*/
		int q = EXT.GetKey(i);
		/*int q = TI.GetKey();*/
		TIntV R = last_R;


		R.Add(q);
		
		THashSet<TInt> nbrsQ;
		GetNbrs(q, nbrsQ);

		THashSet<TInt> SUBGq;
		GetIntersection(SUBG, nbrsQ, SUBGq);

		THashSet<TInt> CANDq;
		GetIntersection(CAND, nbrsQ, CANDq);

		Expand(R, SUBGq, CANDq, maximal, level + 1);

		#pragma omp critical
		{
			CAND.DelKey(q);
		}

		
		
		/*m_Q.DelLast();*/

	}

}

void TCliqueOverlap::GetMaximalCliques(const PUNGraph& G, int MinMaxCliqueSize, TVec<TIntV>& MaxCliques) {
	if (G->GetNodes() == 0) return;
	//
	m_G = G;
	m_minMaxCliqueSize = MinMaxCliqueSize;
	m_maxCliques = &MaxCliques;
	m_Q.Clr();
	//
	THashSet<TInt> SUBG;
	THashSet<TInt> CAND;
	for (TUNGraph::TNodeI NI = m_G->BegNI(); NI < m_G->EndNI(); NI++) {
		TInt nId = NI.GetId();
		SUBG.AddKey(nId);
		CAND.AddKey(nId);
	}
	//
	Expand(m_Q, SUBG, CAND);
}

//void TCliqueOverlap::GetMaximalCliquesWithCand(const PUNGraph& G, int MinMaxCliqueSize, TVec<TIntV>& MaxCliques, TIntH CandMaintain) {
//	if (G->GetNodes() == 0) return;
//	//
//	m_G = G;
//	m_minMaxCliqueSize = MinMaxCliqueSize;
//	m_maxCliques = &MaxCliques;
//	m_Q.Clr();
//	//
//	THashSet<TInt> SUBG;
//	THashSet<TInt> CAND;
//	for (TUNGraph::TNodeI NI = m_G->BegNI(); NI < m_G->EndNI(); NI++) {
//		TInt nId = NI.GetId();
//		SUBG.AddKey(nId);
//		CAND.AddKey(nId);
//	}
//	//
//	Expand(SUBG, CAND);
//	for()
//}

void TCliqueOverlap::GetCliques(const PUNGraph& G, int CliqueSize, TVec<TIntV>& Cliques) {
	if (G->GetNodes() == 0) return;
	//
	m_G = G;
	m_CliqueSize = CliqueSize;
	m_Cliques = &Cliques;
	m_Q.Clr();
	//
	THashSet<TInt> SUBG;
	THashSet<TInt> CAND;
	for (TUNGraph::TNodeI NI = m_G->BegNI(); NI < m_G->EndNI(); NI++) {
		TInt nId = NI.GetId();
		SUBG.AddKey(nId);
		CAND.AddKey(nId);
	}
	//
	Expand(m_Q, SUBG, CAND, false);
}

void TCliqueOverlap::CalculateOverlapMtx(const TVec<TIntV>& MaxCliques, int MinNodeOverlap, TVec<TIntV>& OverlapMtx) {
	OverlapMtx.Clr();
	//
	int n = MaxCliques.Len();
	//Convert max cliques to HashSets
	TVec<THashSet<TInt> > cliques;
	for (int i = 0; i < n; i++) {
		const int len = MaxCliques[i].Len();
		cliques.Add();
		if (len < MinNodeOverlap) { continue; }
		THashSet<TInt>& set = cliques.Last();  set.Gen(len);
		for (int j = 0; j < len; j++) { set.AddKey(MaxCliques[i][j]); }
	}
	//Init clique clique overlap matrix
	n = cliques.Len();
	OverlapMtx.Gen(n);
	for (int i = 0; i < n; i++) OverlapMtx[i].Gen(n);
	//Calculate clique clique overlap matrix
	for (int i = 0; i < n; i++) {
		OverlapMtx[i][i] = cliques[i].Len();
		for (int j = i + 1; j < n; j++) {
			OverlapMtx[i][j] = Intersection(cliques[i], cliques[j]);
		}
	}
}

PUNGraph TCliqueOverlap::CalculateOverlapMtx(const TVec<TIntV>& MaxCliques, int MinNodeOverlap) {
	const int n = MaxCliques.Len();
	//Convert max cliques to HashSets
	TVec<THashSet<TInt> > cliques;
	for (int i = 0; i < n; i++) {
		const int len = MaxCliques[i].Len();
		cliques.Add();
		if (len < MinNodeOverlap) { continue; }
		THashSet<TInt>& set = cliques.Last();  set.Gen(len);
		for (int j = 0; j < len; j++) { set.AddKey(MaxCliques[i][j]); }
	}
	//Init clique clique overlap matrix
	PUNGraph OverlapMtx = TUNGraph::New();
	for (int i = 0; i < n; i++) {
		OverlapMtx->AddNode(i);
	}
	//Calculate clique clique overlap matrix
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (Intersection(cliques[i], cliques[j]) >= MinNodeOverlap) {
				OverlapMtx->AddEdge(i, j);
			}
		}
	}
	return OverlapMtx;
}

void TCliqueOverlap::GetOverlapCliques(const TVec<TIntV>& OverlapMtx, int MinNodeOverlap, TVec<TIntV>& CliqueIdVV) {
	int n = OverlapMtx.Len();
	for (int i = 0; i < n; i++) {
		bool isCommunity = false;
		for (int j = i + 1; j < n; j++) {
			if (OverlapMtx[i][j] >= MinNodeOverlap) {
				if (!isCommunity) {
					TIntV v; v.Add(i);
					CliqueIdVV.Add(v);
					isCommunity = true;
				}
				CliqueIdVV.Last().Add(j);
			}
		}
	}
}

void TCliqueOverlap::GetOverlapCliques(const TVec<TIntV>& OverlapMtx, const TVec<TIntV>& MaxCliques, double MinOverlapFrac, TVec<TIntV>& CliqueIdVV) {
	int n = OverlapMtx.Len();
	for (int i = 0; i < n; i++) {
		bool isCommunity = false;
		int size1 = MaxCliques[i].Len();
		for (int j = i + 1; j < n; j++) {
			int size2 = MaxCliques[j].Len();
			double ratio = OverlapMtx[i][j];
			if (size1 < size2) ratio /= size1;
			else ratio /= size2;
			if (ratio >= MinOverlapFrac) {
				if (!isCommunity) {
					TIntV v; v.Add(i);
					CliqueIdVV.Add(v);
					isCommunity = true;
				}
				CliqueIdVV.Last().Add(j);
			}
		}
	}
}

/// Enumerate maximal cliques of the network on more than MinMaxCliqueSize nodes
void TCliqueOverlap::GetMaxCliques(const PUNGraph& G, int MinMaxCliqueSize, TVec<TIntV>& MaxCliques) {
	TCliqueOverlap CO;
	MaxCliques.Clr(false);
	CO.GetMaximalCliques(G, MinMaxCliqueSize, MaxCliques);
}

//void TCliqueOverlap::GetMaxCliquesWithCand(const PUNGraph& G, int MinMaxCliqueSize, TVec<TIntV>& MaxCliques, TIntH CandMaintain)
//{
//	TCliqueOverlap CO;
//	MaxCliques.Clr(false);
//	CO.GetMaximalCliques(G, MinMaxCliqueSize, MaxCliques);
//}

/// Clique Percolation method communities
void TCliqueOverlap::GetCPMCommunities(const PUNGraph& G, int MinMaxCliqueSize, TVec<TIntV>& NIdCmtyVV) {
	printf("Clique Percolation Method\n");
	TExeTm ExeTm;
	TVec<TIntV> MaxCliques;
	TCliqueOverlap::GetMaxCliques(G, MinMaxCliqueSize, MaxCliques);
	// op RS 2012/05/15, commented out next line, a parameter is missing,
	//   creating a warning on OS X
	// printf("...%d cliques found\n");
	// get clique overlap matrix (graph)
	PUNGraph OverlapGraph = TCliqueOverlap::CalculateOverlapMtx(MaxCliques, MinMaxCliqueSize - 1);
	printf("...overlap matrix (%d, %d)\n", G->GetNodes(), G->GetEdges());
	// connected components are communities
	TCnComV CnComV;
	TSnap::GetWccs(OverlapGraph, CnComV);
	NIdCmtyVV.Clr(false);
	TIntSet CmtySet;
	for (int c = 0; c < CnComV.Len(); c++) {
		CmtySet.Clr(false);
		for (int i = 0; i < CnComV[c].Len(); i++) {
			const TIntV& CliqueNIdV = MaxCliques[CnComV[c][i]];
			CmtySet.AddKeyV(CliqueNIdV);
		}
		NIdCmtyVV.Add();
		CmtySet.GetKeyV(NIdCmtyVV.Last());
		NIdCmtyVV.Last().Sort();
	}
	printf("done [%s].\n", ExeTm.GetStr());
}

void TCliqueOverlap::GetCliquesSizeof(const PUNGraph& G, int CliqueSize, TVec<TIntV>& Cliques) {

	////时间复杂度O(n^k)
	//TVec<TIntV> cliques;
	//TIntV tmpV;
	////2-cliques
	//for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++) {
	//	//std::cout << EI.GetSrcNId() <<" " << EI.GetDstNId() << std::endl;
	//	tmpV.Clr();
	//	tmpV.Add(EI.GetSrcNId());
	//	tmpV.Add(EI.GetDstNId());
	//	cliques.Add(tmpV);
	//	
	//}
	//int k = 2;
	//if (CliqueSize == 2) {
	//	Cliques = cliques;
	//	return;
	//}

	//while (!cliques.Empty()) {  //O(k)

	//	//yield k,cliques
	//	
	//	//merge k-cliques into (k+1)-cliques

	//	TVec<TIntV> cliques_1;

	//	k += 1;

	//	//O(n^2)
	//	for (TVec<TIntV>::TIter TI1 = cliques.BegI(); TI1 < cliques.EndI(); TI1++) {
	//		for (TVec<TIntV>::TIter TI2 = TI1; TI2 < cliques.EndI(); TI2++) {
	//			if (TI1 == TI2) continue;
	//			TIntV intersection, difference;
	//			//找交集 O(TI1*TI2)  怎么优化？
	//			TIntH tmp;
	//			TIntV cq;
	//			for (TIntV::TIter TI = TI1->BegI(); TI < TI1->EndI(); TI++) {
	//				tmp.AddDat(*TI, 1);
	//			}
	//			for (TIntV::TIter TI = TI2->BegI(); TI < TI2->EndI(); TI++) {
	//				//判断是否存在Key
	//				if (tmp.IsKey(*TI)) {
	//					tmp.GetDat(*TI)++;
	//				}
	//				else {
	//					tmp.AddDat(*TI, 1);
	//				}					
	//			}
	//			for (TIntH::TIter HI = tmp.BegI(); HI < tmp.EndI(); HI++) {
	//				if (HI.GetDat() == 1) {
	//					difference.Add(HI.GetKey());
	//				}						
	//			}

	//			if (difference.Len() == 2 && G->IsEdge(difference[0], difference[1])) {
	//				cq.AddV(*TI1);
	//				cq.AddVMerged(difference);			
	//				cliques_1.Add(cq);
	//			}
	//		}
	//	}
	//	
	//	//TODO:

	//	cliques = cliques_1;
	//	cliques.Merge();

	//	if (k == CliqueSize) {
	//		Cliques = cliques;
	//		return;
	//	}
	//	
	//}
	//printf("Do Not Exist k-cliques!!!\n");
	//return;
		
	TCliqueOverlap CO;
	Cliques.Clr(false);
	CO.GetCliques(G, CliqueSize, Cliques);
}
