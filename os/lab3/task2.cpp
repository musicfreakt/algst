#include <iomanip>
#include <iostream>
#include <windows.h>
#include <omp.h>

const size_t N = 100000000;
const size_t BLOCKSIZE = 9308060;

long double calculatePI(const unsigned threadNum, DWORD *millisec)
{
    DWORD begin;
    DWORD end;
    long double pi = 0;

    begin = GetTickCount();

    #pragma omp parallel shared(begin, end) reduction (+: pi) num_threads(threadNum)
    {
        #pragma omp for schedule(dynamic, BLOCKSIZE) nowait
        for (size_t i = 0; i < N; ++i)
        {
            long double x_i;
            x_i = (i + 0.5) * 1.0 / N;
            pi += (long double) 4.0 / (1.0 + x_i * x_i);
        }
    }

    pi /= (long double)N;

    end = GetTickCount();

    *millisec = end - begin;
    return pi;
}

void printArray(unsigned* a, size_t n)
{
    std::cout << "[";
    for(size_t i = 0; i < n; ++i)
    {
        std::cout << std::to_string(a[i]);
        if(i != n-1)
            std::cout << ", ";
    }
    std::cout << "];\n";
}

int main(int argc, char **argv)
{
    if(argc == 1) // Для замеров
    {
        const unsigned maxThreads = 32; // 72
        const unsigned attempts = 10;
        DWORD currentMillis;
        unsigned *results = new unsigned[maxThreads];
        unsigned *x = new unsigned[maxThreads];
        for(unsigned i = 1; i <= maxThreads; ++i)
        {
            DWORD averageMillis = 0;
            std::cout << "Number of threads: " << i << std::endl;
            for(unsigned j = 0; j < attempts; ++j)
            {
                std::cout << std::setprecision(5) << "Result of pi calculations (attempt " << (j+1) << "): " << calculatePI(i, &currentMillis) << std::endl;
                averageMillis += currentMillis;
            }
            averageMillis /= attempts;
            results[i-1] = averageMillis;
            x[i-1] = i;
            std::cout << "Time of pi calculations " << averageMillis << " millisec (" << (long double) averageMillis / 1000 << " sec)." << std::endl;
        }
        printArray(x, maxThreads);
        printArray(results, maxThreads);
        delete results;
        delete x;
    }
    else if(argc == 2) // для тестов
    {
        unsigned threadNum = atoi(argv[1]);
        DWORD millisec = -1;
        std::cout << "Number of threads: " << threadNum << std::endl;
        long double pi = calculatePI(threadNum, &millisec);
        std::cout << std::setprecision(N) << "Result of pi calculations: " << pi << std::endl;
        std::cout << "Time of pi calculations " << millisec << " millisec (" << (long double)millisec / 1000 << " sec)." << std::endl;
    }
    else
    {
        return -1;
    }
    return 0;
}
