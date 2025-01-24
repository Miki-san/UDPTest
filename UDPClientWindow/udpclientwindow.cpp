#include "udpclientwindow.h"

UDPClientWindow::UDPClientWindow(QWidget* parent)
    : QWidget(parent), io_context(new boost::asio::io_context), udpClient(new UDPClient(*io_context)), ioThread(new QThread(this)) {
    setupUI();

    // Подключение сигнала из UDP клиента
    connect(this, &UDPClientWindow::dataReceived, this, &UDPClientWindow::updateTable);

    // Запуск Boost.Asio в отдельном потоке
    connect(ioThread, &QThread::started, [this]() {
        try {
            io_context->run();
        }
        catch (const exception& ex) {
            qCritical("Error in io_context: %s", ex.what());
        }
    });

    // Установка callback для получения данных
    udpClient->onDataReceived = [this](const DataStruct& data) {
        qDebug("Data received in callback: %s", qPrintable(QString::fromStdString(data.toString())));
        QMetaObject::invokeMethod(this, [this, data]() {
            emit dataReceived(data);
        }, Qt::QueuedConnection);
    };
    
    ioThread->start();
}

UDPClientWindow::~UDPClientWindow() {
    io_context->stop();
    ioThread->quit();
    ioThread->wait();

    delete udpClient;
    delete io_context;
}

void UDPClientWindow::setupUI() {
    QVBoxLayout* layout = new QVBoxLayout(this);

    statusLabel = new QLabel("Waiting for data...", this);
    layout->addWidget(statusLabel);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(8);
    tableWidget->setHorizontalHeaderLabels({ "Time", "Fuel Weight", "Address", "Data OK",
                                            "On Land", "Speed", "Matrix", "Crashed" });
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(tableWidget);

    setLayout(layout);
    setWindowTitle("UDP Client");
    resize(800, 600);
}

void UDPClientWindow::updateTable(const DataStruct& data) {
    qDebug("UpdateTable called with data: %s", qPrintable(QString::fromStdString(data.toString())));

    // Добавляем строку с данными
    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);

    // Заполняем ячейки строки
    tableWidget->setItem(row, 0, new QTableWidgetItem(QString("%1:%2:%3")
        .arg(data.hour, 2, 10, QChar('0'))
        .arg(data.min, 2, 10, QChar('0'))
        .arg(data.sec, 2, 10, QChar('0'))));
    tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(data.fuelWeight, 'f', 2)));
    tableWidget->setItem(row, 2, new QTableWidgetItem(QString("0x%1").arg(data.dataUnion.data.addr, 2, 16, QChar('0')).toUpper()));
    tableWidget->setItem(row, 3, new QTableWidgetItem(data.dataUnion.data.dataOk ? "Yes" : "No"));
    tableWidget->setItem(row, 4, new QTableWidgetItem(data.dataUnion.data.onLand ? "Yes" : "No"));
    tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(data.dataUnion.data.speed)));
    tableWidget->setItem(row, 6, new QTableWidgetItem(
        data.dataUnion.data.matrix == 0 ? "Failure" :
        (data.dataUnion.data.matrix == 1 ? "No Data" :
            (data.dataUnion.data.matrix == 2 ? "Test" : "Normal"))
    ));
    tableWidget->setItem(row, 7, new QTableWidgetItem(data.isCrashed ? "Yes" : "No"));

    // Обновляем статус
    statusLabel->setText("<font color='green'>Data received successfully.</font>");
}