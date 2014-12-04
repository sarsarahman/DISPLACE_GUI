#include "edgeaddmousemode.h"

#include <displacemodel.h>

EdgeAddMouseMode::EdgeAddMouseMode(DisplaceModel *model)
    : MouseMode(),
      mModel(model),
      mStatus(0)
{
}

bool EdgeAddMouseMode::releaseEvent(const QPointF &point)
{
    QList<std::shared_ptr<NodeData> > nodes = mModel->getAllNodesWithin(point, 1);

    qDebug() << "Nodes IN " << point << nodes.size() << mStatus;

    if (nodes.size() != 1)
        return false;

    switch (mStatus) {
    case 0:     // first node
        mNodeFrom = nodes[0]->get_idx_node();
        ++mStatus;
        break;
    case 1:
        mNodeTo = nodes[0]->get_idx_node();
        emit edgeAdded(mNodeFrom, mNodeTo);
        break;
    }

    return true;
}

bool EdgeAddMouseMode::endMode(bool success)
{
    Q_UNUSED(success);
    return true;
}

