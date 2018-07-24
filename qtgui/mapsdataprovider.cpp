#include <storage/tables/poptable.h>
#include "mapsdataprovider.h"
#include "storage/tables/nodesenvttable.h"

#include "storage/cacheddatastorage.h"

namespace {

template <typename T>
void setAt(std::vector<T> &vec, size_t idx, T value) {
    while (vec.size() <= idx)
        vec.push_back(T());
    vec.at(idx) = value;
}

class EnvironmentCachedData : public CachedDataStorage<types::EnvironmentData> {
    NodesEnvtTable &table;
public:
    EnvironmentCachedData(NodesEnvtTable &t) : table(t) {}
    void queryAllData(types::tstep_t step) override {
        table.queryAllNodesAtStep(step,[this,step](NodesEnvtTable::NodeEnvt n){
            updateData(step, n);
            return true;
        } );
    }
};

class NodePopCachedData : public CachedDataStorage<types::NodesPopData> {
    PopTable &table;
public:
    NodePopCachedData(PopTable &t) : table(t) {}
    void queryAllData(types::tstep_t step) override {
        table.queryAllNodesAtStep(step.value(),[this,step](PopTable::Stat n) {
            auto &d = getRecord(n.nodeId);
            d.dirty = false;
            d.cachedTstep = step;
            if (!d.data)
                d.data = std::make_shared<types::NodesPopData>();

            d.data->tstep = types::tstep_t(n.tstep);
            d.data->nodeId = n.nodeId;

            setAt(d.data->totN, n.popId, n.totNid);
            setAt(d.data->totW, n.popId, n.totWid);
            setAt(d.data->cumC, n.popId, n.cumC);
            setAt(d.data->cumD, n.popId, n.cumD);
            setAt(d.data->impact, n.popId, n.impact);

            return true;
        } );
    }
};

}

struct MapsDataProvider::Impl
{
    std::shared_ptr<SQLiteOutputStorage> db;
    std::shared_ptr<NodesEnvtTable> envTable;

    std::shared_ptr<EnvironmentCachedData> environmentData;
    std::shared_ptr<NodePopCachedData> nodeData;
};

MapsDataProvider::MapsDataProvider()
    : p(std::make_unique<Impl>())
{

}

MapsDataProvider::~MapsDataProvider() noexcept = default;

void MapsDataProvider::attach(std::shared_ptr<SQLiteOutputStorage> storage)
{
    if (p->db == storage)
        return;

    p->db = storage;
    p->envTable = p->db->getNodesEnvtTable();

    p->environmentData = std::make_shared<EnvironmentCachedData>(*p->envTable);
    p->environmentData->invalidateAllCache();

    auto t = p->db->getPopTable();
    p->nodeData = std::make_shared<NodePopCachedData>(*t);
    p->nodeData->invalidateAllCache();
}

std::shared_ptr<types::EnvironmentData> MapsDataProvider::getEnvironmentData(types::NodeId nodeId, types::tstep_t step)
{
    // not attached. No data.
    if (p->db == nullptr)
        return nullptr;
    return p->environmentData->getData(nodeId, step);
}

std::shared_ptr<types::NodesPopData> MapsDataProvider::getNodesPopData(types::NodeId nodeId, types::tstep_t tstep)
{
    if (p->db == nullptr)
        return nullptr;
    return p->nodeData->getData(nodeId, tstep);
}
