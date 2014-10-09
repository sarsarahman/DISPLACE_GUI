#include "statscontroller.h"
#include <displacemodel.h>
#include <QVector>
#include <QtAlgorithms>

StatsController::StatsController(QObject *parent)
    : QObject(parent),
      mPalette(),
      mPlotPopulations(0),
      mLastModel(0)
{
    QFile file (":/palettes/iso1996_2.p2c");
    mPalette.loadFromFile(&file);
}

void StatsController::setPopulationPlot(QCustomPlot *plot)
{
    mPlotPopulations = plot;
    mPlotPopulations->legend->setVisible(true);
}

void StatsController::updateStats(DisplaceModel *model)
{
    if (!model)
        return;

    if (mPlotPopulations) {
        mPlotPopulations->clearGraphs();

        QList<int> ipl = model->getInterestingPops();

        foreach (int ip, ipl) {
            QVector<double> keyData;
            QVector<double> valueData;

            QCPGraph *graph = mPlotPopulations->addGraph();
            graph->setPen(QPen(mPalette.colorForIndexMod(ip % mPalette.colorCount())));
            graph->setName(QString(QObject::tr("Population %1")).arg(ip));

            int n = model->getPopulationsValuesCount();
            DisplaceModel::PopulationStatContainer::Container::const_iterator it = model->getPopulationsFirstValue();
            for (int i = 0; i <n; ++i) {
                keyData << it.key();

                switch (mSelectedPopStat) {
                case Aggregate:
                    valueData << it.value().at(ip).getAggregate();
                    break;
                case Mortality:
                    valueData << it.value().at(ip).getMortality();
                    break;
                }

                ++it;
            }

            graph->setData(keyData, valueData);
        }

        mPlotPopulations->rescaleAxes();
        mPlotPopulations->replot();
    }

    mLastModel = model;
}

void StatsController::setPopulationStat(StatsController::PopulationStat stat)
{
    mSelectedPopStat = stat;
    updateStats(mLastModel);
}

void StatsController::initPlots()
{
    if (mPlotPopulations) {

    }
}

