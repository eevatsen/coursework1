/*
                    ������� ������
�������� ����������� �������� ���� ����� ѳ���������
      ��������� ������ �� ������� ����������
                      ������� ���

����: "���������� ������� N*N ������ ������ � N ��������� ������� �����."

�������:
������� 1 �����, ����� ��-34
������� ���� ���������
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

// ������� ��� ����� �������
void printMatrix(double** matrix, int dimension) {
    for (int i = 0; i < dimension; ++i) {
        for (int j = 0; j <= dimension; ++j) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

// ������� ��� ���������� ������� � ������
double** manualMatrixRead(int dimension) {
    cout << "������ ������� ������� ������ ������:\n";
    double** matrix = (double**)malloc(dimension * sizeof(double*));
    for (int i = 0; i < dimension; ++i) {
        matrix[i] = (double*)malloc((dimension + 1) * sizeof(double));
        for (int j = 0; j <= dimension; ++j) {
            cin >> matrix[i][j];
        }
    }
    return matrix;
}

// ������� ��� ���������� ������� � �����
double** readMatrixFromFile(const string& filename, int dimension) {
    const string RED = "\033[31m";
    const string RESET = "\033[0m";

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << RED << "�������: �� ������� ������� ���� " << RESET << filename << endl;
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

// ������� ��� ��������� ��������� �������
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

// ������� ��� ���������� ���������� � ����
void saveResultsToFile(const string& filename, const double* solution, int dimension) {
    const string RED = "\033[31m";
    const string RESET = "\033[0m";

    ofstream file(filename);
    if (!file) {
        cerr << RED << "������� �������� ����� ��� ������ ����������!" << RESET << endl;
        exit(1);
    }

    for (int i = 0; i < dimension; ++i) {
        file << solution[i] << endl;
    }
    file.close();
}

// ������� ��� ����'������ ������� ������ ������� �����
double* gauss(double** matrix, int dimension, bool show_steps) {
    const string RED = "\033[31m";
    const string GREEN = "\033[32m";
    const string RESET = "\033[0m";

    // ������ ���
    for (int i = 0; i < dimension - 1; ++i) {
        if (show_steps) {
            cout << RED << "���� " << i + 1 << ":" << endl << RESET;
        }
        for (int k = i + 1; k < dimension; ++k) {
            try {
                if (matrix[i][i] == 0)
                    throw runtime_error("ĳ����� �� ����!");
                double coeff = matrix[k][i] / matrix[i][i];
                if (show_steps) {
                    cout << GREEN << "����� " << k + 1 << " - (" << coeff << " * ����� "
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
                cerr << RED << "�������: " << e.what() << ". ��������� ����������." << RESET << endl;
                continue;
            }
        }
    }

    // �������� ���
    double* solution = (double*)malloc(dimension * sizeof(double));
    for (int i = dimension - 1; i >= 0; --i) {
        solution[i] = matrix[i][dimension];
        for (int j = i + 1; j < dimension; ++j) {
            solution[i] -= matrix[i][j] * solution[j];
        }
        try {
            if (matrix[i][i] == 0)
                throw runtime_error("ĳ����� �� ����!");
            solution[i] /= matrix[i][i];
        }
        catch (const exception& e) {
            cerr << RED << "�������: " << e.what() << ". ��������� ����������." << RESET << endl;
            solution[i] = numeric_limits<double>::quiet_NaN(); // ���������� �������� NaN
        }
    }

    return solution;
}

// ������� ��� ������ ������� �������� �������
int makeChoice() {
    int choice;
    cout << "������� ����� �������� ������� ������ ������:" << endl;
    cout << "1. ������� � �����" << endl;
    cout << "2. ������ ������" << endl;
    cout << "3. ����������� ���������" << endl;
    cout << "��� ����: ";

    while (true) {
        cin >> choice;
        if ((choice < 1) || (choice > 3))
            cerr << "\n������� ���! ������ ��������� ����� (1-3): ";
        else
            break;
    }
    return choice;
}

// ������� ��� ���������� ������ �������
int readDimension() {
    int dimension;
    cout << "������ ����� ������� (N): ";

    while (true) {
        cin >> dimension;
        if ((dimension < 1) || (dimension > 1000))
            cerr << "\n������� ���!\n����� ������� �� �� ���� ������ �� 0 � "
            "������ �� 1000.\n�������� ������: ";
        else
            break;
    }
    return dimension;
}

// ������� ��� ������, �� ������� ���������� ����� ��������
bool askShowSteps() {
    string response;
    cout << "������ �������� ����� ��������? (Yes/No): ";
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
            cout << "\n���� �����, ������ Yes ��� No: ";
        }
    }
}

// ������� �������
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

        // ���������� ������� � �����
        if (inputWay == 1) {
            string input_filename;

            while (true) {
                cout << "������ ����� �����, � ����� ����������� ������� �������: ";
                cin >> input_filename;
                systemMatrix = readMatrixFromFile(input_filename, dimension);
                if (systemMatrix != nullptr)
                    break;
                cout << endl;
            }

            // �������� ������� ������
        }
        else if (inputWay == 2) {
            systemMatrix = manualMatrixRead(dimension);

            // ��������� ��������� �������
        }
        else if (inputWay == 3) {
            systemMatrix = generateRandomSystem(dimension);
        }

        cout << "\n������� ������ ������:" << endl;
        printMatrix(systemMatrix, dimension);
        cout << endl;
        cout << RESET;
        double* solution = gauss(systemMatrix, dimension, show_steps);

        // ���� ����'����
        cout << "����'����:" << endl;
        for (int i = 0; i < dimension; ++i) {
            cout << GREEN << "x" << i + 1 << CYAN << " = " << solution[i] << endl << RESET;
        }

        string outputFilename;
        cout << "�������� ���������� � ����? (Yes/No): ";
        string saveToFile;

        // ����� �� ���������� ���������� � ����
        while (true) {
            cin >> saveToFile;
            if ((saveToFile != "Yes") && (saveToFile != "No") && (saveToFile != "no") &&
                (saveToFile != "yes") && (saveToFile != "N") && (saveToFile != "Y") &&
                (saveToFile != "n") && (saveToFile != "y")) {
                cout << "\n���� �����, ������ ��� ��� ͳ: ";
            }
            else
                break;
        }

        // ���������� ���������� � ����
        if ((saveToFile == "Yes") || (saveToFile == "yes") || (saveToFile == "Y") || (saveToFile == "y")) {
            cout << "������ ��'� ����� ��� ���������� ����������: ";
            cin >> outputFilename;
            saveResultsToFile(outputFilename, solution, dimension);
        }
        else {
            string choice;
            cout << "������ ����������? (Yes/No): ";
            cin >> choice;
            if (choice != "Yes" && choice != "yes" && choice != "Y" && choice != "y") {
                break;
            }
        }

        // �������� ���'��
        for (int i = 0; i < dimension; ++i) {
            free(systemMatrix[i]);
        }
        free(systemMatrix);
        free(solution);
    }

    return 0;
}
