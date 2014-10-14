#include "nodedata.h"

#include <displacemodel.h>

NodeData::NodeData(Node *nd, DisplaceModel *model)
    : mNode (nd),
      mModel(model),
      mPop(0),
      mPopTot(0)
{
    if (nd) {
        int N = nd->get_nbpops();
        mPop = new double[N] ;
        mImpact = new double[N];
        for (int i = 0; i < N; ++i) {
            mPop[i] = 0.0;
            mImpact[i] = 0.0;
        }
    }
}

NodeData::~NodeData()
{
    delete []mPop;
    delete []mImpact;
}

int NodeData::getPopCount() const
{
    return mNode->get_nbpops();
}

void NodeData::setPop(int pop, double v)
{
    if (pop < mNode->get_nbpops() && pop > 0)
        mPop[pop] = v;
}

void NodeData::setPop(QList<double> v, double tot)
{
    for (int i=0; i < v.size() && i < mNode->get_nbpops(); ++i) {
        mPop[i] = v[i];
    }
    mPopTot = tot;
}

double NodeData::getPop(int pop) const
{
    if (pop < mNode->get_nbpops() && pop >= 0)
        return mPop[pop];

    return -1;
}

void NodeData::setImpact(int pop, double impact)
{
    if (pop < mNode->get_nbpops() && pop >= 0)
        mImpact[pop] = impact;
}

