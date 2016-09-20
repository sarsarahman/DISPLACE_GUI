#ifndef VESSELEDITORMAINWINDOW_H
#define VESSELEDITORMAINWINDOW_H

#include <QMainWindow>

#include <memory>

namespace Ui {
class VesselEditorMainWindow;
}

class CsvTableModel;

QT_BEGIN_NAMESPACE
class QSortFilterProxyModel;
class QProcess;
QT_END_NAMESPACE

class VesselEditorMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit VesselEditorMainWindow(QWidget *parent = 0);
    ~VesselEditorMainWindow();

    static const QString VesselsSpecFilename;

private slots:
    void processStarted();
    void readOutput();
    void readError();
    void processExit(int);

    void on_run_clicked();
    void on_genConfig_clicked();
    void on_actionRscript_location_triggered();
    void on_actionScripts_location_triggered();
    void on_browseInputPath_clicked();
    void on_browseGISPath_clicked();
    void on_genMetVar_clicked();
    void on_genMetSelectivity_clicked();
    void on_browseBasePath_clicked();
    void on_tabWidget_currentChanged(int index);

private:
    Ui::VesselEditorMainWindow *ui;

    QProcess *mProcess;

    bool runScript(QString script);
    void checkEnv();
};

#endif // VESSELEDITORMAINWINDOW_H
