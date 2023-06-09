
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
    const int n = 1000000;
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
    cout << "Сумма Asgl: " << sum_Asgl << "\n";

    //Сумма Adbl
    float sum_Adbl = 0;
    auto begin1 = std::chrono::steady_clock::now();
    for (int i = 0; i < Adbl.size(); i++) {
        sum_Adbl += Adbl[i];
    }
    auto end1 = std::chrono::steady_clock::now();
    auto elapsed_ms1 = std::chrono::duration_cast<std::chrono::milliseconds>(end1 - begin1);
    std::cout << "The time Adbl: " << elapsed_ms1.count() << " ms\n";
    cout << "Сумма Adbl: " << sum_Adbl << "\n";



    //параллельное суммирование Asgl
    float sum_Asgl_par = 0;
    auto begin_par = std::chrono::steady_clock::now();
    #pragma omp parallel
    {
        #pragma omp parallel for reduction(+:sum_Asgl_par)
        for (int i = 0; i < Asgl.size(); i++) {
            sum_Asgl_par += Asgl[i];
        }
    }
    auto end_par = std::chrono::steady_clock::now();
    auto elapsed_ms_par = std::chrono::duration_cast<std::chrono::milliseconds>(end_par - begin_par);
    std::cout << "The time Asgl parallel: " << elapsed_ms_par.count() << " ms\n";
    cout << "Сумма Asgl parallel: " << sum_Asgl_par << "\n";


    //Параллельное суммирование Adbl
    float sum_Adbl_par = 0;
    auto begin1_par = std::chrono::steady_clock::now();
    #pragma omp parallel
    {
        #pragma omp parallel for reduction(+:sum_Adbl_par)
        for (int i = 0; i < Adbl.size(); i++) {
            sum_Adbl_par += Adbl[i];
        }
    }
    auto end1_par = std::chrono::steady_clock::now();
    auto elapsed_ms1_par = std::chrono::duration_cast<std::chrono::milliseconds>(end1_par - begin1_par);
    std::cout << "The time Adbl parallel: " << elapsed_ms1_par.count() << " ms\n";
    cout << "Сумма Adbl parallel: " << sum_Adbl_par << "\n";
}