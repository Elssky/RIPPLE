#include"KVCC_Algorithm.h"
#include"vector"
#include"algorithm"
#include"time.h"
#include <iostream>

VCCE::VCCE() {
}

VCCE::VCCE(PUNGraph G_, int k_){
	G = G_;
	k = k_;
	//int Mxnode = G->GetMxNId();
	//vertex_map_ = new int[Mxnode];
	//memset(vertex_map_, 0, sizeof(int) * Mxnode);
	//int i = 0;
	//for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
	//	vertex_map_[NI.GetId()] = i++;
	//}
}

TIntVIntV VCCE::KVCC_ENUM(PUNGraph & sub_G, int k) {
	//func: Find k-VCCs in Graph G
	//params:
	//	G: Undirected Graph
	//	k: an integer
	//return:
	//	all k-vertex connected components (Type: TUNGraV )


	//printf("G: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G), TSnap::CntUniqUndirEdges(G));
	//for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
	//	printf("%d\n", NI.GetId());
	//}
	//step1: initialize set VCC as empty
	TIntVIntV VCC;  // return as K-Vccs;

	//step2: remove vertices u and incident edges, that d(u) < k
	PUNGraph G2 = TSnap::GetKCore(G, k);
	//printf("G2: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G2), TSnap::CntUniqUndirEdges(G2));

	//step3: identify connected compotent set in G
	TCnComV ConV;
	TSnap::GetSccs(G2, ConV);
	//printf("1\n");

	TIntVIntV G_set; // Connected Component in G2;
	for (TCnComV::TIter I = ConV.BegI(); I < ConV.EndI(); I++) {
		//printf("%d\n", I->NIdV);
		G_set.Add(I->NIdV);
	}
	//printf("%d\n", G_set.Len());
	//printf("2\n");
	//step4: find vertex cut in G
	TIntV S; //Vertex_Cut
	//printf("G_set_len = %d\n", G_set.Len());
	for (TIntVIntV::TIter GI = G_set.BegI(); GI < G_set.EndI(); GI++) {
		//clock_t t1 = clock();
		S = Global_Cut(*GI, k, G2);
		//_time += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;
		//printf("%fs\n", (clock() - t1) * 1.0 / CLOCKS_PER_SEC);
		//m++;
		//printf("%d\n", S.Empty());
		if (S.Empty()) {

			VCC.Add(*GI);
			//printf("GI: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));
		}
		else {
			TIntVIntV G_i = Overlap_Partition(*GI, S, G2);
			//printf("%d\n", G_i.Len());
			for (TIntVIntV::TIter G_ij = G_i.BegI(); G_ij < G_i.EndI(); G_ij++) {

				
				PUNGraph temp_G = TSnap::GetSubGraph(G, *G_ij);
				TIntVIntV VCC_i = KVCC_ENUM(temp_G, k);
				//printf("%d\n", VCC_i.Len());
				for (TIntVIntV::TIter GI_j = VCC_i.BegI(); GI_j < VCC_i.EndI(); GI_j++) {

					VCC.Add(*GI_j);
					//for (TUNGraV::TIter GI = VCC.BegI(); GI < VCC.EndI(); GI++) {
					//	printf("GI: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));
					//}
				}

			}
		}

	}
	//printf("3\n");
	return VCC;
}


TIntV VCCE::Global_Cut(TIntV& subG, int k, PUNGraph All_G) {
	clock_t t1 = clock();
	//printf("%d\n", 1);
	TIntV S;
	//1. compute sparse certification SC
	PUNGraph SC = Compute_SC(subG, k, All_G);
	//2. select a source vertex u with the minimum degree
	int u = GetMnDegNId(SC);
	//3. SC_bar = Construct_DG(SC);
	PNEANet SC_bar = Construct_DG(SC);

	int e;//take place

	
	_time += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;
	//printf("%fs\n", (clock() - t1) * 1.0 / CLOCKS_PER_SEC);
	m++;
	for (TUNGraph::TNodeI NI = SC->BegNI(); NI < SC->EndNI(); NI++) {
		S = Loc_Cut(u, NI.GetId(), SC_bar, SC, k);
		if (S.Empty() == false) {
			//printf("1\n");
			return S;
		}
	}

	for (int i = 0; i < SC->GetNI(u).GetDeg(); i++) {
		for (int j = i + 1; j < SC->GetNI(u).GetDeg(); j++) {
			S = Loc_Cut(SC->GetNI(u).GetNbrNId(i), SC->GetNI(u).GetNbrNId(j), SC_bar, SC, k);
		}
	}

	//PUNGraph Neigh = TSnap::GetEgonet(SC, u, e); //N(u)
	//for (TUNGraph::TNodeI NI1 = Neigh->BegNI(); NI1 < Neigh->EndNI(); NI1++) {
	//	for (TUNGraph::TNodeI NI2 = NI1; NI2 < Neigh->EndNI(); NI2++) {
	//		S = Loc_Cut(NI1.GetId(), NI2.GetId(), SC_bar, SC, k);
	//		if (S.Empty() == false) {
	//			//printf("2\n");
	//			return S;
	//		}
	//	}
	//}
	//printf("3\n");
	return {};
}

PUNGraph VCCE::Compute_SC(TIntV subG, int k, PUNGraph All_G) {
	
	PUNGraph SC = TUNGraph::New();
	PUNGraph G_ = TSnap::GetSubGraph(All_G, subG);
	PNGraph BFSTree;
	int NodeNums = TSnap::CntNonZNodes(G_);
	std::vector<int> visited; //mark visited node
	std::vector<int> del;  //fix wrong BFS Tree
	for (int i = 0; i < k; i++) {
		visited.clear();
		//printf("F%d:\n", i);
		int j = 0;
		while (visited.size() < NodeNums) {
			for (TUNGraph::TNodeI NI = G_->BegNI(); NI < G_->EndNI(); NI++) {

				if (std::find(visited.begin(), visited.end(), NI.GetId()) == visited.end()) {
					//printf("T%d:\n", j++);
					BFSTree = TSnap::MyGetBfsTree(G_, NI.GetId(), true, false);  //My BFS Function modified in bfsdfs.h
					del.clear();
					//printf("%d\n", TSnap::CntUniqDirEdges(BFSTree));

					for (TNGraph::TNodeI TNI = BFSTree->BegNI(); TNI < BFSTree->EndNI(); TNI++) {
						visited.push_back(TNI.GetId());
						if (i == 0) {
							if (!SC->IsNode(TNI.GetId()))
								SC->AddNode(TNI.GetId());
						}

					}

					for (TNGraph::TEdgeI TEI = BFSTree->BegEI(); TEI < BFSTree->EndEI(); TEI++) {
						// if not use MyGetBfsTree() before, use the follow code
						//if (std::find(del.begin(), del.end(), TEI.GetDstNId()) == del.end()) {
						//	//printf("%d -> %d\n", TEI.GetSrcNId(), TEI.GetDstNId());
						//	SC->AddEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						//	del.push_back(TEI.GetDstNId());
						//	G->DelEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						//}
						//printf("%d -> %d\n", TEI.GetSrcNId(), TEI.GetDstNId());
						SC->AddEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						G_->DelEdge(TEI.GetSrcNId(), TEI.GetDstNId());
					}
				}

			}

		}
	}

	

	return SC;

}

TIntVIntV VCCE::Overlap_Partition(TIntV subG, TIntV Vertex_Cut, PUNGraph All_G) {
	//params:
	//	G: Undirected Graph
	//	S: Vertex_Cut
	//return:
	//	Overlap_Partition Graphs (Type: TUNGraV )
	PUNGraph G_ = TSnap::GetSubGraph(All_G, subG);
	TIntVIntV G_set;
	//printf("Vertex_Cut: %d\n", Vertex_Cut.Len());
	//printf("G: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G), TSnap::CntUniqUndirEdges(G));
	TSnap::DelNodes(G_, Vertex_Cut); // 锟斤拷应锟侥边伙拷锟皆讹拷删锟斤拷锟斤拷 To test
	//printf("G_: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(G_), TSnap::CntUniqUndirEdges(G_));
	TCnComV ConV;
	TSnap::GetSccs(G_, ConV);
	//printf("ConV: %d\n", ConV.Len());
	for (TCnComV::TIter I = ConV.BegI(); I < ConV.EndI(); I++) {
		//printf("%d\n", I->NIdV);
		for (TIntV::TIter NI = Vertex_Cut.BegI(); NI < Vertex_Cut.EndI(); NI++) {
			I->Add(NI->Val);
			//printf("%d\n", NI->Val);
		}
		G_set.Add(I->NIdV);
	}



	return G_set;
}

PNEANet VCCE::Construct_DG(PUNGraph G) {
	//Construct corresponding attribute directed graph,
	//prepare for maximum flow calculate.(According to 'Effective K-Vertex connected 
	//component detection in large-scale networks')
	PNEANet DG = TNEANet::New();
	DG->AddIntAttrE("capacity", 0);
	int offset = G->GetMxNId();
	int i, eid;
	//step 1

	for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
		i = NI.GetId();
		DG->AddNode(i); //v
		DG->AddNode(i + offset); //v'
		eid = DG->AddEdge(i, i + offset);
		DG->AddIntAttrDatE(eid, 1, "capacity");

	}

	//step 2
	for (TUNGraph::TEdgeI EI = G->BegEI(); EI < G->EndEI(); EI++) {
		// u -> v
		eid = DG->AddEdge(EI.GetSrcNId() + offset, EI.GetDstNId());  //u''->v'
		DG->AddIntAttrDatE(eid, 1, "capacity");
		eid = DG->AddEdge(EI.GetDstNId() + offset, EI.GetSrcNId()); //v''->u'
		DG->AddIntAttrDatE(eid, 1, "capacity");
	}

	return  DG;
}


TIntV VCCE::Loc_Cut(int source, int sink, PNEANet DG, PUNGraph G, int k) {
	clock_t t1 = clock();
	//printf("DG: \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(DG), TSnap::CntUniqUndirEdges(DG));
	int offset = G->GetMxNId();
	source += offset;
	//std::cout << DG->IsNode(sink);
	TIntV vertex_cut = {};
	TIntV ResNet;
	PNEANet DG2 = TNEANet::New();
	*DG2 = *DG;
	//int e;//take place
	//PUNGraph Neigh = TSnap::GetEgonet(G, source - offset, e); //N(u)
	
	if (source == sink || G->GetNI(source - offset).IsNbrNId(sink)) {
		_time4 += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;
		////printf("%fs\n", (clock() - t1) * 1.0 / CLOCKS_PER_SEC);
		m4++;
		return {};
	}
		
	int lambda = TSnap::MyGetMaxFlowIntEK(DG, source, sink, ResNet, k);
	if (lambda >= k) {
		_time2 += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;
		m2++;
		return {};
	}

	else {
		for (int i = 0; i < ResNet.Len(); i++) {
			//printf("%d: %d\n", i, ResNet[i]);
			if (ResNet[i] == 1) {
				//printf("%d: %d->%d\n", i, DG->GetEI(i).GetSrcNId(), DG->GetEI(i).GetDstNId());
				DG2->AddEdge(DG2->GetEI(i).GetDstNId(), DG2->GetEI(i).GetSrcNId());
				DG2->DelEdge(DG2->GetEI(i).GetSrcNId(), DG2->GetEI(i).GetDstNId());		
			}
		}
	}
	PNGraph BFSTree;
	BFSTree = TSnap::MyGetBfsTree(DG2, source, true, false);
	for (TNEANet::TEdgeI EI = DG->BegEI(); EI < DG->EndEI(); EI++) {
		if (BFSTree->IsNode(EI.GetSrcNId()) && !BFSTree->IsNode(EI.GetDstNId())) {
				if (G->IsNode(EI.GetSrcNId())) 
					vertex_cut.Add(EI.GetSrcNId());
				else
					vertex_cut.Add(EI.GetDstNId());
			
			//printf("%d->%d\n", EI.GetSrcNId(), EI.GetDstNId());
		}
	}
	//printf("\n");
	_time3 += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;
	//printf("%fs\n", (clock() - t1) * 1.0 / CLOCKS_PER_SEC);
	m3++;
	return vertex_cut;
	//compute the minimum edge cut in DG
	//return the corrrsponding vertex cut in G
}

template <class PGraph>
int GetMnDegNId(const PGraph& Graph) {
	TIntV MnDegV;
	int MnDeg = INT_MAX;
	for (typename PGraph::TObj::TNodeI NI = Graph->BegNI(); NI < Graph->EndNI(); NI++) {
		if (MnDeg > NI.GetDeg()) { MnDegV.Clr(); MnDeg = NI.GetDeg(); }
		if (MnDeg == NI.GetDeg()) { MnDegV.Add(NI.GetId()); }
	}
	EAssertR(!MnDegV.Empty(), "Input graph is empty!");
	return MnDegV[TInt::Rnd.GetUniDevInt(MnDegV.Len())];
}


