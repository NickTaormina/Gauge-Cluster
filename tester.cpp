#include "tester.h"


tester::tester(QObject *parent)
    : QObject{parent}
{
    qRegisterMetaType<QCanBusFrame>("QCanBusFrame");

}

void tester::testFileSelected(QString url){

    // Create a new QThread and move this object to the thread
        QThread* thread = new QThread;
        this->moveToThread(thread);

        // Connect the thread's started signal to a slot that runs the file processing
        connect(thread, &QThread::started, this, [this, url]{ processFile(url); });


        // Connect the finished signal to the thread's quit slot, which will cause the
        // thread to exit once the file processing is complete
        connect(this, &tester::processFinished, thread, &QThread::quit);

        // Connect the thread's finished signal to a slot that will delete the thread object
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);

        // Start the thread
        thread->start();

}

void tester::processFile(QString url)
{
    // Open the file
    url.remove("file:///");
    qDebug() << url;
    QFile file(url);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file";
        return;
    }

    // Create a text stream from the file
    QTextStream in(&file);

    // Read each line (i.e. frame) from the file
    while (!in.atEnd()) {
        QString line = in.readLine();
        //qDebug() << "Sending message: " << line;

        // Split the line into fields based on whitespace
        QStringList fields = line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        // Check if we have the correct number of fields
        if (fields.size() > 11) {
            qDebug() << "Invalid frame: " << line;
            continue;
        }

        // Convert the fields to the correct data types
        bool ok;
        quint32 id = fields[0].toUInt(&ok, 16);
        if (!ok) {
            //qDebug() << "Invalid frame: " << line;
            continue;
        }
        QByteArray data;
        for (int i = 2; i < fields.size(); i++) {
            data.append(static_cast<char>(fields[i].toUInt(&ok, 16)));
            if (!ok) {
                qDebug() << "Invalid frame: " << line;
                continue;
            }
        }
        // Create the QCanBusFrame from the extracted data
        QCanBusFrame frame(id, data);
        emit testFrameProcessed(frame);
        emit processFinished();
        //qDebug() << "test frame: " << frame.toString();

       // Here you can send the frame using your desired method.
       // For example, you could use a QCanBusDevice to send it.
    }

   // Close the file
   file.close();
}
