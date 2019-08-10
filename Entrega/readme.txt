Entrega de TP Integrador Seminario De Lenguajes - Opcion C
Alumno Calderaro Guillermo
15 de Julio del 2019

El trabajo integrado presentado cumple con las funcionalidades de la primera y segunda parte de la entrega.
El mismo tambien resuelve la consigna del recuperatorio.
Incluyo el archivo "archivo.csv" que es el disponible en la web para realizar la prueba del programa.


Impresion de ayuda: 
	-el archivo imprime la ayuda cuando se ingresa alguno de los comandos correspondientes  abriendo un archivo "ayuda", 
leyendo e imprimiendo su contenido a salida standard. Dicha implementacion me parecio atractiva ya que me permitio reforzar los 
contenidos relacionados a archivos. El archivo "ayuda" debe incluirse en la misma carpeta que el programa principal. 
De no hacerlo se obtendra un error en tiempo de ejecucion.

Respecto a los .c y .h: 
	-los archivos my_getopt.c y .h realizan el parseo de los argumentos contenidos en argv. Para ello invocan 
a my_getopt_fun que NO es una funcion generica y fue creada para este proposito particular. 
my_getopt contiene todas las funciones para realizar el parseo de los argumentos. 
En caso de que el parseo se realice con exito la funcion abre los archivos de entrada y salida segun haya sido especificado. Si detecta alguna
falla cierra los que hubiera abierto y devuelve el tipo de error hallado.
	-los archivos muninipio.c y .h contienen todas las funcionalidades para solucionar las partes 1 y 2, asi como
el recuperatorio. En su interior se hallan 2 funciones, proceso_sin_municipio y proceso_con_municipio (ademas de varias funciones accesorias) 
que reciben dos FILE* y al ser invocadas desde el main realizan lo solicitado para el punto 1 y 2 respectivamente.
Ambas funciones no se encargan ni de abrir ni de cerrar los archivos en cuestion. Dicha accion debe realizarce 
desde la funcion que los invoque.

Manejo de errores.
	 En caso de obtener un error al intentar procesar una linea se han asumido soluciones distintas a fin de ejemplificar las 
distintas formas del manejo de errores. En el caso de que no se ingrese el flag -m (parte 1) se ha optado por imprimir el errror a stderr. 
El mismo puede ser redirigido desde la linea de comando. Se indica que ocurrio un error y, ademas, se indica que tipo de error ocurrio. 
En caso de que se ingrese el flag -m (parte2) se ha optado por ignorar el error y descartarlo ya que solo interesan los valores que no hayan 
arrojado error durante su procesamiento para la totalizacion de los municipios.
	En caso de obtener un error de la lectura de argumentos ingresados por linea de comandos se opto por realizar 
un return (CODIGO_DE_ERROR) y no un exit(CODIGO_DE_ERROR). Esto fue asi para permitir tomar acciones desde 
el main( si es que fuese necesario) luego de detectados los mismos. Asimismo el codigo de error es el valor de retorno 
del programa encaso de haber encontrado uno. 

	Los codigos de error son los siguientes -solo se documentaron dos nuevos tipos de error, el 64 y el 70-
64: Se ingresaron argumentos desconocidos.
65: Falta argumento requerido.
66: No se ingreso parametro para -l.
67: Se ingresaron argumentos extra.
68: No se pudo abrir el archivo de lectura.
69: No se pudo abrir/crear el archivo de escritura
70: El argumento -m|--municipio debe ser acompanado por -s|--salida
	Los siguientes codigos de error se corresponden con la resolucion del recuperatorio del trabajo.
99: Se ingreso -M sin -m|--municipio.
100: Se ingreso -M|--mayores cantidad   con un valor invalido de cantidad (<=0) o sin una cantidad.
101: Los argumentos se ingresaron correctamente pero no se pudo procesar el archivo sea porque habia menor cantidad de elementos totales 
para dar la salida adecuada o porque la cantidad de elementos mayores a N era menor a N.

Alocacion de memoria:
	para la resolucion de ambas partes se aloco memoria de manera dinamica. Todos los bloques de memoria fueron liberados
para la finalizacion del programa. La verificacion de este hecho se llevo a cabo con el uso de valgrind.
	valgrind --leak-check=yes ./main.exe <opciones de argumentos>
