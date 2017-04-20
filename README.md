# EjemplosMPI
Ejemplos usando la biblioteca MPI en lenguaje C

Se recopilan algunos ejemplos sencillos de uso de la biblioteca [MPI](https://es.wikipedia.org/wiki/Interfaz_de_Paso_de_Mensajes). Estos ejemplos son tomados del libro *Beowulf Cluster Computing with Linux (Scientific and Engineering Computation)* de William Gropp y Ewing Lusk. Del capitulo Parallel Programming with MPI (8 ó 9 segun la edición) para ser exactos.
A diferencia de algunos ejemplos listados en el libro, los que aqui incluyo son funcionales y presentan varios cambios para adaptarlos a mis necesidades.

### Como usar
Para tener un entorno donde ejecutar estos ejemplos recomiendo crear un cluster usando maquinas virtuales. En mi caso use [Vagrant](https://www.vagrantup.com/) para crear el entorno facilmente, el Vagrantfile (la receta) que use lo pueden encontrar en la carpeta extra. 
Al ejecutar el comando: 
`$ vagrant up`
Se levantarán las maquinas virtuales y se instalaran las librerías necesarias para ejecutar los ejemplos (Mpich2 y Sprng). 
Esta idea me la copie de Matthew Rahtz que publico su receta [MPI Cluster Vagrantfile](https://github.com/mrahtz/mpi-vagrant), ahí se listan el resto de los comandos utiles y agregar mas nodos al cluster. La diferencia entre las recetas es que él uso OpenMPI en vez de MPICH, no instala automaticamente la biblioteca SPRNG (requerida para un ejemplo) y que yo creo la ruta "/home/mpi" en las maquinas virtuales para ejecutar mis programas. Sería muy interesante que alguien probará estos ejemplos usando su configuración para ver si son compatibles con OpenMPI, que es otra implementación de MPI distinta a la que uso.

Para compilar y crear el ejecutable de los programas, basta con hacer: 
`$ mpicc -o nombre_ejecutable programa.c`  

Y para ejecutar la aplicación:
`$ mpirun -np 5 --host node1,node2,node3 ./nombre_ejecutable`

Notese que se compila usando el compilador de c que usa la biblioteca mpi, y que ejecuto los programas usando mpirun: al que se le debe pasar el número de procesos a levantar (en el caso anterior 5) y los nodos (maquinas fisicas o virtuales) en que se va a ejecutar.

### Descripción de los programas
 - holabasico.c Un hola mundo muy sencillo, usalo para probar que tu ambiente tengas las librerías correctas.
 - holampi.c Otro hola mundo solo que este imprime el proceso actual y el total de procesos en ejecución. Aqui empiezas a notar el paralelismo de la biblioteca.
 - holampi_adv.c Muy parecido al anterior solo que los mensajes de cada proceso son enviados al proceso principal para que los imprima ordenadamente. Ejemplo del patrón manager/worker.
 - multmatrizxvector.c Calculo de matriz por vector. Ejemplo mas completo del manager/worker aunque no muy eficiente. 
 - calcular_pi.c Calcula el valor aprox de pi, al evaluar valores de una integral trigonometrica. Al ejecutar esta aplicacion se debe pasar por parámetros un número entero que indica en cuantos fragmentos se dividirá el rango de números a evaluar, mientras mas grande sea el número mas fragmentos se crearán. Aquí tambien se usan funciones para determinar cuantos segundos se tardan en ejecutar los calculos en paralelo. 
 - calcular_pi_azar.c Calcula el valor aprox de pi, usando [Integración de MonteCarlo](https://es.wikipedia.org/wiki/Integraci%C3%B3n_de_Montecarlo). Este programa usa la biblioteca [SPRNG](http://www.sprng.org/) para obtener números aleatorios. Se debe pasar como argumento cuantos lotes se quieren ejecutar para devolver los números aproximados. 
 
Para compilar el código fuente calcular_pi_azar.c se deben agregar nuevos argumentos para añadir la biblioteca de SPRNG, el comando quedaría asi:
`$ mpicc -o static_pi staticpi.c -I /usr/include/sprng/ -lsprng`

Siempre y cuando la ruta /usr/include/sprng/ exista, en todo caso se debe colocar la ruta donde se encuentre el archivo sprng.h.

