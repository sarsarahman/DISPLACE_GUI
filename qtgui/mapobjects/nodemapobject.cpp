#include "nodemapobject.h"

#include <displacemodel.h>
#include <mapobjects/nodegraphics.h>

NodeMapObject::NodeMapObject(MapObjectsController *controller, int indx, Role role, NodeData *node)
    : mNode(node),
      mController(controller),
      mGeometry()
{
    switch (role) {

    case GraphNodeRole:
        mGeometry = std::shared_ptr<NodeGraphics>(
                    new NodeGraphics(mNode, mController, indx));
        break;

    case GraphNodeWithPopStatsRole:
        mGeometry = std::shared_ptr<NodeGraphics>(
                    new NodeWithPopStatsGraphics(mNode, mController, indx));
        break;

    case GraphNodeWithCumFTimeRole:
        mGeometry = std::shared_ptr<NodeGraphics>(
                    new NodeWithCumFTimeGraphics(mNode, mController, indx));
        break;

    }

}
