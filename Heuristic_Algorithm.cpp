#include "Heuristic_Algorithm.h"
#include"Sweep_Algorithm.h"
#include <queue>


char* k_str = new char[5];
char* alpha_str = new char[10];


BkVCC::BkVCC()
{


}

BkVCC::BkVCC(PUNGraph G_, int k_)
{
	G = G_;
	k = k_;
}

TIntVIntV BkVCC::BkVCC_ENUM(PUNGraph& G, int k, int alpha)
{
	omp_set_num_threads(threads);
	TIntVIntV G_R, G_S, G_S_prime;

	//strcpy(alg_str, alg.c_str());
	// itoa(k, k_str, 10);
	// itoa(alpha, alpha_str, 10);
	// char* k_str = new char[5];
	// itoa(k, k_str, 10);
	sprintf(k_str, "%d", k);
	sprintf(alpha_str, "%d", alpha);


	//find k-core
	G = TSnap::GetKCore(G, k);
	
	printf("G[%d-core]: \nnode_nums = %d, edge_nums = %d\n", k, G->GetNodes(), G->GetEdges());
	//Seeding
	double t1 = omp_get_wtime( );
	int j = 0;




	//加载已经生成的结果
	/*try
	{
		TFIn inFile(".//community/" + dataset + "_k=" + TStr(k_str) + ".kvcc");
		G_R.Load(inFile);
	}
	catch (TPt<TExcept>)
	{
		cout << endl << "***kvcc result does not exist, running BkVCC function...***" << endl;
	}*/

	G_S = Seeding(G, k, alpha);
	

	cout << "Seeding subgraph num:" << G_S.Len() << endl;
	_time += (double)(omp_get_wtime( ) - t1);

	do {

		G_S_prime = G_S;
		

		if (strcmp(mergeMethod.c_str(), "") != 0) {
			//打乱顺序
			//G_S.Reverse();
			Merging(k, G_S, G_R);
		}

		//expanding G_S
		if (strcmp(expandMethod.c_str(), "") != 0)
			Expanding(k, G_S);

		

	
	} while (G_S != G_S_prime); 


	//09/04 在图的内部也进行排序操作 比如其中有两个元素[3,5,7,9,22]与[3,9,5,22,7]，
	//这两个是不会合并的，因为merge判断需要完全相同，所以需要在每个元素内部在进行一次merging操作
	G_R.AddVMerged(G_S);
	for (TIntVIntV::TIter TI = G_R.BegI(); TI < G_R.EndI(); TI++) {
		TI->Sort();
	}
	G_R.Merge();
	//保存结果
	TFOut outFile("./community/" + dataset + "_k=" + TStr(k_str) +
		"_alpha=" + TStr(alpha_str) + "_algorithm=" + TStr(alg.c_str()) + "_expand=" + expandMethod.c_str()
		+ "_merge=" + mergeMethod.c_str() + "_t=" + std::to_string(threads).c_str() + ".kvcc");
	G_R.Save(outFile);

	return G_R;
}


TIntV BkVCC::LkVCS(PUNGraph G, int k, int u, int alpha)
{
	// params: 
	//	  alpha: upper bound for selecting subset with size k


	PUNGraph P = TSnap::GetEgonetHop(G, u, 2); // P = N_2(u)

	//printf("P: \nnode_nums = %d, edge_nums = %d\n", P->GetNodes(), P->GetEdges());

	PUNGraph P_prime = TSnap::GetKCore(P, k);

	
	//printf("P_prime: \nnode_nums = %d, edge_nums = %d\n", P_prime->GetNodes(), P_prime->GetEdges());
	int e; //take place
	if (!P_prime->IsNode(u)) {
		return {};
	}



	TIntV nb_set;
	TUNGraph::TNodeI u_Node = P_prime->GetNI(u);
	for (int i = 0; i < u_Node.GetInDeg(); ++i) {
		nb_set.Add(u_Node.GetInNId(i));
	}

	//nb_set.Add(u);

	TIntVIntV res = randSample(nb_set, k + 1, alpha);


	int wr_n1 = 0;
	//int wr_n2 = 0;
	TIntV wr_n2 = {};
	for (int i = 0; i < res.Len() ; i++) {
		TIntV R = res[i];
		R.AddUnique(u); // R = R Union u
		R.Merge();
		int a = 0, b = 0;
		int flag = 0;
		//if(R.IsIn(wr_n1) && R.IsIn(wr_n2)) continue;
		/*if (R.IsIn(wr_n1)) {
			for (int j = 0; j < wr_n2.Len(); j++) {
				if (R.IsIn(wr_n2[j])) {
					flag = 1;
					break;
					
				}
			}	
			if (flag == 1) continue;
		}*/
		
		bool direct = true;
		while (direct) {

			//R may be updated
			PUNGraph G_R = TSnap::GetSubGraph(G, R);
			//set flag1 = ...
			if (flag1(G_R, a, b)) {
				
		
				return R;
			}
			else {

				if(flag2(P_prime, G_R, wr_n1, wr_n2)) {			
					direct = false;
				}
				else {
					//find ... and radomly choose ... vertices from ... adding into R
					Adding2Subset2(P_prime, G_R, R, a, b);
				}
			}

		}

	}
	
	return {};
}



bool BkVCC::flag1(PUNGraph G_R, int &u, int &v)
{

	for (TUNGraph::TNodeI NI1 = G_R->BegNI(); NI1 < G_R->EndNI(); NI1++) {
		for (TUNGraph::TNodeI NI2 = NI1; NI2 < G_R->EndNI(); NI2++) {

			if (NI2 == NI1 || G->IsEdge(NI1.GetId(), NI2.GetId())) continue;

			if (TSnap::GetCmnNbrs(G_R, NI1.GetId(), NI2.GetId()) < k) {
				u = NI1.GetId(), v = NI2.GetId();
				
				return false;
			}
		}
	}
	return true;
}



bool BkVCC::flag2(PUNGraph P_prime, PUNGraph G_R, int &u, TIntV& vs)
{

	for (TUNGraph::TNodeI NI1 = G_R->BegNI(); NI1 < G_R->EndNI(); NI1++) {
		for (TUNGraph::TNodeI NI2 = NI1; NI2 < G_R->EndNI(); NI2++) {
			if (NI2 == NI1 || G->IsEdge(NI1.GetId(), NI2.GetId()))continue;

			if (TSnap::GetCmnNbrs(P_prime, NI1.GetId(), NI2.GetId()) < k) {
				//u = NI1.GetId(), vs.AddUnique(NI2.GetId());
				return true;
			}
		}
	}
	return false;
}

void BkVCC::Adding2Subset(PUNGraph P_prime, PUNGraph G_R, TIntV& R)
{

	TIntV NbrV1, NbrV2;
	int num = 0;
	int total;
	for (TUNGraph::TNodeI NI1 = G_R->BegNI(); NI1 < G_R->EndNI(); NI1++) {
		for (TUNGraph::TNodeI NI2 = NI1; NI2 < G_R->EndNI(); NI2++) {
			if (NI2 == NI1 || G->IsEdge(NI1.GetId(), NI2.GetId())) continue;
			if (TSnap::GetCmnNbrs(G_R, NI1.GetId(), NI2.GetId(), NbrV1) < k) {
				TSnap::GetCmnNbrs(P_prime, NI1.GetId(), NI2.GetId(), NbrV2);
				total = k - NbrV1.Len();
				//This is not randomly choose, which is different with paper
				for (TIntV::TIter NI = NbrV2.BegI(); NI < NbrV2.EndI(); NI++) {
					if (!NbrV1.IsIn(*NI)) {
						R.Add(*NI);
						num++;
						if (num >= total) return;
					}
				}
			}
		}
	}
}

void BkVCC::Adding2Subset2(PUNGraph P_prime, PUNGraph G_R, TIntV& R, int u, int v)
{

	TIntV NbrV1, NbrV2;
	TIntH deg;
	int num = 0, node;
	int total;
	TSnap::GetCmnNbrs(G_R, u, v, NbrV1);
	TSnap::GetCmnNbrs(P_prime, u, v, NbrV2);
	total = k - NbrV1.Len();
	for (TIntV::TIter NI = NbrV2.BegI(); NI < NbrV2.EndI(); NI++) {
		deg.AddDat(*NI, P_prime->GetNI(*NI).GetDeg());
	}
	deg.SortByDat(false);
	/*NbrV2.Sort*/
	for (TIntH::TIter HI = deg.BegI(); HI < deg.EndI(); HI++) {
		node = HI.GetKey();
		if (!NbrV1.IsIn(node)) {
			R.Add(node);
			num++;
			if (num >= total) return;
		}
	}
}

TIntVIntV BkVCC::Seeding(PUNGraph G, int k, int alpha)
{

	TIntVIntV G_S;
	TIntV G_C;
	TIntH CandMaintain, deg;
	int i = 0;


	// try
	// {
	// 	TFIn inFile("./seedgraph/" + dataset + "_k=" + TStr(k_str) + "_alpha=" + TStr(alpha_str) + "_seed=" + TStr(seed.c_str()) + ".seed");
	// 	//TFIn inFile("./seedgraph/DBLP_k=5_alpha=1000_algorithm=BkVCC.seed");
	// 	G_S.Load(inFile);
	// }
	// catch (TPt<TExcept>)
	// {
	// 	cout << endl << "***seedgraph does not exist, running seeding function...***" << endl;
	// }

	if (G_S.Len() != 0) return G_S;


	if (strcmp(seed.c_str(), "rand") == 0) {

		
		for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
			CandMaintain.AddDat(NI.GetId(), 0);
			deg.AddDat(NI.GetId(), NI.GetDeg());
		}
		deg.SortByDat(); //non-decreasing order
		

		#pragma omp parallel
			{
				//int threadId = omp_get_thread_num();
				/*cout << omp_get_num_threads() << endl;*/
			#pragma omp for private(G_C)
				for (int i = 0; i < deg.Len(); i++) {

					int v = CandMaintain.GetKey(i);
					if (CandMaintain.GetDat(v) == 0) {

						G_C = LkVCS(G, k, v, alpha);

						if (G_C.Empty()) continue;

					#pragma omp critical
						{
							G_S.Add(G_C); //怎么判断是否相等（Unique）？

							for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {
			
								CandMaintain.GetDat(*TI) = 1;
							}
						}

					}


				}			
			}
			G_S.Merge();
	
		

	}
	else if (strcmp(seed.c_str(), "clique") == 0) {
		TCliqueOverlap::GetMaxCliques(G, k + 1, G_S);
	}
	else if (strcmp(seed.c_str(), "k-2-clique") == 0) {
		TCliqueOverlap::GetMaxCliques(G, k -2 , G_S);
	}
	else if (strcmp(seed.c_str(), "3-clique") == 0) {
		TCliqueOverlap::GetMaxCliques(G, 3, G_S);
	}
	else if (strcmp(seed.c_str(), "mix_Com") == 0) {
		//先用clique生成seed，在使用rand的方法选中不属于任何seed的点作为中心来生成新的seed

		//step1. 获取k-core的每个连通分量
		TCnComV CnComV;
		TSnap::GetSccs(G, CnComV);
		cout << "CnCon(Len): " << CnComV.Len() << endl;
	#pragma omp parallel 
		{
			#pragma omp for private(G_C, CandMaintain, deg)
			for (int i = 0; i < CnComV.Len(); i++) {
				TCnCom C = CnComV.GetVal(i);
				cout << i << endl;
				
				TIntVIntV G_Subcliques;
				PUNGraph G_CnCom = TSnap::GetSubGraph(G, C());
				TCliqueOverlap::GetMaxCliques(G_CnCom, k + 1, G_Subcliques);
				if (G_Subcliques.Empty()) {
					for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
						CandMaintain.AddDat(NI.GetId(), 0);
						deg.AddDat(NI.GetId(), NI.GetDeg());
					}
					deg.SortByDat(); //non-decreasing order

				
					for (int j = 0; j < deg.Len(); j++) {
						
						int v = CandMaintain.GetKey(j);
						if (CandMaintain.GetDat(v) == 0) {
						
							G_C = LkVCS(G, k, v, alpha);
			
							if (G_C.Empty()) continue;

						#pragma omp critical
							G_S.Add(G_C); //怎么判断是否相等（Unique）？

							for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {

								CandMaintain.GetDat(*TI) = 1;
							}

						}

					}
				}
				else {
					#pragma omp critical
						G_S.AddVMerged(G_Subcliques);
				}
			}
		}		
		
		G_S.Merge();
	}
	else if (strcmp(seed.c_str(), "mix") == 0) {

		TUNGraph G_temp = *G;
		//step1.先生成max_cliques

		TCliqueOverlap::GetMaxCliques(G, k + 1, G_S);

		cout << G_S.Len()<< endl;

		for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
			//cout << NI.GetId() << endl;
			CandMaintain.AddDat(NI.GetId(), 0);
			deg.AddDat(NI.GetId(), NI.GetDeg());
		}

		//step2.将处于max_cliques的点从CandMaintain设为1，表示已被选入子图
		//TODO：直接将max_cliques中的点删除
		//TUNGraph G_temp = *G;
		for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
			for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
				CandMaintain.GetDat(*NI) = 1;
				if(G_temp.IsNode(*NI)) G_temp.DelNode(*NI);


			}
		}
		cout <<"G_temp: " << G_temp.GetNodes() << endl;	

		deg.SortByDat(); //non-decreasing order

		#pragma omp parallel
		{
			//int threadId = omp_get_thread_num();
			/*cout << omp_get_num_threads() << endl;*/
			#pragma omp for private(G_C)
			for (int i = 0; i < deg.Len(); i++) {
				int v = CandMaintain.GetKey(i);
				if (CandMaintain.GetDat(v) == 0) {


					G_C = LkVCS(G, k, v, alpha);

					if (G_C.Empty()) continue;

					#pragma omp critical
						G_S.Add(G_C); //怎么判断是否相等（Unique）？
					#pragma omp critical
						for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {

							CandMaintain.GetDat(*TI) = 1;
						}
				}
			}
		}
		G_S.Merge();
	}
	else if (strcmp(seed.c_str(), "k'-vcc") == 0) {
		TUNGraph G_temp = *G;
		int k_prime = k + 3;
		VCCE_S VCCE_S(G, k_prime, 1);
		PUNGraph tmp = TSnap::GetKCore(G, k_prime);
		printf("G(%d-core): \nnode_nums = %d, edge_nums = %d\n", k_prime, tmp->GetNodes(), tmp->GetEdges());
		VCCE_S.dataset = dataset;
		G_S = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);

		cout << G_S.Len() << endl;

		for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
			//cout << NI.GetId() << endl;
			CandMaintain.AddDat(NI.GetId(), 0);
			deg.AddDat(NI.GetId(), NI.GetDeg());
		}

		//step2.将处于max_cliques的点从CandMaintain设为1，表示已被选入子图
		//TODO：直接将max_cliques中的点删除
		//TUNGraph G_temp = *G;
		for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
			for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
				CandMaintain.GetDat(*NI) = 1;
				if (G_temp.IsNode(*NI)) G_temp.DelNode(*NI);
			}
		}
		cout << "G_temp: " << G_temp.GetNodes() << endl;


		deg.SortByDat(); //non-decreasing order

		#pragma omp parallel
		{
			//int threadId = omp_get_thread_num();
			/*cout << omp_get_num_threads() << endl;*/
			#pragma omp for private(G_C)
			for (int i = 0; i < deg.Len(); i++) {
				/*cout << i << ": " << deg[i] << endl;*/
				int v = CandMaintain.GetKey(i);
				if (CandMaintain.GetDat(v) == 0) {
					G_C = LkVCS(G, k, v, alpha);
					/*cout << "G_C: " << G_C.Len() << endl;*/
					/*cout<< ++i <<endl;*/
					if (G_C.Empty()) continue;

					#pragma omp critical
						G_S.Add(G_C); 
					#pragma omp critical
						for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {

							CandMaintain.GetDat(*TI) = 1;
						}
				}
			}
		}
		G_S.Merge();
	}	
	else if (strcmp(seed.c_str(), "k'-vcc+clique") == 0) {
		TUNGraph G_temp = *G;

		for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
			//cout << NI.GetId() << endl;
			CandMaintain.AddDat(NI.GetId(), 0);
			deg.AddDat(NI.GetId(), NI.GetDeg());
		}

		TCliqueOverlap::GetMaxCliques(G, k + 1, G_S);

		for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
			for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
				CandMaintain.GetDat(*NI) = 1;
				if (G_temp.IsNode(*NI)) G_temp.DelNode(*NI);

				//cout << i++ << ": " << CandMaintain.GetDat(*NI) << endl;

			}
		}
		cout << "G_temp(after-mc): " << G_temp.GetNodes() << endl;
		if (G_temp.GetNodes() != 0) {
			int k_prime = k + 11;
			VCCE_S VCCE_S(G, k_prime, 1);
			PUNGraph tmp = TSnap::GetKCore(G, k_prime);
			printf("G(%d-core): \nnode_nums = %d, edge_nums = %d\n", k_prime, tmp->GetNodes(), tmp->GetEdges());
			VCCE_S.dataset = dataset;
			TIntVIntV k_prime_vcc = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);
			G_S.AddV(k_prime_vcc);
		}
		
		G_S.Merge();

		//step2.将处于max_cliques的点从CandMaintain设为1，表示已被选入子图
		//TODO：直接将max_cliques中的点删除
		//TUNGraph G_temp = *G;
		for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
			for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
				CandMaintain.GetDat(*NI) = 1;
				if (G_temp.IsNode(*NI)) G_temp.DelNode(*NI);

				//cout << i++ << ": " << CandMaintain.GetDat(*NI) << endl;

			}
		}
		cout << "G_temp: " << G_temp.GetNodes() << endl;
		//cout << G_S.Len() << endl;

		//int i = 0;

		deg.SortByDat(); //non-decreasing order

#pragma omp parallel
		{
			//int threadId = omp_get_thread_num();
			/*cout << omp_get_num_threads() << endl;*/
#pragma omp for private(G_C)
			for (int i = 0; i < deg.Len(); i++) {
				/*cout << i << ": " << deg[i] << endl;*/
				int v = CandMaintain.GetKey(i);
				if (CandMaintain.GetDat(v) == 0) {
					/*if (true) {*/
						/*cout << v << " " << deg.GetDat(v) << endl;*/
					G_C = LkVCS(G, k, v, alpha);
					/*cout << "G_C: " << G_C.Len() << endl;*/
					/*cout<< ++i <<endl;*/
					if (G_C.Empty()) continue;

#pragma omp critical
					G_S.Add(G_C); //怎么判断是否相等（Unique）？
#pragma omp critical
					for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {

						CandMaintain.GetDat(*TI) = 1;
					}

				}


			}

		}
		G_S.Merge();

	}
	else if (strcmp(seed.c_str(), "LkVCS+clique+SG") == 0) { //side-groups

		double seed_t1 = omp_get_wtime();
		for (TUNGraph::TNodeI NI = G->BegNI(); NI < G->EndNI(); NI++) {
			//cout << NI.GetId() << endl;
			CandMaintain.AddDat(NI.GetId(), 0);
			deg.AddDat(NI.GetId(), NI.GetDeg());
		}

		TIntVIntV mc;
		PUNGraph G_temp = TUNGraph::New();
		*G_temp = *G;
		//TCliqueOverlap::GetMaxCliques(G, k + 1, G_S);
		/*PUNGraph G_tmp = TSnap::LoadEdgeList<PUNGraph>("./dataset/" + dataset + ".txt", 0, 1);*/
		
		printf("G(before SC): \nnode_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());
		Compute_SC(k, G_S, G);

		printf("Compute Sparese certificate Time: %.2f\n", omp_get_wtime()-seed_t1);

		double seed_t2 = omp_get_wtime();

		//printf("G_tmp: \nnode_nums = %d, edge_nums = %d\n", k, G_tmp->GetNodes(), G_tmp->GetEdges());
		printf("G_temp(after SC): \nnode_nums = %d, edge_nums = %d\n", G_temp->GetNodes(), G_temp->GetEdges());
		printf("G(after SC): \nnode_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());
		TCliqueOverlap::GetMaxCliques(G, k + 1, mc);
		printf("Compute Maximal Cliques Time: %.2f\n", omp_get_wtime()-seed_t2);
		G_S.AddV(mc);
		G_S.Merge();

		double seed_t3 = omp_get_wtime();
		

		// for (int i = 0; i < G_S.Len(); i++) {
		// 	for (int j = 0; j < G_S[i].Len(); j++) {
		// 		if (G_temp->IsNode(G_S[i][j]))
		// 			G_temp->DelNode(G_S[i][j]);
		// 	}
		// }

		for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
			for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
				if (G_temp->IsNode(*NI))
					G_temp->DelNode(*NI);
				CandMaintain.GetDat(*NI) = 1;
			}
		}
		cout << "G_temp(after-mc): " << G_temp->GetNodes() << endl;
		
		/*if (G_temp.GetNodes() != 0) {
			int k_prime = k + 11;
			VCCE_S VCCE_S(G, k_prime, 1);
			PUNGraph tmp = TSnap::GetKCore(G, k_prime);
			printf("G(%d-core): \nnode_nums = %d, edge_nums = %d\n", k_prime, tmp->GetNodes(), tmp->GetEdges());
			VCCE_S.dataset = dataset;
			TIntVIntV k_prime_vcc = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);
			G_S.AddV(k_prime_vcc);
		}*/

		G_S.Merge();

		

		deg.SortByDat(); //non-decreasing order

		#pragma omp parallel shared(CandMaintain, G_S) private(G_C)
		{			
			#pragma omp for 
				for (int i = 0; i < deg.Len(); i++) {
					/*cout << i << ": " << deg[i] << endl;*/
					int v = CandMaintain.GetKey(i);
					if (CandMaintain.GetDat(v) == 0) {

						G_C = LkVCS(G, k, v, alpha);

						if (G_C.Empty()) continue;

						#pragma omp critical
							{
								G_S.Add(G_C); //怎么判断是否相等（Unique）？
								for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {

									CandMaintain.GetDat(*TI) = 1;
								}
							}
												

					}


				}

		}
		printf("Compute LkVCS Time: %.2f\n", omp_get_wtime()-seed_t3);
		G_S.Merge();
		
	}
	else if (strcmp(seed.c_str(), "SG+mc") == 0) { //side-groups
		TIntVIntV sg;
		TCliqueOverlap::GetMaxCliques(G, k + 1, G_S);
		/*PUNGraph G_tmp = TSnap::LoadEdgeList<PUNGraph>("./dataset/" + dataset + ".txt", 0, 1);*/
		
		printf("G(before SC): \nnode_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());
		Compute_SC(k, G_S, G);
		//printf("G_tmp: \nnode_nums = %d, edge_nums = %d\n", k, G_tmp->GetNodes(), G_tmp->GetEdges());
		printf("G(after SC): \nnode_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());
		G_S.AddV(sg);
		G_S.Merge();
	}
	else if (strcmp(seed.c_str(), "SG+clique+LkVCS") == 0) { //side-groups

		

		TIntVIntV mc;
		PUNGraph G_temp = TUNGraph::New();
		*G_temp = *G;
		//TCliqueOverlap::GetMaxCliques(G, k + 1, G_S);
		/*PUNGraph G_tmp = TSnap::LoadEdgeList<PUNGraph>("./dataset/" + dataset + ".txt", 0, 1);*/

		printf("G(before SC): \nnode_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());
		Compute_SC(k, G_S, G);
		

		//printf("G_tmp: \nnode_nums = %d, edge_nums = %d\n", k, G_tmp->GetNodes(), G_tmp->GetEdges());
		// printf("G_temp(after SC): \nnode_nums = %d, edge_nums = %d\n", G_temp->GetNodes(), G_temp->GetEdges());
		printf("G(after SC): \nnode_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());
		TCliqueOverlap::GetMaxCliques(G, k + 1, mc);
		G_S.AddV(mc);
		G_S.Merge();


		for (int i = 0; i < G_S.Len(); i++) {
			for (int j = 0; j < G_S[i].Len(); j++) {
				if (G_temp->IsNode(G_S[i][j]))
					G_temp->DelNode(G_S[i][j]);
			}
		}

		cout << "G_temp(after-mc): " << G_temp->GetNodes() << endl;

		for (TUNGraph::TNodeI NI = G_temp->BegNI(); NI < G_temp->EndNI(); NI++) {

			CandMaintain.AddDat(NI.GetId(), 0);
			deg.AddDat(NI.GetId(), NI.GetDeg());
		}
		
		/*if (G_temp.GetNodes() != 0) {
			int k_prime = k + 11;
			VCCE_S VCCE_S(G, k_prime, 1);
			PUNGraph tmp = TSnap::GetKCore(G, k_prime);
			printf("G(%d-core): \nnode_nums = %d, edge_nums = %d\n", k_prime, tmp->GetNodes(), tmp->GetEdges());
			VCCE_S.dataset = dataset;
			TIntVIntV k_prime_vcc = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);
			G_S.AddV(k_prime_vcc);
		}*/

		G_S.Merge();

		

		deg.SortByDat(); //non-decreasing order

		#pragma omp parallel shared(CandMaintain, G_S) private(G_C)
		{			
			#pragma omp for 
				for (int i = 0; i < deg.Len(); i++) {
					/*cout << i << ": " << deg[i] << endl;*/
					int v = CandMaintain.GetKey(i);
					if (CandMaintain.GetDat(v) == 0) {

						G_C = LkVCS(G_temp, k, v, alpha);

						if (G_C.Empty()) continue;

						#pragma omp critical
							{
								G_S.Add(G_C); //怎么判断是否相等（Unique）？
								for (TIntV::TIter TI = G_C.BegI(); TI < G_C.EndI(); TI++) {

									CandMaintain.GetDat(*TI) = 1;
								}
							}
												

					}


				}

		}
		G_S.Merge();
		
	}


	TFOut outFile("./seedgraph/" + dataset + "_k=" + TStr(k_str) + "_alpha=" + TStr(alpha_str) + "_seed=" + TStr(seed.c_str()) + ".seed");
	G_S.Save(outFile);

	/*for (TIntVIntV::TIter TI = G_S.BegI(); TI < G_S.EndI(); TI++) {
		cout << TI->Len() << " ";
	}
	cout << endl;*/
	return G_S;
}


void BkVCC::Expanding(int k, TIntVIntV& G_S) {
	double t2 = omp_get_wtime( );
	//int u;
	int seed_num = 0;
	if (threads <= 1) {
		TIntV delta_S, delta_S_bar;
		for (TIntVIntV::TIter GI = G_S.BegI(); GI < G_S.EndI(); GI++) {
			
			//TODO:修改获取cand的方式
			// 通过IMF算法来将多个点同时expand,而这些点单独expand时是不满足条件的
			int cand;
			cout << "seed(" << seed_num++ << "):";
			do {
				if (strcmp(expandMethod.c_str(), "flow") == 0) {
					cand = GetCand1(*GI, delta_S, delta_S_bar);
				}
				else if (strcmp(expandMethod.c_str(), "nbr") == 0) {
					cand = flag3(*GI, delta_S, delta_S_bar);
				}
				else if (strcmp(expandMethod.c_str(), "ring") == 0) {
					cand = ring_expanding(*GI, delta_S, delta_S_bar);
					/*for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
						cout << *NI << " ";
					}
					cout << endl;*/
				}

				//cout << "cand:" << cand << endl;

			} while (cand != 0);

			//while (!cand.Empty()) { //exist u belong delta_S_bar, intersection > k

			//	/*Update(G, *GI, u);*/
			//	GI->AddVMerged(cand);
			//	/*for (TIntV::TIter NI = cand.BegI(); NI < cand.EndI(); NI++) {
			//		cout << *NI << " ";
			//	}
			//	cout << endl;*/
			//	//cout << "Nodes:" <<cand.Len() << endl;  //Test if graph updates valid
			//	cand = flag3(*GI, delta_S, delta_S_bar);
			//}
		}
	}
	else if (threads >= 2) {
#pragma omp parallel 
		{
			//int threadId = omp_get_thread_num();
#pragma omp for schedule(dynamic)
			for (int i = 0; i < G_S.Len(); i++) {
				int cand;
				TIntV delta_S, delta_S_bar;
				TIntV GI = G_S.GetVal(i);
				do {
					if (strcmp(expandMethod.c_str(), "flow") == 0) {
						cand = GetCand(GI, delta_S, delta_S_bar);
					}
					else if (strcmp(expandMethod.c_str(), "nbr") == 0) {
						cand = flag3(GI, delta_S, delta_S_bar);
					}
					else if (strcmp(expandMethod.c_str(), "ring") == 0) {
						cand = ring_expanding(GI, delta_S, delta_S_bar);
						
					}
					//cout << "cand:" << cand << endl;

				} while (cand != 0);
				G_S.GetVal(i) = GI;
				//std::cout << "thread: " << threadId << endl;
			}
		}
	}
	G_S.Merge();
	_time2 += (double)(omp_get_wtime( ) - t2);

	return;


}

//用于ring_expanding中节点加入到local-kvcc中， 其邻居更新到local-kvcc边数

void update_neighbour(TIntVIntV& S, TIntIntVH& in_neighs, TIntIntVH& out_neighs, int v, TIntV& res, int level = 0) {
	TIntV neigh = out_neighs.GetDat(v);
	//cout << neigh.Len() << endl;

	//cout << level << endl;
	for (TIntV::TIter NI = neigh.BegI(); NI < neigh.EndI(); NI++) {
		/*cout << v <<" " << *NI << endl;*/
		if (in_neighs.GetDat(*NI).Len() != 0) {
			out_neighs.GetDat(*NI).DelIfIn(v);
			in_neighs.GetDat(*NI).Add(v);
		}

	}
	for (TIntV::TIter NI = neigh.BegI(); NI < neigh.EndI(); NI++) {
		int idx = in_neighs.GetDat(*NI).Len() - 1; //上面的循环已经更新过，所以这里为获得idx初始值应该-1
		/*cout << idx << endl;*/
		if (idx < 1) continue;
		if (idx == k - 1) {
			S[idx].DelIfIn(*NI);
			//S[idx + 1].AddMerged(*NI);
			res.Add(*NI);
			
			in_neighs.GetDat(*NI) = {};
			
			update_neighbour(S, in_neighs, out_neighs, *NI, res, level++);
		}
		else if (idx < k - 1) {
			S[idx].DelIfIn(*NI);
			S[idx + 1].AddMerged(*NI);
		}

	}
}

int BkVCC::ring_expanding(TIntV& G_S, TIntV& delta_S, TIntV& delta_S_bar) {
	//step1. 仅考虑每个local-kvcc的边界节点和一跳邻居节点之间的边(诱导子图)
	delta_S = GetBoundary(G_S, delta_S_bar);
	TIntV G_sub = delta_S;
	G_sub.AddVMerged(delta_S_bar);
	PUNGraph Gra = TSnap::GetSubGraph(G, G_sub);

	//step2. 每个节点计算自己到local-kvcc的边个数，分成集合S_(k+), S_(k-1), S_(k-2), ...
	// 使用二维数组存储集合
	TIntVIntV S;
	for (int i = 0; i <= k; i++) {
		S.Add({});
	}
	TIntV res = {};
	TIntV nb_u1 = {}, nb_u2 = {};
	//in_neighs存储下每个候选节点的属于local-kvcc的邻居，节省计算时间
	//out_neighs存储下每个候选节点的属于delta_S_bar的邻居
	TIntIntVH in_neighs, out_neighs;

	for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
		//cout << *TI << endl;
		nb_u1.Clr();
		nb_u2.Clr();
		//nb_u = getneighbor(G, *TI);
		TUNGraph::TNodeI CtrNode = Gra->GetNI(*TI);
		for (int i = 0; i < CtrNode.GetInDeg(); ++i) {
			if (delta_S.IsIn(CtrNode.GetInNId(i))) //只把local-kvcc中的邻居加进去
				nb_u1.AddMerged(CtrNode.GetInNId(i));
			else
				nb_u2.AddMerged(CtrNode.GetInNId(i));


		}
		in_neighs.AddDat(*TI, nb_u1);
		out_neighs.AddDat(*TI, nb_u2);
		int index = nb_u1.Len();

		//index为S集合下标索引
		if (index >= k) index = k; //这里k统一存储k+的节点
		S[index].AddMerged(*TI);

	}

	//step3. S_(k+)的点首先加入到kvcc中，其邻居更新到kvcc的边数，更新S
	int S_Len;

	for (TIntV::TIter TI = S[k].BegI(); TI < S[k].EndI(); TI++) {
		res.Add(*TI);
		in_neighs.GetDat(*TI) = {};
		/*if (res[20] == 50825) {
			for (TIntV::TIter NI = res.BegI(); NI < res.EndI(); NI++) {
				cout << *NI << " ";
			}
			cout << endl;
		}*/
		update_neighbour(S, in_neighs, out_neighs, *TI, res);

		
	}


	//step4. 从S_(k-1)开始，找clique，具体来讲S_(k-1)找2-clique，S_(k-2)找3-clique，S_(k-3)找4-clique，...,S_(k-i)找size>= (i+1)的maximal clique
	//然后判断这些clique在local-kvcc中并集的邻居的个数是否$\geq k$
	//若是，则同时加入到local-kvcc中，并更新其周围邻居。
	int num = 0;
	TIntV S_total;
	//int stop_num =  k - G_S.Len()
	for (int i = k - 1; i > 0; i--) {
		TIntVIntV cliques;
		TIntV neigh_Union;
		TIntV S_i_temp;
		do{
			S_i_temp = S[i];
			S_total.Clr();

			S_total.AddVMerged(S[i]);

			//S_remain.Clr();
			if (S_total.Len() < k - i + 1) continue;
			PUNGraph sub_G = TSnap::GetSubGraph(Gra, S_total);
			PUNGraph sub_core = TSnap::GetKCore(sub_G, k - i);
			//printf("sub_G: \nnode_nums = %d, edge_nums = %d\n", sub_G->GetNodes(), sub_G->GetEdges());
			/*printf("sub_core: \nnode_nums = %d, edge_nums = %d\n", sub_core->GetNodes(), sub_core->GetEdges());*/
			TCliqueOverlap::GetMaxCliques(sub_core, k - i + 1, cliques);
			//TCliqueOverlap::GetCliquesSizeof(sub_core, k - i + 1, cliques);
			/*cout <<"i=" << i << ":" << cliques.Len() << endl;*/

			for (int clique_idx = 0; clique_idx < cliques.Len(); clique_idx++) {
				neigh_Union.Clr();
				int flag = 0;
				for (TIntV::TIter TI = cliques[clique_idx].BegI(); TI < cliques[clique_idx].EndI(); TI++) {
					int idx = in_neighs.GetDat(*TI).Len();
					if (idx == 0) {
						flag = 1;
						break;
					}
					/*if (!S_total.IsIn(*TI)) {
						flag = 1;
						break;
					}*/
					/*cout << *TI <<" ";
					cout << "in_neigh" << endl;
					if (*TI == 7) {
						cout << "here" << in_neighs.GetDat(*TI).Len() << endl;;
					}
					for (TIntV::TIter nb = in_neighs.GetDat(*TI).BegI(); nb < in_neighs.GetDat(*TI).EndI(); nb++) {
						cout << *nb << " ";
					}
					cout << endl;*/
					neigh_Union.AddV(in_neighs.GetDat(*TI));
					neigh_Union.Merge();
					/*if (neigh_Union.Len() >= k) break;*/
				}
				/*cout << endl;*/
				if (neigh_Union.Len() >= k && flag == 0) {
					for (TIntV::TIter TI = cliques[clique_idx].BegI(); TI < cliques[clique_idx].EndI(); TI++) {

						res.Add(*TI);
						/*if (*TI == 50825)
							cout << i <<" 50825_in_nei:" << in_neighs.GetDat(*TI).Len() << endl;*/
						int idx = in_neighs.GetDat(*TI).Len();
						/*if (idx == 0) break;*/
						if (idx > k) idx = k;
						in_neighs.GetDat(*TI) = {};
						//cout << *TI << endl;
						//cout << idx << endl;
						S[idx].DelIfIn(*TI);
						S_total.DelIfIn(*TI);


						/*if (res[20] == 50825) {
							for (TIntV::TIter NI = res.BegI(); NI < res.EndI(); NI++) {
								cout << *NI << " ";
							}
							cout << endl;
						}*/

					}
					for (TIntV::TIter TI = cliques[clique_idx].BegI(); TI < cliques[clique_idx].EndI(); TI++) {

						update_neighbour(S, in_neighs, out_neighs, *TI, res);
						/*if (*TI == 4572) cout << "here 1//////////////" << endl;*/
					}

					/*num += cliques[clique_idx].Len();*/
				}
			}
			//S_remain.AddVMerged(S[i]);
			/*for (TIntV::TIter TI = S[k - i].BegI(); TI < S[k - i].EndI(); TI++) {

			}*/
		} while ((S_i_temp != S[i]));

		
	}
	/*cout << "expanding_num:" << num << endl;*/
	/*if (num != 0) {
		cout << "expanding_num:" << res.Len() << endl;
	}*/
	
	/*cout<<"1: " << res.Len() << endl;
	res.Merge();
	cout << "2: " << res.Len() << endl;*/
	TIntV G_before = G_S;
#pragma omp critical
	{
		G_S.AddV(res);
	}
	
	return res.Len();
}


int BkVCC::GetCand1(TIntV& G_S, TIntV& delta_S, TIntV& delta_S_bar) {
	TIntV res = {};
	delta_S = GetBoundary(G_S, delta_S_bar);
	TIntV nb_u;

	/*cout << "G[delta_S]:";
	for (int i = 0; i < delta_S.Len(); i++) {
		cout << delta_S[i] << " ";
	}
	cout << endl;*/

	//原有的expand方式
	//for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
	//	//cout << *TI << endl;
	//	nb_u.Clr();
	//	//nb_u = getneighbor(G, *TI);
	//	TUNGraph::TNodeI CtrNode = G->GetNI(*TI);
	//	for (int i = 0; i < CtrNode.GetInDeg(); ++i) {
	//		if (!nb_u.IsIn(CtrNode.GetInNId(i)))
	//			nb_u.Add(CtrNode.GetInNId(i));
	//	}
	//	if (TSnap::GetCommon(nb_u, delta_S) >= k) {
	//		res.AddUnique(*TI);
	//	}
	//}

	//记录每个节点的max_flow值
	TIntH flow;
	//源节点，目标节点
	int s = -1, t = -1;

	//源节点必须是非边缘点
	/*for (TIntV::TIter TI = G_S.BegI(); TI < G_S.EndI(); TI++) {

		if (!delta_S.IsIn(*TI)) { s = *TI; break; }
	}*/

	//将满足条件的节点直接expand到kvcc子图中
	//Attention:这里必须先确定source vertex之后再将进行expand，
	//不然expand的点也不属于delta_S,会误选为source vertex，结果会出错
	//cout << "G[S]:";
	//for (int i = 0; i < G_S.Len(); i++) {
	//	cout << G_S[i] << " ";
	//}
	//cout << endl;
	//G_S.AddVMerged(res);

	//cout << "G[S]:";
	//for (int i = 0; i < G_S.Len(); i++) {
	//	cout << G_S[i] << " ";
	//}
	//cout << endl;

	//if (G_S.Len() >= 100) return res.Len(); //太大就不用maxflow判断，耗时太长

	//不存在非边缘点时应该如何处理？这里是直接结束，应该做进一步考虑
	/*if (s == -1) { 

		cout << "end" << endl; 
		return res.Len();
	}*/

	//为便于最大流计算，将满足条件的节点从delta_S_bar删除
	/*for (TIntV::TIter TI = res.BegI(); TI < res.EndI(); TI++) {
		delta_S_bar.DelIfIn(*TI);
	}*/
	TIntV G_U = G_S;
	/*printf("G_U: \n node_nums = %d\n", G_U.Len());
	for (TIntV::TIter NI = G_U.BegI(); NI < G_U.EndI(); NI++) {
		cout << *NI << " ";
	}*/
	
	G_U.AddVMerged(delta_S_bar);

	//cout << s << endl;
	TIntV delta_S_bar_star;//记录候选顶点

	PUNGraph Gra_temp = TSnap::GetSubGraph(G, G_U);
	//cout << Gra_temp->GetNodes() << endl;
	PUNGraph Gra = TSnap::GetKCore(Gra_temp, k);
	for (TIntV::TIter NI = delta_S_bar.BegI(); NI < delta_S_bar.EndI(); NI++) {
		if (Gra->IsNode(*NI)) delta_S_bar_star.Add(*NI);
	}
	delta_S_bar = delta_S_bar_star;

	
	//cout << Gra->GetNodes() << endl;
	//不存在非边缘点时，添加虚拟的非边缘点
	//if (s == -1) {
	//printf("G(before): \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(Gra), TSnap::CntUniqUndirEdges(Gra));
	if (true) {
		int virtual_node = Gra->GetMxNId();
		Gra->AddNode(virtual_node);
		for (TUNGraph::TNodeI NI = Gra->BegNI(); NI < Gra->EndNI(); NI++) {
			if (G_S.IsIn(NI.GetId())) {
				Gra->AddEdge(virtual_node, NI.GetId());
			}
			
		}
		s = virtual_node;
	}
	//cout << "s=" << s << endl;

	PNEANet DG = Construct_DG(Gra);
	int offset = Gra->GetMxNId();
	int num = 0;
	/*TIntH deg;

	for (TIntV::TIter  NI = delta_S_bar.BegI(); NI < delta_S_bar.EndI(); NI++) {
		deg.AddDat(*NI, Gra->GetNI(*NI).GetDeg());
	}
	deg.SortByDat();
	delta_S_bar.Clr();
	int node;
	for (TIntH::TIter NI = deg.BegI(); NI < deg.EndI(); NI++) {
		node = NI.GetKey();
		delta_S_bar.Add(node);
	}

	delta_S_bar.Sort();*/
	/*cout << "delta_S_bar_Len(after): " << delta_S_bar.Len();
	cout << "delta_S_bar(after): ";
	for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
		cout << *TI << " ";
	}
	cout << endl;*/
	

	while (true) {
		
		TIntV ResNet;
		TIntV G_delete = {};
		TIntV S = {};
		int loc = 0;
		for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
			t = *TI;
			S = Loc_Cut(s, t, DG, Gra, k);
			num++;
			loc++;
			if (S.Empty()) {
				continue;	
			}			
			else {
				/*cout << "S: ";
				for (int i = 0; i < S.Len(); i++) {
					cout << S[i] << " ";
				}
				cout << endl;*/
				break;
			}
		}
		if (S.Empty()) {
			break;
		}
		else {
			PUNGraph temp = TUNGraph::New();
			*temp = *Gra;
			//cout << Gra->GetNodes() << endl;
			TSnap::DelNodes(temp, S);
			//cout << Gra->GetNodes();
			TCnComV ConV;
			TSnap::GetSccs(temp, ConV);
			for (TCnComV::TIter I = ConV.BegI(); I < ConV.EndI(); I++) {
				/*for (int i = 0; i < I->Len(); i++) {
					cout << I->GetVal(i) << " ";
				}
				cout << endl;*/
				I->Sort();
				if (I->IsNIdIn(s)) {
					//cout << "S: ";
					for (int i = 0; i < S.Len(); i++) {
						//cout << S[i] << " ";
						I->Add(S[i]);
					}
					//cout << endl;
					I->Sort();
					/*for (TIntV::TIter NI = S.BegI(); NI < S.EndI(); NI++) {
						I->Add(NI->Val);			
					}*/
					for (TIntV::TIter NI = delta_S_bar.BegI(); NI < delta_S_bar.EndI(); NI++) {
						if (!I->IsNIdIn(*NI)) Gra->DelNode(*NI);
					}
					DG = Construct_DG(Gra);
					break;
				}
				else {
					continue;
				}	
			}								
			//printf("G(after): \n node_nums = %d, edge_nums = %d\n", TSnap::CntNonZNodes(Gra), TSnap::CntUniqUndirEdges(Gra));
			
			//int offset = Gra->GetMxNId();
			delta_S_bar_star.Clr();
			for (int i = loc; i < delta_S_bar.Len(); i++) {
				int node = delta_S_bar[i];
				if (Gra->IsNode(node)) delta_S_bar_star.Add(node);  
			}
			for (int i = 0; i < loc; i++) {
				int node = delta_S_bar[i];
				if (Gra->IsNode(node)) delta_S_bar_star.Add(node);
			}
			/*for (TIntV::TIter NI = delta_S_bar.BegI(); NI < delta_S_bar.EndI(); NI++) {
				if (Gra->IsNode(*NI)) delta_S_bar_star.Add(*NI);
			}*/
			//delta_S_bar.Sort();
			delta_S_bar = delta_S_bar_star;
		  /*for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
		 		cout << *TI << " ";
			}
			cout << endl;*/
		}
			
	}
	cout << "add_num: " << delta_S_bar.Len() << endl;
	cout << "iter:" << num << endl;
	G_S.AddVMerged(delta_S_bar);
	return res.Len()+ delta_S_bar.Len();
}

TIntV BkVCC::Loc_Cut(int source, int sink, PNEANet DG, PUNGraph G, int k) {
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
		
		return {};
	}

	int lambda = TSnap::MyGetMaxFlowIntEK(DG, source, sink, ResNet, k);
	if (lambda >= k) {
	
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


	return vertex_cut;
	//compute the minimum edge cut in DG
	//return the corrrsponding vertex cut in G
}

int BkVCC::GetCand(TIntV& G_S, TIntV& delta_S, TIntV& delta_S_bar) {
	TIntV res = {};
	delta_S = GetBoundary(G_S, delta_S_bar);
	TIntV nb_u;

	/*cout << "G[delta_S]:";
	for (int i = 0; i < delta_S.Len(); i++) {
		cout << delta_S[i] << " ";	}
	cout << endl;*/

	//原有的expand方式
	//for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
	//	//cout << *TI << endl;
	//	nb_u.Clr();
	//	//nb_u = getneighbor(G, *TI);
	//	TUNGraph::TNodeI CtrNode = G->GetNI(*TI);
	//	for (int i = 0; i < CtrNode.GetInDeg(); ++i) {
	//		if (!nb_u.IsIn(CtrNode.GetInNId(i)))
	//			nb_u.Add(CtrNode.GetInNId(i));
	//	}
	//	if (TSnap::GetCommon(nb_u, delta_S) >= k) {
	//		res.AddUnique(*TI);
	//	}
	//}

	//记录每个节点的max_flow值
	TIntH flow;
	//源节点，目标节点
	int s = -1, t = -1;

	//源节点必须是非边缘点
	/*for (TIntV::TIter TI = G_S.BegI(); TI < G_S.EndI(); TI++) {

		if (!delta_S.IsIn(*TI)) { s = *TI; break; }
	}*/

	//将满足条件的节点直接expand到kvcc子图中
	//Attention:这里必须先确定source vertex之后再将进行expand，
	//不然expand的点也不属于delta_S,会误选为source vertex，结果会出错
	//cout << "G[S]:";
	//for (int i = 0; i < G_S.Len(); i++) {
	//	cout << G_S[i] << " ";
	//}
	//cout << endl;
	G_S.AddVMerged(res);

	//cout << "G[S]:";
	//for (int i = 0; i < G_S.Len(); i++) {
	//	cout << G_S[i] << " ";
	//}
	//cout << endl;

	//if (G_S.Len() >= 100) return res.Len(); //太大就不用maxflow判断，耗时太长

	//不存在非边缘点时应该如何处理？这里是直接结束，应该做进一步考虑
	/*if (s == -1) { 

		cout << "end" << endl; 
		return res.Len();
	}*/

	//为便于最大流计算，将满足条件的节点从delta_S_bar删除
	for (TIntV::TIter TI = res.BegI(); TI < res.EndI(); TI++) {
		delta_S_bar.DelIfIn(*TI);
	}

	TIntV G_U = G_S;
	G_U.AddVMerged(delta_S_bar);

	//cout << s << endl;
	TIntV delta_S_bar_star;//记录候选顶点

	// PUNGraph Gra = TSnap::GetSubGraph(G, G_U);
	PUNGraph Gra_temp = TSnap::GetSubGraph(G, G_U);
	//cout << Gra_temp->GetNodes() << endl;
	PUNGraph Gra = TSnap::GetKCore(Gra_temp, k);
	for (TIntV::TIter NI = delta_S_bar.BegI(); NI < delta_S_bar.EndI(); NI++) {
		if (Gra->IsNode(*NI)) delta_S_bar_star.Add(*NI);
	}
	delta_S_bar = delta_S_bar_star;
	//不存在非边缘点时，添加虚拟的非边缘点
	//if (s == -1) {
	if (true) {
		int virtual_node = Gra->GetMxNId();
		Gra->AddNode(virtual_node);
		for (TUNGraph::TNodeI NI = Gra->BegNI(); NI < Gra->EndNI(); NI++) {
			if (G_S.IsIn(NI.GetId())) {
				Gra->AddEdge(virtual_node, NI.GetId());
			}
			
		}
		s = virtual_node;
	}
	PNEANet DG = Construct_DG(Gra);
	int offset = Gra->GetMxNId();
	int num=0;

	// while (true) {
	// 	delta_S_bar_star.Clr();
	// 	TIntV ResNet;
	// 	TIntV G_delete;

		
	// /*	cout <<"s:" << s << endl;
	// 	cout << "s+offset:" << s + offset << endl;*/
	// 	for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
	// 		t = *TI;
	// 		int lambda = TSnap::MyGetMaxFlowIntEK(DG, s + offset, t, ResNet, k);
	// 		if (lambda >= k) delta_S_bar_star.Add(t);
	// 		else {
	// 			G_delete.AddMerged(*TI);//移除最大流小于k的顶点	
	// 		}

	// 		//flow.AddDat(*TI, lambda);
	// 	}
		

	// 	if (delta_S_bar == delta_S_bar_star) break;

	// 	for (int i = 0; i < G_delete.Len(); i++) {
	// 		Gra->DelNode(G_delete[i]);
	// 		DG->DelNode(G_delete[i]);
	// 		DG->DelNode(G_delete[i] + offset);
	// 	}
	// 	delta_S_bar = delta_S_bar_star;
	// }
	// 是否使用按度排序
	// TIntH deg;

	// for (TIntV::TIter  NI = delta_S_bar.BegI(); NI < delta_S_bar.EndI(); NI++) {
	// 	deg.AddDat(*NI, Gra->GetNI(*NI).GetDeg());
	// }
	// deg.SortByDat();
	// delta_S_bar.Clr();
	// int node;
	// for (TIntH::TIter NI = deg.BegI(); NI < deg.EndI(); NI++) {
	// 	node = NI.GetKey();
	// 	delta_S_bar.Add(node);
	// }

	while (true) {
		//i++;
		delta_S_bar_star.Clr();
		TIntV ResNet;
		TIntV G_delete = {};

		//for (TIntV::TIter TI = delta_S_bar_star.BegI(); TI < delta_S_bar_star.EndI(); TI++) {

		for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
			t = *TI;
			int lambda = TSnap::MyGetMaxFlowIntEK(DG, s + offset, t, ResNet, k);
			num++;
			if (lambda >= k) {
				//delta_S_bar_star.Add(t);
			}
			else {
				G_delete.AddMerged(*TI);//移除最大流小于k的顶点
				TUNGraph::TNodeI Node = Gra->GetNI(*TI);
				for (int j = 0; j < Node.GetInDeg(); j++) {
					int nbrnode = Node.GetNbrNId(j);
					TUNGraph::TNodeI nbrNode = Gra->GetNI(nbrnode);
					if (nbrNode.GetInDeg() < k) {
						G_delete.AddMerged(nbrnode);
					}

				}
				break;
			}
			//flow.AddDat(*TI, lambda);
		}

		for (int i = 0; i < G_delete.Len(); i++) {
			//cout << G_delete[i] << " ";
			Gra->DelNode(G_delete[i]);
			DG->DelNode(G_delete[i]);
			DG->DelNode(G_delete[i] + offset);
			delta_S_bar.DelIfIn(G_delete[i]);			
		}
		

		if (G_delete.Len() == 0) break;
	}

	/*std::cout << "G_S(before): " << G_S.Len() << endl;*/
	G_S.AddVMerged(delta_S_bar);
	// G_S.AddVMerged(delta_S_bar_star);
	
	/*cout << "G[S]:";
	for (int i = 0; i < G_S.Len(); i++) {
		cout << G_S[i] << " ";
	}
	cout << endl;*/


	/*std::cout << "G_S(after): " << G_S.Len() << endl;*/
	//std::cout << delta_S_bar_star.Len() << endl;
	cout << "iter:" << num << endl;

	return res.Len()+ delta_S_bar_star.Len();
}

TIntV BkVCC::GetBoundary(TIntV G_S, TIntV& delta_S_bar)
{
	TIntV delta_S;
	delta_S.Clr();
	delta_S_bar.Clr();
	
	for (TIntV::TIter NI = G_S.BegI(); NI < G_S.EndI(); NI++) {
		/*cout << *NI << endl;*/
		TUNGraph::TNodeI Node = G->GetNI(*NI);
		for (int i = 0; i < Node.GetInDeg(); i++) {
			/*cout << "nbr:" << Node.GetNbrNId(i) << endl;*/
			if (!G_S.IsIn(Node.GetNbrNId(i))) {
				delta_S.Add(Node.GetId());
				delta_S_bar.Add(Node.GetNbrNId(i));
				//cout << Node.GetNbrNId(i) << endl;
				//break;
			}
		}
	}
	delta_S.Merge();
	delta_S_bar.Merge();
	//cout << delta_S.Len() << endl;
	//cout << delta_S_bar.Len() << endl;

	return delta_S;
}


int BkVCC::flag3(TIntV& G_S, TIntV& delta_S, TIntV& delta_S_bar) {
	TIntV res = {};

	delta_S = GetBoundary(G_S, delta_S_bar);
	TIntV nb_u;

	for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
		//cout << *TI << endl;
		nb_u.Clr();
		//nb_u = getneighbor(G, *TI);
		TUNGraph::TNodeI CtrNode = G->GetNI(*TI);
		for (int i = 0; i < CtrNode.GetInDeg(); ++i) {
			if (!nb_u.IsIn(CtrNode.GetInNId(i)))
				nb_u.Add(CtrNode.GetInNId(i));
		}
		if (TSnap::GetCommon(nb_u, delta_S) >= k) {
			res.AddUnique(*TI);
		}
	}

	G_S.AddVMerged(res);
	return res.Len();
}

bool BkVCC::flag3(TIntV G_S, int& u, TIntV& delta_S, TIntV& delta_S_bar)
{
	u = -1;
	delta_S = GetBoundary(G_S, delta_S_bar);
	TIntV nb_u;

	for (TIntV::TIter TI = delta_S_bar.BegI(); TI < delta_S_bar.EndI(); TI++) {
		//cout << *TI << endl;
		nb_u.Clr();
		//nb_u = getneighbor(G, *TI);
		TUNGraph::TNodeI CtrNode = G->GetNI(*TI);
		for (int i = 0; i < CtrNode.GetInDeg(); ++i) {
			if (!nb_u.IsIn(CtrNode.GetInNId(i)))
				nb_u.Add(CtrNode.GetInNId(i));
		}
		
		if (TSnap::GetCommon(nb_u, delta_S) >= k) {
			u = *TI;
			return true;
		}

	}
	return false;
}





void BkVCC::Merging(int k, TIntVIntV& G_S, TIntVIntV& G_R)
{
	double t3 = omp_get_wtime( );
	//cout << G_S.Len() << endl;
	
	// 不满足大小条件的seed删除
	for (int i = G_S.Len() - 1; i >= 0 ; i--) {
		if (G_S[i].Len() <= k) {
			G_S.Del(i);
		}
	}
	TIntVIntV G_S_temp = G_S;

	
	if (threads >= 2) {
		int i;
		TIntV GI1, GI2;
#pragma omp parallel shared(G_S, G_R) private(i, GI1, GI2) 
		{
#pragma omp for schedule(dynamic)
		
		for (int i = 0; i < G_S.Len(); i++) {
			int j = i + 1;
			while (j < G_S.Len()) {
#pragma omp critical
				{
					GI1 = G_S[i];
					GI2 = G_S[j];
				}
				if (strcmp(mergeMethod.c_str(), "flow") == 0) {
					if (IsMergeValid_maxflow(GI1, GI2)) {
						TIntV G_Union = {};
						G_Union.AddVMerged(GI1);
						G_Union.AddVMerged(GI2);
		#pragma omp critical
						{
							G_S.Add(G_Union);
							G_S.DelIfIn(GI1);
							G_S.DelIfIn(GI2);
						}
						// std::cout << "After Merging:" << G_Union.Len() << endl;
						// std::cout << "G_S.Len:" << G_S.Len() << endl;
						/*i--;
						j = i + 1;*/
						continue;
					}
					else {
						j++;
					}
				}
				else {
					if (IsMergeValid(GI1, GI2)) {


						TIntV G_Union = {};
						G_Union.AddVMerged(GI1);
						G_Union.AddVMerged(GI2);

#pragma omp critical
						{
							if (IskVCC(G_Union, k)) {
								G_R.Add(G_Union);
								G_S.DelIfIn(GI1);
								G_S.DelIfIn(GI2);
								//G_R.Merge();
							}
							else {
								G_S.Add(G_Union);
								G_S.DelIfIn(GI1);
								G_S.DelIfIn(GI2);
								//G_S.Merge();
							}
						}
						// std::cout << "After Merging:" << G_Union.Len() << endl;
						// std::cout << "G_S.Len:" << G_S.Len() << endl;
						/*i--;
						j = i + 1;*/
						continue;

					}
					else {
						j++;
					}
				}
			}
		}


		}
	}
	else if (threads <= 1) {
	
		//最简单的遍历，有没有时间复杂度更低的算法
		for (int i = 0; i < G_S.Len(); ) {
			for (int j = i + 1; j < G_S.Len();) {
				TIntV GI1 = G_S[i];
				TIntV GI2 = G_S[j];
				/*cout << "G_S.Len:" << G_S.Len()<<endl;
				cout << "i:" << i << endl;
				cout << "j:" << j << endl;*/
				if (strcmp(mergeMethod.c_str(), "flow") == 0) {
					if (IsMergeValid_maxflow(GI1, GI2)) {


						TIntV G_Union = {};
						G_Union.AddVMerged(GI1);
						G_Union.AddVMerged(GI2);
						
						//if (IskVCC(G_Union, k)) 
						if (false) {
							G_R.Add(G_Union);
							G_S.DelIfIn(GI1);
							G_S.DelIfIn(GI2);
							//G_R.Merge();
						}
						else {
							G_S.Add(G_Union);
							G_S.DelIfIn(GI1);
							G_S.DelIfIn(GI2);
							//G_S.Merge();
						}
						// std::cout << "After Merging:" << G_Union.Len() << endl;
						// std::cout << "G_S.Len:" << G_S.Len() << endl;
						continue;
					}
					j++;
				}
				else {
					if (IsMergeValid(GI1, GI2)) {


						TIntV G_Union = {};
						G_Union.AddVMerged(GI1);
						G_Union.AddVMerged(GI2);
						
						if (IskVCC(G_Union, k)) {
							G_R.Add(G_Union);
							G_S.DelIfIn(GI1);
							G_S.DelIfIn(GI2);
							//G_R.Merge();
						}
						else {
							G_S.Add(G_Union);
							G_S.DelIfIn(GI1);
							G_S.DelIfIn(GI2);
							//G_S.Merge();
						}
						// std::cout << "After Merging:" << G_Union.Len() << endl;
						// std::cout << "G_S.Len:" << G_S.Len() << endl;
						continue;
					}
					j++;
				}

			}
			i++;
		}
	}
	if(strcmp(mergeMethod.c_str(), "old") == 0) {
		for (TIntVIntV::TIter GI = G_S_temp.BegI(); GI < G_S_temp.EndI(); GI++) {
			/*cout << "GI: ";
			for (TIntV::TIter TI = GI->BegI(); TI < GI->EndI(); TI++) {
				cout << *TI << " ";
			}
			cout << endl;*/
			if (IskVCC(*GI, k)) {
				G_R.Add(*GI);
				G_S.DelIfIn(*GI);
			}
		}
	}
	

	_time3 += (double)(omp_get_wtime( ) - t3) ;
	return;
}

//为什么这里同样的代码能出现不一样的结果？laugh_cry..
TPrVIntV BkVCC::flag4(TIntVIntV G_S)
{
	TPrVIntV G_Merge_cand = {};
	for (TIntVIntV::TIter GI1 = G_S.BegI(); GI1 < G_S.EndI(); GI1++) {
		for (TIntVIntV::TIter GI2 = GI1; GI2 < G_S.EndI(); GI2++) {
			if (*GI2 == *GI1) continue;
			if (IsMergeValid(*GI1, *GI2)) {
				//G1 = *GI1;
				//G2 = *GI2;
				G_Merge_cand.AddMerged(TPair<TIntV, TIntV>(*GI1, *GI2));
				//return true;
			}
		}
	}
	//return false;
	return G_Merge_cand;
}

bool BkVCC::IskVCC(TIntV G_S, int k)
{
	//Corollary 1
	int u;
	TIntV delta_S, delta_S_bar;
	if (!flag3(G_S, u, delta_S, delta_S_bar) && min(delta_S.Len(), delta_S_bar.Len()) < k) {
		return true;
	}
	return false;
}

bool BkVCC::IsMergeValid(TIntV G_S, TIntV G_S_prime)
{
	//Theorem 7
	TIntV S_Nodes, S_prime_Nodes, S_ints_S_prime, S_sub_S_prime, S_prime_sub_S;
	TIntV nb_1 = {}, nb_2 = {};
	S_Nodes = G_S;
	S_prime_Nodes = G_S_prime;
	//S_ints_S_prime.Clr();
	//S_sub_S_prime.Clr();
	//S_prime_sub_S.Clr();
	//G_S->GetNIdV(S_Nodes);
	//G_S_prime->GetNIdV(S_prime_Nodes);
	for (TIntV::TIter TI = S_Nodes.BegI(); TI < S_Nodes.EndI(); TI++) {
		if (S_prime_Nodes.IsIn(*TI)) {
			S_ints_S_prime.Add(*TI);
		}
		//公共顶点>=k，直接返回
		if (S_ints_S_prime.Len() >= k) return true;
	}

	//把加入改成删除
	S_sub_S_prime = S_Nodes;
	S_prime_sub_S = S_prime_Nodes;
	for (TIntV::TIter TI = S_ints_S_prime.BegI(); TI < S_ints_S_prime.EndI(); TI++) {
		S_sub_S_prime.DelIfIn(*TI);
		S_prime_sub_S.DelIfIn(*TI);
	}


	for (TIntV::TIter TI = S_sub_S_prime.BegI(); TI < S_sub_S_prime.EndI(); TI++) {
		TUNGraph::TNodeI NI = G->GetNI(*TI);
		for (int i = 0; i < NI.GetInDeg(); i++) {
			if (S_prime_sub_S.IsIn(NI.GetNbrNId(i))) {
				nb_2.AddUnique(*TI);
				nb_1.AddUnique(NI.GetNbrNId(i));
				//如果nb_1够大就不需要继续计算，nb_1不会是限制条件
				//if (nb_1.Len() >= k - S_ints_S_prime.Len()) break;
			}
		}
		//跳出双重循环
		//if (nb_1.Len() >= k - S_ints_S_prime.Len()) break;
	}

	if (S_ints_S_prime.Len() + min(nb_1.Len(), nb_2.Len()) >= k) {
		return true;
	}

	return false;
}

bool BkVCC::IsMergeValid_maxflow(TIntV G_S, TIntV G_S_prime) {

	TIntV S_Nodes, S_prime_Nodes, S_ints_S_prime, S_sub_S_prime, S_prime_sub_S, delta_S, delta_S_prime;
	TIntV nb_1 = {}, nb_2 = {};
	S_Nodes = G_S;
	S_prime_Nodes = G_S_prime;
	int s = -1, t = -1;


	for (TIntV::TIter TI = S_Nodes.BegI(); TI < S_Nodes.EndI(); TI++) {
		if (S_prime_Nodes.IsIn(*TI)) {
			S_ints_S_prime.Add(*TI);
		}
		//公共顶点>=k，直接返回
		if (S_ints_S_prime.Len() >= k) return true;
	}



	S_sub_S_prime = S_Nodes;
	S_prime_sub_S = S_prime_Nodes;
	for (TIntV::TIter TI = S_ints_S_prime.BegI(); TI < S_ints_S_prime.EndI(); TI++) {
		S_sub_S_prime.DelIfIn(*TI);
		S_prime_sub_S.DelIfIn(*TI);
	}

	TUNGraph::TNodeI NI;

	
	//找到delta_S 和 delta_S_prime
	//这里的delta_S指的是在另一个子图中有邻居的顶点集合
	//10/18 这里有必要全部找到吗，是不是找到一个没有邻居在对方集合中的就可以停止
	for (TIntV::TIter TI = S_sub_S_prime.BegI(); TI < S_sub_S_prime.EndI(); TI++) {
		 NI = G->GetNI(*TI);

		for (int i = 0; i < NI.GetInDeg(); i++) {
			if (S_prime_sub_S.IsIn(NI.GetNbrNId(i))) {
				delta_S.AddUnique(*TI);
				delta_S_prime.AddUnique(NI.GetNbrNId(i));
			}
		}
	}

	if (min(delta_S.Len(), delta_S_prime.Len()) + S_ints_S_prime.Len() < k) return false;

	/*for (TIntV::TIter TI = S_prime_sub_S.BegI(); TI < S_prime_sub_S.EndI(); TI++) {
		if (!delta_S_prime.IsIn(*TI)) {
			s = *TI;
			break;
		}
	}


	for (TIntV::TIter TI = S_sub_S_prime.BegI(); TI < S_sub_S_prime.EndI(); TI++) {
		if (!delta_S.IsIn(*TI)) {
			t = *TI;
			break;
		}

	}*/


	TIntV G_U = {};
	G_U.AddVMerged(G_S);
	G_U.AddVMerged(G_S_prime);
	TIntV ResNet = {};

	PUNGraph Gra = TSnap::GetSubGraph(G, G_U);
	int sigma = Gra->GetMxNId();
	int tau = Gra->GetMxNId() + 1;
	Gra->AddNode(sigma);
	Gra->AddNode(tau);
	for (TUNGraph::TNodeI NI = Gra->BegNI(); NI < Gra->EndNI(); NI++) {
		if (G_S.IsIn(NI.GetId())) {
			Gra->AddEdge(sigma, NI.GetId());
		}

	}
	for (TUNGraph::TNodeI NI = Gra->BegNI(); NI < Gra->EndNI(); NI++) {
		if (G_S_prime.IsIn(NI.GetId())) {
			Gra->AddEdge(tau, NI.GetId());
		}

	}
	s = sigma;
	t = tau;

	// [24] Lutz Volkmann, On local connectivity of graphs, Appl. Math. Lett. 21 (1) (2008) 63–66. 
	//cout << Gra->GetNI(s).GetDeg() + Gra->GetNI(t).GetDeg() + 2 - Gra->GetNodes() << endl;
	//if (Gra->GetNI(s).GetDeg() + Gra->GetNI(t).GetDeg() + 2 - Gra->GetNodes() >= k) { //Lower-bound-based reduction rule 

	//	cout << "here" << endl;
	//	return true;
	//}
		
	
	PNEANet DG = Construct_DG(Gra);

	if (s != -1 && t != -1) {
		int offset = Gra->GetMxNId();
		int lambda = TSnap::MyGetMaxFlowIntEK(DG, s + offset, t, ResNet, k);
		if (lambda + S_ints_S_prime.Len() >= k) {
			return true;
		}
	}

	return false;

}



PNEANet BkVCC::Construct_DG(PUNGraph G)
{
	//Construct corresponding attribute directed graph,
	//prepare for maximum flow calculate.(According to 'Effective K-Vertex connected 
	//component detection in large-scale networks')
	PNEANet DG = TNEANet::New();
	DG->AddIntAttrE("capacity", 0);
	int offset = G->GetMxNId();
	int i, eid;
	//step 1
	TUNGraph::TNodeI NI;
	for (NI = G->BegNI(); NI < G->EndNI(); NI++) {
		i = NI.GetId();
		DG->AddNode(i); //v
		DG->AddNode(i + offset); //v'
		eid = DG->AddEdge(i, i + offset);
		DG->AddIntAttrDatE(eid, 1, "capacity");

	}

	//step 2
	TUNGraph::TEdgeI EI;
	for (EI = G->BegEI(); EI < G->EndEI(); EI++) {
		// u -> v
		eid = DG->AddEdge(EI.GetSrcNId() + offset, EI.GetDstNId());  //u''->v'
		DG->AddIntAttrDatE(eid, 1, "capacity");
		eid = DG->AddEdge(EI.GetDstNId() + offset, EI.GetSrcNId()); //v''->u'
		DG->AddIntAttrDatE(eid, 1, "capacity");
	}

	return  DG;
}


void BkVCC::Compute_SC(int k, TIntVIntV& CS, PUNGraph G)
{

	PNGraph BFSTree;
	PUNGraph F_k = TUNGraph::New();//Forest k
	/*TUNGraph G_P = *G;
	PUNGraph G_ = &G_P;*/
	PUNGraph G_ = G;
	TCnComV CS_CC;
	//int NodeNums = TSnap::CntNonZNodes(G_);
	int NodeNums = G_->GetNodes();
	TIntVIntV Del_Edge;
	TIntV edge;
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
						
						if (i == k - 1) {
							F_k->AddNode(TNI.GetId());
						}
					}
					for (TNGraph::TEdgeI TEI = BFSTree->BegEI(); TEI < BFSTree->EndEI(); TEI++) {					
						G_->DelEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						edge.Clr();
						edge.Add(TEI.GetSrcNId());
						edge.Add(TEI.GetDstNId());
						Del_Edge.Add(edge);
						if (i == k - 1) {
							F_k->AddEdge(TEI.GetSrcNId(), TEI.GetDstNId());
						}
					}
				}
			}
		}
	}
	TSnap::GetSccs(F_k, CS_CC);
	for (TCnComV::TIter I = CS_CC.BegI(); I < CS_CC.EndI(); I++) {
		if (I->Len() < k + 1) {
			CS_CC.DelAll(*I);
		}
	}
	for (TCnComV::TIter I = CS_CC.BegI(); I < CS_CC.EndI(); I++) {
		CS.Add(I->NIdV);
	}
	for (int i = 0; i < Del_Edge.Len(); i++) {
		G_->AddEdge(Del_Edge[i][0], Del_Edge[i][1]);
	}
	

	return;

}