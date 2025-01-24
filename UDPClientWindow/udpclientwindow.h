#ifndef UDPCLIENTWINDOW_H
#define UDPCLIENTWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QThread>
#include <QString>
#include <QHeaderView>
#include "udplib.h"

using namespace std;

struct DataStruct;

class UDPClientWindow : public QWidget {
    Q_OBJECT

public:
    explicit UDPClientWindow(QWidget* parent = nullptr);
    ~UDPClientWindow();

private:
    QTableWidget* tableWidget;  // Таблица для отображения данных
    QLabel* statusLabel;        // Лейбл для статуса
    boost::asio::io_context* io_context;
    UDPClient* udpClient;
    QThread* ioThread;

    void setupUI();

signals:
    void dataReceived(const DataStruct& data);

private slots:
    void updateTable(const DataStruct& data);
};

#endif // UDPCLIENTWINDOW_H
