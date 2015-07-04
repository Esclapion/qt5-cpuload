#ifndef Cpuload_H
#define Cpuload_H

#include <QMainWindow>
#include <qcustomplot.h>
#include <QVector>

namespace Ui {
class Cpuload;
}

class Cpuload : public QMainWindow
{
    Q_OBJECT
public:
    explicit Cpuload(QWidget *parent = 0);
    ~Cpuload();

private:
    Ui::Cpuload *ui;
    QVector<double> m_x, m_y;
    void setupGraph(QCustomPlot *customPlot);
    int m_userPrev, m_nicePrev, m_systPrev, m_idlePrev;
    int m_user, m_nice, m_syst, m_idle;
    double m_load, m_total, m_usage;
    QFile *m_file;
    QCPBars *m_bars;
    int m_nbval;

private slots:
    void calcLoad();
};

#endif // Cpuload_H
