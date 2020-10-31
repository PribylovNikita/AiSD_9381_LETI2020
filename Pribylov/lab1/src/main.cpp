#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

const char TASK[] = "Синтаксический анализатор понятия скобки:\n"
              "скобки ::= квадратные | круглые | фигурные\n"
              "квадратные ::= [круглые фигурные] | +\n"
              "круглые ::= (фигурные квадратные) | -\n"
              "фигурные ::= {квадратные круглые} | 0\n\n";
const char STOP[] = "STOP";

// Классификация результата анализатора
enum class Result {
    GOOD, BADVERYFIRSTCHAR,
    BADFIRSTCHARSQUARE, BADLASTCHARSQUARE,
    BADFIRSTCHARROUND, BADLASTCHARROUND,
    BADFIRSTCHARCURLY, BADLASTCHARCURLY,
    EXCESSCHAR
};

// Анализатор нескольких последовательностей символов,
// принимает поток infile (файловый или стандартный) для чтения строк
// и файловый поток outfile для записи результатов
void analyzer(std::istream& infile, std::ofstream& outfile);

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


void analyzer(std::istream& infile, std::ofstream& outfile)
{
    std::string line;
    while(getline(infile, line)) {
        if (line == STOP) return;
        if (!line.length()) continue;
        std::cout << "Проверяется: " << line << "\n";
        outfile   << "Проверяется: " << line << "\n";
        int pos = 0;
        Result k = analyzeLine(line, pos, outfile, 1);

        switch (k) {
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
    if (k == Result::GOOD && pos != line.length()) {
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

    // проверка подпоследовательности на соответствие понятию "круглые"
    k = round(line, pos, outfile, recLevel + 1);
    if (k != Result::GOOD) {
        return k;
    }
    // проверка подпоследовательности на соответствие понятию "фигурные"
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

    // проверка подпоследовательности на соответствие понятию "фигурные"
    k = curly(line, pos, outfile, recLevel + 1);
    if (k != Result::GOOD) {
        return k;
    }
    // проверка подпоследовательности на соответствие понятию "квадратные"
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

    // проверка подпоследовательности на соответствие понятию "квадратные"
    k = square(line, pos, outfile, recLevel + 1);
    if (k != Result::GOOD) {
        return k;
    }
    // проверка подпоследовательности на соответствие понятию "круглые"
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
    std::cout << std::string(recLevel, '\t') << "Глубина рекурсии " << std::setw(2) << recLevel << ": " << line.substr(0, pos) << "\n";
    outfile   << std::string(recLevel, '\t') << "Глубина рекурсии " << std::setw(2) << recLevel << ": " << line.substr(0, pos) << "\n";
}

int main()
{
    std::string inputFileName, logFileName;
    std::ifstream infile;
    std::ofstream outfile;

    std::cout << TASK;
    do {
        std::cout << "Для считывания данных с клавиатуры введите \"NUL\".\n"
                     "Для считывания данных с файла введите название файла: ";
        std::cin >> inputFileName;
        if (inputFileName == "NUL") break;
        infile.open(inputFileName);
        if (!infile) {
            std::cout << "Файла \"" << inputFileName << "\" не существует.\n";
        }
    } while (!infile);

    std::cout << "Введите название файла для записи промежуточных результатов: ";
    std::cin >> logFileName;
    outfile.open(logFileName);

    std::cout << "\nЧтение данных прекратится на строке \"" << STOP << "\".\n";
    if (inputFileName == "NUL") {
        std::cout << "Вводите данные:\n";
        analyzer(std::cin, outfile);
    } else {
        analyzer(infile, outfile);
    }

    if (infile.is_open()) {
        infile.close();
    }
    outfile.close();
    return 0;
}
