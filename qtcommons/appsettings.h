#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QString>
#include <QSettings>

namespace displace {

class AppSettings
{
    QSettings settings;
public:
    AppSettings();

    QPointF getMapCenterPoint() const;
    void setMapCenterPoint(const QPointF &);

    static void initialize();

    static const QString MapCenterPoint;
};

}


#endif // APPSETTINGS_H