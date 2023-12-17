
#ifndef KVCC_ALGORITHM_H
#define KVCC_ALGORITHM_H
#include<Snap.h>

class MyTUNGraph;

typedef TVec<TInt> TIntV;
typedef TVec<PUNGraph> TUNGraV;
typedef TPt<MyTUNGraph> MyPUNGraph;
typedef TVec<TIntV> TIntVIntV;


class MyTUNGraph : TUNGraph {
	TIntV SSV;
};

class VCCE {
public:
	int* vertex_map_;
	double _time = 0;
	double _time2 = 0;
	double _time3 = 0;
	double _time4 = 0;
	int m = 0;
	int m2 = 0;
	int m3 = 0;
	int m4 = 0;
	PUNGraph G;
	int k;
	VCCE();
	VCCE(PUNGraph G_, int k_);
	TIntVIntV KVCC_ENUM(PUNGraph & sub_G, int k);
protected:
	TIntV Global_Cut(TIntV& subG, int k, PUNGraph All_G);
	TIntVIntV Overlap_Partition(TIntV subG, TIntV Vertex_Cut, PUNGraph All_G);
	PNEANet Construct_DG(PUNGraph G);
	PUNGraph Compute_SC(TIntV subG, int k, PUNGraph All_G);
	
	TIntV Loc_Cut(int source, int sink, PNEANet DG, PUNGraph G, int k);
};

template <class PGraph>
int GetMnDegNId(const PGraph& Graph);

/// Returns the maximum integer valued flow in the network \c Net from source \c SrcNId to sink \c SnkNId according to Ford-Fulkerson algorithm. 
//int GetMaxFlowIntFF(PNEANet& Net, const int& SrcNId, const int& SnkNId, PNEANet& ResNet);

#endif
