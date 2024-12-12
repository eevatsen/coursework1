/*
                    Курсова робота
Київський політехнічний інститут імені Ігоря Сікорського
      Інституту атомної та теплової енергетики
                      Кафедра ЦТЕ

Тема: "Розв’язання системи N*N лінійних рівнянь з N невідомими методом Гауса."

Виконав:
Студент 1 курсу, групи ТР-34
Луценко Іван Сергійович
*/

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#ifdef _WIN32
#include <Windows.h>
#endif
#include <limits>

using namespace std;

// Функція для друку матриці
void printMatrix(double** matrix, int dimension) {
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j <= dimension; ++j) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

// Функція для зчитування матриці з консолі
double** manualMatrixRead(int dimension) {
    cout << "Введіть матрицю системи лінійних рівнянь:\n";
    double** matrix = (double**)malloc(dimension * sizeof(double*));
    for (int i = 0; i < dimension; ++i) {
        matrix[i] = (double*)malloc((dimension + 1) * sizeof(double));
        for (int j = 0; j <= dimension; ++j) {
            cin >> matrix[i][j];
        }
    }
    return matrix;
}

// Функція для зчитування матриці з файлу
double** readMatrixFromFile(const string& filename, int dimension) {
    const string RED = "\033[31m";
    const string RESET = "\033[0m";

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << RED << "Помилка: не вдалося відкрити файл " << RESET << filename << endl;
        return nullptr;
    }
    double** matrix = (double**)malloc(dimension * sizeof(double*));
    for (int i = 0; i < dimension; ++i) {
        matrix[i] = (double*)malloc((dimension + 1) * sizeof(double));
        for (int j = 0; j <= dimension; ++j) {
            file >> matrix[i][j];
        }
    }
    file.close();
    return matrix;
}

// Функція для генерації випадкової матриці
double** generateRandomSystem(int dimension) {
    double** matrix = (double**)malloc(dimension * sizeof(double*));
    srand(time(nullptr));
    for (int i = 0; i < dimension; ++i) {
        matrix[i] = (double*)malloc((dimension + 1) * sizeof(double));
        for (int j = 0; j <= dimension; ++j) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
    return matrix;
}

// Функція для збереження результатів у файл
void saveResultsToFile(const string& filename, const double* solution, int dimension) {
    const string RED = "\033[31m";
    const string RESET = "\033[0m";

    ofstream file(filename);
    if (!file) {
        cerr << RED << "Помилка відкриття файлу для запису результатів!" << RESET << endl;
        exit(1);
    }

    for (int i = 0; i < dimension; ++i) {
        file << solution[i] << endl;
    }
    file.close();
}

// Функція для розв'язання системи рівнянь методом Гауса
double* gauss(double** matrix, int dimension, bool show_steps) {
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string RESET = "\033[0m";

    // Прямий хід
    for (int i = 0; i < dimension - 1; ++i) {
        if (show_steps) {
            cout << RED << "Крок " << i + 1 << ":" << endl << RESET;
        }
        for (int k = i + 1; k < dimension; ++k) {
            try {
                if (matrix[i][i] == 0)
                    throw runtime_error("Ділення на нуль!");
                double coeff = matrix[k][i] / matrix[i][i];
                if (show_steps) {
                    cout << GREEN << "Рядок " << k + 1 << " - (" << coeff << " * Рядок "
                        << i + 1 << "):" << endl << RESET;
                }
                for (int j = i; j < dimension + 1; ++j) {
                    matrix[k][j] -= coeff * matrix[i][j];
                }
                if (show_steps) {
                    printMatrix(matrix, dimension);
                    cout << endl;
                }
            }
            catch (const exception& e) {
                cerr << RED << "Помилка: " << e.what() << ". Продовжую обчислення." << RESET << endl;
                continue;
            }
        }
    }

    // Зворотній хід
    double* solution = (double*)malloc(dimension * sizeof(double));
    for (int i = dimension - 1; i >= 0; --i) {
        solution[i] = matrix[i][dimension];
        for (int j = i + 1; j < dimension; ++j) {
            solution[i] -= matrix[i][j] * solution[j];
        }
        try {
            if (matrix[i][i] == 0)
                throw runtime_error("Ділення на нуль!");
            solution[i] /= matrix[i][i];
        }
        catch (const exception& e) {
            cerr << RED << "Помилка: " << e.what() << ". Продовжую обчислення." << RESET << endl;
            solution[i] = numeric_limits<double>::quiet_NaN(); // Встановити значення NaN
        }
    }

    return solution;
}

// Функція для вибору способу введення матриці
int makeChoice() {
    int choice;
    cout << "Виберіть спосіб введення системи лінійних рівнянь:" << endl;
    cout << "1. Зчитати з файлу" << endl;
    cout << "2. Ввести вручну" << endl;
    cout << "3. Згенерувати випадково" << endl;
    cout << "Ваш вибір: ";

    while (true) {
        cin >> choice;
        if ((choice < 1) || (choice > 3))
            cerr << "\nНевірний ввід! Введіть правильну цифру (1-3): ";
        else
            break;
    }
    return choice;
}

// Функція для зчитування розміру матриці
int readDimension() {
    int dimension;
    cout << "Введіть розмір системи (N): ";

    while (true) {
        cin >> dimension;
        if ((dimension < 1) || (dimension > 1000))
            cerr << "\nНевірний ввід!\nРозмір матриці не має бути меншим за 0 і "
            "більшим за 1000.\nПовторіть спробу: ";
        else
            break;
    }
    return dimension;
}

// Функція для запиту, чи потрібно показувати кроки вирішення
bool askShowSteps() {
    string response;
    cout << "Бажаєте побачити кроки вирішення? (Yes/No): ";
    while (true) {
        cin >> response;
        cout << endl;
        if (response == "Yes" || response == "yes" || response == "Y" || response == "y") {
            return true;
        }
        else if (response == "No" || response == "no" || response == "N" || response == "n") {
            return false;
        }
        else {
            cout << "\nБудь ласка, введіть Yes або No: ";
        }
    }
}

// Головна функція
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(1251);
#endif

    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string YELLOW = "\033[33m";
    const string CYAN = "\033[36m";
    const string RESET = "\033[0m";

    cout << RED << "---------------------gauss---------------------\n\n" << RESET;

    while (true) {
        int dimension = 0;
        double** systemMatrix = nullptr;
        cout << YELLOW;
        int inputWay = makeChoice();
        cout << RESET;
        cout << endl;
        cout << CYAN;
        dimension = readDimension();
        cout << endl;
        bool show_steps = askShowSteps();

        // Зчитування матриці з файлу
        if (inputWay == 1) {
            string input_filename;

            while (true) {
                cout << "Введіть назву файлу, в якому знаходиться матриця системи: ";
                cin >> input_filename;
                systemMatrix = readMatrixFromFile(input_filename, dimension);
                if (systemMatrix != nullptr)
                    break;
                cout << endl;
            }

            // Введення матриці вручну
        }
        else if (inputWay == 2) {
            systemMatrix = manualMatrixRead(dimension);

            // Генерація випадкової матриці
        }
        else if (inputWay == 3) {
            systemMatrix = generateRandomSystem(dimension);
        }

        cout << "\nСистема лінійних рівнянь:" << endl;
        printMatrix(systemMatrix, dimension);
        cout << endl;
        cout << RESET;
        double* solution = gauss(systemMatrix, dimension, show_steps);

        // Вивід розв'язку
        cout << "Розв'язок:" << endl;
        for (int i = 0; i < dimension; ++i) {
            cout << GREEN << "x" << i + 1 << CYAN << " = " << solution[i] << endl << RESET;
        }

        string outputFilename;
        cout << "Зберігати результати у файл? (Yes/No): ";
        string saveToFile;

        // Запит на збереження результатів у файл
        while (true) {
            cin >> saveToFile;
            if ((saveToFile != "Yes") && (saveToFile != "No") && (saveToFile != "no") &&
                (saveToFile != "yes") && (saveToFile != "N") && (saveToFile != "Y") &&
                (saveToFile != "n") && (saveToFile != "y")) {
                cout << "\nБудь ласка, введіть Так або Ні: ";
            }
            else
                break;
        }

        // Збереження результатів у файл
        if ((saveToFile == "Yes") || (saveToFile == "yes") || (saveToFile == "Y") || (saveToFile == "y")) {
            cout << "Введіть ім'я файлу для збереження результатів: ";
            cin >> outputFilename;
            saveResultsToFile(outputFilename, solution, dimension);
        }
        else {
            string choice;
            cout << "Бажаєте продовжити? (Yes/No): ";
            cin >> choice;
            if (choice != "Yes" && choice != "yes" && choice != "Y" && choice != "y") {
                break;
            }
        }

        // Очищення пам'яті
        for (int i = 0; i < dimension; ++i) {
            free(systemMatrix[i]);
        }
        free(systemMatrix);
        free(solution);
    }

    return 0;
}
