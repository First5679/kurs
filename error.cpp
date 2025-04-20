#include "server.h"

void Error::errors(std::string error, std::string name)
{
    std::ofstream file;
    file.open(name, std::ios::app);
    if(file.is_open()) {
        time_t seconds = time(NULL);
        tm* timeinfo = localtime(&seconds);
        file<<error<<':'<<asctime(timeinfo)<<std::endl;
        std::cout << "error: " << error << std::endl;
    }
}
int Error::er(std::string file_name, std::string file_error)
{
    std::fstream file;
    file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    try {
        file.open(file_name);
        return 1;
    } catch(const std::exception & ex) {
        std::string error = "Ошибка открытия файла";
        errors(error, file_error);
        return 12;
    }
}

