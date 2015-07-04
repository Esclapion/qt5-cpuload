#include "cpuload.h"
#include "ui_cpuload.h"

Cpuload::Cpuload(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cpuload)
{
    ui->setupUi(this);
    setGeometry(400, 250, 542, 390);
    m_nbval = 62;
    m_x.resize(m_nbval);
    m_y.resize(m_nbval);
    for(int i = 0; i < m_nbval; i ++)
    {
        m_x[i] = 0;
        m_y[i] = 0;
    }
    QTimer *timer = new QTimer(this);
    m_file = new QFile("/proc/stat");
    QString line;
    QStringList splitLine;
    m_file->open(QIODevice::ReadOnly | QIODevice::Text);
    line = m_file->readLine();
    splitLine = line.split(' ');
    m_userPrev = splitLine.at(2).toInt();
    m_nicePrev = splitLine.at(3).toInt();
    m_systPrev = splitLine.at(4).toInt();
    m_idlePrev = splitLine.at(5).toInt();
    m_file->close();
    connect(timer, SIGNAL(timeout()), this, SLOT(calcLoad()));
    timer->start(1000);
    m_bars = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    ui->customPlot->addPlottable(m_bars);
    setupGraph(ui->customPlot);
}

Cpuload::~Cpuload()
{
    delete ui;
}

void Cpuload::setupGraph(QCustomPlot *customPlot)
{

    for (int i=0; i < m_x.size(); ++i)
        m_x[i] = i;
    m_bars->clearData();
    m_bars->setWidth((9/(double)m_x.size() * 4) + (customPlot->width() + 200) / 4000.0 );
    m_bars->setData(m_x, m_y);
    m_bars->setPen(Qt::NoPen);
    m_bars->setBrush(QColor(10, 140, 70, 160));
    customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    customPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    customPlot->axisRect()->setBackground(axisRectGradient);

    customPlot->rescaleAxes();
    double valmax = 0.0;
    for(int i = 0; i < m_nbval; i ++)
        if(m_y[i] > valmax)
            valmax = m_y[i];
    customPlot->yAxis->setRange(0, valmax);
    customPlot->replot();
}

void Cpuload::calcLoad()
{
    QString line;
    QStringList splitLine;
    m_file->open(QIODevice::ReadOnly | QIODevice::Text);
    line = m_file->readLine();
    splitLine = line.split(' ');
    m_user = splitLine.at(2).toInt();
    m_nice = splitLine.at(3).toInt();
    m_syst = splitLine.at(4).toInt();
    m_idle = splitLine.at(5).toInt();
    m_file->close();

    m_load = (m_user - m_userPrev) + (m_nice - m_nicePrev) + (m_syst - m_systPrev);
    double idle = m_idle - m_idlePrev;
    m_total = m_load + idle;
    m_usage = (m_total - idle)/ m_total * 100;
    for(int i = m_nbval; i > 0; i--)
        m_y[i] = m_y[i -1];
    m_y[0] = m_usage;
    m_userPrev = m_user;
    m_nicePrev = m_nice;
    m_systPrev = m_syst;
    m_idlePrev = m_idle;
    setupGraph(ui->customPlot);
}
