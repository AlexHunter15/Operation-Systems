Написать программы для консольного процесса Boss и консольных процессов Parent, Child.  Для моделирования передачи сообщений ввести специальные события(c автоматическим сбросом), которые обозначают «А» , «B», «C» , «D»      и конец сеанса для процессов Parent и Child. 
Принимать сообщение можно только от двух АКТИВНЫХ процессов Child(использовать семафор) и одного АКТИВНОГО процесса Parent(использовать мьютекс), передача остальных сообщений от других процессов должна временно блокироваться (находиться в режиме ожидания).
Процесс Boss:
Инициализация объектов синхронизации;
запрашивает у пользователя количество процессов Parent и количество процессов Child, которые он должен запустить; 
запрашивает кол-во сообщений, принятых от Parent или Child
запускает заданное количество процессов Parent, Child;
принимает от каждого процесса Parent, Child сообщения, выводит и кто его отправил  на консоль в одной строке. 
завершает свою работу.

Процесс Parent: 
синхронизировать работу процессов Parent с помощью мьютекса
после захвата мьютекса выводить сообщение : мьютекс захвачен
передачу сообщений реализовать с помощью событий
запрашивает с консоли сообщения, состоящее «А» или «B» и передает их (по одному) процессу Boss;
завершает свою работу.

Процесс Child:
синхронизировать работу процессов Child с помощью семафора
после захвата мьютекса выводить сообщение : семафор захвачен
передачу сообщений реализовать с помощью событий
 запрашивает с консоли сообщения, состоящее  «C»  или «D»   и передает их (по одному) процессу Boss;
завершает свою работу.