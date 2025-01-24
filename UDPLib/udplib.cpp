#include "udplib.h"

//UDPServer realization
UDPServer::UDPServer(boost::asio::io_context& io_context) 
    :socket_(io_context, udp::endpoint(udp::v4(), SERVER_PORT)), client_endpoint_(boost::asio::ip::address_v4::broadcast(), CLIENT_PORT){
            socket_.set_option(boost::asio::socket_base::broadcast(true));
            start_sender();
            cout << "UDP server started" << endl;
        }

UDPServer::~UDPServer() {
    try{
        if(socket_.is_open()){
            socket_.cancel();
            socket_.close();
        }
        cout << "UDP server closed" << endl;
    } catch(exception& e){
        cerr << "Error: " << e.what() << endl;
    }
}

void UDPServer::start_sender(){
    static DataStruct data;
    data.randInit();
    socket_.async_send_to(
        boost::asio::buffer(&data, sizeof(DataStruct)),
        client_endpoint_, 
        [this](const boost::system::error_code& error, size_t bytes_transferred) {
            if(!error){
                cout << "Data sent: " << endl;
                cout << data.toString() << endl;
            } else{
                cerr << "Sending error" << error.message() << endl;
            }
            this_thread::sleep_for(chrono::seconds(SEND_RATE_SECONDS));
            start_sender();
        }
    );
}

//UDPClient realization
UDPClient::UDPClient(boost::asio::io_context& io_context)
    :socket_(io_context, udp::endpoint(udp::v4(), CLIENT_PORT)), server_endpoint_(boost::asio::ip::make_address(LOCALHOST), SERVER_PORT){
        start_reciever();
    }

UDPClient::~UDPClient(){
    try{
        if(socket_.is_open()){
            socket_.cancel();
            socket_.close();
        }
        cout << "UDP client closed" << endl;
    } catch(exception& e){
        cerr << "Error: " << e.what() << endl;
    }
}

void UDPClient::start_reciever(){
    socket_.async_receive_from(
        boost::asio::buffer(buffer_), 
        server_endpoint_, 
        [this](const boost::system::error_code& error, size_t bytes_transferred){
            if(!error && bytes_transferred == sizeof(DataStruct)){
                static DataStruct data;
                memcpy(&data, buffer_.data(), sizeof(DataStruct));
                if (onDataReceived) {
                    onDataReceived(data); // Передача данных через callback
                    cout << "Data recieved: " << endl;
                    cout << data.toString() << endl;
                }
            } else{
                cerr << "Reciever error" << error.message() << endl;
            }
            start_reciever();
        }
    );
}