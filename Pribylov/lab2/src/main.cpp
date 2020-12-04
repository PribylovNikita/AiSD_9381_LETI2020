#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "HList.h"
#include "Logger.h"

char STOP[] = "STOP";

void printTask();
void initiate();
void performTask(std::istream& infile);

void printTask() {
    Logger::instance().log("Проверка идентичности двух иерархических списков.\n");
}

void initiate() {
    std::string inputFileName;
    std::ifstream infile;

    printTask();
    do {
        std::cout << "Для считывания данных с клавиатуры введите \"NUL\".\n"
                     "Для считывания данных с файла введите название файла: ";
        getline(std::cin, inputFileName);
        if (inputFileName == "NUL") break;
        infile.open(inputFileName);
        if (!infile) {
            std::cout << "Файла \"" << inputFileName << "\" не существует.\n";
        }
    } while (!infile);

    std::cout << "\nЧтение данных прекратится на строке \"" << STOP << "\".\n";
    if (inputFileName == "NUL") {
        std::cout << "Вводите данные:\n";
        performTask(std::cin);
    } else {
        performTask(infile);
    }

    if (infile.is_open()) {
        infile.close();
    }
}

void performTask(std::istream& infile)
{
    std::string first, second;

    while (!infile.eof()) {
        getline(infile, first);
        // удаляются все пробелы
        first.erase(std::remove(first.begin(), first.end(), ' '), first.end());
        if (!first.length()) continue;

        Logger::instance().log("Первый список: " + first);
        if (first == STOP) {
            Logger::instance().log("Встретилась терминальная строка.\n");
            return;
        }

        getline(infile, second);
        second.erase(std::remove(second.begin(), second.end(), ' '), second.end());
        if (!second.length()) continue;

        Logger::instance().log("Второй список: " + second);
        if (second == STOP) {
            Logger::instance().log("Встретилась терминальная строка.\n");
            return;
        }

        hlist::HList x(first), y(second);
        x == y ?
            Logger::instance().log("Списки идентичны.\n")
        :
            Logger::instance().log("Списки не идентичны.\n");
    }
}

int main() {
    initiate();
    return 0;
}
