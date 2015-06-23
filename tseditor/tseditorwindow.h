#ifndef TSEDITORWINDOW_H
#define TSEDITORWINDOW_H

#include <QMainWindow>
#include <QProcess>

#include <memory>

namespace Ui {
class TsEditorWindow;
}

class CsvTableModel;

class TsEditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit TsEditorWindow(QWidget *parent = 0);
    ~TsEditorWindow();

private slots:
    void on_action_Open_triggered();
    void on_varSelect_currentIndexChanged(const QString &arg1);
    void on_areaSelect_activated(const QString &arg1);
    void on_adimSelect_activated(const QString &arg1);

    void on_action_Log_Window_triggered();

    void on_clearLog_clicked();

    void on_dockLogWindow_visibilityChanged(bool visible);

public slots:
    void readOutput();
    void readError();
    void processExit(int);

protected:
    void closeEvent(QCloseEvent *event);

private:
    Ui::TsEditorWindow *ui;

    QString mFilename;
    std::shared_ptr<QList<QStringList> > mData;
    CsvTableModel *mModel;

    int colVar, colArea, colADim;

    QString mDestFile;
    QProcess *mProcess;

    void load(QString filename);
    void updateKeys();
    void genSampleFile();
    void loadSampleFileGraph();

    void generate(QString dest, QString variable, QString area, QString adim);
};

#endif // TSEDITORWINDOW_H
