#include "server.h"

void Authorized::msgsend(int work_sock, const std::string& mess){
    char *buffer = new char[4096];
    std::strcpy(buffer, mess.c_str());
    send(work_sock, buffer, mess.length(), 0);
}

std::string Authorized::MD(const std::string& sah){
Weak::MD5 hash;
    std::string digest;
    StringSource(sah, true,  new HashFilter(hash, new HexEncoder(new StringSink(digest))));
      return digest;
 }
 
std::string Authorized::salt_generator(const std::size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string salt;
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_int_distribution<int> dist(0, sizeof(charset) - 2);

    for (std::size_t i = 0; i < length; i++) {
        salt += charset[dist(rng)];
    }

    return salt;
}

int Authorized::authorized(int work_sock, std::string file_name, std::string file_error)
{    std::string ok = "OK";
    std::string err = "ERR";
    std::string error;
    char msg[255];

    recv(work_sock, &msg, sizeof(msg), 0);
    std::string message = msg;
    std::string login, hashq;
    std::fstream file;
    file.open(file_name);
    getline (file, login, ':');
    getline (file, hashq);

    if(message != login){
        msgsend(work_sock,  err);
        error = "Ошибка логина";
        e_error.errors(error, file_error);
        close(work_sock);
        return 1;
    }else{

        std::string salt = salt_generator(16); 
        msgsend(work_sock,  salt);
        recv(work_sock, msg, sizeof(msg), 0);
        std::string sah = salt + hashq;
        std::string digest;
        digest = MD(sah);

        if(digest != msg){
            std::cout << digest << std::endl;
            std::cout << msg << std::endl;
            msgsend(work_sock,  err);
            error = "Ошибка пароля";
            e_error.errors(error, file_error);
            close(work_sock);
            return 1;
        }else{
            msgsend(work_sock,  ok);
        }
    }
    return 1;
}
