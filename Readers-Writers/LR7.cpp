#include <iostream>
#include <windows.h>
#include <tchar.h>
#define NUMREADERS 5
#define NUMWRITERS 5
#define NUMREPEAT 5
#define TIMEDELAY 2

// Глобальные переменные
bool bWritersActive = false; // Флаг активности писателей
int counter = 1; // Счетчик

// Глобальные дескрипторы
HANDLE hSignalEvent; // Событие для сигнализации
HANDLE hWriterMutex; // Мьютекс для писателей
HANDLE hReaderMutex; // Мьютекс для читателей
CRITICAL_SECTION CriticalSection; // Критическая секция

// Функция для закрытия массива дескрипторов
void CloseHandleArr(HANDLE* arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        CloseHandle(arr[i]);
    }
}

// Потоковая функция для писателя
DWORD WINAPI writer(void* ptrID)
{
    int id = (int)ptrID;
    WaitForSingleObject(hSignalEvent, INFINITE);
    WaitForSingleObject(hWriterMutex, INFINITE);
    counter = counter * 2;
    std::cout << "Писатель #" << id << " изменил счетчик на  " << counter << std::endl;
    ReleaseMutex(hWriterMutex);
    SetEvent(hSignalEvent);
    return 0;
}

// Потоковая функция для читателя
DWORD WINAPI reader(void* ptrID)
{
    int id = (int)ptrID;
    static int readers = 0;
    WaitForSingleObject(hReaderMutex, INFINITE);
    readers++;
    if (readers == 1)
    {
        WaitForSingleObject(hSignalEvent, INFINITE);
    }
    ReleaseMutex(hReaderMutex);
    EnterCriticalSection(&CriticalSection);
    std::cout << "Читатель #" << id << " прочитал " << counter << std::endl;
    LeaveCriticalSection(&CriticalSection);
    WaitForSingleObject(hReaderMutex, INFINITE);
    readers--;
    if (readers == 0)
        SetEvent(hSignalEvent);
    ReleaseMutex(hReaderMutex);
    return 0;
}

// Функция для выполнения потоков писателей
DWORD WINAPI runWriters(void*)
{
    HANDLE writers[NUMWRITERS];
    bWritersActive = true;
    for (int i = 0; i < NUMWRITERS; i++)
    {
        Sleep(TIMEDELAY);
        writers[i] = CreateThread(NULL, 0, writer, (void*)i, NULL, 0);;
    }
    WaitForMultipleObjects(NUMWRITERS, writers, true, INFINITE);
    CloseHandleArr(writers, NUMWRITERS);
    return 0;
}

// Функция для выполнения потоков читателей
DWORD WINAPI runReaders(void*)
{
    HANDLE readers[NUMREADERS];
    while (!bWritersActive) // Предотвращение ситуации, когда читатели начинают чтение из пустого файла.
        Sleep(1);
    for (int i = 0; i < NUMREADERS; i++)
    {
        Sleep(TIMEDELAY);
        readers[i] = CreateThread(NULL, 0, reader, (void*)i, NULL, 0);
    }
    WaitForMultipleObjects(NUMREADERS, readers, true, INFINITE);
    CloseHandleArr(readers, NUMREADERS);
    return 0;
}

int main()
{
    setlocale(LC_ALL, "rus");
    hSignalEvent = CreateEvent(NULL, FALSE, TRUE, _T("SignalEvent"));
    hWriterMutex = CreateMutex(NULL, FALSE, _T("WriterMutex"));
    hReaderMutex = CreateMutex(NULL, FALSE, _T("ReaderMutex"));
    InitializeCriticalSection(&CriticalSection);
    for (int i = 0; i < NUMREPEAT; i++)
    {
        HANDLE RWThreads[] =
        {
            CreateThread(NULL, 0, runReaders, nullptr, 0, 0),
            CreateThread(NULL, 0, runWriters, nullptr, 0, 0)
        };
        WaitForMultipleObjects(2, RWThreads, true, INFINITE);
        std::cout << " " << i << " повтор\n" << std::endl;
        counter = 1;
        Sleep(TIMEDELAY);
        CloseHandleArr(RWThreads, 2);
    }
    DeleteCriticalSection(&CriticalSection);
    if (hReaderMutex != NULL)
        CloseHandle(hReaderMutex);
    if (hSignalEvent != NULL)
        CloseHandle(hSignalEvent);
    if (hWriterMutex != NULL)
        CloseHandle(hWriterMutex);

    return 0;
}
