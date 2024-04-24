#include <iostream>
#include <iomanip>
#include "/usr/local/opt/libomp/include/omp.h"

using namespace std;

double calculatePi(double a, double b, int n) {
    double sum = 0.0;

#pragma omp parallel for reduction(+:sum)
    for (int i = 0; i < n; i++) {
        double x = a + (b - a) * i / n;
        double value = 4.0 / (1.0 + x * x);
        sum += value;
    }
    sum *= (b - a) / n;
    return sum;
}

int main() {
    setlocale(LC_ALL, "ru");
    cout << "[Вычисление числа PI по формуле средних прямоугольников]" << endl << endl;
    int n; // количество интегралов
    cout << "Введите число интегралов для получения числа PI (например, 10000): ";
    cin >> n;

    double start_time = omp_get_wtime();
    double pi = calculatePi(0.0, 1.0, n);
    double end_time = omp_get_wtime();
    
    cout << "Полученное число PI: " << setprecision(15) << fixed << pi << endl;
    cout << "Время выполнения: " << end_time - start_time << " секунд" << endl;

    return 0;
}