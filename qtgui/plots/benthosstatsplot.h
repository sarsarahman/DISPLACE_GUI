#ifndef BENTHOSSTATSPLOT_H
#define BENTHOSSTATSPLOT_H

#include <plots/plottypes.h>
#include <palettemanager.h>

#include <QPen>
#include <QColor>

class DisplaceModel;
class QCustomPlot;
class QCPItemLine;

class BenthosStatsPlot
{
    QCustomPlot *mPlot;
    QCPItemLine *mTimeline;
    QPen pen;
    double timelineMax = 1e20;
    double timelineMin = -1e20;
    Palette mPalette;

public:
    BenthosStatsPlot(QCustomPlot *plot, QCPItemLine *timeLine);

    void setTimelineLimits(double min, double max) {
        timelineMin = min;
        timelineMax = max;
    }

    void update(DisplaceModel *model, displace::plot::BenthosStat stat);

private:
    double getStatValue(DisplaceModel *model, int tstep, int popid, int szid, displace::plot::BenthosStat stattype);
};

#endif // BENTHOSSTATSPLOT_H
