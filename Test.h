#ifndef _TEST_H_
#define _TEST_H_

#include "Graph.h"
#include "Utility.h"

#include <string>
#include <vector>

/**
 * This file is based on [VLDB 2015]:
 *     Community Detection in Social Networks:
 *     An Indepth Benchmarking Study with a Procedure Oriented Framework
 *
 * @author:ned_chu
 * */

typedef long long LL;

typedef std::vector<int> Community;
typedef std::vector<Community> CommunitySet;
typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

/**
 * Every Community in CommunitySet should be sorted!
 * Don't forget to turn new id to old id!
 * */

namespace Test {
struct TestReport {
    /**
     * May be we need long long to store Fsame
     * */
    double Fsame, Jaccard, NMI, ClusterCoef, Strength, Modularity;

    /**
     * Get human readable report of *this
     * */
    std::string getReport();
};

/**
 * ${commSet} has new id, turn it into old id
 *
 * Maybe return bool is better?
 * */
void toOldId(CommunitySet& commSet, const Graph* g);

/**
 * Get mapping from old id to new id
 * */
VI getOldToNew(const Graph* g);


void sortCommunitySet(CommunitySet& commSet);

/**
 * Read community inforamation
 * A community in a line, seperated by space
 * Every community will be sorted
 * */
CommunitySet readCommunity(const std::string commFile);

void writeCommunity(const std::string commFile, const CommunitySet& commSet);

/**
 * Accuracy metrics
 * Need result of your algo & ground truth
 * Every Community in CommunitySet should be sorted!
 * */
double computeFsame(const CommunitySet& commSet,
                    const CommunitySet& groundTruth);

double computeJaccard(const CommunitySet& commSet,
                      const CommunitySet& groundTruth);

double computeNMI(const CommunitySet& commSet, const CommunitySet& groundTruth);

/**
 * Effectiveness metrics
 * Need result of your algo & ground truth & graph itself
 * Every Community in CommunitySet should be sorted!
 * */
double computeClusterCoef(const CommunitySet& commSet, const Graph* g);

double computeStrength(const CommunitySet& commSet, const Graph* g);

double computeModularity(const CommunitySet& commSet, const Graph* g);

/**
 * Run all test in a batch
 * */
TestReport getTestReport(const CommunitySet& commSet,
                         const CommunitySet& groundTruth, const Graph* g);
};  // namespace Test

#endif  //_TEST_H_