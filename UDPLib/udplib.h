#ifndef UDPLIB_H
#define UDPLIB_H

#include <iostream>
#include <array>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <boost/asio.hpp>

using namespace std;
using boost::asio::ip::udp;
using boost::asio::ip::address;

#define BUFLEN 1024
#define SERVER_PORT 8080
#define CLIENT_PORT 8090
#define LOCALHOST "127.0.0.1"
#define BROADCAST "255.255.255.255"
#define SEND_RATE_SECONDS 2

#pragma pack(push, 1)
struct DataStruct
{
    int hour, min, sec; // �����
    double fuelWeight;  // ��� �������, ��
    union {
        unsigned int word; // ����� ������
        struct {
            unsigned char addr : 8;     // ����� (� 16-�������� �������������)
            unsigned char reserve1 : 4; // �����-�� ������, �� ����� ��� ������������� 6 -> 4 ��� ������!!!
            bool dataOk : 1;            // ������� ������������ ������
            bool onLand : 1;            // ������� ���������� �� �����
            unsigned int speed : 10;    // ��������, ��/� (���� �������� ������� - 512)
            unsigned char reserve2 : 5; // �����-�� ������, �� ����� ��� �������������
            unsigned char matrix : 2;   // ������� ���������(00 - �����, 01 � ��� ������, 10 � ����, 11 - ���������� ������)
            bool ctrlBit : 1;           // ��� �������� ��������
        } data;
    } dataUnion;
    bool isCrashed;     // ������� ����������

    const char* toString() const {
        static char buffer[BUFLEN];  // ����� ��� �������� ������
        // ��������� ������ � �������
        snprintf(buffer, sizeof(buffer),
            "Time: %02d:%02d:%02d\n"
            "Fuel Weight: %.2f kg\n"
            "Address: 0x%02X\n"
            "Data OK: %s\n"
            "On Land: %s\n"
            "Speed: %d km/h\n"
            "Matrix: %s\n"
            "Control Bit: %s\n"
            "Crashed: %s\n",
            hour, min, sec,                                         // �����
            fuelWeight,                                             // ��� �������
            dataUnion.data.addr,                                    // �����
            dataUnion.data.dataOk ? "Yes" : "No",                   // Data OK
            dataUnion.data.onLand ? "Yes" : "No",                   // On Land
            dataUnion.data.speed,                                   // ��������
            dataUnion.data.matrix == 0 ? "Failure" :
            (dataUnion.data.matrix == 1 ? "No Data" :
            (dataUnion.data.matrix == 2 ? "Test" : "Normal")),      // Matrix
            dataUnion.data.ctrlBit ? "Error" : "OK",                // Control Bit
            isCrashed ? "Yes" : "No"                                // Crashed
        );                              

        // ���������� ��������� �� �����
        return buffer;
    }

    void randInit() {
        srand(static_cast<unsigned int>(time(nullptr)));
        hour = rand() % 24;     // �� 0 �� 23
        min = rand() % 60;      // �� 0 �� 59
        sec = rand() % 60;      // �� 0 �� 59
        fuelWeight = static_cast<double>(rand() % 1000) + 1.0;  // �� 1.0 �� 1000.0 ��
        dataUnion.data.addr = rand() % 256;  // �� 0 �� 255 (8 ���)
        dataUnion.data.reserve1 = rand() % 16; // �� 0 �� 15 (4 ����)
        dataUnion.data.dataOk = rand() % 2;   // �������� 0 ��� 1
        dataUnion.data.onLand = rand() % 2;   // �������� 0 ��� 1
        dataUnion.data.speed = rand() % 1024; // �� 0 �� 1023 (10 ���)
        dataUnion.data.reserve2 = rand() % 32; // �� 0 �� 31 (5 ���)
        dataUnion.data.matrix = rand() % 4;   // �� 0 �� 3 (2 ����)
        dataUnion.data.ctrlBit = rand() % 2;  // �������� 0 ��� 1
        isCrashed = rand() % 2; // �������� 0 ��� 1
    }
};
#pragma pack(pop)

class UDPServer{
    public:
        UDPServer(boost::asio::io_context& io_context);
        ~UDPServer();
    
    private:
        void start_sender();
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::endpoint client_endpoint_;
};

class UDPClient{
    public:
        UDPClient(boost::asio::io_context& io_context);
        ~UDPClient();

        function<void(const DataStruct&)> onDataReceived;

    private:
        void start_reciever();
        boost::asio::ip::udp::socket socket_;
        boost::asio::ip::udp::endpoint server_endpoint_;
        array<char, sizeof(DataStruct)> buffer_;
};

#endif