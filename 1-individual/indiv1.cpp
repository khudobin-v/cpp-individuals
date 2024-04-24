#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <mpi.h>

#define MAX_FILENAME_LENGTH 256
#define MAX_ELEMENTS 1000

using namespace std;

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Проверка количества аргументов
    if (argc < 2) {
        if (rank == 0) {
            cout << "Usage: mpirun -n <num_processes> " << argv[0] << " <file1> <file2> ... <fileN>" << endl;
        }
        MPI_Finalize();
        exit(1);
    }

    vector<string> input_filenames(argc - 1);
    vector<string> output_filenames(argc - 1);

    // Создание имён выходных файлов
    for (int i = 1; i < argc; i++) {
        input_filenames[i - 1] = argv[i];
        output_filenames[i - 1] = argv[i];
        output_filenames[i - 1].append("_res");
    }

    // Цикл обработки файлов
    for (int i = 0; i < argc - 1; i++) {
        ifstream input_file(input_filenames[i]);
        if (!input_file.is_open()) {
            if (rank == 0) {
                cerr << "Error opening file: " << input_filenames[i] << endl;
            }
            continue;
        }

        // Чтение данных из файла
        double elements[MAX_ELEMENTS];
        int num_elements = 0;
        double element;
        while (input_file >> element) {
            elements[num_elements++] = element;
        }
        input_file.close();

        // Обработка нулевых элементов
        vector<double> non_zero_elements;
        for (int j = 0; j < num_elements; j++) {
            if (elements[j] != 0) {
                non_zero_elements.push_back(elements[j]);
            }
        }

        // Запись данных в выходной файл
        ofstream output_file(output_filenames[i]);
        if (!output_file.is_open()) {
            if (rank == 0) {
                cerr << "Error opening file: " << output_filenames[i] << endl;
            }
            continue;
        }
        for (double non_zero_element : non_zero_elements) {
            output_file << non_zero_element << endl;
        }
        output_file.close();
    }

    MPI_Finalize();
    return 0;
}