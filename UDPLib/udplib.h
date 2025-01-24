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
    int hour, min, sec; // время
    double fuelWeight;  // вес топлива, кг
    union {
        unsigned int word; // слово данных
        struct {
            unsigned char addr : 8;     // адрес (в 16-тиричном представлении)
            unsigned char reserve1 : 4; // какой-то резерв, не нужно для использования 6 -> 4 ибо ошибка!!!
            bool dataOk : 1;            // признак корректности данных
            bool onLand : 1;            // признак нахождения на земле
            unsigned int speed : 10;    // скорость, км/ч (цена старшего разряда - 512)
            unsigned char reserve2 : 5; // какой-то резерв, не нужно для использования
            unsigned char matrix : 2;   // матрица состояния(00 - отказ, 01 – нет данных, 10 – тест, 11 - нормальная работа)
            bool ctrlBit : 1;           // бит контроля чётности
        } data;
    } dataUnion;
    bool isCrashed;     // признак разрушения

    const char* toString() const {
        static char buffer[BUFLEN];  // Буфер для хранения строки
        // Формируем строку с данными
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
            hour, min, sec,                                         // Время
            fuelWeight,                                             // Вес топлива
            dataUnion.data.addr,                                    // Адрес
            dataUnion.data.dataOk ? "Yes" : "No",                   // Data OK
            dataUnion.data.onLand ? "Yes" : "No",                   // On Land
            dataUnion.data.speed,                                   // Скорость
            dataUnion.data.matrix == 0 ? "Failure" :
            (dataUnion.data.matrix == 1 ? "No Data" :
            (dataUnion.data.matrix == 2 ? "Test" : "Normal")),      // Matrix
            dataUnion.data.ctrlBit ? "Error" : "OK",                // Control Bit
            isCrashed ? "Yes" : "No"                                // Crashed
        );                              

        // Возвращаем указатель на буфер
        return buffer;
    }

    void randInit() {
        srand(static_cast<unsigned int>(time(nullptr)));
        hour = rand() % 24;     // от 0 до 23
        min = rand() % 60;      // от 0 до 59
        sec = rand() % 60;      // от 0 до 59
        fuelWeight = static_cast<double>(rand() % 1000) + 1.0;  // от 1.0 до 1000.0 кг
        dataUnion.data.addr = rand() % 256;  // от 0 до 255 (8 бит)
        dataUnion.data.reserve1 = rand() % 16; // от 0 до 15 (4 бита)
        dataUnion.data.dataOk = rand() % 2;   // случайно 0 или 1
        dataUnion.data.onLand = rand() % 2;   // случайно 0 или 1
        dataUnion.data.speed = rand() % 1024; // от 0 до 1023 (10 бит)
        dataUnion.data.reserve2 = rand() % 32; // от 0 до 31 (5 бит)
        dataUnion.data.matrix = rand() % 4;   // от 0 до 3 (2 бита)
        dataUnion.data.ctrlBit = rand() % 2;  // случайно 0 или 1
        isCrashed = rand() % 2; // случайно 0 или 1
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