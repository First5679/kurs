#include "server.h"

Error::Error()
{

}

void alarm_handler(int signal) {
    std::cout << "Время ожидания истекло\n";
    exit(EXIT_FAILURE);
}

int Server::self_addr(std::string& error, std::string& file_error, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }
    
    int on = 1;
    int rc = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (rc == -1) {
        throw std::system_error(errno, std::generic_category(), "Ошибка установки сокета");
    }
    
    signal(SIGALRM, alarm_handler);
    alarm(240);
    
    struct timeval timeout {240, 0};
    rc = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (rc == -1) {
        throw std::system_error(errno, std::generic_category(), "Ошибка установки сокета");
    }
    
    sockaddr_in self_addr;
    self_addr.sin_family = AF_INET;
    self_addr.sin_port = htons(port);
    self_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    std::cout << "Ожидание подключения клиента...\n";
    
    int b = bind(sock, reinterpret_cast<sockaddr*>(&self_addr), sizeof(self_addr));
    if (b == -1) {
        std::cout << "Ошибка привязки\n";
        error = "Ошибка";
        e_error.errors(error, file_error);
        return 1;
    }
    
    listen(sock, SOMAXCONN);
    
    return sock;
}

int Server::client_addr(int s, std::string& error, std::string& file_error) {
        sockaddr_in * client_addr = new sockaddr_in;
        socklen_t len = sizeof (sockaddr_in);
        
        int work_sock = accept(s, (sockaddr*)(client_addr), &len);
        if(work_sock == -1) {
            std::cout << "Ошибка\n";
            error = "Ошибка";
            e_error.errors(error, file_error);
            return 1;
        }
        else {
            std::cout << "Клиент успешно подключился!\n";
            return work_sock;
        }
    }
