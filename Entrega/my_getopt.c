#include "my_getopt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************************************************
my_getopt_func NO es una funcion generica como el resto de la familia getopt. Fue ideada para este caso particular.
esta funcion recibe por referencia el flag correspondiente al municipio y lo modifica en 1 si se ingreso la opcion por linea de comando. 
valor de retorno: devuelve 0 en caso de que los argumentos se hallan procesado sin problema. 
En caso contrario devuelve el numero de error correspondiente. Tambien recib 
por referencia los FILE *. En caso de que no detecte errores en el parseo de argumentos my_getopt_func 
devuelve dos streams  listos para ser usados. 
Los mismos deben cerrarse luegod e su uso. 
En caso contrario cierra los que hubiese abierto y devuelve el numero de error detectado.
Se podria considerar reemplazar los return(CODIGO DE ERROR) por exit (COSIGO DE ERROR) aunque eso impediria tomar otras acciones
desde el programa principal.
Si se ingresase la opcion -M|--mayores con alguna de sus alternativas validas la funcion retornaria el valor ingresado como 
argumento acompaniante en la variable numero, pasada por referencia.
Asimismo, dado que devuelve el codigo de error correspondiente la impresiones de los mensajes de error podria realizarce fuera de my_getopt_func, desde el main.
NOTA: solo se imprime la ayuda si se ingresa como parametro -h (o sus equivalentes) o una convinacion valida de argumentos seguida o 
precedida por -h (o sus equivalentes). Si se ingresa -h con alguna combinacion ilegal o faltante de argumentos se informara el error pero no la ayuda. 
Asi se interpreto el primer item de la guia "Modo de procesar los parametros"(pag 1)
 ****************************************************/

int my_getopt_func(int* flag_m, FILE** in ,FILE** out, int argc, char** argv, int* numero){
	int i, flag_h, flag_l,flag_may, flag_s,flag_def,return_atoi, abri_archivo_escritura, abri_archivo_lectura;
    	flag_h = flag_l= flag_may = flag_s = flag_def= return_atoi = abri_archivo_escritura = abri_archivo_lectura = 0;
	i = 1;

	while ( i < argc) {		//recorro argv procesando cada uno de sus elementos
		if (es_ayuda(argv[i]))  {
			if (flag_h == 1){
				if (abri_archivo_lectura)
				{
					fclose(*in);
				}
				if (abri_archivo_escritura)
				{
					fclose(*out);
				}
	
				fprintf(stderr,"ERROR 67: Se ingresaron argumentos extra\n");
				return(67);
			}else {
				flag_h ++;
			}
		}else if (es_leer(argv[i]) )   {	//en caso de que se ingrese -l devo ademas verificar el nombre ingresado a cont. de ahi el segundo if
			if (flag_l == 1){
				if (abri_archivo_lectura)
				{
					fclose(*in);
				}
				if (abri_archivo_escritura)
				{
					fclose(*out);
				}
				fprintf(stderr,"ERROR 67: Se ingresaron argumentos extra\n");
				return(67);
			}else {
				flag_l ++;
			}

			if ((argc <= (i + 1)) || (es_arg(argv[i+1][0]) )){	//si '-l' no se sigue de nada o se sigue de un argumento ERROR 66
				fprintf(stderr,"ERROR 66: No se ingreso un parametro para -l.\n");
				return(66);
			}else {		//else captura los casos en que hay otro valor siguiendo a -l y este no es un argumento, o sea, el nombre del archivo a abrir
				i++;
				*in = fopen( argv[i] , "rb");
				if ( (*in) == NULL) {
					fprintf(stderr,"ERROR 68: No se pudo abrir/crear el archivo de lectura %s.\n", argv[i]);
					if (abri_archivo_escritura){
						fclose(*out);
					}
					return(68);
				}else {
					abri_archivo_lectura = 1;
				}
			}
			

		}else if  (es_salida(argv[i]))   {	//en caso de que se ingrese -s verifico que la sig posic de argv sea el nombre del archivo. de ahi el segundo if
			if (flag_s == 1){
				if (abri_archivo_lectura)
				{
					fclose(*in);
				}
				if (abri_archivo_escritura)
				{
					fclose(*out);
				}
				fprintf(stderr,"ERROR 67: Se ingresaron argumentos extra\n");
				return(67);
			}else {
				flag_s ++;
			}
			if (!( (argc <= (i +1)) || (es_arg(argv[i+1][0])) ) ) {		//verifico el sig. argumento ingresado
				i++;
				*out = fopen(argv[i], "wb+");
				if ( out == NULL){
					fprintf(stderr,"ERROR 69: No se pudo abrir el archivo %s para escritura.\n", argv[i]);
					if (abri_archivo_lectura){
						fclose(*in);
					}
					return(69);
				}else {
					abri_archivo_escritura = 1;
				}
			}
		}else if (es_municipio(argv[i])) {
			if (*flag_m == 1){
				if (abri_archivo_lectura)
				{
					fclose(*in);
				}
				if (abri_archivo_escritura)
				{
					fclose(*out);
				}
				fprintf(stderr,"ERROR 67: Se ingresaron argumentos extra\n");
				return(67);
			}else {
				(*flag_m) ++;
			}

		}else if(es_mayores(argv[i])){
			if(flag_may==1){
				if (abri_archivo_lectura)
				{
					fclose(*in);
				}
				if (abri_archivo_escritura)
				{
					fclose(*out);
				}
				fprintf(stderr,"ERROR 67: Se ingresaron elementos extra\n");
				return(67);
			}else{
				flag_may ++;
			}
			if (!( (argc <= (i +1))))  {
				return_atoi = atoi(argv[i+1]);
				if (return_atoi > 0)
				{
					(*numero)= atoi(argv[i+1]);
					i++;
				}else {
					if (abri_archivo_lectura)
					{
						fclose(*in);
					}
					if (abri_archivo_escritura)
					{
						fclose(*out);
					}
					fprintf(stderr,"ERROR 100: Valor incorrecto o faltante para argumento --mayores\n");
					return(100);
				}	

			}else {
				if (abri_archivo_lectura)
				{
					fclose(*in);
				}
				if (abri_archivo_escritura)
				{
					fclose(*out);
				}
				fprintf(stderr,"ERROR 100: Valor incorrecto o faltante para argumento --mayores\n");
				return(100);
			}	

		}else {
			if (abri_archivo_lectura)
			{
				fclose(*in);
			}
			if (abri_archivo_escritura)
			{
				fclose(*out);
			}
			fprintf(stderr,"ERROR 64: Se ingresaron argumentos desconocidos\n");

			return(64);
		}
		i++;
	}
	
	
	
	if (flag_l == 0){
		fprintf(stderr,"ERROR 65: Falta argumento requerido\n");
		return(65);

	}
	if ((flag_s == 0) && (*flag_m == 1)){
		if (abri_archivo_lectura)
		{
			fclose(*in);
		}
		if (abri_archivo_escritura)
		{
			fclose(*out);
		}
		fprintf(stderr, "ERROR 70: -m|--municipio requiere de -s|--salida\n");
		return 70;

	}
	if (flag_h){
		if (abri_archivo_lectura)
		{
			fclose(*in);
		}
		if (abri_archivo_escritura)
		{
			fclose(*out);
		}
		imprimir_ayuda();
		exit (0);
	}

	if ((flag_may==1) && ((*flag_m) !=1)){
		if (abri_archivo_lectura)
		{
			fclose(*in);
		}
		if (abri_archivo_escritura)
		{
			fclose(*out);
		}
		fprintf(stderr,"ERROR 99: El argumento -M|--mayores solo puede ser ingresado junto con -m|--municipio\n");
		return(99);
	}
	if ( ! abri_archivo_escritura){
		*out = stdout;
	}
	return 0;
}


/************************************
 * devuelve 1 (TRUE) si el valor pasado como parametro coincide con los argumentos de ayuda, 0 si no
 * ********************************/
int es_ayuda(char* valor){
	if  ((! strcmp(valor, "--ayuda")) || 
			(!strcmp (valor, "--help") ) ||  
				(! strcmp(valor, "-a"))  || 
					(! strcmp(valor, "-h"))   ) {
		return 1;
	} else {
		return 0;
	}
}
/*********************************************
 *devuelve 1 (TRUE) si el valor pasado como parametro coincide con los argumentos de leer, 0 si no
 * *************************************************/
int es_leer(char* valor){
	if ( (! strcmp(valor, "--leer")) || 
				(! strcmp (valor, "-l"))  ) {
		return 1;
	}else {
		return 0;
	}
}
/******************************************
 * devuelve 1 (TRUE) si el valor pasado como parametro coincide con los argumentos de salida, 0 si no
 * ****************************************/
int es_salida(char* valor){
	if  ((! strcmp(valor, "--salida")) || 
				(! strcmp(valor, "-s")) )  {
		return 1;
	}else {
		return 0;
	}
}
/*******************************************
 * devuelve 1 (TRUE) si el valor pasado como parametro coincide con los argumentos de municipio, 0 si no
 * *****************************************/
int es_municipio(char* valor){
	if ((! strcmp (valor, "--municipio")) || 
				(! strcmp (valor, "-m")) ) {
		return 1;
	}else {
		return 0;
	}
}
/*************************************************************
 * devuelve 1 (TRUE) si el parametro pasado comienza con '-'
 * dado que recibe como parametro un char se deberia invocar con argv[i][0] <el primer caracter de la cadena>
 * **********************************/
int es_arg(char valor){
	if (valor == '-'){
		return 1;
	} else {
		return 0;
	}
}
/*****************************************************************
 *devuelve 1 (TRUE) si el parametro pasado coincide con los parametros validos para mayores
 * ************************************************************/
int es_mayores(char* valor){
	if ((! strcmp (valor, "--mayores")) || 
				(! strcmp (valor, "-M")) ) {
		return 1;
	}else {
		return 0;
	}
}

/*********************************************************
 esta funcion imprime por salida standar la ayuda. la misma se halla en un archivo adjuntado. Me parecio interesante imprimir
 desde un archivo para asi reforzar los conceptos de archivos. La funcion cierra el archivo antes de finalizar.
 * ******************************************************/
void imprimir_ayuda(void){
	FILE* ayuda;
	ayuda = fopen("ayuda", "rb");
	int  cont, c, mem;

	char* buffer;
	while (!feof(ayuda)){
		cont = 0;
		mem = 2;
		buffer = malloc(mem);
		while( ((c = getc(ayuda)) != EOF)){

			if (cont ==  mem - 1 ){
				mem *= 2;
				buffer = realloc (buffer, mem);
			}	
			buffer[cont ++] = (char) c;
			
			buffer[cont] = '\n';

		}
	
		fprintf(stdout, "%s",buffer);
		free(buffer);
	}
	fclose(ayuda);
}

