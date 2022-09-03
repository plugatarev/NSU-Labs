#include<iostream>
#include<fstream>
template<class TLog>
void log(std::string & msg, TLog log) {
    log(msg);
}


void f(std::string & msg){
    std::cerr << msg << std::endl;
}

void error(std::string & msg) {
    // TODO: call log function so that msg would be printed to std::cerr. use function pointer
    log(msg, f);
}

void file(std::string & msg) {
    // TODO: call log function so that msg would be printed to file log.txt . use functor
    // TODO: how to work with files: https://www.cplusplus.com/doc/tutorial/files/
    class MyGt {
    public:
        MyGt(){};
        bool operator()(std::string& msg){
            std::ofstream myfile ("log.txt");
            myfile << msg;
            myfile.close(); 
        }
    };
    const MyGt & gt = MyGt();
    log(msg,gt);
}

void info(std::string & msg) {
    // TODO: call log function so that msg would be printed to std::cout. use lambda
    log(msg, [=](std::string&){std::cout << msg << std::endl;});
}

int main(int argc, char const *argv[]) {
    std::string str1 = "error!";
    std::string str2 = "file!";
    std::string str3 = "info!";
    error(str1);
    file(str2);
    info(str3);
    return 0;
}