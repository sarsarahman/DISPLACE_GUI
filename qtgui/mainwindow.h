#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <outputfileparser.h>
#include <statscontroller.h>

#include <QMainWindow>
#include <QProcess>

#include <memory>
#include <QMapControl/Point.h>

using namespace qmapcontrol;

namespace Ui {
class MainWindow;
}

namespace qmapcontrol {
class QMapControl;
class PointWorldCoord;
}

class ObjectTreeModel;
class DisplaceModel;
class Simulator;
class MapObjectsController;

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

#define MAX_MODELS 2

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void simulatorLogging(QString);
    void simulatorProcessStateChanged (QProcess::ProcessState);
    void simulatorProcessStepChanged (int step);
    void simulatorNodeStatsUpdate(QString);

    void vesselMoved (int step, int idx, float x, float y, float course, float fuel, int state);
    void updateModelState();
    void updateOutputFile (QString,int);
    void outputUpdated();
    void mapFocusPointChanged(PointWorldCoord);

    void errorImportingStatsFile(QString);

    void on_action_Load_triggered();
    void on_modelSelector_currentIndexChanged(int index);
    void on_cmdStart_clicked();
    void on_cmdStop_clicked();
    void on_actionScenario_triggered();
    void on_actionSave_triggered();
    void on_treeView_doubleClicked(const QModelIndex &index);
    void on_saveConsoleButton_clicked();
    void on_cmdSetup_clicked();
    void on_action_Link_database_triggered();
    void on_actionImport_results_triggered();
    void on_actionLoad_results_triggered();
    void on_play_step_valueChanged(int step);
    void on_play_fwd_clicked();
    void on_play_bk_clicked();
    void on_play_fbk_clicked();
    void on_play_ffwd_clicked();
    void on_play_first_clicked();
    void on_play_last_clicked();
    void on_play_auto_clicked();
    void on_actionPalettes_triggered();
    void on_popStatSelector_currentIndexChanged(int index);

signals:
    void modelStateChanged();

protected:
    void updateModelList();
    void updateAllDisplayObjects();
    void closeEvent ( QCloseEvent * event );

    void centerMap(const qmapcontrol::PointWorldCoord &pt);
    void centerMapOnHarbourId (int id);
    void centerMapOnNodeId (int id);
    void centerMapOnVesselId (int id);

private:
    Ui::MainWindow *ui;

    // Data model
    DisplaceModel *models[MAX_MODELS];
    DisplaceModel *currentModel;
    int currentModelIdx;
    Simulator *mSimulation;
    MapObjectsController *mMapController;
    StatsController *mStatsController;

    // Geospatial objects
    qmapcontrol::QMapControl *map;

    // tree model adapter
    ObjectTreeModel *treemodel;

    static const QString dbSuffix;
    static const QString dbFilter;
    static const QString dbLastDirKey;
    static const int maxModels;
};

#endif // MAINWINDOW_H
