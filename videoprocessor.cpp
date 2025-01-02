#include <QDebug>

#include "videoprocessor.h"

VideoProcessor::VideoProcessor() {

}

VideoProcessor::~VideoProcessor() {

}

int VideoProcessor::setPlayUrl(const QString& playUrl) {
    m_playUrl = playUrl;
    return 0;
}

int VideoProcessor::gstHandle() {
    if (m_gstData.pipeline != nullptr) {
        gst_element_set_state(m_gstData.pipeline, GST_STATE_NULL);
    }
    if (m_gstData.sinkVideo != nullptr) {
        gst_object_unref(m_gstData.pipeline);
    }

    gst_init(nullptr, nullptr);

    GError *err = nullptr;
    /*QString pipeStr;
    if (playUrl == "rtsp://admin:12qweasd@192.168.3.19:554/Streaming/Channels/101") {
        pipeStr = "rtspsrc location=" + playUrl +
                  " ! decodebin ! videoconvert ! \
                  appsink name=mysink max-buffers=5 sync=0 \
            caps=\"video/x-raw, format=BGR\"";
    } else {
        pipeStr = "rtspsrc location=" + playUrl +
                  " ! rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! \
                  appsink name=mysink max-buffers=5 sync=0 \
            caps=\"video/x-raw, format=BGR\"";
    }*/
    QString pipeStr = "rtspsrc location=" + m_playUrl +
                      " ! decodebin ! videoconvert ! \
                      appsink name=mysink max-buffers=2 sync=1 \
                      caps=\"video/x-raw, format=BGR\"";
    m_gstData.pipeline = gst_parse_launch(pipeStr.toStdString().c_str(), &err);
    if (err) {
        qDebug() << "checkErr : " << err->message;
        return -1;
    }
    if (!m_gstData.pipeline) {
        qDebug() << "gstData.pipeline is nullptr!";
    }

    m_gstData.sinkVideo = gst_bin_get_by_name(GST_BIN (m_gstData.pipeline), "mysink");
    if (m_gstData.sinkVideo == nullptr) {
        qDebug() << "gstData.sinkVideo is nullptr!";
        return -1;
    }

    if(!gst_element_set_state(m_gstData.pipeline, GST_STATE_PLAYING)) {
        qDebug() << "gst_element_set_state failed!";
        return -1;
    }
    return 0;
}

int VideoProcessor::setCloseFlag(bool isClosed) {
    m_isClosed = isClosed;
    return 0;
}

void VideoProcessor::startVideo() {
    for (;;) {
        // Exit on EOS
        if (gst_app_sink_is_eos(GST_APP_SINK(m_gstData.sinkVideo))) {
            qDebug() << "EOS !";
            break;
        }

        // Pull the sample (synchronous, wait)
        GstSample *sample = gst_app_sink_pull_sample(GST_APP_SINK(m_gstData.sinkVideo));
        if (sample == nullptr) {
            qDebug() << "NO sample !";
            break;
        }

        // Get width and height from sample caps (NOT element caps)
        GstCaps *caps = gst_sample_get_caps(sample);
        if (caps != nullptr) {
            qDebug() << "NO sample, caps is nullptr!";
        }
        GstStructure *s = gst_caps_get_structure(caps, 0);
        int imW, imH;
        if (!gst_structure_get_int(s, "width", &imW)) {
            qDebug() << "gst_structure_get_int() for width failed!";
            break;
        }
        if (!gst_structure_get_int(s, "height", &imH)) {
            qDebug() << "gst_structure_get_int() for height failed!";
            break;
        }
        // qDebug() << "Sample: W = " << imW << ", H = " << imH;

        //        cout << "sample !" << endl;
        // Process the sample
        // "buffer" and "map" are used to access raw data in the sample
        // "buffer" is a single data chunk, for raw video it's 1 frame
        // "buffer" is NOT a queue !
        // "Map" is the helper to access raw data in the buffer
        GstBuffer *buffer = gst_sample_get_buffer(sample);
        GstMapInfo m;
        if (!gst_buffer_map(buffer, &m, GST_MAP_READ)) {
            qDebug() << "gst_buffer_map() failed!";
            break;
        }
        if (m.size != imW * imH * 3) {
            qDebug() << "m.size = " << m.size <<
                "imW * imH * 3 = " << imW * imH * 3;
            continue;
        }
        QImage image = QImage((const uchar*)m.data,
            imW, imH, QImage::Format_BGR888).copy();

        emit inDisplay(image);
        // Don't forget to unmap the buffer and unref the sample
        gst_buffer_unmap(buffer, &m);
        gst_sample_unref(sample);
        //if (27 == key)
        //    exit(0);
        if (m_isClosed) {
            break;
        }
    }
}
