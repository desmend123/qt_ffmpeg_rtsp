#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQueue>
#include <QScreen>
#include <QThread>
#include <QTime>
#include <QWindow>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);
    setAppIcon();
    m_ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_scene = new QGraphicsScene();
    m_ui->graphicsView->setScene(m_scene);

    m_videoProcessor = QSharedPointer<VideoProcessor>::create();

    m_videoProcessor->moveToThread(new QThread(this));

    connect(m_videoProcessor->thread(), SIGNAL(started()),
            m_videoProcessor.data(), SLOT(startVideo()));
    connect(m_videoProcessor.data(), SIGNAL(inDisplay(QImage)),
            this, SLOT(onInDisplay(QImage)));


    connect(m_ui->openNetStream, SIGNAL(triggered()), this,
            SLOT(onOpenNetStreamTriggered()));
    onOpenNetStreamTriggered();
}

MainWindow::~MainWindow()
{
    delete m_ui;

    m_scene->removeItem(m_GraphicItem.data());
    delete m_scene;
}

void MainWindow::onOpenNetStreamTriggered() {
    if (m_openMediaWindow == nullptr) {
        m_openMediaWindow = QSharedPointer<OpenMediaWindow>::create(this);

        QList<QScreen*> screens = QApplication::screens();
        int i = 0;
        for (; i <= screens.size() - 1; i++) {
            QRect geometry = screens[i]->geometry();

            int x = geometry.x() + geometry.width() / 4;
            int y = geometry.y() + geometry.height() / 5;
            m_openMediaWindow->setGeometry(x, y, geometry.width() / 2, geometry.height() * 3 / 5);
            break;
        }

        m_openMediaWindow->setWindowFlags(Qt::Tool);
        m_openMediaWindow->show();

        connect(m_openMediaWindow.data(), SIGNAL(playUrlNeeded(QString)),
            this, SLOT(play(QString)));
    } else {
        m_openMediaWindow->show();
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    event->accept();
    m_openMediaWindow->close();

    m_videoProcessor->setCloseFlag(true);
    m_videoProcessor->thread()->quit();
    m_videoProcessor->thread()->wait();
}

static bool busProcessMsg(GstElement *pipeline, GstMessage *msg, const QString &prefix) {
    using namespace std;

    GstMessageType mType = GST_MESSAGE_TYPE(msg);
    // qDebug() << "[" << prefix << "] : mType = " << mType << " ";
    switch (mType) {
    case (GST_MESSAGE_ERROR):
        // Parse error and exit program, hard exit
        GError *err;
        gchar *dbg;
        gst_message_parse_error(msg, &err, &dbg);
        qDebug() << "ERR = " << err->message << " FROM " << GST_OBJECT_NAME(msg->src);
        qDebug() << "DBG = " << dbg;
        g_clear_error(&err);
        g_free(dbg);
        exit(1);
    case (GST_MESSAGE_EOS) :
        // Soft exit on EOS
        qDebug() << " EOS !";
        return false;
    case (GST_MESSAGE_STATE_CHANGED):
        // Parse state change, print extra info for pipeline only
        // qDebug() << "State changed !";
        if (GST_MESSAGE_SRC(msg) == GST_OBJECT(pipeline)) {
            GstState sOld, sNew, sPenging;
            gst_message_parse_state_changed(msg, &sOld, &sNew, &sPenging);
            qDebug() << "Pipeline changed from " << gst_element_state_get_name(sOld) << " to " <<
                gst_element_state_get_name(sNew);
        }
        break;
    case (GST_MESSAGE_STEP_START):
        // qDebug() << "STEP START !";
        break;
    case (GST_MESSAGE_STREAM_STATUS):
        // qDebug() << "STREAM STATUS !";
        break;
    case (GST_MESSAGE_ELEMENT):
        // qDebug() << "MESSAGE ELEMENT !";
        break;

        // You can add more stuff here if you want

    default:
        // qDebug() << '\n';
        break;
    }
    return true;
}

int MainWindow::busProcess(GstData data, const QString prefix) {
    GstBus *bus = gst_element_get_bus(data.pipeline);

    int res;
    while (true) {
        GstMessage *msg = gst_bus_timed_pop(bus, GST_CLOCK_TIME_NONE);
        if (msg == nullptr) {
            qDebug() << "Msg is nullptr!";
            return -1;
        }
        res = busProcessMsg(data.pipeline, msg, prefix);
        gst_message_unref(msg);
        if (!res)
            break;
    }
    gst_object_unref(bus);
    qDebug() << "BUS THREAD FINISHED : " << prefix;
    return 0;
}



void MainWindow::play() {
    if (m_videoProcessor->thread()->isRunning()) {
        m_videoProcessor->thread()->quit();
    }
    m_videoProcessor->gstHandle();
    m_videoProcessor->thread()->start();
    if (m_openMediaWindow != nullptr) {
        m_openMediaWindow->hide();
    }
}

void MainWindow::play(const QString& playUrl) {
    if (playUrl.isEmpty()) {
        return;
    }
    m_videoProcessor->setPlayUrl(playUrl);
    play();
}

void MainWindow::setAppIcon() {
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/icon/app/icons/appicon.png"), QSize(), QIcon::Normal, QIcon::Off);
    setWindowIcon(icon);
}

void MainWindow::onInDisplay(QImage image) {
    pixmap = QPixmap::fromImage(image);
    m_GraphicItem =
        QSharedPointer<QGraphicsPixmapItem>::create(pixmap);
    double viewWidth = m_ui->graphicsView->width();
    double viewHeight = m_ui->graphicsView->height();
    m_GraphicItem->setScale(viewWidth / pixmap.width());
    m_GraphicItem->setScale(viewHeight / pixmap.height());
    m_scene->addItem(m_GraphicItem.data());
}
