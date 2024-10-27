# Ejercicios de Programación con MPI

Este repositorio contiene soluciones en C++ para varios ejercicios de programación paralela utilizando la interfaz de paso de mensajes MPI. Cada ejercicio aborda una tarea de computación paralela, desde cálculos de histogramas hasta algoritmos de ordenamiento y estimación de constantes matemáticas. Los programas están diseñados para ejecutarse en sistemas que soporten MPI, como OpenMPI o MPICH.

## Ejercicios

### 3.1 Histograma Paralelo
Implementación de un programa paralelo para calcular un histograma. El proceso 0 lee el conjunto de datos y lo distribuye entre los procesos. Cada proceso calcula los conteos de los intervalos en sus datos locales, y el proceso 0 recopila y muestra el histograma final.


- **Ejecución:** `mpirun -np <n_procesos> ./histograma`

### 3.2 Estimación de π usando el Método de Monte Carlo
Este programa estima el valor de π lanzando "dardos" a una cuadrícula. Cada proceso genera coordenadas aleatorias dentro de un cuadrado y cuenta cuántos puntos caen dentro del círculo inscrito. El proceso 0 recopila los resultados de todos los procesos para calcular una estimación de π utilizando la proporción de puntos en el círculo respecto al total.


- **Ejecución:** `mpirun -np <n_procesos> ./estimacion_pi`

### 3.3 Suma Global en Estructura de Árbol
Implementación de una suma global usando una estructura de árbol binario. Los procesos calculan sus sumas locales y las comunican en pares, reduciendo el resultado final en el proceso 0. El programa permite manejar cualquier número de procesos, adaptando la estructura de comunicación cuando el número de procesos no es una potencia de dos.


- **Ejecución:** `mpirun -np <n_procesos> ./suma_global`

### 3.5 Multiplicación Matriz-Vector con Distribución por Bloques de Columnas
Programa que realiza la multiplicación de una matriz cuadrada y un vector mediante una distribución de bloques de columnas. El proceso 0 lee la matriz completa y distribuye las columnas entre los procesos. La función `MPI_Reduce_scatter` es utilizada para distribuir y recopilar los resultados parciales en cada proceso.


- **Ejecución:** `mpirun -np <n_procesos> ./matriz_vector`

### 3.8 Merge Sort Paralelo
Implementación de un algoritmo de ordenamiento por mezcla (Merge Sort) en paralelo. Cada proceso genera una lista aleatoria local y la ordena. Luego, en una estructura de árbol, cada proceso fusiona su lista ordenada con la de otros procesos hasta que el proceso 0 obtiene la lista completamente ordenada y la muestra.


- **Ejecución:** `mpirun -np <n_procesos> ./merge_sort_paralelo`

## Requisitos
- **MPI**: Se requiere una biblioteca MPI compatible (OpenMPI, MPICH) para compilar y ejecutar los programas.
- **Compilador de C++**: Los programas fueron escritos y probados en `mpic++`.


