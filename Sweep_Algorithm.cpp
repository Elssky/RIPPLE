#include"Sweep_Algorithm.h"
#include"set"
#include"vector"
#include <map>
#include"algorithm"
#include <iterator>




VCCE_S::VCCE_S(PUNGraph G_, int k_, int Compute_SSV_times_) {
	G = G_;
	k = k_;
	Compute_SSV_times = Compute_SSV_times_;
	Nodes = G->GetNodes();
	//Nodes = G->GetMxNId() + 1;
	int Mxnode = G->GetMxNId();

	//vertex_map_ = new int[Mxnode];
	//memset(vertex_map_, 0, sizeof(int) * Mxnode);
	//int i = 0;
	//for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
	//	vertex_map_[NI.GetId()] = i++;
	//}
}

TIntVIntV VCCE_S::KVCC_ENUM(PUNGraph &sub_G, int k, int flag) {
	//func: Find k-VCCs in Graph G
	//params:
	//	G: Undirected Graph
	//	k: an integer
	//return:
	//	all k-vertex connected components (Type: TIntVIntV)

	//step1: initialize set VCC as empty
	TIntVIntV VCC;  // return as K-Vccs;

	//step2: remove vertices u and incident edges, that d(u) < k
	PUNGraph G2 = TSnap::GetKCore(sub_G, k);
	//printf("G: \nnode_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());
	//printf("G2: \nnode_nums = %d, edge_nums = %d\n", G2->GetNodes(), G2->GetEdges());
	//for (TIntV::TIter NI = G->SSV.BegI(); NI < G->SSV.EndI(); NI++) {	//G2->SSV = G->SSV(node in G2)
	//	if (G2->IsNode(*NI)) G2->SSV.Add(*NI);
	//}
	//if (flag == 0) {//G2->SSV.Len() == 0
	//	//printf("G2_");
	//	//Detect_SSV(G2);
	//	Get_SSV(G2);
	//	flag = 1;
	//}
	//printf("flag:%d\n", flag);
	//step3: identify connected compotent set in G
	TCnComV ConV;
	TSnap::GetSccs(G2, ConV);
	TIntVIntV G_set; // Connected Component in G2;
	for (TCnComV::TIter I = ConV.BegI(); I < ConV.EndI(); I++) {
		//GI->SSV = G2->SSV;
		//for (TIntV::TIter NI = G2->SSV.BegI(); NI < G2->SSV.EndI(); NI++) { //or GI->SSV = G2->SSV;
		//	if (GI->IsNode(*NI)) GI->SSV.Add(*NI);
		//}
		G_set.Add(I->NIdV);
	}
	//printf("G_set_len: %d\n", G_set.Len());
	
	//step4: find vertex cut in G
	TIntV S; //Vertex_Cut

	for (TIntVIntV::TIter GI = G_set.BegI(); GI < G_set.EndI(); GI++) {
		//clock_t t1 = clock();

		S = Global_Cut(*GI, k, flag, G2);
		//m++;
		//_time += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;
		//printf("SSV_len: %d\n", (*GI)->SSV.Len());
		//printf("%d\n", S.Empty());
		if (S.Empty()) {

			VCC.Add(*GI);
		}
		else {
			TIntVIntV G_i = Overlap_Partition(*GI, S, G2);
			//printf("%d\n", G_i.Len());
			for (TIntVIntV::TIter G_ij = G_i.BegI(); G_ij < G_i.EndI(); G_ij++) {
				//printf("SSV_len: %d\n", (*G_ij)->SSV.Len());
				PUNGraph temp_G = TSnap::GetSubGraph(G2, *G_ij);
				TIntVIntV VCC_i = KVCC_ENUM(temp_G, k, 1);
				VCC.AddV(VCC_i);
				//printf("%d\n", VCC_i.Len());

			}
		}

	}

	for (TIntVIntV::TIter GI = VCC.BegI(); GI < VCC.EndI(); GI++) {
		GI->Sort();
	}
	VCC.Sort();

	char* k_str = new char[5];
	//char* alg_str = new char[20];
	//string alg[4] = { "VCCE", "BkVCC", "BkVCC(clique)" ,"BkVCC(k+1 clique)" };

	//strcpy(alg_str, alg[a].c_str());
	//itoa(k, k_str, 10);
	sprintf(k_str,"%d",k);
	//sprintf(alpha_str,"%d",alpha);
	//cout << dataset.CStr() << endl;
	TFOut outFile("./community/" + dataset + "_k=" + TStr(k_str)  + "_algorithm=" + alg.c_str() + ".kvcc"); // "_maxflow=" + mergeMethod.c_str() 
	VCC.Save(outFile);

	return VCC;
}

TIntV VCCE_S::Global_Cut(TIntV &subG, int k, int flag, PUNGraph All_G)
{
	
	TIntV S;
	MyTCnComV CS;	
	int u, v;
	
	//1. compute sparse certification SC

	PUNGraph SC = Compute_SC(subG, k, CS, All_G);
	TIntH deposit, dist, pru;
	//SC->SSV = G->SSV;
	//2. direct graph
	PNEANet SC_bar = Construct_DG(SC);
	
	////3. detect strong side vertices
	//if (Compute_SSV_times == 1) {
	//	if (flag == 0) {
	//		//Get_SSV(SC);
	//		Detect_SSV(SC);
	//	}
	//		
	//}
	//else if (Compute_SSV_times > 1) {
	//	//Get_SSV(SC);
	//	Detect_SSV(SC);
	//}
	//else {

	//}

	////printf("SC_");
	////Detect_SSV(SC);
	//G->SSV = SC->SSV;
	////printf("SSV_len: %d\n", SC->SSV.Len());
	////printf("G->SSV: %d\n", G->SSV.Len());
	//
	////printf("%fs\n", (clock() - t1) * 1.0 / CLOCKS_PER_SEC);
	//
	//

	//if (SC->SSV.Empty()) {
	//	u = GetMnDegNId(SC);		
	//}
	//else {
	//	//printf("1\n");
	//	u = SC->SSV.GetRndVal();
	//}

	u = GetMnDegNId(SC);

	//4.Init
	
	for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {
		//printf("%d\n", I->NIdV);
		I->g_deposit = 0;
	}
	
	for (TUNGraph::TNodeI NI = SC->BegNI(); NI < SC->EndNI(); NI++) {
		deposit.AddDat(NI.GetId(), 0);
		pru.AddDat(NI.GetId(), 0);
	}

	
	TSnap::GetShortPath(All_G, u, dist, false);
	dist.SortByDat(false);


	//5.Phase1 & Sweep
	Sweep(SC, u, pru, deposit, CS);
	
	for (TIntH::TIter HI = dist.BegI(); HI < dist.EndI(); HI++) {
		
		v = HI.GetKey();
		
		if (pru.GetDat(v) == true) {
			pru_node++;
			continue;
		}
		S = Loc_Cut(u, v, SC_bar, SC, k);
		non_pru_node++;
		if (!S.Empty()) {
			
			return S;
		}
		Sweep(SC, v, pru, deposit, CS);
		
	}
	
	//6.Phase2
	//printf("%d\n", SC->SSV.IsIn(u));
	int e;
	for (int i = 0; i < SC->GetNI(u).GetDeg(); i++) {
		for (int j = i + 1; j < SC->GetNI(u).GetDeg(); j++) {
			//if (NI1 == NI2) continue;
			int NI1 = SC->GetNI(u).GetNbrNId(i);
			int NI2 = SC->GetNI(u).GetNbrNId(j);
			bool flag = false;
			for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {
				if (I->IsNIdIn(NI1) || I->IsNIdIn(NI2)) {
					if (I->IsNIdIn(NI1) && I->IsNIdIn(NI2)) {
						flag = true;
					}
					break;
				}
			}
			if (flag == true) {		
				//pru_node++;
				continue;
			}
			S = Loc_Cut(NI1, NI2, SC_bar, SC, k);
			//non_pru_node++;
			c++;
			if (!S.Empty()) {
				return S;
			}
		}
	}
	//if (!SC->SSV.IsIn(u)) {
	//	int e;
	//	PUNGraph Neigh = TSnap::GetEgonet(SC, u, e);
	//	for (TUNGraph::TNodeI NI1 = Neigh->BegNI(); NI1 < Neigh->EndNI(); NI1++) {
	//		for (TUNGraph::TNodeI NI2 = NI1; NI2 < Neigh->EndNI(); NI2++) {
	//			if (NI1 == NI2) continue;
	//			bool flag = false;
	//			for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {
	//				if (I->IsNIdIn(NI1.GetId()) || I->IsNIdIn(NI2.GetId())) {
	//					if (I->IsNIdIn(NI1.GetId()) && I->IsNIdIn(NI2.GetId())) {
	//						flag = true;
	//					}
	//					break;
	//				}
	//			}
	//			if (flag == true) {
	//				//pru_node++;
	//				continue;
	//			}
	//			S = Loc_Cut(NI1.GetId(), NI2.GetId(), SC_bar, SC, k);
	//			//non_pru_node++;
	//			c++;
	//			if (!S.Empty()) {
	//				return S;
	//			}
	//		}
	//	}
	//}
	
	return {};
}

TIntVIntV VCCE_S::Overlap_Partition(TIntV subG, TIntV Vertex_Cut, PUNGraph All_G) {
	//params:
	//	G: Undirected Graph
	//	S: Vertex_Cut
	//return:
	//	Overlap_Partition Graphs (Type: TIntVIntV )
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
		for (TIntV::TIter NI = Vertex_Cut.BegI(); NI < Vertex_Cut.EndI(); NI++) {
			I->Add(NI->Val);		
		}
		//锟教筹拷strong side vertex
		/*Check_SSV(G, GI, Vertex_Cut);*/
		G_set.Add(I->NIdV);
	}


	return G_set;
}

PUNGraph VCCE_S::Compute_SC(TIntV subG, int k, MyTCnComV &CS, PUNGraph All_G)
{
	
	PUNGraph SC = TUNGraph::New();
	PUNGraph G_ = TSnap::GetSubGraph(All_G, subG);
	PNGraph BFSTree;
	PUNGraph F_k = TUNGraph::New();//Forest k
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
					
					BFSTree = TSnap::MyGetBfsTree(G_, NI.GetId(), true, false);  //My BFS Function modified in bfsdfs.h
					del.clear();
					

					for (TNGraph::TNodeI TNI = BFSTree->BegNI(); TNI < BFSTree->EndNI(); TNI++) {
						visited.push_back(TNI.GetId());
						if (i == 0) {
							if (!SC->IsNode(TNI.GetId()))
								SC->AddNode(TNI.GetId());
						}
						if (i == k - 1) {
							F_k->AddNode(TNI.GetId());
						}
					}

					for (TNGraph::TEdgeI TEI = BFSTree->BegEI(); TEI < BFSTree->EndEI(); TEI++) {
						SC->AddEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						G_->DelEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						if (i == k - 1) {
							F_k->AddEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						}
					}
				}
			}
		}
	}
	TSnap::GetSccs(F_k, CS);
	for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {	
		if (I->Len() < 2) {
			CS.DelAll(*I);
		}
	}
	/*SC->SSV = G->SSV;*/
	return SC;

}


void VCCE_S::Detect_SSV(PUNGraph& G)
{	

	//锟叫讹拷锟角凤拷为锟斤拷图锟斤拷锟斤拷锟角ｏ拷锟斤拷锟斤拷锟斤拷P57 Lemma11锟斤拷12锟狡碉拷锟斤拷锟侥加匡拷锟斤拷SSV锟侥凤拷锟斤拷锟斤拷
	//Todo: P57 Lemma11锟斤拷12锟狡碉拷锟斤拷锟侥加匡拷锟斤拷SSV锟侥凤拷锟斤拷

	clock_t t1 = clock();
	if (G->SSV.Len() > 0) {
		for (TIntV::TIter NI = G->SSV.BegI(); NI < G->SSV.EndI(); NI++) {
			if (!IsSSV(G, *NI)) {
				G->SSV.Del(*NI);
			}
		}
	}
	else {
		for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
			if (IsSSV(G, NI.GetId())) {
				G->SSV.Add(NI.GetId());
			}
		}
	}
	m++;
	_time += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;
	//printf("Len:%d\n", G->SSV.Len());
}




//void VCCE_S::Get_SSV(PUNGraph &G) {
//	//printf("call here\n");
//	clock_t t1 = clock();
//	//int Nodes = G->GetNodes(); //Nodes 
//	int** Com_Neighbor = new int* [Nodes]; //Com_Neighbor[Nodes][Nodes]
//	for (int i = 0; i < Nodes; i++) {
//		Com_Neighbor[i] = new int[Nodes];
//		memset(Com_Neighbor[i], 0, sizeof(int)* Nodes);
//	}
//
//	
//	for(TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
//		for (int i = 0; i < NI.GetInDeg(); i++) {
//			int node1 = vertex_map_[NI.GetInNId(i)];			
//			for (int j = i; j < NI.GetInDeg(); j++) {
//				if (i == j) continue;
//				int node2 = vertex_map_[NI.GetInNId(j)];
//				Com_Neighbor[node1][node2]++;
//			}
//		}
//
//	}
//	for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
//		if (IsSSV(G, NI.GetId(), Com_Neighbor))
//			G->SSV.Add(NI.GetId());
//	}
//	for (int i = 0; i < Nodes; i++)
//		delete[] Com_Neighbor[i];
//	delete[] Com_Neighbor;
//
//	m++;
//	_time += (double)(clock() - t1) * 1.0 / (double)CLOCKS_PER_SEC;
//	printf("Len:%d\n", G->SSV.Len());
//}

//bool VCCE_S::IsSSV(PUNGraph G, int NId, int** Com_Neighbor) {
//	TUNGraph::TNodeI NI = G->GetNI(NId);
//	for (int i = 0; i < NI.GetInDeg(); i++) {
//		int node1 = vertex_map_[NI.GetInNId(i)];
//		for (int j = i; j < NI.GetInDeg(); j++) {
//			if (i == j) continue;
//			int node2 = vertex_map_[NI.GetInNId(j)];
//			if (!G->IsEdge(NI.GetInNId(i), NI.GetInNId(j))) return false;
//			if (Com_Neighbor[node1][node2] < k) {
//				return false;
//			}
//
//		}
//	}
//	return true;
//}

bool VCCE_S::IsSSV(PUNGraph G, int NId) {

	int e;
	std::set<int> set1, set2, set3;
	PUNGraph Neigh = TSnap::GetEgonet(G, NId, e); //N(u)
	for (TUNGraph::TNodeI NI1 = Neigh->BegNI(); NI1 < Neigh->EndNI(); NI1++) {

		set1.clear();
		
		for (int i = 0; i < NI1.GetInDeg(); i++) {
			set1.insert(NI1.GetInNId(i));
		}

		for (TUNGraph::TNodeI NI2 = NI1; NI2 < Neigh->EndNI(); NI2++) {

			set2.clear();
			set3.clear();

			if (NI1 == NI2) continue;
			if (!G->IsEdge(NI1.GetId(), NI2.GetId())) return false;
			//for (int i = 0; i < NI2.GetInDeg(); i++) {
			//	set2.insert(NI2.GetInNId(i));
			//}

			//std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::insert_iterator<std::set<int>>(set3, set3.begin()));

			//if (set3.size() < k) return false;

		}
	}

	return true;
}

void VCCE_S::Check_SSV(PUNGraph& G, PUNGraph& subG, TIntV S) {

	//锟叫讹拷锟角凤拷为锟斤拷图锟斤拷锟斤拷锟角ｏ拷锟斤拷锟斤拷锟斤拷P57 Lemma11锟斤拷12锟狡碉拷锟斤拷锟侥加匡拷锟斤拷SSV锟侥凤拷锟斤拷锟斤拷
	//Todo: P57 Lemma11锟斤拷12锟狡碉拷锟斤拷锟侥加匡拷锟斤拷SSV锟侥凤拷锟斤拷
	//printf("%d\n", G->IsSub);

	std::set<int> set1, set2, set3;

	for (TIntV::TIter NI2 = S.BegI(); NI2 < S.EndI(); NI2++) {
		set2.insert(*NI2);
	}
	
	for (TIntV::TIter NI = G->SSV.BegI(); NI < G->SSV.EndI(); NI++) {
		set1.clear();
		set3.clear();

		TUNGraph::TNodeI NI1 = G->GetNI(*NI);
		for (int i = 0; i < NI1.GetInDeg(); i++) {
			set1.insert(NI1.GetInNId(i));
		}
		std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::insert_iterator<std::set<int> >(set3, set3.begin()));
		if (set3.empty())
			subG->SSV.Add(*NI);
	}
}


void VCCE_S::Sweep(PUNGraph G, int v, TIntH& pru, TIntH& deposit, MyTCnComV& CS) {
	//printf("%d\n", pru.IsKey(v));

	pru.GetDat(v) = true;
	int e;
	

	for (int i = 0; i < G->GetNI(v).GetDeg(); i++) {
		int w = G->GetNI(v).GetNbrNId(i);
		if (pru.GetDat(w) == false) {
			deposit.GetDat(w)++;
			//if (G->SSV.IsIn(v) || deposit.GetDat(w) >= k
			if (deposit.GetDat(w) >= k)
				Sweep(G, w, pru, deposit, CS);
		}
	}
	/*PUNGraph Neigh = TSnap::GetEgonet(G, v, e);
	for (TUNGraph::TNodeI NI = Neigh->BegNI(); NI < Neigh->EndNI(); NI++) {
		int w = NI.GetId();
		if (pru.GetDat(w) == false) {
			deposit.GetDat(w)++;
			if (G->SSV.IsIn(v) || deposit.GetDat(w) >= k)
				Sweep(G, w, pru, deposit, CS);
		}
	}*/

	for (MyTCnComV::TIter I = CS.BegI(); I < CS.EndI(); I++) {
		if (I->IsNIdIn(v)) {
			if (I->processed == false) {
				I->g_deposit++;
				//if (G->SSV.IsIn(v) || I->g_deposit >= k) {
				if (I->g_deposit >= k) {
					I->processed = true;
					PUNGraph CC = TSnap::GetSubGraph(G, I->NIdV);
					for (TUNGraph::TNodeI NI = CC->BegNI(); NI < CC->EndNI(); NI++) {
						int w = NI.GetId();
						if (pru.GetDat(w) == false)
							Sweep(G, w, pru, deposit, CS);
					}
				}
			}
			break;
		}
	}

}
