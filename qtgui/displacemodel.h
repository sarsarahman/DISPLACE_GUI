// --------------------------------------------------------------------------
// DISPLACE: DYNAMIC INDIVIDUAL VESSEL-BASED SPATIAL PLANNING
// AND EFFORT DISPLACEMENT
// Copyright (c) 2012, 2013, 2014, 2015, 2016, 2017 Francois Bastardie <fba@aqua.dtu.dk>

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

#ifndef DISPLACEMODEL_H
#define DISPLACEMODEL_H

#include <scenario.h>
#include <config.h>
#include <nodepenalty.h>

#include <modelobjects/nodedata.h>
#include <modelobjects/vesseldata.h>
#include <modelobjects/firmdata.h>
#include <modelobjects/shipdata.h>
#include <modelobjects/fishfarmdata.h>
#include <modelobjects/windmilldata.h>
#include <modelobjects/benthos.h>
#include <modelobjects/metierdata.h>
#include <modelobjects/populationdata.h>
#include <modelobjects/nationdata.h>
#include <modelobjects/harbourdata.h>
#include <historicaldatacollector.h>
#include <outputfileparser.h>
#include <graphbuilder_shp.h>
#include <objects/metiersentity.h>
#include <utils/interestinglist.h>
#include <utils/interestinglistwithspecialvalues.h>
#include <stats/benthosstats.h>
#include <stats/fishfarmsstats.h>

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QThread>
#include <QMutex>

#include <memory>
#include <functional>

#include <gdal/ogrsf_frmts.h>

#include <QDebug>

class DbHelper;
class MapObjectsController;
class Calendar;


class DisplaceModel : public QObject
{
    Q_OBJECT
public:
    typedef QVector<PopulationData> PopulationStat;
    typedef HistoricalDataCollector<PopulationStat> PopulationStatContainer;
    typedef QVector<NationStats> NationsStats;
    typedef HistoricalDataCollector<NationsStats> NationsStatsContainer;
    typedef QVector<HarbourStats> HarboursStats;
    typedef HistoricalDataCollector<HarboursStats> HarboursStatsContainer;
    typedef QVector<MetierStats> MetiersStats;
    typedef HistoricalDataCollector<MetiersStats> MetiersStatsContainer;
    typedef HistoricalDataCollector<BenthosStats> BenthosStatsContainer;
    typedef HistoricalDataCollector<FishfarmsStats> FishfarmsStatsContainer;



    enum ModelType {
        LiveModelType, EditorModelType, OfflineModelType,
        EmptyModelType
    };

    enum SpecialGroups { Total, Average, Min, Max,
                          LastSpecialGroup };

    DisplaceModel();

    ModelType modelType() const { return mModelType; }

    bool edit(QString modelname);
    void setIndex(int idx) { mIndex = idx; }
    int index() const { return mIndex; }

    bool load (QString path, ModelType type);
    bool loadDatabase (QString path);
    bool linkDatabase (QString path);
    bool prepareDatabaseForSimulation ();
    bool clearStats();
    bool saveScenarioAs(const QString &path);
    bool saveScenario();
    bool saveConfig();

    std::shared_ptr<Calendar> calendar() const { return mCalendar; }

    /** \brief Signals the simulation has ended. Flush all buffers */
    void simulationEnded();

    // Getter
    QString fullpath() const { return mFullPath; }
    QString inputName() const { return mInputName; }
    QString basepath() const { return mBasePath; }
    QString outputName() const { return mOutputName; }
    void setOutputName(const QString &name) { mOutputName = name; }
    QString simulationName() const { return mSimuName; }
    void setSimulationName(const QString &name) { mSimuName = name; }

    QString linkedDatabase() const { return mLinkedDbName; }
    bool isModelLoaded() const { return !mInputName.isEmpty(); }

    int getNBPops() const {
        return mConfig.getNbpops();
    }
    int getNBBenthosPops() const {
        return mConfig.getNbbenthospops();
    }
    int getSzGrupsCount() const {
        return mConfig.getSzGroups();
    }
    int getBenthosIdx(int benthosId) const;

    int getSimulationSteps() const;
    void setSimulationSteps(int value);
    int getUseStaticPaths() const;
    void setUseStaticPaths(int value);
    int getPreexistingPathsShop() const;
    void setPreexistingPathsShop(int value);

    /* Graphs operation */

    const QList<std::shared_ptr<HarbourData> > &getHarboursList() const { return mHarbours; }
    int getHarboursCount() const;
    QString getHarbourId(int idx) const;

    const QList<std::shared_ptr<NodeData> > &getNodesList() const { return mNodes; }
    int getNodesCount() const;
    QString getNodeId(int idx) const;
    QList<std::shared_ptr<NodeData> > getAllNodesWithin (const QPointF &centerpoint, double dist_km) const;

    /** \brief receive a Stats update for nodes from the Simulator
     *
     * The string passed has the following format:
     *    stat,tstep,first,number,data
     *
     * */
    void updateNodesStatFromSimu(QString);

    const QList<std::shared_ptr<VesselData> > &getVesselList() const { return mVessels; }
    int getVesselCount() const;
    QString getVesselId(int idx) const;
    void updateVessel (int tstep, int idx, float x, float y, float course, float fuel, int state );

    const QList<std::shared_ptr<FishfarmData> > &getFishfarmList() const { return mFishfarms; }
    int getFishfarmsCount() const;
    QString getFishfarmId(int idx) const;
    void updateFishfarm (int idx, float x, float y);

    const QList<std::shared_ptr<FirmData> > &getFirmList() const { return mFirms; }
    int getFirmCount() const;
    QString getFirmId(int idx) const;
    void updateFirm (int idx, float x, float y);


    const QList<std::shared_ptr<WindmillData> > &getWindmillList() const { return mWindmills; }
    int getWindmillCount() const;
    QString getWindmillId(int idx) const;
    void updateWindmill (int idx, float x, float y);

    const QList<std::shared_ptr<ShipData> > &getShipList() const { return mShips; }
    int getShipCount() const;
    QString getShipId(int idx) const;
    void updateShip (int tstep, int idx, float x, float y, float course);

    const QList<std::shared_ptr<Benthos> > &getBenthosList() const { return mBenthos; }
    int getBenthosCount() const;

    const QList<std::shared_ptr<objecttree::MetiersInterest> > &getMetiersList() const { return mMetiers; }
    int getMetiersCount() const {
        return mMetiers.size();
    }
    void addMetier(int id);

    /* Access to Population statistics */
    int getPopulationsCount() const;
    int getBenthosPopulationsCount() const;
    const PopulationData &getPopulationsAtStep (int step, int idx) const {
        if (idx >= mStatsPopulations.getValue(step).size()) {
            qDebug() << step << idx << mStatsPopulations.getValue(step).size();
            Q_ASSERT(false);
        }
        return mStatsPopulations.getValue(step).at(idx);
    }
    int getPopulationsValuesCount() const {
        return mStatsPopulations.getUniqueValuesCount();
    }
    PopulationStatContainer::Container::const_iterator getPopulationsFirstValue() const {
        return mStatsPopulations.getFirst();
    }

    const PopulationData &getPopulations(int idx) const { return getPopulationsAtStep(mCurrentStep,idx); }

    /* Access to Nations statistics */

    const QList<std::shared_ptr<NationData> > &getNationsList() const { return mNations; }
    const NationData &getNation(int idx) const { return *mNations.at(idx); }

    int getNationsStatsCount() const {
        return mStatsNations.getUniqueValuesCount();
    }
    NationsStatsContainer::Container::const_iterator getNationsStatsFirstValue() const {
        return mStatsNations.getFirst();
    }
    const NationsStats &getNationsStatAtStep(int step) const {
        return mStatsNations.getValue(step);
    }
    const NationStats &getNationStatAtStep(int step, int idx) const {
        return mStatsNations.getValue(step).at(idx);
    }

    /* Access to Harbour statistics */

    const QList<std::shared_ptr<HarbourData> > &getHarbourList() const { return mHarbours; }
    const HarbourData &getHarbourData(int idx) const { return *mHarbours.at(idx); }

    int getHarboursStatsCount() const {
        return mStatsHarbours.getUniqueValuesCount();
    }
    HarboursStatsContainer::Container::const_iterator getHarboursStatsFirstValue() const {
        return mStatsHarbours.getFirst();
    }
    const HarboursStats &getHarboursStatAtStep(int step) const {
        return mStatsHarbours.getValue(step);
    }
    const HarbourStats &getHarboursStatAtStep(int step, int idx) const {
        return mStatsHarbours.getValue(step).at(idx);
    }
    /** Retrieve the statistics for a specific Harbour from the DB, or the latest available if it's a live simulation */
    HarbourStats retrieveHarbourIdxStatAtStep (int idx, int step);

    /* Access Metier Statistics */

    int getMetiersStatsCount() const {
        return mStatsMetiers.getUniqueValuesCount();
    }
    MetiersStatsContainer::Container::const_iterator getMetiersStatsFirstValue() const {
        return mStatsMetiers.getFirst();
    }
    /*
    const MetiersStatsContainer &getMetiersStatAtStep(int step) const {
        return mStatsMetiers.getValue(step);
    }
    const MetiersStatsContainer &getMetiersStatAtStep(int step, int idx) const {
        return mStatsMetiers.getValue(step).at(idx);
    }*/

    /* Benthos Statistics */
    const BenthosStatsContainer &getBenthosStatistics() { return mStatsBenthos; }


    /* Fishfarms Statistics */
    const FishfarmsStatsContainer &getFishfarmsStatistics() { return mStatsFishfarms; }


    /* Scenario and configuration */

    Scenario scenario() const;
    void setScenario(const Scenario &scenario);

    Config config() const;
    void setConfig(const Config &config);

    /* Stock names */

    const QMap<QString, int> getStockNames() const { return mStockNames; }
    void setStockNames (const QMap<QString,int> &names) { mStockNames = names; }

    /* Live Simulation data */

    void setCurrentStep(int step);
    int getCurrentStep() const { return mCurrentStep; }
    int getLastStep() const { return mLastStep; }


    /* Interesting pop access functions */
    const QList<int> &getInterestingPops() const { return mInterestingPop; }
    const QList<int> &getInterestingPops2() const { return mInterestingPop2; }

    /** \brief insert the pop into the list of interest for pops */
    void setInterestingPop(int n);
    void setInterestingPop2(int n);

    /** \brief remove the pop from the list of interest for pops */
    void remInterestingPop(int n);
    void remInterestingPop2(int n);
    bool isInterestingPop(int n);
    bool isInterestingPop2(int n);
    void clearInterestingPop();
    void clearInterestingPop2();

    QList<int> getInterestingBenthos() const { return mInterestingBenthos.list(); }
    void setInterestingBenthos(int n) { mInterestingBenthos.set(n); }
    void remInterestingBenthos(int n) { mInterestingBenthos.rem(n); }
    bool isInterestingBenthos(int n) const { return mInterestingBenthos.has(n); }
    void clrInterestingBenthos() { mInterestingBenthos.clear(); }

    QList<int> getInterestingFishfarms() const { return mInterestingFishfarms.list(); }
    void setInterestingFishfarms(int n) { mInterestingFishfarms.set(n); }
    void remInterestingFishfarms(int n) { mInterestingFishfarms.rem(n); }
    bool isInterestingFishfarms(int n) const { return mInterestingFishfarms.has(n); }
    void clrInterestingFishfarms() { mInterestingFishfarms.clear(); }

    /* Interesting pop access functions */
    bool isInterestingSizeTotal() const { return mInterestingSizeTotal; }
    void setInterestingSizeTotal(bool b) { mInterestingSizeTotal = b; }
    bool isInterestingSizeAvg() const { return mInterestingSizeAvg; }
    void setInterestingSizeAvg(bool b) { mInterestingSizeAvg = b; }
    bool isInterestingSizeMin() const { return mInterestingSizeMin; }
    void setInterestingSizeMin(bool b) { mInterestingSizeMin = b; }
    bool isInterestingSizeMax() const { return mInterestingSizeMax; }
    void setInterestingSizeMax(bool b) { mInterestingSizeMax = b; }

    const QList<int> &getInterestingSizes() const { return mInterestingSizes; }

    int getNumFuncGroups() const;
    std::shared_ptr<InterestingListWithSpecialValues<int>>  getFunctionalGroupsList() const { return mFuncGroups; }

    int getNumFishfarmTypes() const;
    std::shared_ptr<InterestingListWithSpecialValues<int>>  getFishfarmsTypesList() const { return mFishfarmsTypes; }

    /** \brief insert the pop into the list of interest for pops */
    void setInterestingSize(int n);

    /** \brief remove the pop from the list of interest for pops */
    void remInterestingSize(int n);
    bool isInterestingSize(int n);

    /* Interesting harbours - see pop */
    const QList<types::NodeId> &getInterestingHarbours() const { return mInterestingHarb; }

    /** \brief insert the pop into the list of interest for pops */
    void setInterestingHarb(types::NodeId n);

    /** \brief remove the pop from the list of interest for pops */
    void remInterestingHarb(types::NodeId n);
    bool isInterestingHarb(types::NodeId n);

    /* Interesting Nations */
    const QList<int> &getInterestingNations() const { return mInterestingNations; }

    /** \brief insert the pop into the list of interest for pops */
    void setInterestingNations(int n) {
        if (!mInterestingNations.contains(n))
            mInterestingNations.append(n);
            qSort(mInterestingNations);
    }

    /** \brief remove the pop from the list of interest for pops */
    void remInterestingNations(int n) {
        mInterestingNations.removeAll(n);
    }
    bool isInterestingNations(int n) {
        return mInterestingNations.contains(n);
    }

    //

    QString getLastError() const { return mLastError; }

    void parseOutputStatsFile (QString file, int tstep);

    void commitNodesStatsFromSimu(int tstep, bool force = false);

    // Stats collecting. Must store data to db
    void startCollectingStats();
    void endCollectingStats();
    void collectNodePopStats (int tstep, int node_idx, const QList<double> &stats, const QList<double> &stats_w, double tot, double wtot);
    void collectPopCumftime(int step, int node_idx, double cumftime);
    void collectPopCumsweptarea(int step, int node_idx, double cumsweptarea);
    void collectPopCumcatches(int step, int node_idx, double cumcatches);
    void collectPopTariffs(int step, int node_idx, vector<double> tariffs);
    void collectPopImpact(int step, int node_idx, int popid, double impact);
    void collectPopCumcatchesPerPop(int step, int node_idx, int popid, double cumcatchesperpop);
    void collectPopBenthosBiomass(int step, int node_idx, int funcid, double benthosbiomass);
    void collectPopBenthosNumber(int step, int node_idx, int funcid, double benthosnumber);
    void collectPopBenthosMeanWeight (int step, int node_idx, int funcid, double meanweight);
    void collectPopBenthosBiomassOverK(int step, int node_idx, int funcid, double benthosbiomassoverK);
    void collectPopBenthosNumberOverK(int step, int node_idx, int funcid, double benthosnumberoverK);

    void collectFishfarmFishMeanWeight (int step, int node_idx, int farmid, int farmtype, double meanw_kg);
    void collectFishfarmFishHarvestedKg (int step, int node_idx,  int farmid, int farmtype, double fish_harvested_kg);
    void collectFishfarmEggsHarvestedKg (int step, int node_idx, int farmid, int farmtype, double eggs_harvested_kg);
    void collectFishfarmAnnualProfit (int step, int node_idx, int farmid, int farmtype, double fishfarm_annualprofit);
    void collectFishfarmNetDischargeN (int step, int node_idx, int farmid, int farmtype, double fishfarm_netdischargeN);
    void collectFishfarmNetDischargeP (int step, int node_idx, int farmid, int farmtype, double fishfarm_netdischargeP);


    void collectPopdynN(int step, int popid, const QVector<double> &pops, double value);
    void collectPopdynF(int step, int popid, const QVector<double> &pops, double value);
    void collectPopdynSSB(int step, int popid, const QVector<double> &pops, double value);

    void collectVesselStats (int step, const VesselStats &stats);

    /* Editor stuff */

    void clearAllNodes();
    bool addGraph(const QList<GraphBuilder::Node> &points, MapObjectsController *controller);
    bool removeNode(std::shared_ptr<NodeData> node);
    int addEdge(std::shared_ptr<NodeData> nodedata, types::NodeId targetidx, double weight);
    int addEdge(types::NodeId srcidx, types::NodeId targetidx, double weight);
    bool exportGraph(const QString &path);
    bool importHarbours (QList<std::shared_ptr<HarbourData> > &list);
    void addPenaltyToNodesByAddWeight(const QList<QPointF> &poly, double weight, bool closed_for_fishing, bool onQ1, bool onQ2, bool onQ3, bool onQ4, vector<bool> checkedMonths, const vector<int> &checkedVesSizes, vector<int> bannedMetiers);
    void addPenaltyToNodesByAddWeight(OGRGeometry *geometry, double weight, bool closed_for_fishing,  bool onQ1, bool onQ2, bool onQ3, bool onQ4, vector<bool> checkedMonths, const vector<int> &checkedVesSizes, vector<int> bannedMetiers);

#if 0 // TODO remove me
    int countPenaltyPolygons(int quarter) const;
    const QList<int> getPenaltyPolygonsAt (int quarter, int ndx) const ;
#endif

    const QList<displace::NodePenalty> getPenaltyCollection() const { return mPenaltyNodes; }

    bool isShortestPathFolderLinked() const { return !mShortestPathFolder.isEmpty(); }
    void linkShortestPathFolder(QString path) { mShortestPathFolder = path; }
    QString linkedShortestPathFolder() const { return mShortestPathFolder; }

    bool isGraphFolderLinked() const { return !mGraphFolder.isEmpty(); }
    void linkGraphFolder(QString path) { mGraphFolder = path; }
    QString linkedGraphFolder() const { return mGraphFolder; }

    void setLandscapeCodesFromFeature(OGRGeometry *geometry, int code);
    void setWindFromFeature(OGRGeometry *geometry, double wind);
    void setSSTFromFeature(OGRGeometry *geometry, double sst);
    void setSalinityFromFeature(OGRGeometry *geometry, double psu);
    void setBenthosBiomassFromFeature(OGRGeometry *geometry, double bio);
    void setBenthosNumberFromFeature(OGRGeometry *geometry, double nb);
    void setAreaCodesFromFeature(OGRGeometry *geometry, int code);

protected:
    bool loadNodes();
    bool loadVessels();
    bool loadGraphs();
    bool initShips();
    bool initFirm();
    bool initFishfarm();
    bool initWindmill();
    bool initBenthos();
    bool initPopulations();
    bool initNations();
    void createFeaturesLayer();

    bool loadNodesFromDb();
    bool loadVesselsFromDb();
    bool loadHistoricalStatsFromDb();

    void checkStatsCollection(int tstep);
    bool parse(const QString &path, QString *basepath, QString *inputname, QString *outputname);
    void setCodeFromFeature(OGRGeometry *geometry, int code, std::function<void (std::shared_ptr<NodeData>, int)> func);
    void setWdFromFeature(OGRGeometry *geometry, double wd, std::function<void (std::shared_ptr<NodeData>, int)> func);
    void setTFromFeature(OGRGeometry *geometry, double t, std::function<void (std::shared_ptr<NodeData>, int)> func);
    void setSalFromFeature(OGRGeometry *geometry, double sal, std::function<void (std::shared_ptr<NodeData>, int)> func);
    void setBenthosBioFromFeature(OGRGeometry *geometry, double bio, std::function<void (std::shared_ptr<NodeData>, double)> func);
    void setBenthosNbFromFeature(OGRGeometry *geometry, double bio, std::function<void (std::shared_ptr<NodeData>, double)> func);

signals:
    void parseOutput(QString, int);
    void outputParsed();
    void errorParsingStatsFile(QString);

private:
    mutable QMutex mMutex;

    ModelType mModelType;
    DbHelper *mDb;
    std::shared_ptr<Calendar> mCalendar;
    QString mFullPath;
    QString mInputName;
    QString mBasePath;
    QString mOutputName;
    QString mSimuName;
    QString mLinkedDbName;
    int mIndex;

    int mSimulSteps;
    int mUseStaticPaths;
    int mPreexistingPathsShop;
    int mCurrentStep, mLastStep;
    int mLastStats;
    bool mNodesStatsDirty;
    bool mPopStatsDirty;
    bool mVesselsStatsDirty;
    bool mFirmsStatsDirty;
    bool mShipsStatsDirty;
    bool mFishfarmStatsDirty;
    bool mWindmillStatsDirty;

    Scenario mScenario;
    Config mConfig;

    QList<int> mInterestingPop;
    QList<int> mInterestingPop2;
    bool mInterestingSizeTotal, mInterestingSizeAvg, mInterestingSizeMin, mInterestingSizeMax;
    QList<int> mInterestingSizes;
    QList<types::NodeId> mInterestingHarb;
    QList<int> mInterestingNations;
    InterestingList<int> mInterestingBenthos;  
    InterestingList<int> mInterestingFishfarms;
    std::shared_ptr<InterestingListWithSpecialValues<int>> mFuncGroups;
    std::shared_ptr<InterestingListWithSpecialValues<int>> mFishfarmsTypes;

    QList<displace::NodePenalty> mPenaltyNodes;

    QList<std::shared_ptr<HarbourData>> mHarbours;
    QList<std::shared_ptr<NodeData> > mNodes;
    QList<std::shared_ptr<VesselData> > mVessels;
    QList<std::shared_ptr<ShipData> > mShips;
    QList<std::shared_ptr<FishfarmData> > mFishfarms;
    QList<std::shared_ptr<FirmData> > mFirms;
    QList<std::shared_ptr<WindmillData> > mWindmills;
    QList<std::shared_ptr<Benthos> > mBenthos;
    QList<std::shared_ptr<objecttree::MetiersInterest>> mMetiers;
    QList<std::shared_ptr<NationData> > mNations;

    PopulationStatContainer mStatsPopulations;
    PopulationStat mStatsPopulationsCollected;
    NationsStatsContainer mStatsNations;
    NationsStats mStatsNationsCollected;
    HarboursStatsContainer mStatsHarbours;
    HarboursStats mStatsHarboursCollected;
    MetiersStatsContainer mStatsMetiers;
    MetiersStats mStatsMetiersCollected;
    BenthosStatsContainer mStatsBenthos;
    BenthosStats mStatsBenthosCollected;
    FishfarmsStatsContainer mStatsFishfarms;
    FishfarmsStats mStatsFishfarmsCollected;

    QMap<int, std::shared_ptr<Benthos> > mBenthosInfo;
    QMap<int, std::shared_ptr<Fishfarm> > mFishfarmInfo;
    QMap<QString, int> mStockNames;

    // --- Working objects

    OutputFileParser *mOutputFileParser;
    QThread *mParserThread;

    QString mLastError;

    /* Editor stuff */
    enum OgrType { OgrTypeNode = 0, OgrTypeEdge = 1 };
    OGRDataSource *mDataSource;
    OGRLayer *mNodesLayer;
    int mNodesLayerIndex;

    OGRSpatialReference *mSpatialRef;

    QString mShortestPathFolder;
    QString mGraphFolder;
};

#endif // DISPLACEMODEL_H
