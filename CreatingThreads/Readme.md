Лабораторная работа №1
Тема: «Создание потоков».
Общее задание:
Задача. Написать программу для консольного процесса, который состоит из двух потоков: main и worker.
a) Поток main должен выполнить следующие действия:
1. Создать массив целых чисел, размерность и элементы которого вводятся с консоли( или сгенерировать
случайно).
2. Ввести время для остановки и запуска потока worker.
3. Создать поток worker, передать в поток данные: размер маcсива, масив и т.д.
4. Приостановить поток worker(SuspendThread), затем через некоторое время снова запустить поток.
5. Уметь создавать поток фкнкциями _beginthreadex и CreateThread
6. Дождаться завершения потока worker.
7. Вывести на консоль результат работы потока worker
8. Завершить работу.
b) Глобальные переменные не использовать!
c) Розбраться с типами данных, которые используются
d) Обяснить: что такое идентификатор и дескриптор, работу функций.
Ввести новый элемент Х.Найти количество элементов массива, больших Х. Пред началом поиска «спать» 200
миллисекунд Завершить свою работу.