#include "vesselmapobject.h"

#include <mapobjectscontroller.h>

#include <QMapControl/QMapControl.h>
#include <QMapControl/Point.h>
#include <QMapControl/Projection.h>
#include <QMapControl/LayerGeometry.h>

#include <m_constants.h>

#include <QPainter>
#include <QDebug>

VesselMapObject::VesselMapObject(MapObjectsController *controller, VesselData *vessel)
    : mController(controller),
      mVessel(vessel),
      mWidget(0)
{
    mGeometry = std::shared_ptr<VesselGraphics> (new VesselGraphics(mVessel));

    mGeometry->setAncillaryData(new MapObjectsController::WidgetAncillaryData(this));
}

bool VesselMapObject::showProperties()
{
    if (!mWidget) {
        mWidget = new NodeDetailsWidget(mController->mapWidget());
        connect (mWidget, SIGNAL(destroyed()), this, SLOT(widgetClosed()));
    }

    updateProperties();
    mController->showDetailsWidget(mGeometry->coord(),mWidget);

    return true;
}

void VesselMapObject::updateProperties()
{
    if (!mWidget)
        return;

    QString text = QString("<b>Name</b>: %1<br/>"
                           "<b>Coords: </b>%2 %3<br/>")
            .arg(QString::fromStdString(mVessel->mVessel->get_name()))
            .arg(mVessel->mVessel->get_y())
            .arg(mVessel->mVessel->get_x());

    text += "<br/>";
    text += QString("<b>Fuel:</b> %1<br/>").arg(mVessel->mVessel->get_cumfuelcons());
    text += QString("<b>State:</b> %1<br/>").arg(mVessel->mVessel->get_state());
    text += QString("<b>Cum Catches:</b> %1<br/>").arg(mVessel->mVessel->get_cumcatches());
    text += QString("<b>Time at sea:</b> %1<br/>").arg(mVessel->mVessel->get_timeatsea());
    text += QString("<b>Reason To go Back:</b> %1<br/>").arg(mVessel->mVessel->get_reason_to_go_back());

    mWidget->setText(text);
}

void VesselMapObject::vesselUpdated()
{
    mGeometry->layer()->removeGeometry(mGeometry);
    mGeometry->setCoord(qmapcontrol::PointWorldCoord(mVessel->mVessel->get_x(), mVessel->mVessel->get_y()));
    mGeometry->layer()->addGeometry(mGeometry);
}

void VesselMapObject::widgetClosed()
{
    mWidget = 0;
}


QBrush *VesselMapObject::VesselGraphics::color = 0;
QBrush *VesselMapObject::VesselGraphics::statFishing= 0;
QBrush *VesselMapObject::VesselGraphics::statHarbour = 0;
QBrush *VesselMapObject::VesselGraphics::statSteaming = 0;

VesselMapObject::VesselGraphics::VesselGraphics(VesselData *vessel)
    : qmapcontrol::GeometryPointShapeScaled(qmapcontrol::PointWorldCoord(vessel->mVessel->get_x(), vessel->mVessel->get_y()), QSizeF(20.0, 40.0), 11, 7, 17),
      mVessel(vessel)
{
    if (color == 0)
        color = new QBrush(Qt::darkYellow);
    if (statHarbour == 0)
        statHarbour = new QBrush(Qt::green);
    if (statFishing == 0)
        statFishing = new QBrush(Qt::red);
    if (statSteaming == 0)
        statSteaming = new QBrush(Qt::black);

    setNonlinearZoomFactor(0.9);
}

void VesselMapObject::VesselGraphics::updated()
{
    setCoord(qmapcontrol::PointWorldCoord(mVessel->mVessel->get_x(), mVessel->mVessel->get_y()));
    emit positionChanged(this);
    emit requestRedraw();
}

void VesselMapObject::VesselGraphics::drawShape(QPainter &painter, const qmapcontrol::RectWorldPx &rect)
{
    Q_UNUSED(rect);

    painter.rotate(mVessel->mVessel->get_course());

    painter.setBrush(*color);
    painter.drawEllipse(-10, -20, 20, 40);

    // Ears
    switch (mVessel->mVessel->get_state()) {
    case 1:
        painter.setBrush(*statFishing);
        break;
    case 2:
        painter.setBrush(*statSteaming);
        break;
    default:
    case 3:
        painter.setBrush(*statHarbour);
        break;
    }

    painter.drawEllipse(-6, -12, 12, 24);

    painter.rotate(-mVessel->mVessel->get_course());
}
