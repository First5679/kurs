#include <iostream>
#include <cstdint>
#include <sys/socket.h>
#include <unistd.h>
#include <climits>
#include "server.h"

int Calculator::calc(int work_sock) {
    uint32_t Quantity;        // Количество векторов
    uint32_t Length;          // Длина текущего вектора
    uint64_t Number;          // Число из вектора
    uint64_t SumOfSquares;    // Сумма квадратов чисел вектора

    // Получаем количество векторов
    if (recv(work_sock, &Quantity, sizeof(Quantity), 0) <= 0) {
        std::cerr << "Ошибка при получении количества векторов" << std::endl;
        close(work_sock);
        return -1;
    }

    for (uint32_t j = 0; j < Quantity; j++) {
        // Получаем длину текущего вектора
        if (recv(work_sock, &Length, sizeof(Length), 0) <= 0) {
            std::cerr << "Ошибка при получении длины вектора" << std::endl;
            close(work_sock);
            return -1;
        }

        SumOfSquares = 0;

        // Обрабатываем каждый элемент вектора
        for (uint32_t i = 0; i < Length; i++) {
            // Получаем число из вектора
            if (recv(work_sock, &Number, sizeof(Number), 0) <= 0) {
                std::cerr << "Ошибка при получении числа" << std::endl;
                close(work_sock);
                return -1;
            }

            // Добавляем квадрат числа к сумме
            SumOfSquares += Number * Number;
        }

        // Отправляем результат клиенту
        if (send(work_sock, &SumOfSquares, sizeof(SumOfSquares), 0) <= 0) {
            std::cerr << "Ошибка при отправке результата" << std::endl;
            close(work_sock);
            return -1;
        }
    }

    std::cout << "Вычисления завершены" << std::endl;
    close(work_sock);
    return 0;
}
