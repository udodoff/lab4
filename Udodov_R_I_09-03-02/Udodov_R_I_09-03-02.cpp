
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <vector>
#include <chrono>
using namespace std;

double random(double min, double max)
{
    return (double)(rand()) / RAND_MAX * (max - min) + min;
}

int main()
{
    omp_set_num_threads(1);
    setlocale(LC_ALL, "Russian");
    std::cout << omp_get_num_procs() << "\n";
    const int n = 5000000;
    vector <float> Asgl;
    vector <double> Adbl;
    for (int i = 0; i < n; i++) {
        Asgl.push_back(random(0, 200));
        Adbl.push_back(random(0, 200));
    }

    // Сумма Asgl
    float sum_Asgl = 0;
    auto begin = std::chrono::steady_clock::now();
    for (int i = 0; i < Asgl.size(); i++) {
        sum_Asgl += Asgl[i];
    }
    auto end = std::chrono::steady_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
    std::cout << "The time Asgl: " << elapsed_ms.count() << " ms\n";
    std::cout << "Сумма Asgl: " << sum_Asgl << "\n";

    //Сумма Adbl
    float sum_Adbl = 0;
    auto begin1 = std::chrono::steady_clock::now();
    for (int i = 0; i < Adbl.size(); i++) {
        sum_Adbl += Adbl[i];
    }
    auto end1 = std::chrono::steady_clock::now();
    auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - begin1);
    std::cout << "The time Adbl: " << elapsed_ms1.count() << " ms\n";
    std::cout << "Сумма Adbl: " << sum_Adbl << "\n";



    //параллельное суммирование Asgl
    float sum_Asgl_par = 0;
    auto begin_par = std::chrono::steady_clock::now();
    #pragma omp parallel for reduction(+ : sum_Asgl_par)
    for (int i = 0; i < Asgl.size(); i++) {
        sum_Asgl_par += Asgl[i];
    }
    auto end_par = std::chrono::steady_clock::now();
    auto elapsed_ms_par = std::chrono::duration_cast<std::chrono::milliseconds>(end_par - begin_par);
    std::cout << "The time Asgl parallel: " << elapsed_ms_par.count() << " ms\n";
    std::cout << "Сумма Asgl parallel: " << sum_Asgl_par << "\n";


    //Параллельное суммирование Adbl
    float sum_Adbl_par = 0;
    auto begin1_par = std::chrono::steady_clock::now();
    #pragma omp parallel for reduction(+ : sum_Adbl_par)
    for (int i = 0; i < Adbl.size(); i++) {
        sum_Adbl_par += Adbl[i];
    }
    auto end1_par = std::chrono::steady_clock::now();
    auto elapsed_ms1_par = std::chrono::duration_cast<std::chrono::milliseconds>(end1_par - begin1_par);
    std::cout << "The time Adbl parallel: " << elapsed_ms1_par.count() << " ms\n";
    std::cout << "Сумма Adbl parallel: " << sum_Adbl_par << "\n";




    //Умножение матриц
    vector <vector<int>> A;
    vector <vector<int>> B;
    vector <vector<int>> C = { {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0} };
    vector <vector<int>> C1 = {{0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}, {0,0,0,0,0,0}};

    //заполняем
    for (int i = 0; i < 6; i++) {
        vector<int> v;
        for (int j = 0; j < 6; j++) {
            v.push_back(i + 1);
        }
        A.push_back(v);
        B.push_back(v);
    }
    //выводим
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            std::cout << A[i][j] << " ";
        }
        std::cout << "\n";
    }
    //перемножаем
    auto begin2 = std::chrono::steady_clock::now();
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 6; col++) {
            for (int inner = 0; inner < 6; inner++) {
                C[row][col] += A[row][inner] * B[inner][col];
            }
            std::cout << C[row][col] << "  ";
        }
        std::cout << "\n";
    }
    auto end2 = std::chrono::steady_clock::now();
    auto elapsed_ms2 = std::chrono::duration_cast<std::chrono::milliseconds>(end2 - begin2);
    std::cout << "The time multiply matrix: " << elapsed_ms2.count() << " ms\n";

    //перемножаем параллельно
    int aMatrix[6][6] = { {1, 4, 1, 4, 5, 2}, {2, 5, 2, 5, 6, 3}, {3, 6, 3, 6, 7, 4}, {4, 7, 4, 7, 8, 5}, {5, 8, 5, 8, 9, 6}, {6, 9, 6, 9, 10, 7} };
    int bMatrix[6][6] = { {1, 4, 1, 4, 5, 2}, {2, 5, 2, 5, 6, 3}, {3, 6, 3, 6, 7, 4}, {4, 7, 4, 7, 8, 5}, {5, 8, 5, 8, 9, 6}, {6, 9, 6, 9, 10, 7} };
    int product[6][6] = { {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0} };

    auto begin2_par = std::chrono::steady_clock::now();
    #pragma omp parallel for
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 6; col++) {
            for (int inner = 0; inner < 6; inner++) {
                product[row][col] += aMatrix[row][inner] * bMatrix[inner][col];
            }
            std::cout << product[row][col] << "  ";
        }
        std::cout << "\n";
    }
    auto end2_par = std::chrono::steady_clock::now();
    auto elapsed_ms2_par = std::chrono::duration_cast<std::chrono::milliseconds>(end2_par - begin2_par);
    std::cout << "The time multiply matrix parallel: " << elapsed_ms2_par.count() << " ms\n";


    /*
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 6; col++) {
            #pragma omp parallel for reduction(+:C1[:row][:col])
            for (int inner = 0; inner < 6; inner++) {
                C1[row][col] += A[row][inner] * B[inner][col];
            }
            std::cout << C1[row][col] << "  ";
        }
        std::cout << "\n";
    }
    */
}