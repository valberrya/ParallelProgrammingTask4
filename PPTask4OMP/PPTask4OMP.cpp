﻿// OPENMP
// Не забыть включить в настройках поддержку OPENMP!
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <iostream>
#include <omp.h>

const int imageSize = 1900; //размер изображения в байтах
const int coefficient = 1;
const int maxThreads = 8;
const int threadStep = imageSize / maxThreads;

void change_bytes(int start, int end, unsigned char buffer[1900]) {
    for (int i = start; i > end; i -= threadStep) {
        buffer[i] += (i * coefficient) & 255;
    }
}

int main()
{
    FILE* original_file;
    FILE* new_file;
    unsigned char f[1900];

    original_file = fopen("C://Users//707//Downloads//corrupted_binary.jpg", "rb");
    fread(f, sizeof(f), 1, original_file);

    omp_set_num_threads(maxThreads);
#pragma omp parallel for

    for (int i = 1; i < maxThreads + 1; i++) { //идем по потокам
        int myid = omp_get_thread_num();
        std::cout << "Thread: " << myid << "\n";
        //первый поток расшифровывает от N-1, N-1-N/B, второй N-2, N-2-N/B, N-2-2*N/B...
        //N - размер файла, B = max_threads

        change_bytes(imageSize - i, 0, f);
    }

    new_file = fopen("C://Users//707//Downloads//changed_binary_opemp.jpg", "wb");
    fwrite(f, sizeof(f), 1, new_file);

    fclose(original_file);
    fclose(new_file);
}