# Домашнее задание №3: openmp

1. Исправить программы omp_bugreduction.c, omp_bugparfor.c из starting pack
2.  Написать параллельную программу, использующую метод Монте-Карло для оценки числа pi. Случайным образом (аккуратнее с генератором случайных чисел! Пример в лекции) кидаете точку в единичный квадрат. В этот же квадрат вписан круг. Если точка попала в круг, увеличиваете счетчик. Затем находите отношение точек, попавших в круг к общему числу точек. Зная площади квадрата и круга, находите приблизительно число pi. (

### Сборка:
<pre>
$ gcc -fopenmp maic.c
</pre>