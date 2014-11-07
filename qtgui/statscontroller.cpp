#include "statscontroller.h"
#include <displacemodel.h>
#include <QVector>
#include <QtAlgorithms>

StatsController::StatsController(QObject *parent)
    : QObject(parent),
      mPalette(),
      mPlotPopulations(0),
      mSelectedPopStat(Aggregate),
      mPlotHarbours(0),
      mPlotNations(0),
      mSelectedNationsStat(Catches),
      mLastModel(0)
{
    mPalette = PaletteManager::instance()->palette(PopulationRole);
}

void StatsController::setPopulationPlot(QCustomPlot *plot)
{
    mPlotPopulations = plot;
    mPlotPopulations->legend->setVisible(true);
}

void StatsController::setHarboursPlot(QCustomPlot *plot)
{
    mPlotHarbours = plot;
    mPlotHarbours->legend->setVisible(true);
}

void StatsController::setNationsPlot(QCustomPlot *plot)
{
    mPlotNations = plot;
    mPlotNations->legend->setVisible(true);
}

void StatsController::updateStats(DisplaceModel *model)
{
    if (!model)
        return;

    if (mPlotPopulations) {
        updatePopulationStats(model);
    }
    if (mPlotNations) {
        updateNationStats(model);
    }
    if (mPlotHarbours) {
        updateHarboursStats(model);
    }

    mLastModel = model;
}

void StatsController::setPopulationStat(StatsController::PopulationStat stat)
{
    mSelectedPopStat = stat;
    updateStats(mLastModel);
}

void StatsController::setNationsStat(StatsController::NationsStat stat)
{
    mSelectedNationsStat = stat;
    updateStats(mLastModel);
}

void StatsController::setHarbourStat(StatsController::HarboursStat stat)
{
    mSelectedHarboursStat = stat;
    updateStats(mLastModel);
}

void StatsController::initPlots()
{
    if (mPlotPopulations) {

    }
}

void StatsController::updatePopulationStats(DisplaceModel *model)
{
    static const QPen pen(QColor(0,0,255,200));
    mPlotPopulations->clearGraphs();
    double val;

    QList<int> interPopList = model->getInterestingPops();
    QList<int> interSizeList = model->getInterestingSizes();
    QList<int> graphList;
    bool showtotal = model->isInterestingSizeTotal();
    bool showavg =  model->isInterestingSizeAvg();
    bool showmin =  model->isInterestingSizeMin();
    bool showmax =  model->isInterestingSizeMax();

    if (showmax)
        graphList.push_front(-4);
    if (showmin)
        graphList.push_front(-3);
    if (showavg)
        graphList.push_front(-2);
    if (showtotal)
        graphList.push_front(-1);

    if (graphList.size() == 0)
        graphList.append(interSizeList);

    /* If no size is selected, but aggregate is selected, select all sizes */
    if (interSizeList.size() == 0 && graphList.size() != 0) {
        for(int i = 0; i < model->getSzGrupsCount(); ++i)
            interSizeList.push_back(i);
    }

    int szNum = interSizeList.size();
    int graphNum = graphList.size();

    QList<QCPGraph *>graphs;
    QList<QVector<double> >keyData;
    QList<QVector<double> >valueData;

    Palette::Iterator col_it = mPalette.begin();
    foreach (int ipop, interPopList) {
        for (int igraph = 0; igraph < graphNum; ++igraph) {
            if (col_it == mPalette.end())
                col_it = mPalette.begin();

            // Creates graph. Index in list are: ip * nsz + isz
            QCPGraph *graph = mPlotPopulations->addGraph();
            graph->setPen(pen);
            graph->setLineStyle(QCPGraph::lsLine);
            QColor col = col_it != mPalette.end() ? *col_it : QColor();

            col.setAlpha(128);
            graph->setBrush(QBrush(col));

            switch (graphList[igraph]) {
            case -4:
                graph->setName(QString(QObject::tr("Pop %1 Max")).arg(ipop));
                break;
            case -3:
                graph->setName(QString(QObject::tr("Pop %1 Min")).arg(ipop));
                break;
            case -2:
                graph->setName(QString(QObject::tr("Pop %1 Avg")).arg(ipop));
                break;
            case -1:
                graph->setName(QString(QObject::tr("Pop %1 Total")).arg(ipop));
                break;
            default:
                graph->setName(QString(QObject::tr("Pop %1 Group %2")).arg(ipop).arg(graphList[igraph]+1));
            }

            graphs.push_back(graph);
            keyData.push_back(QVector<double>());
            valueData.push_back(QVector<double>());

            ++col_it;
        }
    }

    int nsteps = model->getPopulationsValuesCount();

    DisplaceModel::PopulationStatContainer::Container::const_iterator it = model->getPopulationsFirstValue();
    for (int istep = 0; istep <nsteps; ++istep) {
        int ninterPop = interPopList.size();
        for (int iinterpPop = 0; iinterpPop < ninterPop; ++iinterpPop) {

            // calculate transversal values...
            double mMin = 0.0,mMax = 0.0,mAvg = 0.0,mTot = 0.0;
            for (int iInterSize = 0; iInterSize < interSizeList.size(); ++iInterSize) {
                val = getPopStatValue(model, it.key(), interPopList[iinterpPop], interSizeList[iInterSize], mSelectedPopStat);
                if (iInterSize == 0) {
                    mMin = val;
                    mMax = val;
                } else {
                    if (mMin > val)
                        mMin = val;
                    if (mMax < val)
                        mMax = val;
                }
                mAvg += val;
                mTot += val;
            }
            if (szNum > 0)
                mAvg /= szNum;

            for (int isz = 0; isz < graphNum; ++isz) {
                int gidx = iinterpPop * graphNum + isz;

                keyData[gidx] << it.key();
                switch (graphList[isz]) {
                case -4:
                    val = mMax;
                    break;
                case -3:
                    val = mMin;
                    break;
                case -2:
                    val = mAvg;
                    break;
                case -1:
                    val = mTot;
                    break;
                default:
                    val = getPopStatValue(model, it.key(), interPopList[iinterpPop], graphList[isz], mSelectedPopStat);
                    break;
                }

                valueData[gidx] << val;
            }
        }
        ++it;
    }

    for (int i = 0; i < graphs.size(); ++i) {
        graphs[i]->setData(keyData.at(i), valueData.at(i));
    }

    mPlotPopulations->rescaleAxes();
    mPlotPopulations->replot();
}

double StatsController::getPopStatValue(DisplaceModel *model, int tstep, int popid, int szid, StatsController::PopulationStat stattype)
{
    switch (stattype) {
    case Aggregate:
        return model->getPopulationsAtStep(tstep, popid).getAggregate().at(szid);
    case Mortality:
        return model->getPopulationsAtStep(tstep, popid).getMortality().at(szid);
    }

    return 0;
}

void StatsController::updateNationStats(DisplaceModel *model)
{
    static const QPen pen(QColor(0,0,255,200));
    mPlotNations->clearGraphs();

    QList<int> ipl = model->getInterestingNations();

    int cnt;
    Palette::Iterator col_it = mPalette.begin();
    foreach (int ip, ipl) {
        if (col_it == mPalette.end())
            col_it = mPalette.begin();

        QVector<double> keyData;
        QVector<double> valueData;

        QCPGraph *graph = mPlotNations->addGraph();
        graph->setPen(pen);
        graph->setLineStyle(QCPGraph::lsLine);
        QColor col = col_it != mPalette.end() ? *col_it : QColor();

        col.setAlpha(128);
        graph->setBrush(QBrush(col));
        ++cnt;

        graph->setName(QString(model->getNation(ip).getName()));

        int n = model->getNationsStatsCount();
        DisplaceModel::NationsStatsContainer::Container::const_iterator it = model->getNationsStatsFirstValue();
        for (int i = 0; i <n; ++i) {
            if (it.value().size() > ip) {
                keyData << it.key();

                switch (mSelectedNationsStat) {
                case Catches:
                    valueData << it.value().at(ip).mTotCatches;
                    break;
                case Earnings:
                    valueData << it.value().at(ip).mRevenues;
                    break;
                case TimeAtSea:
                    valueData << it.value().at(ip).mTimeAtSea;
                    break;
                }
            }
            ++it;
        }

        graph->setData(keyData, valueData);

        ++col_it;
    }

    mPlotNations->rescaleAxes();
    mPlotNations->replot();    
}

void StatsController::updateHarboursStats(DisplaceModel *model)
{
    static const QPen pen(QColor(0,0,255,200));
    mPlotHarbours->clearGraphs();

    QList<int> ipl = model->getInterestingHarbours();

    int cnt;
    Palette::Iterator col_it = mPalette.begin();

    foreach (int ip, ipl) {
        if (col_it == mPalette.end())
            col_it = mPalette.begin();

        QVector<double> keyData;
        QVector<double> valueData;

        QCPGraph *graph = mPlotHarbours->addGraph();
        graph->setPen(pen);
        graph->setLineStyle(QCPGraph::lsLine);
        QColor col = col_it != mPalette.end() ? *col_it : QColor();

        col.setAlpha(128);
        graph->setBrush(QBrush(col));
        ++cnt;

        graph->setName(QString::fromStdString(model->getHarbourData(ip).mHarbour->get_name()));

        int n = model->getHarboursStatsCount();
        DisplaceModel::HarboursStatsContainer::Container::const_iterator it = model->getHarboursStatsFirstValue();
        for (int i = 0; i <n; ++i) {
            if (it.value().size() > ip) {
                keyData << it.key();

                switch (mSelectedHarboursStat) {
                case H_Catches:
                    valueData << it.value().at(ip).mCumCatches;
                    break;
                case H_Earnings:
                    valueData << it.value().at(ip).mCumProfit;
                    break;
                }
            }
            ++it;
        }

        graph->setData(keyData, valueData);

        ++col_it;
    }

    mPlotHarbours->rescaleAxes();
    mPlotHarbours->replot();

}

