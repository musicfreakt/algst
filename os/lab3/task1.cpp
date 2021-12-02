#include <iomanip>
#include <iostream>
#include <windows.h>

const size_t N = 100000000;
const size_t BLOCKSIZE = 9308060;

int currentPos = 0;
long double pi = 0.0;
LPCRITICAL_SECTION section = new CRITICAL_SECTION;

DWORD WINAPI calculatingFunc(LPVOID lpParam)
{
    int* first = (int*)lpParam;
    int end = *first + BLOCKSIZE;
    long double x = 0.0, sl = 0.0;

    do
    {
        sl = 0.0;
        for (int i = *first; (i < end) && (i < N); ++i)
        {
            x = (i + 0.5) / N;
            sl += (4 / (1 + x * x));
        }
        EnterCriticalSection(section);
        pi += sl;
        currentPos += BLOCKSIZE;
        *first = currentPos;
        LeaveCriticalSection(section);
        end = *first + BLOCKSIZE;
    }
    while (*first < N);

    return 0;
}


long double calculatePI (const unsigned threadNum, DWORD *millisec)
{
    DWORD begin;
    DWORD end;

    HANDLE* Threads = new HANDLE[threadNum];
    int* position = new int[threadNum];
    InitializeCriticalSection(section);

    for (int i = 0; i < threadNum; ++i)
    {
        position[i] = BLOCKSIZE * i;
        currentPos = position[i];
        Threads[i] = CreateThread(NULL, 0, calculatingFunc, &position[i], CREATE_SUSPENDED, NULL);
    }

    begin = clock();

    for (int i = 0; i < threadNum; ++i)
        ResumeThread(Threads[i]);

    WaitForMultipleObjects(threadNum, Threads, TRUE, INFINITE);

    pi /= (long double)N;

    end = clock();

    *millisec = end - begin;
    DeleteCriticalSection(section);
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
        const unsigned maxThreads = 32;
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
