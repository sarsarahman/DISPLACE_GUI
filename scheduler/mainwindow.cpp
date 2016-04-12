#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <utils/make_unique.h>

#include <rundialog.h>
#include <utils/MultifieldWriter.h>
#include <utils/MultifieldReader.h>

#include <fstream>

#include <QTableView>
#include <QFileDialog>
#include <QSettings>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mScheduler = utils::make_unique<SchedulerJob>();
    ui->dataView->setModel(mAdapter = new SchedulerJobAdapter(*mScheduler, this));

//#ifdef Q_OS_WIN32
//    ui->action_Generate_Script->setVisible(false);
//#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Add_triggered()
{
    RunDialog dlg;
    if (dlg.exec() == QDialog::Accepted) {
        QList<QString> lsim;

        auto &d = dlg.get();
        QString sim = d.getSimulationName();
        QRegExp r("(.*)\\[([0-9]+)-([0-9]+)\\]");
        if (r.indexIn(sim) == -1) {
            lsim << sim;
        } else {
            bool ok1, ok2;
            QString pat = r.cap(1);
            int first = r.cap(2).toInt(&ok1);
            int last = r.cap(3).toInt(&ok2);

            if (ok1 && ok2 && first <= last) {
                while (first <= last) {
                    lsim << QString("%1%2")
                            .arg(pat)
                            .arg(first);
                    ++first;
                }
            } else {
                lsim << sim;
            }
        }

        for (auto sim : lsim) {
            auto d = dlg.get();
            d.setSimulationName(sim);
            mAdapter->addSimulationRun(d);
        }
    }
}

void MainWindow::on_action_Save_triggered()
{
    QSettings s;
    QString out = QFileDialog::getSaveFileName(this, tr("Save scheduler file"),
                                               s.value("last_scheduler").toString(),
                                               QString("Scheduler files (*.dsf);;All files (*)")
                                               );

    if (!out.isEmpty()) {
        std::ofstream of(out.toStdString(), std::ios_base::out | std::ios_base::trunc);

        bool ok = false;
        if (of) {
            auto jobsAsTuple = mScheduler->getDataAsTuple();

            displace::formats::helpers::MultifieldWriter wr;
            if(wr.exportToStream(of, ",", jobsAsTuple.begin(), jobsAsTuple.end())) {
                QMessageBox::information(this, tr("Displace Scheduler editor"),
                                         tr("File saved successfully"));
            }
            QFileInfo info(out);
            s.setValue("last_scheduler", info.path());
            ok = true;
        }

        if (!ok) {
            QMessageBox::warning(this, tr("Displace Scheduler editor"),
                                 tr("Couldn't save the file. Check the destination has the right write permissions."));
        }
    }
}

void MainWindow::on_action_Open_triggered()
{
    QSettings s;
    QString in = QFileDialog::getOpenFileName(this, tr("Save scheduler file"),
                                               s.value("last_scheduler").toString(),
                                               QString("Scheduler files (*.dsf);;All files (*)")
                                               );

    if (!in.isEmpty()) {
        bool ok = false;
        std::ifstream ins(in.toStdString(), std::ios_base::in);

        if (ins) {
            std::list<SimulationRun> data;
            displace::formats::helpers::MultifieldReader rdr;
            bool r = rdr.importFromStream<SimulationRun::DataAsTuple>(ins, ",", [&data](const SimulationRun::DataAsTuple &d) {
                data.push_back(SimulationRun(d));
                return true;
            });

            if (r) {
                mScheduler->appendData(data);
                mAdapter->refresh();

                QFileInfo info(in);
                s.setValue("last_scheduler", info.path());
                ok = true;
            }
        }

        if (!ok) {
            QMessageBox::warning(this, tr("Displace Scheduler editor"),
                                 tr("Couldn't load the file. Check the destination has the right access permissions and the file exists and it is in the right format."));
        }
    }
}

void MainWindow::on_action_Generate_Script_triggered()
{
    QSettings s;
    QString out = QFileDialog::getSaveFileName(this, tr("Export Script file"),
                                               s.value("last_gen_script").toString(),
                                               QString("Scheduler files (*.bat);;All files (*)")
                                               );

    if (!out.isEmpty()) {
        std::ofstream os(out.toStdString(), std::ios_base::out | std::ios_base::trunc);

        bool ok = false;
        if (os) {
            for (int i = 0; i < mScheduler->jobsCount(); ++i) {
                auto job = mScheduler->job(i);

                os << "start /d " << QCoreApplication::applicationDirPath().toStdString() <<
                      " displace -f \"" << job.getName().toStdString() <<
                      "\" -f2 \"" << job.getSimulationOutputName().toStdString() <<
                      "\" -s \"" << job.getSimulationName().toStdString() <<
                      "\" -i " << job.getSimulationSteps() <<
                      " -p 1 -o 1 -e 1 -v 0 --without-gnuplot -V 1 --num_threads " << job.getNumThreads() <<
                      " > ..\\" << job.getName().toStdString() << "-out.txt" << std::endl;
            }

            os.close();

            QFileInfo info(out);
            s.setValue("last_gen_script", info.path());
            ok = true;

            QMessageBox::information(this, tr("Script generated"),
                                     tr("Simulation script generated successfully."));
        }

        if (!ok) {
            QMessageBox::warning(this, tr("Displace Scheduler editor"),
                                 tr("Couldn't save the file. Check the destination has the right write permissions."));
        }
    }

}
