gcc -ansi -pedantic -Wall -Werror -c Scheduler_Tester.c
gcc -ansi -pedantic -Wall -Werror -c QueueProce.c
gcc -ansi -pedantic -Wall -Werror -c Processo.c
gcc -o Scheduler_Tester.exe Scheduler_Tester.o QueueProce.o Processo.o