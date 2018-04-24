#include "simplenoninterestingnodesgraphsimplifier.h"

#include "modelobjects/nodedata.h"

SimpleNonInterestingNodesGraphSimplifier::SimpleNonInterestingNodesGraphSimplifier()
{

}

bool SimpleNonInterestingNodesGraphSimplifier::operator()(const QList<std::shared_ptr<NodeData> > &relNodes,
                                                          ShortestPathBuilder::graph_t &graph,
                                                          std::vector<ShortestPathBuilder::vertex_descriptor> &predecessors,
                                                          std::vector<double> &dinstances)
{
    std::vector<ShortestPathBuilder::vertex_descriptor> npred;
    std::vector<double> ndist;

    // set relevancy for relevant nodes
    for (auto rnode : mRelevantInternNodes) {
        auto nd = vertex(rnode, graph);
        graph[nd].isRelevant = true;
    }

    for (auto relNode : relNodes) {
        double d = 0;

        ShortestPathBuilder::vertex_descriptor nd = vertex(relNode->get_idx_node().toIndex(), graph);
        auto v = nd;
        ShortestPathBuilder::vertex_descriptor npd = predecessors[nd];
        while (npd != nd) {
            if (graph[npd].isRelevant)
                break;
            d += dinstances[nd];
            nd = npd;
            npd = predecessors[nd];
        }
        while (npred.size() <= v)
            npred.push_back(0);
        npred[v] = npd;
        while (ndist.size() <= v)
            ndist.push_back(0);
        ndist[v] = d;
    }

    predecessors = npred;
    dinstances = ndist;

    return true;
}

