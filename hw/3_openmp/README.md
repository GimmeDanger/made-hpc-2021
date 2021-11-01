# Домашнее задание №3: openmp

1. Исправить программы omp_bugreduction.c, omp_bugparfor.c из starting pack
2.  Написать параллельную программу, использующую метод Монте-Карло для оценки числа pi. Случайным образом (аккуратнее с генератором случайных чисел! Пример в лекции) кидаете точку в единичный квадрат. В этот же квадрат вписан круг. Если точка попала в круг, увеличиваете счетчик. Затем находите отношение точек, попавших в круг к общему числу точек. Зная площади квадрата и круга, находите приблизительно число pi.

### Тестирование метода Монте-Карло для оценки PI:
<pre>
$ gcc -fopenmp omp_monte_carlo_pi.c

$ export OMP_NUM_THREADS=1
$ ./a.out
N = 100000000 round of Monte-Carlo, elapsed time = 1685.546875 ms
Calculated PI = 3.141678
True PI = 3.141593
Diff = 0.000085

$ export OMP_NUM_THREADS=2
$ ./a.out
N = 100000000 round of Monte-Carlo, elapsed time = 845.703125 ms
Calculated PI = 3.141630
True PI = 3.141593
Diff = 0.000038

$ export OMP_NUM_THREADS=4
$ ./a.out
N = 100000000 round of Monte-Carlo, elapsed time = 501.953125 ms
Calculated PI = 3.141705
True PI = 3.141593
Diff = 0.000113

$ export OMP_NUM_THREADS=8
$ ./a.out
N = 100000000 round of Monte-Carlo, elapsed time = 333.984375 ms
Calculated PI = 3.141788
True PI = 3.141593
Diff = 0.000195
</pre>
