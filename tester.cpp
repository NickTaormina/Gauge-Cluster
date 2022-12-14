#include "tester.h"


tester::tester(QObject *parent)
    : QObject{parent}
{
    qRegisterMetaType<QCanBusFrame>("QCanBusFrame");

}

void tester::testFileSelected(QString url){

    // Create a new QThread and move this object to the thread
        thread = new QThread;
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
    removeCharacters(url, "");

    QFile file(url);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error opening file";
        return;
    }
    // Read the file into a QByteArray
        QByteArray data = file.readAll();


        // Process the data in the array
        int index = 0;
        QList<QByteArray> lines = data.split('\n');
        while (index < lines.size()) {
            // Extract the next frame from the data
            //qDebug() << lines.at(index);
            QCanBusFrame frame = extractFrame(lines.at(index));
            if (frame.isValid() && frame.payload().count() >= 2 && frame.payload().count() < 9 && frame.frameId() != 0) {
                emit testFrameProcessed(frame);
                //qDebug() << "test frame: " << frame.toString();
            } else {
                //qDebug() << "Invalid frame: " << data.mid(index);
                //break;
            }
            if(index % 10 == 0){
                thread->msleep(5);
            }
            index++;
        }

        // Close the file
        file.close();
        emit processFinished();


}

QCanBusFrame tester::extractFrame(const QByteArray& data){

        // Split the current line into fields
        QList<QByteArray> fields = data.split(' ');
        int i = 0;

        while(i < fields.count()){
            if(fields.at(i) == ""){
                fields.removeAt(i);
            } else {
                i++;
            }
        }

        if (fields.count() < 5 || fields.count() > 13) {
            return QCanBusFrame();
        }

        // Extract the frame ID and data length from the first field
        bool ok;
        quint32 id = fields[0].toUInt(&ok, 16);
        if (!ok) {
            return QCanBusFrame();
        }
        if(fields[1].length() >= 2){
            quint8 length = fields[1].mid(1, 1).toUInt(&ok);
        }
        if (!ok) {
            return QCanBusFrame();
        }

        // Extract the frame data from the remaining fields
        QByteArray frameData;
        for (int i = 2; i < fields.count(); i++) {
            frameData.append(static_cast<char>(fields[i].toUInt(&ok, 16)));
            if (!ok) {
                return QCanBusFrame();
            }
        }

        // Return the extracted frame
        return QCanBusFrame(id, frameData);
}

void tester::removeCharacters(const QString &fileName, const QString &characters)
{
    // Open the file
        QFile file(fileName);
        if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            qDebug() << "Error opening file";
            return;
        }

        // Read the contents of the file into a QByteArray
        QByteArray data = file.readAll();

        // Convert the QByteArray to a QString
        QString str = QString::fromUtf8(data);

        // Remove the characters from the QString
        str.replace("\"     ", "");
        str.replace("\"\n", "\n");
        //str.replace("\"", "\n");
        //remove the group of 5 spaces
        str.remove(QRegularExpression("\\s{5,}"));

        // Convert the modified QString back to a QByteArray
        data = str.toUtf8();

        // Write the modified QByteArray back to the file
        file.seek(0);
        file.write(data);

        // Close the file
        file.close();
}
