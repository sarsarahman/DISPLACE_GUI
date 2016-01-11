// --------------------------------------------------------------------------
// DISPLACE: DYNAMIC INDIVIDUAL VESSEL-BASED SPATIAL PLANNING
// AND EFFORT DISPLACEMENT
// Copyright (c) 2012, 2013, 2014, 2015, 2016 Francois Bastardie <fba@aqua.dtu.dk>

//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.

//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
// --------------------------------------------------------------------------

#include "shortestpathbuilder.h"

#include <modelobjects/nodedata.h>
#include <displacemodel.h>

#include <memory>

#include <QList>
#include <QFile>
#include <QTextStream>

/* Note
 * Part of this code was extracted from:
 * http://www.boost.org/doc/libs/1_57_0/libs/graph/example/dijkstra-example.cpp
 * */

ShortestPathBuilder::ShortestPathBuilder(DisplaceModel *model)
    : mModel (model)
{
    const QList<std::shared_ptr<NodeData> >& nodes = mModel->getNodesList();

    foreach (std::shared_ptr<NodeData> node, nodes) {
        for (int n = 0; n < node->getAdiacencyCount(); ++n) {
            std::shared_ptr<NodeData::Edge> edge = node->getAdiacencyByIdx(n);
            std::shared_ptr<NodeData> tg = edge->target.lock();
            if (tg.get() != nullptr) {
                mEdges.push_back(Edge(node->get_idx_node(), tg->get_idx_node()));
                mWeights.push_back(edge->weight);
            }
        }
    }

    mGraph = graph_t(mEdges.begin(), mEdges.end(), mWeights.begin(), nodes.size());
    mWeightmap = get(boost::edge_weight, mGraph);
    mPredecessors = std::vector<vertex_descriptor> (num_vertices(mGraph));
    mDistances = std::vector<double> (num_vertices(mGraph));
}

void ShortestPathBuilder::create(std::shared_ptr<NodeData> node, QString path, bool simplify,
                                 const QList<std::shared_ptr<NodeData> > &relevantNodes)
{
    vertex_descriptor s;

    s = vertex(node->get_idx_node(), mGraph);
    dijkstra_shortest_paths(mGraph, s,
                             predecessor_map(boost::make_iterator_property_map(mPredecessors.begin(), get(boost::vertex_index, mGraph))).
                             distance_map(boost::make_iterator_property_map(mDistances.begin(), get(boost::vertex_index, mGraph))));

    QString mindist = QString("%1/min_distance_%2.dat").arg(path).arg(node->get_idx_node());
    QString prev = QString("%1/previous_%2.dat").arg(path).arg(node->get_idx_node());
    QFile mindist_file(mindist);
    QFile prev_file (prev);

    mindist_file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    prev_file.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QTextStream strm_min(&mindist_file);
    QTextStream strm_prev(&prev_file);

    strm_prev << " key  value" << endl;
    strm_min << " key  value" << endl;

    foreach (std::shared_ptr<NodeData> n, relevantNodes) {
        vertex_descriptor nd = vertex(n->get_idx_node(), mGraph);

        while (mPredecessors[nd] != nd) {
            if (!mGraph[nd].flag) {
                strm_prev << nd << " " << mPredecessors[nd] << endl;
                strm_min << nd << " " << mDistances[nd] << endl;
            }

            mGraph[nd].flag = true;
            nd = mPredecessors[nd];
        }

        mGraph[nd].flag = true;
    }

#if 0
     std::ofstream dot_file(QString("%1/graph_%2.dot").arg(path).arg(node->get_idx_node()).toStdString());

     dot_file << "digraph D {\n"
       << "  rankdir=LR\n"
       << "  size=\"4,3\"\n"
       << "  ratio=\"fill\"\n"
       << "  edge[style=\"bold\"]\n" << "  node[shape=\"circle\"]\n";

     for (boost::tie(ei, ei_end) = edges(mGraph); ei != ei_end; ++ei) {
       boost::graph_traits < graph_t >::edge_descriptor e = *ei;
       boost::graph_traits < graph_t >::vertex_descriptor
         u = boost::source(e, mGraph), v = boost::target(e, mGraph);
       dot_file << u << " -> " << v
         << "[label=\"" << get(mWeightmap, e) << "\"";
       if (mPredecessors[v] == u)
         dot_file << ", color=\"black\"";
       else
         dot_file << ", color=\"grey\"";
       dot_file << "]";
     }
     dot_file << "}";
#endif
}
