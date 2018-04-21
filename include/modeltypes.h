#ifndef MODELTYPES_H
#define MODELTYPES_H

#include "StrongType.h"
#include "idtypes.h"

namespace types {

struct TStepTag {};
using tstep_t = utils::StrongType<uint16_t, TStepTag>;

struct EnvironmentData {
    types::NodeId nodeId;
    tstep_t tstep;

    int marineLandscape;
    double salinity;
    double sst;
    double wind;
    double nitrogen;
    double phosphorus;
    double oxygen;
    double dissolvedcarbon;
};

struct NodesStatData {
    types::NodeId nodeId;
    tstep_t tstep;
    double cumftime, swarea, ssswarea, cumcat, cumcatthr, cumdis;
};

}

#endif // MODELTYPES_H
