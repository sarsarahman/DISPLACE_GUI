#include "displacemodel.h"
#include <exceptions.h>
#include <dbhelper.h>
#include <calendar.h>

#include <mapobjects/harbourmapobject.h>
#include <profiler.h>

#include <mapobjectscontroller.h>

#include <readdata.h>
#include <qdebug.h>
#include <QtAlgorithms>

const char *FLD_NODEID="nodeid";

DisplaceModel::DisplaceModel()
    : mModelType(EmptyModelType),
      mDb(0),
      mCalendar(),
      mInputName(),mBasePath(),mOutputName(),
      mSimuName("simu2"),
      mLinkedDbName(),
      mIndex(-1),
      mSimulSteps(8761),
      mCurrentStep(0), mLastStep(0),
      mLastStats(-1),
      mNodesStatsDirty(false),
      mPopStatsDirty(false),
      mInterestingPop(),
      mInterestingSizeTotal(true),
      mInterestingSizeAvg(true),
      mInterestingSizeMin(false),
      mInterestingSizeMax(false),
      mInterestingSizes(),
      mOutputFileParser(new OutputFileParser(this)),
      mParserThread(new QThread(this))
{
    OGRRegisterAll();

    const char *pszDriverName = "Memory";
    OGRSFDriver *poDriver;
    poDriver = OGRSFDriverRegistrar::GetRegistrar()->GetDriverByName(pszDriverName);

    mDataSource = poDriver->CreateDataSource("memory");
    Q_ASSERT(mDataSource);

    mSpatialRef = new OGRSpatialReference();
    mSpatialRef->SetWellKnownGeogCS("WGS84");

    mNodesLayer = mDataSource->CreateLayer("nodes", mSpatialRef, wkbPoint);
    Q_ASSERT(mNodesLayer);

    // Create any field
    OGRFieldDefn fld(FLD_NODEID, OFTInteger);
    mNodesLayer->CreateField(&fld);

    mOutputFileParser->moveToThread(mParserThread);
    mParserThread->start();

    connect(this, SIGNAL(parseOutput(QString,int)), mOutputFileParser, SLOT(parse(QString,int)));
    connect (mOutputFileParser, SIGNAL(error(QString)), SIGNAL(errorParsingStatsFile(QString)));
    connect (mOutputFileParser, SIGNAL(parseCompleted()), SIGNAL(outputParsed()));
}

bool DisplaceModel::edit(QString modelname)
{
    if (mModelType != EmptyModelType)
        return false;

    // ...

    mInputName = modelname;
    mModelType = EditorModelType;

    return true;
}

bool DisplaceModel::load(QString path)
{
    if (mModelType != EmptyModelType)
        return false;

    qDebug() << "Loading model from folder" << path;

    if (!parse(path, &mBasePath, &mInputName, &mOutputName)) {
        mLastError = tr("Cannot parse path for base name and simulation name.");
        return false;
    }
    mFullPath = path;

    qDebug() << "Loading: " << mBasePath << mInputName << mOutputName;

    /* Load files ... */

    try {
        mScenario = Scenario::readFromFile(mInputName, mBasePath, mOutputName);
        mConfig = Config::readFromFile(mInputName, mBasePath, mOutputName);
        mCalendar = std::shared_ptr<Calendar> (Calendar::load(mBasePath, mInputName));

        mInterestingHarb = mConfig.m_interesting_harbours;

        loadNodes();
        loadVessels();
        loadGraphs();
        initBenthos();
        initPopulations();
        initNations();
    } catch (DisplaceException &ex) {
        mLastError = ex.what();
        qDebug() << "ERROR: " << mLastError;
        return false;
    }

    mModelType = LiveModelType;
    return true;
}

bool DisplaceModel::parse(const QString &path, QString *basepath, QString *inputname, QString *outputname)
{
    // parse this form:  inputfolder + "/simusspe_" + folder_name_parameterization + "/" + namefolderoutput+".dat";

    QRegExp regexp("(.*)/simusspe_([a-zA-Z0-9]+)/([a-zA-Z09]+).dat");

    if (regexp.indexIn(path) == -1) {
        return false;
    }

    *basepath = regexp.cap(1);
    *inputname = regexp.cap(2);
    *outputname = regexp.cap(3);

    return true;
}

bool DisplaceModel::loadDatabase(QString path)
{
    if (mModelType != EmptyModelType || mDb != 0)
        return false;

    mDb = new DbHelper;
    if (!mDb->attachDb(path))
        return false;

    mInputName = mDb->getMetadata("name");
    mBasePath = mDb->getMetadata("basepath");
    mOutputName = mDb->getMetadata("output");

    mDb->loadConfig(mConfig);
    mDb->loadScenario(mScenario);
    loadNodesFromDb();
    loadVesselsFromDb();
    loadHistoricalStatsFromDb();
    initPopulations();
    initNations();

    mLastStep = mDb->getLastKnownStep();
    setCurrentStep(0);

    mModelType = OfflineModelType;
    return true;
}

/** Link a database to a opened "live" simulation
 *
 * Note: when this is called, the live simulation has been already loaded so
 * Vessel names, harbours and other objects are still known.
 *
 */
bool DisplaceModel::linkDatabase(QString path)
{
    if (mModelType != LiveModelType) {
        mLastError = tr("Model is not a live simulation");
        return false;
    }

    mDb = new DbHelper;
    if (!mDb->attachDb(path)) {
        mLastError = mDb->lastDbError();
        return false;
    }

    mLinkedDbName = path;

    return true;
}

bool DisplaceModel::prepareDatabaseForSimulation()
{
    if (mDb) {
        /* start a transaction to speedup insertion */
        mDb->beginTransaction();

        mDb->setMetadata("name", mInputName);
        mDb->setMetadata("basepath", mBasePath);
        mDb->setMetadata("output", mOutputName);

        mDb->saveConfig(mConfig);
        mDb->saveScenario(mScenario);

        mDb->removeAllStatsData();

        /* load nodes */
        mDb->removeAllNodesDetails();
        for (int i = 0; i < mNodes.size(); ++i) {
            mDb->addNodesDetails(i, mNodes.at(i));
        }

        /* load vessels */
        mDb->removeAllVesselsDetails();
        for (int i = 0; i< mVessels.size(); ++i) {
            mDb->addVesselDetails(i, mVessels.at(i));
        }

        /* end: commit transaction */
        mDb->endTransaction();
    }

    return true;
}

bool DisplaceModel::clearStats()
{
    mStatsPopulations.clear();
    for (int i = 0; i < mStatsNationsCollected.size(); ++i) {
        mStatsPopulationsCollected[i].clear();
    }

    mStatsNations.clear();
    mStatsNationsCollected.clear();

    mStatsHarbours.clear();
    mStatsHarboursCollected.clear();

    return true;
}

bool DisplaceModel::saveScenarioAs(const QString &path)
{
    if (!parse(path, &mBasePath, &mInputName, &mOutputName))
        return false;

    mFullPath = path;
    return saveScenario();
}

bool DisplaceModel::saveScenario()
{
    QString error;
    if (!mScenario.save(mBasePath, mInputName, mOutputName, &error)) {
        mLastError = error;
        return false;
    }

    return true;
}

bool DisplaceModel::saveConfig()
{
    QString error;
    if (!mConfig.save(mBasePath, mInputName, mOutputName, &error)) {
        mLastError = error;
        return false;
    }

    return true;
}

void DisplaceModel::simulationEnded()
{
    if (mDb) {
        mDb->flushBuffers();
        mDb->createIndexes();
    }
}

int DisplaceModel::getHarboursCount() const
{
    return mHarbours.size();
}

QString DisplaceModel::getHarbourId(int idx) const
{
    return QString::fromStdString(mHarbours.at(idx)->mHarbour->get_name());
}

int DisplaceModel::getNodesCount() const
{
    return mNodes.size();
}

QString DisplaceModel::getNodeId(int idx) const
{
    return QString::fromStdString(mNodes.at(idx)->get_name());
}

void DisplaceModel::checkStatsCollection(int tstep)
{
    if (mLastStats != tstep && mNodesStatsDirty) {
        commitNodesStatsFromSimu(tstep);
    }

    mLastStats = tstep;
}
int DisplaceModel::getSimulationSteps() const
{
    return mSimulSteps;
}

void DisplaceModel::setSimulationSteps(int value)
{
    mSimulSteps = value;
}


void DisplaceModel::updateNodesStatFromSimu(QString data)
{
    QStringList fields = data.split(",");
    int tstep = fields[1].toInt();
    int start = fields[2].toInt();
    int num = fields[3].toInt();

    checkStatsCollection(tstep);

    if (fields[0] == "cumftime") {
        for (int i = 0; i < num; ++i) {
            mNodes.at(start + i)->set_cumftime(fields[4+i].toDouble());
        }
        mNodesStatsDirty = true;
    }
}

void DisplaceModel::commitNodesStatsFromSimu(int tstep)
{
    if (mDb)
        mDb->beginTransaction();

    if (mNodesStatsDirty) {
        if (mDb)
            mDb->addNodesStats(mLastStats, mNodes);
        mNodesStatsDirty = false;
    }

    if (mPopStatsDirty) {
        mStatsPopulations.insertValue(tstep, mStatsPopulationsCollected);
        if (mDb)
            mDb->addPopStats(mLastStats, mStatsPopulationsCollected);
        mPopStatsDirty = false;
    }

    if (mVesselsStatsDirty) {
        mStatsNations.insertValue(tstep, mStatsNationsCollected);
        if (mDb)
            mDb->addNationsStats (mLastStats, mStatsNationsCollected);

        // Harbours stats are not saved on db, but loaded on the fly
        mStatsHarbours.insertValue(tstep, mStatsHarboursCollected);

        if (mCalendar && mCalendar->isYear(tstep)) {
            mStatsNationsCollected.clear();
            mStatsHarboursCollected.clear();
        }

        // don't clear stats: they are cumulated
//        mStatsNationsCollected.clear();
//        mStatsHarboursCollected.clear();
        mVesselsStatsDirty = false;
    }


    if (mDb)
        mDb->endTransaction();
}

void DisplaceModel::startCollectingStats()
{
    if (mDb)
        mDb->beginTransaction();
}

void DisplaceModel::endCollectingStats()
{
    if (mDb)
        mDb->endTransaction();
}

void DisplaceModel::collectNodePopStats(int tstep, int node_idx, const QList<double> &stats, const QList<double> &stats_w, double tot, double wtot)
{
    checkStatsCollection(tstep);
    mNodes.at(node_idx)->setPop(stats, tot);
    mNodes.at(node_idx)->setPopW(stats_w, wtot);

    mNodesStatsDirty = true;
}

void DisplaceModel::collectPopCumftime(int step, int node_idx, double cumftime)
{
    checkStatsCollection(step);
    mNodes.at(node_idx)->set_cumftime(cumftime);
    mNodesStatsDirty = true;
}

void DisplaceModel::collectPopImpact(int step, int node_idx, int popid, double impact)
{
    checkStatsCollection(step);
    mNodes.at(node_idx)->setImpact(popid, impact);
    mNodesStatsDirty = true;
}

void DisplaceModel::collectPopdynN(int step, int popid, const QVector<double> &pops, double value)
{
    checkStatsCollection(step);
    mStatsPopulationsCollected[popid].setAggregate(pops);
    mStatsPopulationsCollected[popid].setAggregateTot(value);
    mPopStatsDirty = true;
}

void DisplaceModel::collectPopdynF(int step, int popid, const QVector<double> &pops, double value)
{
    checkStatsCollection(step);
    mStatsPopulationsCollected[popid].setMortality(pops);
    mStatsPopulationsCollected[popid].setMortalityTot(value);
    mPopStatsDirty = true;
}

void DisplaceModel::collectVesselStats(int tstep, const VesselStats &stats)
{
    std::shared_ptr<VesselData> vessel = mVessels.at(stats.vesselId);

    vessel->setLastHarbour(stats.lastHarbour);
    vessel->setRevenueAV(stats.revenueAV);
    vessel->mVessel->set_reason_to_go_back(stats.reasonToGoBack);
    vessel->mVessel->set_timeatsea(stats.timeAtSea);

    int nat = vessel->getNationality();
    while (mStatsNationsCollected.size() <= nat) {
        mStatsNationsCollected.push_back(NationStats());
    }

    mStatsNationsCollected[nat].mRevenues += stats.revenueAV;
    mStatsNationsCollected[nat].mTimeAtSea += stats.timeAtSea;
    mStatsNationsCollected[nat].mGav += stats.gav;
    mStatsNationsCollected[nat].mVpuf += stats.vpuf();

    int hidx = mNodes[vessel->getLastHarbour()]->getHarbourId();
    while (mStatsHarboursCollected.size() <= hidx)
        mStatsHarboursCollected.push_back(HarbourStats());

    mStatsHarboursCollected[hidx].mCumProfit += stats.revenueAV;
    mStatsHarboursCollected[hidx].mGav += stats.gav;
    mStatsHarboursCollected[hidx].mVpuf += stats.vpuf();

    int n = stats.mCatches.size();
    for (int i = 0; i < n; ++i) {
        vessel->addCatch(i, stats.mCatches[i]);
        mStatsHarboursCollected[hidx].mCumCatches += stats.mCatches[i];
        mStatsNationsCollected[nat].mTotCatches += stats.mCatches[i];
    }

    if (mDb)
        mDb->addVesselStats(tstep,*vessel);

    mVesselsStatsDirty = true;
}

bool DisplaceModel::addGraph(const QList<GraphBuilder::Node> &nodes, MapObjectsController *controller)
{
    if (mModelType != EditorModelType)
        return false;

    QList<std::shared_ptr<NodeData> > newnodes;
    int nodeidx = mNodes.count();
    int cntr = 0;
    foreach(GraphBuilder::Node node, nodes) {
//        if (!node.good) {
//            continue;
//        }

        int nodeid = mNodes.size();

        OGRFeature *feature = OGRFeature::CreateFeature(mNodesLayer->GetLayerDefn());
        feature->SetField(FLD_NODEID, nodeid);

        OGRPoint pt;
        pt.setX(node.point.x());
        pt.setY(node.point.y());

        feature->SetGeometry(&pt);

        mNodesLayer->CreateFeature(feature);

        std::shared_ptr<Node> nd (new Node(cntr, node.point.x(), node.point.y(),0,0,0,0,0));
        std::shared_ptr<NodeData> nodedata (new NodeData(nd, this));
        mNodes.push_back(nodedata);

        if (node.good) {
            foreach (int adidx, node.adiacencies) {
                if (nodes[adidx].good)
                    nodedata->appendAdiancency(adidx + nodeidx, 0.0);
            }
        } else {
            nodedata->setDeleted(true);
        }

        newnodes.push_back(nodedata);
        ++cntr;
    }

    foreach(std::shared_ptr<NodeData> node, newnodes) {
        controller->addNode(mIndex, node);
    }
    controller->redraw();

    return true;
}

bool DisplaceModel::exportGraph(const QString &path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        mLastError = file.errorString();
        return false;
    }

    QTextStream strm(&file);
    foreach (std::shared_ptr<NodeData> nd, mNodes) {
        if (!nd->isDeleted())
            strm << nd->get_x() << " " << nd->get_y() << " " << "0" << endl;
    }

    file.close();
    return true;
}

bool DisplaceModel::importHarbours(QList<std::shared_ptr<HarbourData> > &list)
{
    foreach (std::shared_ptr<HarbourData> h, list) {
        h->mHarbour->set_is_harbour(mHarbours.size());
        mHarbours.push_back(h);

        std::shared_ptr<NodeData> n (new NodeData(h->mHarbour, this));
        mNodes.push_back(n);
    }

    return true;
}

int DisplaceModel::getVesselCount() const
{
    return mVessels.size();
}

QString DisplaceModel::getVesselId(int idx) const
{
    return QString::fromStdString(mVessels.at(idx)->mVessel->get_name());
}

void DisplaceModel::updateVessel(int tstep, int idx, float x, float y, float course, float fuel, int state)
{
    std::shared_ptr<VesselData> v(mVessels.at(idx));
    v->mVessel->set_xy(x,y);
    v->mVessel->set_course(course);
    v->mVessel->set_cumfuelcons(fuel);
    v->mVessel->set_state(state);

    if (mDb) {
        mDb->addVesselPosition(tstep, idx, v);
    }
}

int DisplaceModel::getBenthosCount() const
{
    return mBenthos.size();
}

int DisplaceModel::getPopulationsCount() const
{
    return mConfig.getNbpops();
}

HarbourStats DisplaceModel::retrieveHarbourIdxStatAtStep(int idx, int step)
{
    if (mModelType != OfflineModelType || !mDb) {
        return HarbourStats(); //getHarboursStatAtStep(step, idx);
    }

    return mDb->getHarbourStatsAtStep(idx, step);
}

Scenario DisplaceModel::scenario() const
{
    return mScenario;
}

void DisplaceModel::setScenario(const Scenario &scenario)
{
    mScenario = scenario;
    if (mDb)
        mDb->saveScenario(mScenario);
}

Config DisplaceModel::config() const
{
    return mConfig;
}

void DisplaceModel::setConfig(const Config &config)
{
    mConfig = config;
    if (mDb)
        mDb->saveConfig(mConfig);

    mInterestingHarb = mConfig.m_interesting_harbours;
}

void DisplaceModel::setCurrentStep(int step)
{
    mCurrentStep = step;
    if (mDb) {
        mDb->updateVesselsToStep(mCurrentStep, mVessels);
        mDb->updateStatsForNodesToStep(mCurrentStep, mNodes);

        // re-loading Historical data is not needed!

        /* TODO: Update here all other entries */
    }
}

void DisplaceModel::setInterestingPop(int n)
{
    if (!mInterestingPop.contains(n))
        mInterestingPop.append(n);
        qSort(mInterestingPop);
}

void DisplaceModel::remInterestingPop(int n)
{
    mInterestingPop.removeAll(n);
}

bool DisplaceModel::isInterestingPop(int n)
{
    return mInterestingPop.contains(n);
}

void DisplaceModel::clearInterestingPop()
{
    mInterestingPop.clear();
}

void DisplaceModel::setInterestingSize(int n)
{
    if (!mInterestingSizes.contains(n))
        mInterestingSizes.append(n);
    qSort(mInterestingSizes);
}

void DisplaceModel::remInterestingSize(int n)
{
    mInterestingSizes.removeAll(n);
}

bool DisplaceModel::isInterestingSize(int n)
{
    return mInterestingSizes.contains(n);
}

void DisplaceModel::setInterestingHarb(int n)
{
    if (!mInterestingHarb.contains(n))
        mInterestingHarb.append(n);
    qSort(mInterestingHarb);
}

void DisplaceModel::remInterestingHarb(int n)
{
    mInterestingHarb.removeAll(n);
}

bool DisplaceModel::isInterestingHarb(int n)
{
    return mInterestingHarb.contains(n);
}

void DisplaceModel::parseOutputStatsFile(QString file, int tstep)
{
    emit parseOutput(file, tstep);
}

/* Warn: copy and pasted from simulator's main.cpp */
#define NBSZGROUP 14

bool DisplaceModel::loadNodes()
{
    int nrow_coord = mScenario.getNrow_coord();
    int a_port = mScenario.getA_port();
    vector<string> dyn_alloc_sce = mScenario.getDyn_alloc_sce_asVector();
    int nbpops = mConfig.getNbpops();
    string a_quarter= "quarter1";// start quarter

    // input data, coord nodes of the graph
    ifstream coord_graph;

    // casting a_pop into a string
    stringstream out;
    out << mScenario.getGraph();
    string a_graph_s = out.str();
    string filename_graph = mBasePath.toStdString() +
            "/graphsspe/coord" + a_graph_s + ".dat";

    string filename_code_area_graph = mBasePath.toStdString() +
            "/graphsspe/code_area_for_graph" + a_graph_s + "_points.dat";

    string filename_code_marine_landscape_graph = mBasePath.toStdString() +
            "/graphsspe/coord" + a_graph_s + "_with_landscape.dat";

    coord_graph.open(filename_graph.c_str());
    if(coord_graph.fail()) {
        throw DisplaceException(QString(QObject::tr("Cannot load %1: %2"))
                                .arg(filename_graph.c_str())
                                .arg(strerror(errno)));
    }

    vector<double> graph_coord_x;
    vector<double> graph_coord_y;
    vector<int> graph_coord_harbour;
    fill_from_coord(coord_graph, graph_coord_x, graph_coord_y, graph_coord_harbour, nrow_coord);
    coord_graph.close();

    // input data, code area for each point of the graph (e.g. 1: NS, 2: BW, 3: BE, 10: open sea)
    ifstream code_area_graph;
    code_area_graph.open(filename_code_area_graph.c_str());
    if(code_area_graph.fail())
    {
        throw DisplaceException(QString(QObject::tr("Cannot load %1: %2"))
                                .arg(filename_code_area_graph.c_str())
                                .arg(strerror(errno)));
    }
    vector<int> graph_point_code_area;
    fill_from_code_area(code_area_graph, graph_point_code_area, nrow_coord);

    // input data, for the marine landscape for each point of the graph (e.g. 111, 112, etc. e.g. see the BALANCE map coding)
    ifstream code_landscape_graph;
    code_landscape_graph.open(filename_code_marine_landscape_graph.c_str());
    if(code_landscape_graph.fail())
    {
        throw DisplaceException(QString(QObject::tr("Cannot load %1: %2"))
                                .arg(filename_code_marine_landscape_graph.c_str())
                                .arg(strerror(errno)));
    }
    vector<int> graph_point_code_landscape;
    fill_from_code_marine_landscape(code_landscape_graph, graph_point_code_landscape, nrow_coord);

    // read harbour specific files
    multimap<int, string> harbour_names = read_harbour_names(mInputName.toStdString(), mBasePath.toStdString());

    // creation of a vector of nodes from coord
    // and check with the coord in input.
    // use inheritance i.e. a Harbour is child of a Node
    // use polymorphism i.e. store either Harbour or Node in the vector of pointers 'nodes'

//    vector <Node* > nodes(graph_coord_x.size());
    // REPLACE boost::shared_ptr<Node> by Node* in every files if you want to remove boost

    for (unsigned int i=0; i<graph_coord_x.size(); i++)
    {
        if(graph_coord_harbour[i])
        {
            string a_name="none";
            int a_point=0;
            // get the name of this harbour
            multimap<int,string>::iterator lower_g = harbour_names.lower_bound(i);
            multimap<int,string>::iterator upper_g = harbour_names.upper_bound(i);
            for (multimap<int, string>::iterator pos=lower_g; pos != upper_g; pos++)
            {
                a_point= pos->first;
                a_name= pos->second;
            }

            map<string,double> init_fuelprices;
            multimap<int, double> fishprices_each_species_per_cat;
            if(a_name!="none" && a_point== i)
            {

                cout << "load prices for port " << a_name << " which is point " << a_point << endl;
                //int er = read_prices_per_harbour(a_point, a_quarter, prices, mName.toStdString());
                int er2 = read_prices_per_harbour_each_pop_per_cat(a_point,  a_quarter, fishprices_each_species_per_cat, mInputName.toStdString(), mBasePath.toStdString());
                // if not OK then deadly bug: possible NA or Inf in harbour files need to be checked (step 7)
                cout << "....OK" << endl;
            }
            else
            {

                cout << a_point << " : harbour not found in the harbour names (probably because no declared landings from studied vessels in those ports)" << endl;
                //int er = read_prices_per_harbour(a_port, "quarter1", prices, mName.toStdString()); // delete later on when final parameterisation
                int er2 = read_prices_per_harbour_each_pop_per_cat(a_port, "quarter1", fishprices_each_species_per_cat, mInputName.toStdString(), mBasePath.toStdString());

            }

            // read fuel price (vessel size dependent for the time being)
            if (!binary_search (dyn_alloc_sce.begin(), dyn_alloc_sce.end(),
                                "fuelprice_plus20percent"))
            {
                read_fuel_prices_per_vsize(init_fuelprices, mInputName.toStdString(), mBasePath.toStdString());
            }
            else
            {
                read_fuel_prices_per_vsize(init_fuelprices, mInputName.toStdString(), mBasePath.toStdString());

                map<string,double>::iterator pos;
                for (pos=init_fuelprices.begin(); pos != init_fuelprices.end(); pos++)
                {
                    pos->second = (pos->second)*1.2;
                }

                for (pos=init_fuelprices.begin(); pos != init_fuelprices.end(); pos++)
                {
                    cout << pos->first << " " << pos->second;
                }

            }

            std::shared_ptr<Harbour> h (new Harbour(i,
                                       graph_coord_x[i],
                                       graph_coord_y[i],
                                       graph_coord_harbour[i],
                                       graph_point_code_area[i],
                                       graph_point_code_landscape[i],
                                       nbpops,
                                       NBSZGROUP,
                                       a_name,
                                       //prices,
                                       fishprices_each_species_per_cat,
                                       init_fuelprices
                                       ));
            std::shared_ptr<HarbourData> hd (new HarbourData(h));
            mHarbours.push_back(hd);

            std::shared_ptr<NodeData> n (new NodeData(h, this));
            n->setHarbourId(mHarbours.size()-1);
            mNodes.push_back(n);
        }
        else
        {
            std::shared_ptr<Node> nd (new Node(i,
                                 graph_coord_x[i],
                                 graph_coord_y[i],
                                 graph_coord_harbour[i],
                                 graph_point_code_area[i],
                                 graph_point_code_landscape[i],
                                 nbpops,
                                 NBSZGROUP));
            std::shared_ptr<NodeData> n(new NodeData(nd, this));

            mNodes.push_back(n);

        }
    }

    /* Not sure if we need this...
    // init
    for (unsigned int i=0; i< nodes.size(); i++)
    {
        nodes.at(i)->init_Ns_pops_at_szgroup(nbpops, NBSZGROUP);
        // caution:magic number...i.e. from the R poisson regression on cpue
        nodes.at(i)->init_avai_pops_at_selected_szgroup(nbpops,SEL_NBSZGROUP);
    }
    */

    return true;
}

bool DisplaceModel::loadVessels()
{
    int nrow_coord = mScenario.getNrow_coord();
    int a_port = mScenario.getA_port();
    vector<string> dyn_alloc_sce = mScenario.getDyn_alloc_sce_asVector();
    int nbpops = mConfig.getNbpops();
    string a_quarter= "quarter1";// start quarter
    string a_semester= "semester1";

    int selected_vessels_only = 0;
    string a_graph_name="a_graph";

    stringstream graphnum;
    graphnum << mScenario.getGraph();
    a_graph_name=a_graph_name+graphnum.str();

    // read general vessel features
    // (quarter specific, mainly because of the gamma parameters)
    vector<string> vesselids;
    vector<double> speeds;
    vector<double> fuelcons;
    vector<double> lengths;
    vector<double> KWs;
    vector<double> carrycapacities;
    vector<double> tankcapacities;
    vector<double> nbfpingspertrips;
    vector<double> resttime_par1s;
    vector<double> resttime_par2s;
    vector<double> av_trip_duration;
    vector<double> mult_fuelcons_when_steaming;
    vector<double> mult_fuelcons_when_fishing;
    vector<double> mult_fuelcons_when_returning;
    vector<double> mult_fuelcons_when_inactive;
    read_vessels_features(a_quarter, vesselids, speeds, fuelcons, lengths, KWs,
                          carrycapacities, tankcapacities, nbfpingspertrips,
                          resttime_par1s, resttime_par2s, av_trip_duration,
                          mult_fuelcons_when_steaming, mult_fuelcons_when_fishing,
                          mult_fuelcons_when_returning, mult_fuelcons_when_inactive,
                          mInputName.toStdString(), mBasePath.toStdString(), selected_vessels_only);


    // read the more complex objects (i.e. when several info for a same vessel)...
    // also quarter specific but semester specific for the betas because of the survey design they are comning from...
    multimap<string, int> fgrounds = read_fgrounds(a_quarter, mInputName.toStdString(), mBasePath.toStdString());
    multimap<string, int> harbours = read_harbours(a_quarter, mInputName.toStdString(), mBasePath.toStdString());

    multimap<string, double> freq_fgrounds = read_freq_fgrounds(a_quarter, mInputName.toStdString(), mBasePath.toStdString());
    multimap<string, double> freq_harbours = read_freq_harbours(a_quarter, mInputName.toStdString(), mBasePath.toStdString());
    multimap<string, double> vessels_betas = read_vessels_betas(a_semester, mInputName.toStdString(), mBasePath.toStdString());
    multimap<string, double> vessels_tacs   = read_vessels_tacs(a_semester, mInputName.toStdString(), mBasePath.toStdString());

    // debug
    if(fgrounds.size() != freq_fgrounds.size())
    {
        cout<< "please correct .dat files so that fgrounds and freq_fgrounds have same size!!!" << endl;
        int tmp;
        cin >> tmp;				 // pause
    }
    if(harbours.size() != freq_harbours.size())
    {
        cout<< "please correct .dat files so that harbours and freq_harbours have same size!!!" << endl;
        int tmp;
        cin >> tmp;				 // pause
    }

    // read nodes in polygons for area-based management
    multimap<int, int> nodes_in_polygons= read_nodes_in_polygons(a_quarter, a_graph_name, mInputName.toStdString(), mBasePath.toStdString());

    // check
    //for (multimap<int, int>::iterator pos=nodes_in_polygons.begin(); pos != nodes_in_polygons.end(); pos++)
    //{
    //    cout << " a polygon node is " << pos->second << endl;
    //}
    //cout << " for " << a_graph_name << "in quarter " << a_quarter << endl;

    //creation of a vector of vessels from vesselids, graph, harbours and fgrounds
    // and check the start coord
    multimap<int, int> possible_metiers;
    multimap<int, double> freq_possible_metiers;
    multimap<int, double> gshape_cpue_per_stk_on_nodes;
    multimap<int, double> gscale_cpue_per_stk_on_nodes;
    vector<int> spe_fgrounds;
    vector<int> spe_harbours;
    vector<double> spe_freq_fgrounds;
    vector<double> spe_freq_harbours;
    vector<double> spe_vessel_betas_per_pop;
    vector<double> spe_percent_tac_per_pop;

                                 //here
    vector <Vessel*> vessels(vesselids.size());
    for (int i=0; i<vesselids.size(); i++)
        //vector <Vessel*> vessels(7); //here
        //vesselids.erase (vesselids.begin());
        //for (int i=0; i<7; i++)
    {
        cout<<"create vessel " << i << endl;
        // read vessel and quarter specific multimap
        // quarter specific to capture a piece of seasonality in the fishnig activity
        possible_metiers = read_possible_metiers(a_quarter, vesselids[i], mInputName.toStdString(), mBasePath.toStdString());
        freq_possible_metiers = read_freq_possible_metiers(a_quarter, vesselids[i], mInputName.toStdString(), mBasePath.toStdString());

        //cpue_per_stk_on_nodes = read_cpue_per_stk_on_nodes(a_quarter, vesselids[i], mName.toStdString());
        gshape_cpue_per_stk_on_nodes = read_gshape_cpue_per_stk_on_nodes(a_quarter, vesselids[i], mInputName.toStdString(), mBasePath.toStdString());
        gscale_cpue_per_stk_on_nodes = read_gscale_cpue_per_stk_on_nodes(a_quarter, vesselids[i], mInputName.toStdString(), mBasePath.toStdString());

        // debug
        if(possible_metiers.size() != freq_possible_metiers.size())
        {
            cout<< "please correct .dat files so that possible_metiers and freq_possible_metiers have same size!!!"
                << "for the vessel " << vesselids[i] << endl;
            int tmp;
            cin >> tmp;			 // pause
        }

        // read the even more complex objects (i.e. when several info for a same vessel and a same ground)...
        // for creating the vessel object, search into the multimaps
        spe_fgrounds = find_entries_s_i(fgrounds, vesselids[i]);
        spe_freq_fgrounds = find_entries_s_d(freq_fgrounds, vesselids[i]);
        spe_harbours = find_entries_s_i(harbours, vesselids[i]);
        spe_freq_harbours = find_entries_s_d(freq_harbours, vesselids[i]);
        spe_vessel_betas_per_pop = find_entries_s_d(vessels_betas, vesselids[i]);
        spe_percent_tac_per_pop = find_entries_s_d(vessels_tacs, vesselids[i]);

        // choose a departure (node) harbour for this vessel according to the observed frequency in data
        int start_harbour;
        if(!spe_harbours.empty())
        {
                                 // need to convert in array, see myRutils.cpp
            vector<int> one_harbour = do_sample(1, spe_harbours.size(), &spe_harbours[0], &spe_freq_harbours[0]);
            start_harbour= one_harbour[0];
        }
        else
        {
            // if missing info for a given vessel for this quarter
            cout << "no specified harbour in this quarter for this vessel..." << endl;
                                 // CAUTION: LIKE A MAGIC NUMBER HERE!!!
            start_harbour=find_entries_s_i(harbours, vesselids[0])[0];
            spe_harbours.push_back(start_harbour);
            spe_freq_harbours.push_back(1);
            cout << "then take node: " << start_harbour << endl;
        }

        std::shared_ptr<Vessel> v (new Vessel(mNodes.at(start_harbour)->mNode.get(),
            i,
            vesselids[i],
            nbpops,
            NBSZGROUP,
            spe_harbours,
            spe_fgrounds,
            spe_freq_harbours,
            spe_freq_fgrounds,
            spe_vessel_betas_per_pop,
            spe_percent_tac_per_pop,
            possible_metiers,
            freq_possible_metiers,
            speeds[i],
            fuelcons[i],
            lengths[i],
            KWs[i],
            carrycapacities[i],
            tankcapacities[i],
            nbfpingspertrips[i],
            resttime_par1s[i],
            resttime_par2s[i],
            av_trip_duration[i],
            mult_fuelcons_when_steaming[i],
            mult_fuelcons_when_fishing[i],
            mult_fuelcons_when_returning[i],
            mult_fuelcons_when_inactive[i]
            ));

        std::shared_ptr<VesselData> vd (new VesselData(v));
        mVessels.push_back(vd);

        // some useful setters...
        // will also be useful when change of YEAR-QUARTER
        v->set_spe_fgrounds(spe_fgrounds);
        v->set_spe_harbours(spe_harbours);
        v->set_spe_freq_fgrounds(spe_freq_fgrounds);
        v->set_spe_freq_harbours(spe_freq_harbours);
        v->set_spe_betas_per_pop(spe_vessel_betas_per_pop);
        v->set_spe_percent_tac_per_pop(spe_percent_tac_per_pop);
        v->set_spe_possible_metiers(possible_metiers);
        v->set_spe_freq_possible_metiers(freq_possible_metiers);

        // for dyn sce. CAUTION: MAGIC NUMBERS HERE FOR SOME SCENARIOS....
                                 // dyn sce.
        if (binary_search (dyn_alloc_sce.begin(), dyn_alloc_sce.end(), "reduced_speed_10percent"))
        {
                                 // a decrease by 10%...
            v->set_speed(  v->get_speed()*0.9   );
                                 // corresponds to a decrease by 30% in fuelcons
            v->set_fuelcons( v->get_fuelcons()*0.7 );
            // cubic law  c=v^3, see Ronen 1982
            // e.g. assuming a v at 10, the fuel conso is lowered by (in %) =>  (1- (((seq(0.1,1,by=0.1)*10)^3 ) / (1*10^3)) )*100
        }
                                 // dyn sce.
        if (binary_search (dyn_alloc_sce.begin(), dyn_alloc_sce.end(), "reduced_speed_20percent"))
        {
                                 // a decrease by 20%...
            v->set_speed(  v->get_speed()*0.8   );
                                 // corresponds to a decrease by 48.8% in fuelcons
            v->set_fuelcons( v->get_fuelcons()*0.512 );
            // cubic law  c=v^3, see Ronen 1982
            // e.g. assuming a v at 10, the fuel conso is lowered by (in %) =>  (1- (((seq(0.1,1,by=0.1)*10)^3 ) / (1*10^3)) )*100
        }
                                 // dyn sce.
        if (binary_search (dyn_alloc_sce.begin(), dyn_alloc_sce.end(), "reduced_speed_30percent"))
        {
                                 // a decrease by 30%...
            v->set_speed(  v->get_speed()*0.7   );
                                 // corresponds to a decrease by 65.7% in fuelcons
            v->set_fuelcons( v->get_fuelcons()*0.343 );
            // cubic law  c=v^3, see Ronen 1982
            // e.g. assuming a v at 10, the fuel conso is lowered by (in %) =>  (1- (((seq(0.1,1,by=0.1)*10)^3 ) / (1*10^3)) )*100
        }

        // a particular setters for the CPUE STUFF...
        // for implicit pops or "out of range" fishing: create cpue_nodes_species
        // a vector of vector (with dims [relative index of fishing ground nodes;  pops])
        // we use a vector of vector instead of a multimap in order to speed up the simulation
        // by avoiding a (costly) call to find_entries_i_d() in the do_catch() method
        vector<int> gshape_name_nodes_with_cpue;
        for(multimap<int, double>::iterator iter=gshape_cpue_per_stk_on_nodes.begin(); iter != gshape_cpue_per_stk_on_nodes.end();
            iter = gshape_cpue_per_stk_on_nodes.upper_bound( iter->first ) )
        {
            gshape_name_nodes_with_cpue.push_back (iter->first);
        }

        // init cpue_nodes_species for this vessel
        int nbnodes=gshape_name_nodes_with_cpue.size();
                                 // init the vector of vector with Os
        mVessels.at(i)->mVessel->init_gshape_cpue_nodes_species(nbnodes, nbpops);
                                 // init the vector of vector with Os
        mVessels.at(i)->mVessel->init_gscale_cpue_nodes_species(nbnodes, nbpops);
        for (int n=0; n< gshape_name_nodes_with_cpue.size(); n++)
        {
                                 // look into the multimap...
            vector<double> gshape_cpue_species = find_entries_i_d (gshape_cpue_per_stk_on_nodes, gshape_name_nodes_with_cpue[n]);
                                 // look into the multimap...
            vector<double> gscale_cpue_species = find_entries_i_d (gscale_cpue_per_stk_on_nodes, gshape_name_nodes_with_cpue[n]);
            if(!gshape_cpue_species.empty())
            {
                                 // caution here: the n is the relative index of the node for this vessel i.e. this is not the graph index of the node (because it would have been useless to create a huge matrix filled in by 0 just to preserve the graph idex in this case!)
                mVessels.at(i)->mVessel->set_gshape_cpue_nodes_species(n, gshape_cpue_species);
                                 // caution here: the n is the relative index of the node for this vessel i.e. this is not the graph index of the node (because it would have been useless to create a huge matrix filled in by 0 just to preserve the graph idex in this case!)
                mVessels.at(i)->mVessel->set_gscale_cpue_nodes_species(n, gscale_cpue_species);
            }
        }

        // need to compute expected cpue (averaged over node but cumulated over species)
        // for this particular vessel, in order to scale the prior guess (see below)
        double expected_cpue=0;
        vector <vector<double> > gshape_cpue_nodes_species = mVessels.at(i)->mVessel->get_gshape_cpue_nodes_species();
        vector <vector<double> > gscale_cpue_nodes_species = mVessels.at(i)->mVessel->get_gscale_cpue_nodes_species();
        vector <int> fgrounds= mVessels.at(i)->mVessel->get_fgrounds();
        vector <double> expected_cpue_this_pop (nbpops);
        for(int pop = 0; pop < nbpops; pop++)
        {

            vector<double> cpue_per_fground (fgrounds.size());
                                 // init
            expected_cpue_this_pop.at(pop)=0;

            // compute cpue on nodes
            for(int f = 0; f < fgrounds.size(); f++)
            {
                                 // look into the vector of vector....
                double a_shape = gshape_cpue_nodes_species.at(f).at(pop);
                                 // look into the vector of vector....
                double a_scale = gscale_cpue_nodes_species.at(f).at(pop);
                cpue_per_fground.at(f) = rgamma(a_shape, a_scale);
                //if( v->get_idx() ==2) dout << "cpue_per_fground.at(f)" <<cpue_per_fground.at(f) << endl;
            }
            // compute the average cpue for this pop across all nodes
            for(int f = 0; f < fgrounds.size(); f++)
            {
                expected_cpue_this_pop.at(pop)+=cpue_per_fground.at(f);
            }
                                 // do the mean
            if(expected_cpue_this_pop.at(pop)!=0) expected_cpue_this_pop.at(pop)= expected_cpue_this_pop.at(pop)/fgrounds.size();

                                 // sum over pop
            expected_cpue+= expected_cpue_this_pop.at(pop);
        }

        // init at 0 cumcatch and cumeffort per trip,
        // init at best guest the experiencedcpue_fgrounds
        vector<double > freq_fgrounds= mVessels.at(i)->mVessel->get_freq_fgrounds();
        vector<double > init_for_fgrounds(fgrounds.size());
        vector<double > cumeffort_fgrounds= init_for_fgrounds;
        vector<double > cumcatch_fgrounds= init_for_fgrounds;
        vector<double > experiencedcpue_fgrounds= init_for_fgrounds;
        vector<double > freq_experiencedcpue_fgrounds= init_for_fgrounds;
        vector<vector<double> > cumcatch_fgrounds_per_pop (fgrounds.size(), vector<double>(nbpops));
        vector<vector<double> > experiencedcpue_fgrounds_per_pop (fgrounds.size(), vector<double>(nbpops));
        vector<vector<double> > freq_experiencedcpue_fgrounds_per_pop (fgrounds.size(), vector<double>(nbpops));
        for(int f = 0; f < fgrounds.size(); f++)
        {
            cumcatch_fgrounds[f] = 0;
            cumeffort_fgrounds[f] = 0;
            experiencedcpue_fgrounds[f] = freq_fgrounds[f] * expected_cpue;
            // this should be init so that it constitutes a good qualified guess to be a prior in the bayesian formula...
            // first condition: init different to 0 to allow the ground to be chosen even if it has not been visited yet...
            // second condition: to avoid starting from 0 cpue, init accounting for prior from frequency of visit from the data
            // third condition: to scale the start cpue, multiply by the expectancy of the cpue for this particular vessel

            // init the ones per pop
            for(int pop = 0; pop < nbpops; pop++)
            {
                                 // init
                cumcatch_fgrounds_per_pop[f][pop] = 0;
                experiencedcpue_fgrounds_per_pop[f][pop] = freq_fgrounds[f] * expected_cpue_this_pop.at(pop);
            }
        }
        // per total...
        mVessels.at(i)->mVessel->set_cumcatch_fgrounds(cumcatch_fgrounds);
        mVessels.at(i)->mVessel->set_cumeffort_fgrounds(cumeffort_fgrounds);
        mVessels.at(i)->mVessel->set_experiencedcpue_fgrounds(experiencedcpue_fgrounds);
        mVessels.at(i)->mVessel->set_freq_experiencedcpue_fgrounds(freq_experiencedcpue_fgrounds);
                                 // compute for the first time, to get freq_experiencedcpue_fgrounds...
        mVessels.at(i)->mVessel->compute_experiencedcpue_fgrounds();
        // ...or per pop
        mVessels.at(i)->mVessel->set_cumcatch_fgrounds_per_pop(cumcatch_fgrounds_per_pop);
        mVessels.at(i)->mVessel->set_experiencedcpue_fgrounds_per_pop(experiencedcpue_fgrounds_per_pop);
        mVessels.at(i)->mVessel->set_freq_experiencedcpue_fgrounds_per_pop(freq_experiencedcpue_fgrounds_per_pop);
                                 // compute for the first time, to get freq_experiencedcpue_fgrounds_per_pop...
        mVessels.at(i)->mVessel->compute_experiencedcpue_fgrounds_per_pop();

        // note that, at the start of the simu, freq of visit will be equivalent to freq_fgrounds
        // and then freq of visit will be updated (via the bayes rule) trip after trip from this initial freqency...
        // the expected_cpue is to scale to the encountered cpue i.e. freq of visit will decrease if experienced cpue < expected cpue
        // and vice versa...

        // initialise the individual quota from global_TAC*percent_in_simu*percent_this_vessel


//TODO: check this!
#if 0
        for (unsigned int sp=0; sp<populations.size(); sp++)
        {
            mVessels.at(i)->set_individual_tac_this_pop(export_individual_tacs, 0, populations, sp, 0.0);
        }
#endif

        // check
        cout << "create vessel " << v->get_idx()  << " " << v->get_name() << " " << v->get_nationality() <<" on "
            << v->get_loc()->get_idx_node() << " with coordinates "
            << v->get_loc()->get_x() << " " << v->get_loc()->get_y() << endl;
        //   << " and metier " << v->get_metier()->get_name() <<  endl;
        //vector<double> a_ogive = v->get_metier()->get_selectivity_ogive() ;
        //cout << "with selectivity ogive " << endl;
        //for (int i=0; i<a_ogive.size(); i++)
        //{
        //    cout  << " " << a_ogive[i] << " " ;
        //}
        //cout << endl; // well...nothing there because a metier is still not assigned at this stage...
    }

    return false;
}

bool DisplaceModel::loadGraphs()
{
    struct data {
        int from;
        int to;
        double weight;
    };

    QString filename_graph = mBasePath + "/graphsspe/graph" + QString::number(mScenario.getGraph()) + ".dat";
    qDebug() << "Loading: " << filename_graph << " With " << mScenario.getNrow_graph() << " lines";

    QFile f(filename_graph);
    if (!f.open(QIODevice::ReadOnly)) {
        throw DisplaceException(f.errorString());
        return false;
    }

    QTextStream strm(&f);
    QString line;
    int linenum = 0;
    QList<data> datas;

    while (!(line = strm.readLine()).isNull()) {
        bool ok;
        int stat = linenum / mScenario.getNrow_graph();
        int idx = linenum % mScenario.getNrow_graph();

        switch (stat) {
        case 0: // from
            datas.push_back(data());
            datas[idx].from = line.toInt(&ok);
            break;
        case 1: // to
            datas[idx].to = line.toInt(&ok);
            break;
        case 2: // weight
            datas[idx].weight = line.toDouble(&ok);
            break;
        }

        if (!ok) {
            throw DisplaceException(QString("Graph %2 Parse error at line %1 (%3)").arg(linenum+1).arg(filename_graph).arg(line));
            return false;
        }

        ++linenum;
    }

    /* file has been read. Now feed the data! */
    foreach (data d, datas) {
        mNodes[d.from]->appendAdiancency(d.to, d.weight);
    }

    return true;
}

bool DisplaceModel::initBenthos()
{
    QList<int> ids;

    foreach (std::shared_ptr<NodeData> nd, mNodes) {
        int bm = nd->get_marine_landscape();
        std::shared_ptr<Benthos> benthos;

        QMap<int, std::shared_ptr<Benthos> >::iterator it = mBenthosInfo.find(bm);
        if (it == mBenthosInfo.end()) {
            benthos = std::shared_ptr<Benthos> (new Benthos (bm));
            ids.push_back(bm);
            mBenthosInfo.insert(bm, benthos);
        } else {
            benthos = it.value();
        }

        benthos->appendNode (nd.get());
    }

    qSort(ids);
    foreach (int id, ids) {
        mBenthos.push_back(mBenthosInfo[id]);
    }

    return true;
}

bool DisplaceModel::initPopulations()
{
    mStatsPopulationsCollected.clear();
    for (int i = 0; i < getPopulationsCount(); ++i) {
        mStatsPopulationsCollected.push_back(PopulationData(i));
    }

    QList<int> imp = mConfig.implicit_pops();
    qSort(imp);

    clearInterestingPop();
    int c = 0;
    for (int i = 0; i < imp.size(); ++i) {
        while (c < imp[i]) {
            setInterestingPop(c);
            ++c;
        }
        ++c;
    }

    return true;
}

bool DisplaceModel::initNations()
{
    // nations are read from vessels.
    QMultiMap<QString, std::shared_ptr<VesselData> > nationSet;
    foreach (std::shared_ptr<VesselData> vessel, mVessels) {
        nationSet.insertMulti(QString::fromStdString(vessel->mVessel->get_nationality()), vessel);
    }

    mNations.clear();
    QList<QString> nationsName = nationSet.uniqueKeys();
    for (int i = 0; i < nationsName.size(); ++i) {
        std::shared_ptr<NationData> data(new NationData());
        data->setName(nationsName[i]);

        QList<std::shared_ptr<VesselData> >vessels = nationSet.values(nationsName[i]);
        foreach (std::shared_ptr<VesselData> vessel, vessels) {
            vessel->setNationality(i);
        }
        mNations.push_back(data);

        setInterestingNations(i);
    }

    return true;
}

bool DisplaceModel::loadNodesFromDb()
{
    mNodes.clear();
    if (!mDb->loadNodes(mNodes,mHarbours, this))
        return false;
    return true;
}

bool DisplaceModel::loadVesselsFromDb()
{
    mVessels.clear();
    if (!mDb->loadVessels(mNodes, mVessels))
        return false;
    initNations();

    return true;
}

bool DisplaceModel::loadHistoricalStatsFromDb()
{
    QList<QVector<PopulationData> > dtl;
    QList<QVector<NationStats> > ndl;
    QList<QVector<HarbourStats> > hdl;

    QList<int> steps;
    mDb->loadHistoricalStatsForPops(steps,dtl);
    mDb->loadHistoricalStatsForVessels(steps, mVessels, mNodes, ndl, hdl);

    qDebug() << Q_FUNC_INFO << dtl.size() << steps;

    int i = 0;
    foreach (const QVector<PopulationData> &dt, dtl) {
        int tstep = steps[i];
        mStatsPopulations.insertValue(tstep, dt);
        ++i;
    }

    i = 0;
    foreach (const QVector<NationStats> &dt, ndl) {
        int tstep = steps[i];
        mStatsNations.insertValue(tstep, dt);
        ++i;
    }

    i = 0;
    foreach(const QVector<HarbourStats> &dt, hdl) {
        int tstep = steps[i];
        mStatsHarbours.insertValue(tstep, dt);
        ++i;
    }

    return true;
}
