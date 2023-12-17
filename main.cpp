#include"Sweep_Algorithm.h"
#include "Heuristic_Algorithm.h"
#include <stdio.h>
// #include <windows.h>
#include "getopt.h"
#include "Test.h"
#include <iostream>




using namespace std;

void printNodesNum(PUNGraph G, FILE* outfile) {
	TIntV Nodes;
	G->GetNIdV(Nodes);
	Nodes.Sort();
	for (TIntV::TIter TI = Nodes.BegI(); TI < Nodes.EndI(); TI++) {
		//cout << "Node_ID:" << *TI << endl;
		fprintf(outfile, "%d ", *TI);
	}
	
	
}

void printNodesNum(TIntV G, FILE* outfile, int j) {
	G.Sort();
	for (TIntV::TIter TI = G.BegI(); TI < G.EndI(); TI++) {
		//cout << "Node_ID:" << *TI << endl;
		fprintf(outfile, "%d	%d\n", *TI, j);
	}


}

//int main() {
//	TStr dataset = "smalltestforkvcc";//BkFig  smalltestforkvcc
//	PUNGraph G = TSnap::LoadEdgeList<PUNGraph>("./dataset/" + dataset + ".txt", 0, 1);
//	string dataset_name = dataset.CStr();
//	TCliqueOverlap* CO = new TCliqueOverlap();
//	TIntVIntV s;
//
//	CO->GetMaxCliques(G, 5, s);
//	int j = 0;
//	for (TIntVIntV::TIter GI = s.BegI(); GI < s.EndI(); GI++) {
//		PUNGraph GI_Graph = TSnap::GetSubGraph(G, *GI);
//		printf("clique(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(GI_Graph), TSnap::CntUniqUndirEdges(GI_Graph));
//		for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
//			//cout << *NI<<" ";
//			printf("%d ", *NI);
//		}
//	}
//
//
//}


int main(int argc, char* argv[]) {
	//<Dataset>            <max K-VCC>
	//CA-AstroPh.txt			56
	//Stanford.txt        71	14 28 43 57
	//DBLP.txt				113	
	//Cit.txt				64
	//LiveJournal.txt			
	//CA-CondMat.txt			25

	//TIntV nums = TIntV();
	//for (int i = 1; i <= 321; i++) {
	//	nums.Add(i);
	//}
	//TVec<TIntV> res = subsets(nums, 5, 1000);
	//cout << res.Len();
	int o;
	const char* optstring = "d:a:k:s:m:e:t:"; // 锟斤拷锟斤拷锟斤拷选锟斤拷-abc锟斤拷锟斤拷锟斤拷c选锟斤拷锟斤拷锟矫帮拷牛锟斤拷锟斤拷院锟斤拷锟斤拷锟斤拷锟叫诧拷锟斤拷

	while ((o = getopt(argc, argv, optstring)) != -1) {
		switch (o) {
		case 'd':
			dataset = optarg;
			printf("opt is d, oprarg is: %s\n", optarg);
			break;
		case 'a':
			alg = optarg;
			printf("opt is a, oprarg is: %s\n", optarg);
			break;
		case 'k':
			k = atoi(optarg);
			printf("opt is k, oprarg is: %s\n", optarg);
			break;
		case 's':
			seed = optarg;
			printf("opt is s, oprarg is: %s\n", optarg);
			break;
		case 'm':
			mergeMethod = optarg;
			printf("opt is m, oprarg is: %s\n", optarg);
			break;
		case 'e':
			expandMethod = optarg;
			printf("opt is e, oprarg is: %s\n", optarg);
			break;
		case 't':
			threads = atoi(optarg);
			printf("opt is t, oprarg is: %s\n", optarg);
			break;
		case '?':
			printf("Correct Usage:\n");
			usage(); // 锟斤拷示使锟斤拷说锟斤拷
			return 0;
		}
	}



	//dataset = "DBLP";//BkFig  smalltestforkvcc opsahl-openflights

	//omp_set_num_threads(4); //锟斤拷锟斤拷


	PUNGraph G = TSnap::LoadEdgeList<PUNGraph>("./dataset/" + dataset + ".txt", 0, 1);

	//TStr dataset = "network";
	//PUNGraph G = TSnap::LoadEdgeList<PUNGraph>("./dataset/" + dataset + ".dat", 0, 1);
	string dataset_name = dataset.CStr();
	string folderPath = "output/" + dataset_name;


	//int K_Arr[] = { 5 };

	//a = 4;//锟斤拷a锟斤拷选锟斤拷锟斤拷锟叫碉拷锟姐法 0:VCCE  1:BkVCC  2:BkVCC(clique) 3:BkVCC(k+1 clique)


	/*for (int i = 0; i < sizeof(K_Arr) / sizeof(int); i += 1) {

		int k = K_Arr[i];
		*/
		int alpha = 1000;
		double t0;
		int j = 0;
		char* k_str = new char[5];
		char* alpha_str = new char[10];

		// itoa(k, k_str, 10);
		// itoa(alpha, alpha_str, 10);
		sprintf(k_str,"%d",k);
		sprintf(alpha_str,"%d",alpha);


		/*string filename = "./output/" + dataset_name + "_k=" + k_str + "_alpha=" + alpha_str + ".txt";*/

		//string alg[6] = { "VCCE", "BkVCC", "BkVCC(clique)", "BkVCC(k+1 clique)","BkVCC_flow", "BkVCC(clique)_flow" };
		string filename;

		CreateDirectory(folderPath.c_str(), NULL);
		/*if (!GetFileAttributesA(folderPath.c_str()) & FILE_ATTRIBUTE_DIRECTORY) {
			bool flag = CreateDirectory(folderPath.c_str(), NULL);

		}
		else {
			cout << "Directory already exists." << endl;
		}*/


		// if(strcmp(alg.c_str(), "BkVCC") == 0)
		// 	filename = "./" + folderPath + "/" + dataset_name + "_k=" + k_str + "_" + alg.c_str() + "_seed=" + seed.c_str()
		// 	+"_expand=" + expandMethod.c_str() + "_merge=" + mergeMethod.c_str() + "_t=" + std::to_string(threads).c_str()  + ".txt";
		// else 
		// 	filename = "./" + folderPath + "/"  +  dataset_name + "_k=" + k_str + "_" + alg.c_str() + ".txt";
		// FILE* outFile = fopen(filename.c_str(), "w");
		printf("G: \nnode_nums = %d, edge_nums = %d\n", G->GetNodes(), G->GetEdges());
		

		
		if (strcmp(alg.c_str(), "BkVCC") == 0) {
			//BkVCC
			t0 = omp_get_wtime( );
			BkVCC BkVCC(G, k);
			BkVCC.dataset = dataset;
			TIntVIntV BkVCC_res = BkVCC.BkVCC_ENUM(BkVCC.G, BkVCC.k, alpha);

			// fprintf(outFile, "VCC_Num:%d\n"
			// 	"Time for Seeding: %fs\n"
			// 	"Time for Expanding: %fs\n"
			// 	"Time for Merging: %fs\n"
			// 	"Total Time: %fs\n", BkVCC_res.Len(), BkVCC._time, BkVCC._time2, BkVCC._time3, (clock() - t0) * 1.0 / CLOCKS_PER_SEC);

			printf("VCC_Num:%d\n"
				"Time for Seeding: %fs\n"
				"Time for Expanding: %fs\n"
				"Time for Merging: %fs\n"
				"Total Time: %fs\n", BkVCC_res.Len(), BkVCC._time, BkVCC._time2, BkVCC._time3, (omp_get_wtime() - t0) );
		
			//锟斤拷锟斤拷F-score//		
			TIntVIntV Exact_result;
			vector<vector<int>> groundTruth;
			try
			{	
				//cout << "D:/Git Project/KVCC-ENUM/community/" <<  dataset.CStr() << "_k=" << TStr(k_str).CStr() << "_algorithm=" << alg.c_str() << ".kvcc" << endl;
				TFIn inFile("./community/" + dataset + "_k=" + TStr(k_str) + "_algorithm=VCCE.kvcc");
				Exact_result.Load(inFile);
				
				int i = 0;
				for (TIntVIntV::TIter GI = Exact_result.BegI(); GI < Exact_result.EndI(); GI++) {
					groundTruth.push_back(vector<int>());
					for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
						groundTruth[i].push_back(*NI);

					}
					i++;
				}
			}
			catch (TPt<TExcept>)
			{
				cout << endl << "***Exact kvcc result does not exist***" << endl;
			}

			vector<vector<int>> commSet;
			int i = 0;
			for (TIntVIntV::TIter GI = BkVCC_res.BegI(); GI < BkVCC_res.EndI(); GI++) {
				commSet.push_back(vector<int>());
				for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
					commSet[i].push_back(*NI);
					
				}
				i++;
			}
			cout << "F_same:" << Test::computeFsame(commSet, groundTruth) << endl;	
			// fprintf(outFile, "F_same: %f\n", Test::computeFsame(commSet, groundTruth));
			cout << "Jaccard:" << Test::computeJaccard(commSet, groundTruth) << endl;
			// fprintf(outFile, "Jaccard: %f\n", Test::computeJaccard(commSet, groundTruth));
			cout << "NMI:" << Test::computeNMI(commSet, groundTruth) << endl;
			// fprintf(outFile, "NMI: %f\n", Test::computeNMI(commSet, groundTruth));

			//cout << "F_score: " << computeFscore(BkVCC_res, Exact_result) << endl;
			//fprintf(outFile, "F_score: %f\n", computeFscore(BkVCC_res, Exact_result));
			////锟斤拷锟斤拷F-score//	 
			//cout << "F_same: " << computeFsame(BkVCC_res, Exact_result) << endl;
			//fprintf(outFile, "F_same: %f\n", computeFsame(BkVCC_res, Exact_result));
			////锟斤拷锟斤拷F-same//

			for (TIntVIntV::TIter GI = BkVCC_res.BegI(); GI < BkVCC_res.EndI(); GI++) {
				PUNGraph GI_Graph = TSnap::GetSubGraph(G, *GI);
				printf("K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(GI_Graph), TSnap::CntUniqUndirEdges(GI_Graph));
				for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
					//cout << *NI<<" ";
					printf("%d ", *NI);
				}
				//printNodesNum(*GI, outFile, ++j);
				printf( "\n");
				//cout << endl;

			}


		}




		if (strcmp(alg.c_str(), "VCCE") == 0) {
			//kVCC-ENUM_Sweep
			clock_t t0 = clock();
			j = 0;
			/*fprintf(outFile, "## dataset: %s k = %d\n", dataset_name.c_str(), k);
			printf("k = %d\n", k);
			printf("VCCE*:\n");
			fprintf(outFile, "VCCE*:\n");*/
			VCCE_S VCCE_S(G, k, 1);
			PUNGraph tmp = TSnap::GetKCore(G, k);
			printf("G(%d-core): \nnode_nums = %d, edge_nums = %d\n", k, tmp->GetNodes(), tmp->GetEdges());
			VCCE_S.dataset = dataset;
			TIntVIntV VCCE_S_res = VCCE_S.KVCC_ENUM(VCCE_S.G, VCCE_S.k);


			
			/*if (VCCE_S_res.Len() == 0) break;*/

			// fprintf(outFile, "VCC_Num:%d\n"
			// 	"time for LOC_CUT(flow>=k): %fs\n"
			// 	"Call for LOC_CUT(flow>=k): %d\n"
			// 	"time for LOC_CUT(flow<k): %fs\n"
			// 	"Call for LOC_CUT(flow<k): %d\n"
			// 	"time for LOC_CUT(fake): %fs\n"
			// 	"Call for LOC_CUT(fake): %d\n"
			// 	"Total Time: %fs\n", VCCE_S_res.Len(), VCCE_S._time2, VCCE_S.m2, VCCE_S._time3, VCCE_S.m3, VCCE_S._time4, VCCE_S.m4, (clock() - t0) * 1.0 / CLOCKS_PER_SEC);

			printf("VCC_Num:%d\n"
				"time for LOC_CUT(flow>=k): %fs\n"
				"Call for LOC_CUT(flow>=k): %d\n"
				"time for LOC_CUT(flow<k): %fs\n"
				"Call for LOC_CUT(flow<k): %d\n"
				"time for LOC_CUT(fake): %fs\n"
				"Call for LOC_CUT(fake): %d\n"
				"Total Time: %fs\n", VCCE_S_res.Len(), VCCE_S._time2, VCCE_S.m2, VCCE_S._time3, VCCE_S.m3, VCCE_S._time4, VCCE_S.m4, (clock() - t0) * 1.0 / CLOCKS_PER_SEC);

			for (TIntVIntV::TIter GI = VCCE_S_res.BegI(); GI < VCCE_S_res.EndI(); GI++) {
				GI->Sort();
			}
			VCCE_S_res.Sort();
			/*fprintf(outFile, "Node	KVCC_id\n");*/

			for (TIntVIntV::TIter GI = VCCE_S_res.BegI(); GI < VCCE_S_res.EndI(); GI++) {
				/*printf("K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));

				cout << endl;*/
				PUNGraph GI_Graph = TSnap::GetSubGraph(G, *GI);
				printf("K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++j, TSnap::CntNonZNodes(GI_Graph), TSnap::CntUniqUndirEdges(GI_Graph));
				for (TIntV::TIter NI = GI->BegI(); NI < GI->EndI(); NI++) {
					//cout << *NI<<" ";
					printf( "%d ", *NI);
					
				}
				printf("\n");
			}

		}



		




		//	fprintf(outFile, "VCCE:\n");
		//	printf("VCCE:\n");
		//	VCCE VCCE(G, k);
		//	clock_t t2 = clock();
		//	TUNGraV VCC2 = VCCE.KVCC_ENUM(VCCE.G, VCCE.k);

		//	fprintf(outFile, "VCC_Num:%d\n"
		//		"time for LOC_CUT(flow>=k): %fs\n"
		//		"Call for LOC_CUT(flow>=k): %d\n"
		//		"time for LOC_CUT(flow<k): %fs\n"
		//		"Call for LOC_CUT(flow<k): %d\n"
		//		"time for LOC_CUT(fake): %fs\n"
		//		"Call for LOC_CUT(fake): %d\n"
		//		"Total Time: %fs\n", VCC2.Len(), VCCE._time2, VCCE.m2, VCCE._time3, VCCE.m3, VCCE._time4, VCCE.m4, (clock() - t2) * 1.0 / CLOCKS_PER_SEC);

		//	printf("VCC_Num:%d\n"
		//		"time for LOC_CUT(flow>=k): %fs\n"
		//		"Call for LOC_CUT(flow>=k): %d\n"
		//		"time for LOC_CUT(flow<k): %fs\n"
		//		"Call for LOC_CUT(flow<k): %d\n"
		//		"time for LOC_CUT(fake): %fs\n"
		//		"Call for LOC_CUT(fake): %d\n"
		//		"Total Time: %fs\n", VCC2.Len(), VCCE._time2, VCCE.m2, VCCE._time3, VCCE.m3, VCCE._time4, VCCE.m4, (clock() - t2) * 1.0 / CLOCKS_PER_SEC);

		//	
		//	//for (TUNGraV::TIter GI = VCC2.BegI(); GI < VCC2.EndI(); GI++) {

		//	//	printf("K-VCC(No.%d): node_nums = %d, edge_nums = %d\n", ++h, TSnap::CntNonZNodes(*GI), TSnap::CntUniqUndirEdges(*GI));
		//	//	printNodesNum(*GI, outFile);
		//	//	cout << endl;
		//	//}
		//	//printf("\n");
		//



}
		//}
		//fclose(outFile);
		//
	/*}*/
	





