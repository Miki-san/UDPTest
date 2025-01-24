#include "udplib.h"

int main(){
    try{
        boost::asio::io_context io_context;
        UDPServer server(io_context);
        io_context.run();
    }catch(exception& ex){
        cerr << "Server error: " << ex.what() << endl;
    }
    return 0;
}