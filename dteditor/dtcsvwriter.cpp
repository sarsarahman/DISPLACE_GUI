#include "dtcsvwriter.h"
#include <graphnodeitem.h>

#include <QQueue>
#include <dtree/decisiontree.h>
#include <dtree/dtnode.h>
#include <dtgraphicsscene.h>

const int DtCsvWriter::VERSION = 2;

DtCsvWriter::DtCsvWriter()
{
}

bool DtCsvWriter::exportTree(QTextStream &stream, dtree::DecisionTree *tree, DtGraphicsScene *scene)
{
    Q_UNUSED(tree);

    QQueue<GraphNodeItem*>queue;
    QQueue<int> queueid;

    GraphNodeItem *gnode = scene->root();
    if (!gnode)
        return false;

    stream << "#DTreeVersion: " << VERSION << endl;
    stream << "# id,variable,posx,posy,nchld,children...,value" << endl;

    queue.push_back(gnode);
    queueid.push_back(0);

    int id = 0, nid = 0;
    while (!queue.isEmpty()) {
        gnode = queue.front();
        queue.pop_front();
        id = queueid.front();
        queueid.pop_front();

        boost::shared_ptr<dtree::Node> node = gnode->getNode();
        stream << id << "," << dtree::VariableNames::variableName(node->variable()) << ",";
        // Other data
        QPointF pos = gnode->pos();
        stream << pos.x() << "," << pos.y() << ",";
        stream << gnode->getChildrenCount() << ",";

        for (int i = 0; i < gnode->getChildrenCount(); ++i) {
            ++nid;
            GraphNodeItem *chld = gnode->getChild(i);
            if (chld) {
                stream << nid;
                queue.push_back(chld);
                queueid.push_back(nid);
            }
            stream << ",";
        }

        stream << node->value();
        stream << endl;
    }

    return true;
}