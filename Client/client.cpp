#include <QApplication>
#include "udpclientwindow.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]){
    try{
        QApplication app(argc, argv);

        UDPClientWindow window;
        window.show();

        return app.exec();
    } catch (exception& ex){
        cerr << "Client error: " << ex.what() << endl;
        return EXIT_FAILURE;
    }
}