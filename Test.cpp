#include "Test.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <tuple>
#include <cmath>
#include <set>

struct Table {
public:
    VI tags;
    int round;

    Table(int N) {
        tags = VI(N, -1);
        round = 0;
    }
    void nextRound() {
        round++;
    }
    void set(int x) {
        if (x > tags.size()) {
            throw out_of_range("When setting table");
            return;
        }
        tags[x] = round;
    }
    bool get(int x) {
        if (x > tags.size()) {
            throw out_of_range("When getting table");
            return false;
        }
        return tags[x] == round;
    }
private:
    Table() {}
};


struct IntTable {
public:
    VI tags, values;
    int round;

    IntTable(int N) {
        tags = VI(N, -1);
        values = VI(N, 0);
        round = 0;
    }
    void nextRound() {
        round++;
    }
    void add(int x) {
        if (x > tags.size()) {
            throw out_of_range("When setting table");
            return;
        }
        if (tags[x] != round) {
            tags[x] = round;
            values[x] = 0;
        }
        values[x]++;
    }
    int get(int x) {
        if (x > tags.size()) {
            throw out_of_range("When getting table");
            return false;
        }
        return (tags[x] != round) ? 0 : values[x];
    }
private:
    IntTable() {}
};


void Test::toOldId(CommunitySet& commSet, const Graph* g) {
    for (auto& comm : commSet) {
        for (auto& id : comm) {
            id = g->id_new_to_old_[id];
        }
    }
}

VI Test::getOldToNew(const Graph* g) {
    int oldSize = *std::max_element(std::begin(g->id_new_to_old_), std::end(g->id_new_to_old_));
    VI oldToNew(oldSize + 1, -1);
    for (int i = 0; i < g->id_new_to_old_.size(); i++) {
        oldToNew[g->id_new_to_old_[i]] = i;
    }
    return oldToNew;
}

void Test::sortCommunitySet(CommunitySet& commSet) {
    for (Community& comm : commSet) {
        std::sort(std::begin(comm), std::end(comm));
    }
}

CommunitySet Test::readCommunity(const std::string commFile) {
    std::ifstream fin;
    fin.open(commFile);

    if (!fin) {
        return {};
    }
    CommunitySet commSet;
    while (fin) {
        std::string buff;
        std::getline(fin, buff);

        std::istringstream sin(buff);
        Community comm;
        int id;
        while (sin >> id) {
            comm.push_back(id);
        }
        if (!comm.empty()) {
            commSet.push_back(comm);
        }
    }
    return commSet;
}

void Test::writeCommunity(const std::string commFile,
                          const CommunitySet& commSet) {
    std::ofstream fout;
    fout.open(commFile);

    for (auto& comm : commSet) {
        if (comm.empty()) continue;

        fout << comm[0];
        for (int i = 1; i < comm.size(); i++) {
            fout << '\t' << comm[i];
        }
        fout << std::endl;
    }
    fout.close();
}

int getMax(const CommunitySet& commSet, const CommunitySet &groundTruth) {
    auto getMaxSub = [](const CommunitySet &commSet) {
        int ans = 0;
        for (auto &comm : commSet)
            for (auto &id : comm)
                ans = std::max(ans, id);
        return ans;
    };
    return std::max(getMaxSub(commSet), getMaxSub(groundTruth)) + 1;
}

double Test::computeFsame(const CommunitySet& commSet,
                          const CommunitySet& groundTruth) {
    auto process = [](const CommunitySet& commSet, const CommunitySet& groundTruth) {
        int graphSize = getMax(commSet, groundTruth);

        LL ans = 0;
        LL sum = 0;
        VVI bk(graphSize, VI());
        IntTable cnt(graphSize);
        for (int i = 0; i < groundTruth.size(); i++)
            for (auto &id: groundTruth[i])
                bk[id].push_back(i);
        for (int i = 0; i < commSet.size(); i++) {
            sum += commSet[i].size();
            int tmp = 0;
            for (auto &id: commSet[i]) {
                for (auto &opid: bk[id]) {
                    cnt.add(opid);
                    tmp = std::max(tmp, cnt.get(opid));
                }
            }
            ans += tmp;
            cnt.nextRound();
        }
        return double(ans) / sum;
    };
    return (process(commSet, groundTruth) + process(groundTruth, commSet)) / 2.;
}

double Test::computeJaccard(const CommunitySet& commSet,
                            const CommunitySet& groundTruth) {
    int graphSize = getMax(commSet, groundTruth);
    VVI bkComm(graphSize, VI()), bkGrou(graphSize, VI());
    auto buildBk = [](const CommunitySet &commSet, VVI &bk) {
        for (int i = 0; i < commSet.size(); i++) {
            auto &comm = commSet[i];
            for (auto &id : comm)
                bk[id].push_back(i);
        } 
    };
    buildBk(commSet, bkComm);
    buildBk(groundTruth, bkGrou);

    Table visComm(graphSize), visGrou(graphSize);
    LL PS = 0, PS1 = 0, PS2 = 0;
    for (int i = 0; i < graphSize; i++) {
        if (bkComm[i].empty() || bkGrou[i].empty()) continue;

        int lcnt = 0, rcnt = 0, overlap = 0;
        for (auto &commId : bkComm[i]) {
            for (auto &id : commSet[commId]) {
                if (id == i) continue;
                if (visComm.get(id)) continue;
                visComm.set(id);
                lcnt++;
            }
        }
        for (auto &commId : bkGrou[i]) {
            for (auto &id : groundTruth[commId]) {
                if (id == i) continue;
                if (visGrou.get(id)) continue;
                visGrou.set(id);
                rcnt++;
                if (visComm.get(id))
                    overlap++;
            }
        }
        PS += overlap;
        PS1 += lcnt - overlap;
        PS2 += rcnt - overlap;
        visComm.nextRound(), visGrou.nextRound();
    }
    return double(PS) / (PS + PS1 + PS2);
}

double Test::computeNMI(const CommunitySet& commSet,
                        const CommunitySet& groundTruth) {
    int graphSize = getMax(commSet, groundTruth);
    std::vector<LL> rowSum(commSet.size(), 0), colSum(groundTruth.size(), 0);
    LL sum = 0;

    VVI bk(graphSize, VI());
    IntTable cnt(graphSize);
    for (int i = 0; i < groundTruth.size(); i++)
        for (auto &id: groundTruth[i])
            bk[id].push_back(i);
    for (int i = 0; i < commSet.size(); i++) {
        for (auto &id: commSet[i]) {
            for (auto &opid: bk[id]) {
                rowSum[i]++;
                colSum[opid]++;
            }
        }
        sum += rowSum[i];
    }

    double NT = std::log(sum);
    std::vector<double> NI, NJ;
    for (auto &v : rowSum) NI.push_back(v ? std::log(v) : 0);
    for (auto &v : colSum) NJ.push_back(v ? std::log(v) : 0);

    double up = 0.0;
    for (int i = 0; i < commSet.size(); i++) {
        std::set<int> opidSet;
        for (auto &id: commSet[i]) {
            for (auto &opid: bk[id]) {
                cnt.add(opid);
                opidSet.insert(opid);
            }
        }
        for (auto &opid : opidSet) {
            double NIJ = std::log(cnt.get(opid));
            up += cnt.get(opid) * (NIJ + NT - NI[i] - NJ[opid]);
        }
        cnt.nextRound();
    }

    double down = 0;
    for (int i = 0; i < NI.size(); i++) {
        if (rowSum[i] == 0) continue;
        down += rowSum[i] * (NI[i] - NT);
    }
    for (int i = 0; i < NJ.size(); i++) {
        if (colSum[i] == 0) continue;
        down += colSum[i] * (NJ[i] - NT);
    }
    
    return -2 * up / down;
}

double Test::computeClusterCoef(const CommunitySet& commSet, const Graph* g) {
    VI oldToNew = Test::getOldToNew(g);
    Table vis(g->vertices_num_);
    Table edge(g->vertices_num_);
    double ans = 0.0;
    for (auto &comm : commSet) {
        VI ids;
        for (auto &id : comm) {
            ids.push_back(oldToNew[id]);
            vis.set(ids.back());
        }

        double sum = 0.;
        for (auto &id : ids) {
            int deg = g->vertices_[id]->degree;
            int cnt = 0;
            if (deg < 2) continue;
            for (auto &nbr : g->vertices_[id]->nbr) {
                if (!vis.get(nbr)) continue;
                edge.set(nbr);
                for (auto &cand : g->vertices_[nbr]->nbr) {
                    if (!vis.get(cand) || !edge.get(cand)) continue;
                    cnt++;
                }
            }
            edge.nextRound();
            sum += 2. * cnt / deg / (deg - 1);
        }
        ans += sum / ids.size();
        vis.nextRound();
    }
    return ans / commSet.size();
}

std::tuple<VVI, VVI> computeInOutCommunity(const CommunitySet& commSet, const Graph* g) {
    VI oldToNew = Test::getOldToNew(g);
    Table vis(g->vertices_num_);

    VVI in, out;
    for (int i = 0; i < commSet.size(); i++) {
        auto &comm = commSet[i];
        for (auto& v: comm) {
            vis.set(oldToNew[v]);
        }

        VI inSub, outSub;
        for (auto& v: comm) {
            int newV = oldToNew[v];
            int vIn = 0, vOut = 0;

            auto& allNbr = g->vertices_[newV]->nbr;
            for (auto& nbr: allNbr) {
                if (vis.get(nbr)) {
                    vIn++;
                } else {
                    vOut++;
                }
            }
            inSub.push_back(vIn);
            outSub.push_back(vOut);
        }
        in.push_back(inSub);
        out.push_back(outSub);
        vis.nextRound();
    }
    return std::make_tuple(in, out);
}

double Test::computeStrength(const CommunitySet& commSet, const Graph* g) {
    VVI in, out;
    std::tie(in, out) = computeInOutCommunity(commSet, g);

    int score = 0;
    for (int i = 0; i < commSet.size(); i++) {
        auto &comm = commSet[i];
        int sumOut = 0, sumIn = 0;
        bool strong = true;
        for (int j = 0; j < comm.size(); j++) {
            int vIn = in[i][j], vOut = out[i][j];
            strong &= vIn > vOut;
            sumOut += vOut, sumIn += vIn;
        }
        if (strong) {
            score += 2;
        } else if (sumIn > sumOut) {
            score++;
        }
    }
    return score / 2. / commSet.size();
}

double Test::computeModularity(const CommunitySet& commSet, const Graph* g) {
    VVI in, out;
    std::tie(in, out) = computeInOutCommunity(commSet, g);
    double m = g->edges_num_;

    double ans = 0.0;
    for (int i = 0; i < commSet.size(); i++) {
        auto &comm = commSet[i];
        int sumOut = 0, sumIn = 0;
        for (int j = 0; j < comm.size(); j++) {
            sumOut += out[i][j], sumIn += in[i][j];
        }
        sumIn /= 2;
        double tmp = (2. * sumIn + sumOut) / 2. / m;
        ans += (sumIn / m - tmp * tmp);
    }
    return ans;
}

Test::TestReport Test::getTestReport(const CommunitySet& commSet,
                                     const CommunitySet& groundTruth,
                                     const Graph* g) {
    Test::TestReport report;
    report.Fsame = Test::computeFsame(commSet, groundTruth);
    report.Jaccard = Test::computeJaccard(commSet, groundTruth);
    report.NMI = Test::computeNMI(commSet, groundTruth);
    report.ClusterCoef = Test::computeClusterCoef(commSet, g);
    report.Strength = Test::computeStrength(commSet, g);
    report.Modularity = Test::computeModularity(commSet, g);
    return report;
}
