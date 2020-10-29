#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

char TASK[] = "Синтаксический анализатор понятия скобки:\n"
              "скобки ::= квадратные | круглые | фигурные\n"
              "квадратные ::= [круглые фигурные] | +\n"
              "круглые ::= (фигурные квадратные) | -\n"
              "фигурные ::= {квадратные круглые} | 0\n\n";

// Классификация результата анализатора
enum class Result {
    GOOD, BADVERYFIRSTCHAR,
    BADFIRSTCHARSQUARE, BADLASTCHARSQUARE,
    BADFIRSTCHARROUND, BADLASTCHARROUND,
    BADFIRSTCHARCURLY, BADLASTCHARCURLY,
    EXCESSCHAR
};

// Анализатор нескольких последовательностей символов,
// принимает открытые файловые потоки:
// infile для чтения строк и outfile для записи результатов
void analyzer(std::ifstream& infile, std::ofstream& outfile);

// Анализирует последовательность символов,
// принимает строку line для проверки,
// позицию pos символа, с которого начинается проверка в строке,
// и уровень глубины рекурсии recLevel
Result analyzeLine(const std::string& line, int& pos, std::ofstream& outfile, const int recLevel = 1);

// Проверяет последовательность символов на соответствие понятию "скобки"
Result brackets(const std::string& line, int& pos, std::ofstream& outfile, const int recLevel);

// Следующие три функции проверяют последовательность символов на соответствие понятиям
// "квадратные", "круглые" и "фигурные" соответственно
Result square(const std::string& line, int& pos, std::ofstream& outfile, const int recLevel);
Result round(const std::string& line, int& pos, std::ofstream& outfile, const int recLevel);
Result curly(const std::string& line, int& pos, std::ofstream& outfile, const int recLevel);

// Выводит первые pos символов из line на консоль
// и в файл, связанный с потоком outfile,
// а так же глубину рекурсии recLevel
void printLog(const std::string& line, const int pos, std::ofstream& outfile, const int recLevel);


void analyzer(std::ifstream& infile, std::ofstream& outfile)
{
    std::string line;
    while(getline(infile,line)) {           // читает строки, пока не конец файла,
        if (!line.length()) continue;       // пропуская пустые
        std::cout << "Проверяется: " << line << "\n";
        outfile   << "Проверяется: " << line << "\n";
        int pos = 0;
        Result k = analyzeLine(line, pos, outfile, 1);

        switch (k) { //для обработки результата (верного и ошибочного)
            case Result::GOOD:
                std::cout << "ЭТО СКОБКИ!\n\n";
                outfile   << "ЭТО СКОБКИ!\n\n";
                break;
            case Result::BADVERYFIRSTCHAR:
                std::cout << "ЭТО НЕ СКОБКИ!\nСамый первый символ ошибочен.\n\n";
                outfile   << "ЭТО НЕ СКОБКИ!\nСамый первый символ ошибочен.\n\n";
                break;
            case Result::BADFIRSTCHARSQUARE:
                std::cout << "ЭТО НЕ СКОБКИ!\nОжидался символ \'[\' или \'+\'.\n\n";
                outfile   << "ЭТО НЕ СКОБКИ!\nОжидался символ \'[\' или \'+\'.\n\n";
                break;
            case Result::BADLASTCHARSQUARE:
                std::cout << "ЭТО НЕ СКОБКИ!\nОжидался символ \']\'.\n\n";
                outfile   << "ЭТО НЕ СКОБКИ!\nОжидался символ \']\'.\n\n";
                break;
            case Result::BADFIRSTCHARROUND:
                std::cout << "ЭТО НЕ СКОБКИ!\nОжидался символ \'(\' или \'-\'.\n\n";
                outfile   << "ЭТО НЕ СКОБКИ!\nОжидался символ \'(\' или \'-\'.\n\n";
                break;
            case Result::BADLASTCHARROUND:
                std::cout << "ЭТО НЕ СКОБКИ!\nОжидался символ \')\'.\n\n";
                outfile   << "ЭТО НЕ СКОБКИ!\nОжидался символ \')\'.\n\n";
                break;
            case Result::BADFIRSTCHARCURLY:
                std::cout << "ЭТО НЕ СКОБКИ!\nОжидался символ \'{\' или \'0\'.\n\n";
                outfile   << "ЭТО НЕ СКОБКИ!\nОжидался символ \'{\' или \'0\'.\n\n";
                break;
            case Result::BADLASTCHARCURLY:
                std::cout << "ЭТО НЕ СКОБКИ!\nОжидался символ \'}\'.\n\n";
                outfile   << "ЭТО НЕ СКОБКИ!\nОжидался символ \'}\'.\n\n";
                break;
            case Result::EXCESSCHAR:
                std::cout << "ЭТО НЕ СКОБКИ!\nВстретился лишний символ.\n\n";
                outfile   << "ЭТО НЕ СКОБКИ!\nВстретился лишний символ.\n\n";
                break;
            default:
                std::cout << "Неизвестная ошибка.\n\n";
                outfile   << "Неизвестная ошибка.\n\n";
        }
        line.clear();
    }
}

Result analyzeLine(const std::string& line, int& pos, std::ofstream& outfile, const int recLevel)
{
    Result k = brackets(line, pos, outfile, recLevel);
    if (k == Result::GOOD && pos != line.length()) { // проверка на отсутствие лишних символов на конце
        printLog(line, ++pos, outfile, 0);
        return Result::EXCESSCHAR;
    }
    return k;
}

Result brackets(const std::string& line, int& pos, std::ofstream& outfile, const int recLevel)
{
    char c; Result k;
    c = line[pos];
    if (c == '[' || c == '+') {
        k = square(line, pos, outfile, recLevel);
    } else if (c == '(' || c == '-') {
        k = round(line, pos, outfile, recLevel);
    } else if (c == '{' || c == '0') {
        k = curly(line, pos, outfile, recLevel);
    } else {
        printLog(line, ++pos, outfile, 0);
        k = Result::BADVERYFIRSTCHAR;
    }
    return k;
}

Result square(const std::string& line, int& pos, std::ofstream& outfile, const int recLevel)
{
    char c; Result k;
    c = line[pos++];
    printLog(line, pos, outfile, recLevel);

    if (c == '+') {
        return Result::GOOD;
    }
    if (c != '[') {
        return Result::BADFIRSTCHARSQUARE;
    }

    k = round(line, pos, outfile, recLevel + 1);
    if (k != Result::GOOD) {
        return k;
    }
    k = curly(line, pos, outfile, recLevel + 1);
    if (k != Result::GOOD) {
        return k;
    }

    c = line[pos++];
    printLog(line, pos, outfile, recLevel);
    if (c != ']') {
        return Result::BADLASTCHARSQUARE;
    }

    return Result::GOOD;
}

Result round(const std::string& line, int& pos, std::ofstream& outfile, const int recLevel)
{
    char c; Result k;
    c = line[pos++];
    printLog(line, pos, outfile, recLevel);

    if (c == '-') {
        return Result::GOOD;
    }
    if (c != '(') {
        return Result::BADFIRSTCHARROUND;
    }

    k = curly(line, pos, outfile, recLevel + 1);
    if (k != Result::GOOD) {
        return k;
    }
    k = square(line, pos, outfile, recLevel + 1);
    if (k != Result::GOOD) {
        return k;
    }

    c = line[pos++];
    printLog(line, pos, outfile, recLevel);
    if (c != ')') {
        return Result::BADLASTCHARROUND;
    }

    return Result::GOOD;
}

Result curly(const std::string& line, int& pos, std::ofstream& outfile, const int recLevel)
{
    char c; Result k;
    c = line[pos++];
    printLog(line, pos, outfile, recLevel);

    if (c == '0') {
        return Result::GOOD;
    }
    if (c != '{') {
        return Result::BADFIRSTCHARCURLY;
    }

    k = square(line, pos, outfile, recLevel + 1);
    if (k != Result::GOOD) {
        return k;
    }
    k = round(line, pos, outfile, recLevel + 1);
    if (k != Result::GOOD) {
        return k;
    }

    c = line[pos++];
    printLog(line, pos, outfile, recLevel);
    if (c != '}') {
        return Result::BADLASTCHARCURLY;
    }

    return Result::GOOD;
}


void printLog(const std::string& line, const int pos, std::ofstream& outfile, const int recLevel)
{
    std::cout << std::string(recLevel, '\t') <<"Глубина рекурсии " << std::setw(2) << recLevel << ": " << line.substr(0, pos) << "\n";
    outfile   << std::string(recLevel, '\t') <<"Глубина рекурсии " << std::setw(2) << recLevel << ": " << line.substr(0, pos) << "\n";
}

int main()
{
    std::string readFileName, logFileName;
    std::ifstream infile;  // поток для чтения
    std::ofstream outfile; // поток для вывода

    std::cout << TASK; // вывод задания на экран
    do {               // проверка на корректность введенного имени файла
        std::cout << "Введите название файла для считывания данных: ";
        getline(std::cin, readFileName);
        infile.open(readFileName);
        if (!infile) {
            std::cout << "Файла \"" << readFileName << "\" не существует.\n";
        }
    } while (!infile);

    std::cout << "Введите название файла для записи промежуточных результатов: ";
    getline(std::cin, logFileName);
    outfile.open(logFileName);

    analyzer(infile, outfile);

    infile.close();
    outfile.close();
    return 0;
}
