# Лабораторная работа №1: Создание процессов

## Тема
"Создание процессов"

## Задача
Написать консольную программу `Main` и две утилиты (служебные программы) `Creator` и `Reporter`, которые выполняют следующие действия:

- Утилита `Creator` создает бинарный файл, имя которого и количество записей в котором получает через командную строку. Записи файла вводятся с консоли и имеют следующую структуру:
    ```c
    struct employee {
        int num;            // идентификационный номер сотрудника
        char name[10];      // имя сотрудника
        double hours;       // количество отработанных часов
    };
    ```
- Утилита `Reporter` создает текстовый файл, содержащий упорядоченный список сотрудников с начисленной заработной платой. Отчет формируется на основе бинарного файла, созданного программой `Creator`.

### Данные для утилиты Reporter через командную строку
1. Имя исходного бинарного файла.
2. Имя файла отчета.
3. Оплата за час работы.

### Отчет содержит следующие строки
1. Заголовок: Отчет по файлу "имя исходного бинарного файла".
2. Строка: Номер сотрудника, имя сотрудника, часы, зарплата.
3. Список сотрудников по строкам. Каждая строка содержит информацию из соответствующей записи бинарного файла, а последний столбец строки - начисленную зарплату = кол-ву отработанных часов * оплату за час работы.

## Программа Main
Выполняет следующие действия:
1. Запрашивает с консоли имя бинарного файла и количество записей в этом файле.
2. Запускает утилиту `Creator`, которой передает через командную строку имя создаваемого бинарного файла и количество записей в этом файле.
3. Ждет завершения работы утилиты `Creator`.
4. Выводит содержимое созданного бинарного файла на консоль.
5. Запрашивает с консоли:
    - Имя файла отчета.
    - Оплату за час работы.
6. Запускает утилиту `Reporter`, которой передает через командную строку:
    - Имя исходного бинарного файла.
    - Имя файла отчета.
    - Оплату за час работы.
7. Ждет завершения работы утилиты `Reporter`.
8. Выводит отчет на консоль.
9. Завершает свою работу.

## Примечание
Для ожидания завершения работы процессов `Creator` и `Reporter` использовать функцию:
```c
DWORD WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
```
Где второй параметр установить равным INFINITE, например:
```c
WaitForSingleObject(hCreator, INFINITE);
```
Здесь hCreator – дескриптор процесса Creator.