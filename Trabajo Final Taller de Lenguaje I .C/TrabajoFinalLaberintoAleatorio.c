#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <conio.h>
#include <alloc.h>
#include <windows.h>

#define ARRIBA 72
#define IZQUIERDA 75
#define DERECHA 77
#define ABAJO 80
#define ESC 27
#define PALABRAS_BIN "Palabras.dat"

typedef struct { 
            int total; 
            int faciles;
            int Lfaciles; 
            int medias;
            int Lmedias;
            int dificiles;
            int Ldificiles;
}TCabePala;

void crearArchivoPalabras();	//Crea un archivo binario y lo carga con 30 palabras
void cargarMenu(char modo);		//Carga el menu de opciones
void jugar(char modo);			//Segun el modo de 1 o 2 jugadores habilita las teclas correspondientes para poder recorrer el laberinto		
char opNivel(char nivel);		//Imprime las opciones de Nivel (Facil, Medio, Dificil) y devuelve la opcion elegida
char* GenerarLab(char* palabra,int colum, int filas);	//segun la cantidad de columnas y filas genera una matriz con 0 y 1 y
void ImprimirLab(char *pMat,int fil,int col, char modo, char *palabra);	//Imprime el laberinto y la palabra que estará a la derecha de él
char* Precedimiento(char *pMat, int colum, int filas);	//Segun las filas y columnas creo un laberinto aleatorio dentro de una matriz 
int Resultado(char modo, int j1,int j2);	//Segun el modo de 1 o 2 jugadores y la cantidad de letras conseguidas por los mismos, imprimo un resultado en pantalla
void *ReservarMemoria(int TotBytes);		//Devuelve error si no hay memoria disponible
char *PalabraAleatoria(int largo,int Aleatorio);	//Devuelve una palabra aleatoria del archivo binario del largo deseado

void main(){

	char modo='0';   		 

	randomize();
	_setcursortype(0);		//oculta el cursor
	crearArchivoPalabras();	//creo el archivo binario y lo cargo con 30 palabras
    cargarMenu(modo);		//cargo el menu principal 
    exit(1);
}

void cargarMenu(char modo){

	while(modo!='4' && modo!= ESC){ 	//imprimira las opciones del menu hasta que sea una tecla valida
	
		clrscr();	
	
		printf("-------------------------------------------------------\n");
		printf("----------------------LABERINTO------------------------\n");
		printf("--------------------PROYECTO FINAL---------------------\n");
		printf("-------------------------------------------------------\n");
		printf("\n\t1)Modo 1 Jugador\n\t2)Modo 2 Jugadores\n\t3)Ver Instrucciones\n\t4)Salir\n");
		printf("\nSeleccione la opcion que desee ");

		modo = getch();
		clrscr();

	    jugar(modo);
		
	}

	printf("Adios");	

	exit(1);
}

void jugar(char modo){

	char nivel='0';	//nivel de dificultad, lo inicializo en 0 para entrar al while
	char *pMat;
	char *Palabra;
	int tecla, gano, col, fil, alea, largo, error, stat;
	int y, x, x2, y2, i, j, j1=0, j2=0, l, n;
	FILE *pfile;
 	TCabePala *Cabe;
	
	Cabe = (TCabePala*)ReservarMemoria(sizeof(TCabePala));
 	
 	if((pfile=fopen(PALABRAS_BIN,"rb"))==NULL)
  	{
    	cprintf("Error al abrir el archivo");
   	 	exit(1);
  	}else{

  		fread(Cabe,sizeof(TCabePala),1,pfile);	//Leo la cabecera para sacar info de las palabras
		fclose(pfile);
	}		
	
	if (modo=='1'){			//modo de 1 jugador

		
		clrscr();
		nivel = opNivel(nivel);	

		switch(nivel){ 	// segun el nivel de dificultad (mejorar esto sacando la info de la cabecera del archivo binario)... 
			case '1':{col=15;fil=15;l=Cabe->Lfaciles;alea = random(Cabe->faciles);} break; 
			case '2':{col=23;fil=23;l=Cabe->Lmedias;alea = random(Cabe->medias);} break;
			case '3':{col=29;fil=29;l=Cabe->Ldificiles;alea = random(Cabe->dificiles);} break;
		}

		i=0;
		y=(fil/2)+1;	//para posicionarme en medio del laberinto
		x=(col/2)+1;

		Palabra=(char*)ReservarMemoria(sizeof(char)*l);
		strcpy(Palabra,PalabraAleatoria(l,alea));		//cargo una palabra aleatoria de longitud l en el puntero a char Palabra
		largo = strlen(Palabra);
		pMat = GenerarLab(Palabra,col,fil);				//genero un laberinto y lo cargo en la matriz pMat
		ImprimirLab(pMat,fil,col,modo,Palabra);			//Imprimo el laberinto en pantalla
		gotoxy(x,y);									//Voy a la mitad del laberinto e imprimo una carita 
		cprintf("%c",1);
			
		while(tecla != ESC){

		   	tecla = getch();	
			gotoxy(x,y);	

			if(*(pMat+((y-1)*col + (x-1))) == 1){	//si es un camino en donde estoy posicionado, entonces borro la carita
				cprintf(" ");
			}else{
				if (*(pMat+((y-1)*col + (x-1))) == *(Palabra+(i-1))	){	//si es una letra de prioridad hago un sonido y la "borro" de la matriz
						Beep(500, 50);
		 	     		*(pMat+((y-1)*col + (x-1))) = 1;
				}else{
					cprintf("%c",*(pMat+((y-1)*col + (x-1)))); //Si es una letra que no cuenta para puntaje no hago sonido y la vuelvo a escribir
				}
			} 													


			switch(tecla){ 	//Me fijo la tecla que toca y si no es una pared entonces se mueve
				case ARRIBA:   {if((*(pMat+((y-2)*col + (x-1)))) != 0)y--;} break; 
					case ABAJO:    {if((*(pMat+((y)*  col + (x-1)))) != 0)y++;} break;
					case DERECHA:  {if((*(pMat+((y-1)*col + (x)))) != 0)x++;} break;
					case IZQUIERDA:{if((*(pMat+((y-1)*col + (x-2)))) != 0)x--;} break;
			}

			if(*(pMat+((y-1)*col + (x-1))) == *(Palabra+i)){	//si es una letra de prioridad, la escribo y aumento 'i' que sirve para el control de la palabra
				j=i+i;	//hago esto para poder escribir en pantalla las letras que está obteniendo el jugador con un espacio que seria el doble de la letra
				gotoxy((col + 5)+j, fil/2+1); 
				cprintf("%c ", *(pMat+((y-1)*col + (x-1))));
				i++;
			}

			gotoxy(x,y);	//ya con y o x modificado por la tecla presionada voy a x y y escribo la carita
			cprintf("%c", 1);   	


			if (i==largo){				//si el control es igual al largo de la palabra , llamo a la funcion Resultado para
				Resultado(modo,20,j2);	//que escriba en pantalla el resultado de la partida
				tecla = getch();
				while(tecla != ESC){	//pido una tecla para salir de la pantalla de resultado
					tecla = getch();
				}
			}
		
		}
		if (i<largo){
			Resultado(modo,15,j2);
			tecla = getch();
				while(tecla != ESC){	//pido una tecla para salir de la pantalla de resultado
					tecla = getch();
			}
		}
	}


	if (modo=='2'){
		
		clrscr();		
		nivel = opNivel(nivel);

		switch(nivel){ 	//segun el nivel de dificultad 
			case '1':{col=21;fil=15;l=Cabe->Lfaciles;alea = random(Cabe->faciles);} break; 
			case '2':{col=31;fil=25;l=Cabe->Lmedias;alea = random(Cabe->medias);;} break;
			case '3':{col=37;fil=29;l=Cabe->Ldificiles;alea = random(Cabe->dificiles);;} break;
		}

		x2=(col/2);
		y2=((fil/2)+1);
		x=((col/2)+2);
		y=(((fil/2)+1));	
		i=0;		

		Palabra=(char*)ReservarMemoria(sizeof(char)*l);
		strcpy(Palabra,PalabraAleatoria(l,alea));
		largo = strlen(Palabra);

		pMat = GenerarLab(Palabra,col,fil);
	 

		ImprimirLab(pMat,fil,col,modo,Palabra);

		gotoxy(x,y);
			cprintf("%c",1);

			gotoxy(x2,y2);
			cprintf("%c",2);



		while(tecla != ESC){

	   		tecla = getch();

				
				gotoxy(x,y);

			if(*(pMat+((y-1)*col + (x-1))) == 1){  	//borro lo que tenia antes si es un camino, sino dibujo la letra correspondiente
				printf(" ");
			}else{
				if  (*(pMat+((y-1)*col + (x-1))) == *(Palabra+(i-1)) ) {
					Beep(500, 50);
		 	     		*(pMat+((y-1)*col + (x-1))) = 1;
		 	     		cprintf(" ");							
					} else{
						cprintf("%c",*(pMat+((y-1)*col + (x-1)))); //Si es una letra que no cuenta para puntaje no la borro
				}				
			}



			gotoxy(x2,y2);

			if(*(pMat+((y2-1)*col + (x2-1))) == 1){	//si es camino entonces borro lo que escribir antes
				cprintf(" ");
			}else{
				if(*(pMat+((y2-1)*col + (x2-1))) == *(Palabra+(i-1))){
					Beep(600, 50);
					*(pMat+((y2-1)*col + (x2-1))) = 1;
					cprintf(" ");
				}else {
					cprintf("%c",*(pMat+((y2-1)*col + (x2-1)))); 
				}			
			}


			switch(tecla){ 	//Me fijo la tecla que toca y si no es una pared entonces se mueve
	   			case ARRIBA:   {if((*(pMat+((y-2)*col + (x-1)))) != 0)y--;} break; 
	   			case ABAJO:    {if((*(pMat+((y)*col + (x-1)))) != 0)y++;} break;
	   			case DERECHA:  {if((*(pMat+((y-1)*col + (x)))) != 0)x++;} break;
	   			case IZQUIERDA:{if((*(pMat+((y-1)*col + (x-2)))) != 0)x--;} break;
				case 'w':{if((*(pMat+((y2-2)*col + (x2-1)))) != 0)y2--;} break; 
				case 'W':{if((*(pMat+((y2-2)*col + (x2-1)))) != 0)y2--;} break; 
	   			case 'S':{if((*(pMat+((y2)*col + (x2-1)))) != 0)y2++;} break;
	   			case 's':{if((*(pMat+((y2)*col + (x2-1)))) != 0)y2++;} break;
	   			case 'D':{if((*(pMat+((y2-1)*col + (x2)))) != 0)x2++;} break;
	   			case 'd':{if((*(pMat+((y2-1)*col + (x2)))) != 0)x2++;} break;
	   			case 'A':{if((*(pMat+((y2-1)*col + (x2-2)))) != 0)x2--;} break;
	   			case 'a':{if((*(pMat+((y2-1)*col + (x2-2)))) != 0)x2--;} break;

				}


			if(*(pMat+((y-1)*col + (x-1))) == *(Palabra+i)){
				j=i+i;
				gotoxy((col + 16)+j, fil/2-2);
				cprintf("%c ", *(pMat+((y-1)*col + (x-1))));
				i++;
				j1++;
			}

			if(*(pMat+((y2-1)*col + (x2-1))) == *(Palabra+i)){
				j=i+i;
				gotoxy((col + 16)+j, fil/2+1);
				cprintf("%c ", *(pMat+((y2-1)*col + (x2-1))));
				i++;
				j2++;
			}

				gotoxy(x,y);
			cprintf("%c",1);   	

	   		gotoxy(x2,y2);
			cprintf("%c",2);


			if (i==largo){
				Resultado(modo,j1,j2);
				tecla = getch();
				while(tecla != ESC){
					tecla = getch();
				}
			}		

		}

	}

	if (modo=='3'){

		x=1;
		y=1;

		clrscr();
		printf("------------------------------------------------------------------------------------------------\n");
		printf("------------------------------------------LABERINTO---------------------------------------------\n");
		printf("----------------------------------------INSTRUCCIONES-------------------------------------------\n");
		printf("------------------------------------------------------------------------------------------------\n\n\n");

		printf("MODO 1 JUGADOR:\n\n");
		printf("Despues de elegir la dificultad del laberinto, el jugador ingresar%c a %cl .Deber%c conseguir todas\nlas letras de la palabra que aparece a la derecha (en su orden correspondiente) no podr%c atravesar\nlas paredes y si desea salir, sin completar el objetivo, deber%c presionar la tecla 'ESC'.\n\n",130,160,160,160,160);
		printf("CONTROLES:\n");
		cprintf("\r%c SUBIR\n\r%c BAJAR\n\r%c IZQUIERDA\n\r%c DERECHA\n\n",24,25,27,26);
		printf("\nMODO 2 JUGADORES:\n\n");
		printf("Despues de elegir la dificultad del laberinto, los jugadores ingresar%cn a %cl .Deber%cn conseguir todas\nlas letras de la palabra que aparece a la derecha (en su orden correspondiente). Ganar%c aquel jugador\nque consigua m%cs letras de la palabra, no podr%cn atravesar las paredes, si desean salir se debe presionar\nla tecla 'ESC'\n",160,130,160,160,160,160);
		printf("\nCONTROLES:\n");
		cprintf("\nJUGADOR 1: %c\n",1);
		cprintf("\r%c SUBIR\n\r%c BAJAR\n\r%c IZQUIERDA\n\r%c DERECHA\n\n",24,25,27,26);
		cprintf("\rJUGADOR 2: %c\n\r",2);
		printf("W SUBIR\nS BAJAR\nA IZQUIERDA\nD DERECHA\n\n");
		printf("Presione ESC para volver al menu principal\n");
		gotoxy(x,y);
		tecla = 0;
		y= 30;

		while(tecla != ESC){

			tecla = getch();
			

			if(tecla == ARRIBA){
				y= 1;
				gotoxy(x,y);
				y= 30;
			}
			if(tecla == ABAJO){
				y= y++;
				gotoxy(x,y);
			}

		}
	}
	clrscr();

}

char opNivel(char nivel){

	while((nivel !='4') && (nivel !='3') && (nivel !='2') && (nivel !='1') && (nivel != ESC)){ //imprimira las opciones de nivel hasta que sea una tecla valida

		printf("\n\tDIFICULTAD:\n\n\t1)FACIL\n\t2)MEDIO\n\t3)DIFICIL\n\t4)Volver Al Menu Principal\n");
		printf("\nSeleccione la opcion que desee ");
		nivel =getch();

		clrscr();
   		
   	}

   	if((nivel=='4') || (nivel == ESC)){
   		cargarMenu(0);
   	}
   	return nivel;

}


char* GenerarLab(char *palabra,int colum, int filas){
	
	char *pMat;
	int i,f,c,s,t;
	int largo=strlen(palabra);
	FILE *ArchiMat, *ArchiTexto;
		
	pMat = (char*)malloc(filas*colum*sizeof(int));

	pMat = Precedimiento(pMat, colum, filas);


	for (i=0;i<largo;i++){	

		c = rand()% colum;	
		f = rand()% filas;

		while (*(pMat+(f*colum + c)) != 1){   //busco que el punto random sea un camino para poder poner la letra
			c = rand()% colum;
			f = rand()% filas;
		}
		*(pMat+(f*colum + c)) = *(palabra+i);

	}

	return pMat;

}



char* Precedimiento(char *pMat, int colum, int filas){
	
	int i,f,c,s,min,max; // f = var filas, c= var columnas, s= numeros aleatorios 


	for(f=0;f<filas;f++){ 			//vamos a llenar la matriz de paredes (0)
		for(c=0;c<colum;c++){
				*(pMat+(f*colum + c))=0;
		}
	}

	//ahora creamos unos cuantos caminos 

	for(c=0;c<colum;c++) { 			//linea horizontal en el medio
		*(pMat+((filas/2)*colum +(c)))=1;
	}


	for(f=0;f<filas;f++) {	//linea vertical
		*(pMat+(f*colum + (colum/2)))=1;
	}

	//ahora lineas aleatorias

	for (i=1; i <(colum/2)-1; i ++){ 			//lineas verticales de distintos tamaños a la izquierda del centro
		
		min = random(filas/2);
		max = filas - random((filas/2)+1);
		s = random(2)+1; 

		for (f=min;f<max;f++) {	
			*(pMat+(f*colum + ((colum/2) + (i+s)) ))=1;
		}
		i++;
	}

	for (i=1; i <(colum/2)-1; i ++){ // lineas verticales de distintos tamaños a la derecha del centro
		
		min = (rand()% (filas/2));
		max = filas-(rand()% (filas/2)+1);
		s = random(3)+1;

		for(f=min;f<max;f++) {

			*(pMat+(f*colum + ((colum/2) - (i+s))))=1;
		}
		i++;

	}

	for (i=1; i<(filas/2)-1; i++){ 			//lineas horizontales de distintos tamaños Abajo del centro
		
		min = (rand()% (colum/2));
		max = colum-(rand()% (colum/2)+ 1);
		s = rand()% 2 + 1;

		for(c=min;c<max;c++) {

			*(pMat+(((filas/2)+(i+s)))*colum + c)=1;
		}
		i++;
	}

	for (i=1; i<(filas/2)-1; i++){ 			//lineas horizontales de distintos tamaños arriba del centro
		
		min = (rand()% (colum/2));
		max = colum-(rand()% (colum/2)+ 1);
		s = rand()% 2 + 1;

		for(c=min;c<max;c++) {

			*(pMat+(((filas/2)-(i+s)))*colum + c)=1;
		}
		i++;
	}

	//ahora caminos aleatorios		

	for (i=0;i<colum*filas*2;i++){	

		c = rand()% (colum - 4) +3;
		f = rand()% (filas - 4) +3;

		while (*(pMat+(f*colum + c)) == 0){   //busco que el punto random sea un camino para poder construir a su lado 
			c = rand()% (colum - 4) + 3;
			f = rand()% (filas - 4) + 3;
		}					

		s = rand()% 2 - 1;

		while (s == 0){
			s = rand()% 2 - 1;
		}

		if( (*(pMat+((f)*colum + (c+1))) == 1) || (*(pMat+((f)*colum + (c-1))) == 1) ){ //estoy preguntando si estoy parada en una fila de caminos

				if((*(pMat+((f+s)*colum + (c+1))) != 1) && (*(pMat+((f+s)*colum + (c-1))) != 1) ){	//si cuando bajo tengo paredes entonces abro un camino
					*(pMat+((f+s)*colum + (c))) = 1;												//si no no hago nada
				} 
		}

		if( (*(pMat+((f+1)*colum + (c))) == 1) || (*(pMat+((f-1)*colum + (c))) == 1) ){		//estoy preguntando si estoy parada en una columna de caminos

			if((*(pMat+((f+1)*colum + (c+s))) != 1) && (*(pMat+((f-1)*colum + (c+s))) != 1) ){		//si cuando bajo tengo paredes entonces abro un camino
				*(pMat+((f)*colum + (c+s))) = 1;
			}		
		}
	}
			
	//vuelvo a cargar las paredes de la matriz ya que es un laberinto sin salida
	f=0;
	for(c=0;c<colum-1;c++) {
		*(pMat+((f) *colum + (c))) = 0;
	}

	for(c=0;c<colum-1;c++) {
		*(pMat+((filas-1)*colum +(c)))=0;
	}

	for(f=0;f<filas-1;f++) {
		*(pMat+(f*colum + (0)))=0;
	}

	for(f=0;f<filas-1;f++) {
		*(pMat+(f*colum + (colum - 1)))=0;
	}

	return pMat;

}

void ImprimirLab(char *pMat,int fil,int col, char modo, char *palabra){
	
	int f,c,i;
	int largo = strlen(palabra);

	gotoxy(1,1);

  	for(f=0;f<fil;f++){		//segun los ceros o unos de la matriz imprimo en la pantalla paredes o caminos
    	for(c=0;c<col;c++){
    		if(	*(pMat+(f*col + c)) ==0  ){
				printf("%c",219);
    		}else{
	    		if(	*(pMat+(f*col + c)) ==1  ){
					printf(" ");
    			}else{

	    		printf("%c", *(pMat+(f*col + c)));
    			}	
    		}
      	}
      	printf("\r\n");
    }


	if(modo == '1'){
	
		gotoxy(col + 5, fil/2+2);

		for(i=0;i<largo;i++){
			printf("%c ",*(palabra+i));
		}

		gotoxy(col + 5, fil/2+1);

		for(i=0;i<largo;i++){
			printf("_ ");
		}
	}

	if(modo == '2'){

		gotoxy(col + 5, (fil/2)-1);
		printf("Jugador 1: ");
		for(i=0;i<largo;i++){
			printf("%c ",*(palabra+i));
		}

		gotoxy(col + 16, (fil/2)-2);
		for(i=0;i<largo;i++){
			printf("_ ");
		}

		gotoxy(col + 5, (fil/2)+2);
		printf("Jugador 2: ");
		for(i=0;i<largo;i++){
			printf("%c ",*(palabra+i));
		}

		gotoxy(col + 16, (fil/2)+1);
		for(i=0;i<largo;i++){
			printf("_ ");
		}


	}


}


int Resultado(char modo, int j1,int j2){
	
	clrscr();	
	gotoxy(1,2);	

	if (j1 == 20){

		printf("-------------------------------------------------------\n");
		printf("-----------------------GANASTE-------------------------\n");
		printf("-------------------------------------------------------\n");
	 	printf("\n\t\tPresione ESC para salir\n");
	 	Beep(500, 200);
	    Beep(600, 200);
    	Beep(700, 200);
    	Beep(800, 200);
    	Beep(900, 200);
    	Beep(999, 350);
	 	
	 	return 0;
	}	
	if(j1==j2){
	
		printf("-------------------------------------------------------\n");
		printf("------------------------EMPATE-------------------------\n");
		printf("-------------------------------------------------------\n");
		printf("\nLetras conseguidas por Jugador 1: %d",j1);
		printf("\nLetras conseguidas por Jugador 2: %d",j2);
		printf("\n\n\t\tPresione ESC para salir\n");
		Beep(999, 150);
    	Beep(900, 200);
    	Beep(800, 250);
    	Beep(700, 300);
    	Beep(600, 900);

		return 0;
	}
	
	if(j1==15){
	
		printf("-------------------------------------------------------\n");
		printf("-----------------------PERDISTE-------------------------\n");
		printf("-------------------------------------------------------\n");
		printf("\n\n\t\tPresione ESC para salir\n");
		Beep(999, 150);
    	Beep(900, 200);
    	Beep(800, 250);
    	Beep(700, 300);
    	Beep(600, 900);

		return 0;
	}

	if (j1>j2){

		printf("-------------------------------------------------------\n");
		printf("-----------------GANO EL JUGADOR 1---------------------\n");
		printf("-------------------------------------------------------\n");
		printf("\nLetras conseguidas por Jugador 1: %d",j1);
		printf("\nLetras conseguidas por Jugador 2: %d",j2);
		printf("\n\n\t\tPresione ESC para salir\n");
		Beep(500, 200);
	    Beep(600, 200);
    	Beep(700, 200);
    	Beep(800, 200);
    	Beep(900, 200);
    	Beep(999, 350);
		return 0;
	} 

	if(j1<j2){

		printf("-------------------------------------------------------\n");
		printf("-----------------GANO EL JUGADOR 2---------------------\n");
		printf("-------------------------------------------------------\n");
		printf("\nLetras conseguidas por Jugador 1: %d",j1);
		printf("\nLetras conseguidas por Jugador 2: %d",j2);
		printf("\n\n\t\tPresione ESC para salir\n");
	 	Beep(500, 200);
	    Beep(600, 200);
    	Beep(700, 200);
    	Beep(800, 200);
    	Beep(900, 200);
    	Beep(999, 350);		

		return 0;
	}
	return 0;
}

int ExisteArchivo(char *Nombre){
	
	FILE *Aux;

	if((Aux=fopen(Nombre,"rb"))==NULL){
		return(0);
	}

 	fclose(Aux);

  	return(1);
}

void crearArchivoPalabras(){	//Creo y cargo el archivo binario con 30 palabras
	
	FILE   *pfilebin;
  	TCabePala Cabe[1];
  	int i,alea,error,n,stat;
  	
  	Cabe[0].faciles=15;
  	Cabe[0].Lfaciles=5;
  	Cabe[0].medias=10;
  	Cabe[0].Lmedias=9;
  	Cabe[0].dificiles=10;
  	Cabe[0].Ldificiles=11;
  	Cabe[0].total= Cabe[0].faciles + Cabe[0].medias + Cabe[0].dificiles;
     //crea para la escritura y la lectura del archivo binario
 	if((pfilebin=fopen(PALABRAS_BIN,"wb+"))==NULL) 
    	cprintf("NO PUDO CREAR ARCHIVO DESTINO"); 
    else{ 
        //escribe la palabra en el arch. binario
        fwrite(Cabe,sizeof(TCabePala),1,pfilebin);
        fwrite("GATO",Cabe[0].Lfaciles,1,pfilebin);
	    fwrite("PATO",Cabe[0].Lfaciles,1,pfilebin);
    	fwrite("DURO",Cabe[0].Lfaciles,1,pfilebin);
      	fwrite("ECOS",Cabe[0].Lfaciles,1,pfilebin);
      	fwrite("PICO",Cabe[0].Lfaciles,1,pfilebin);
      	fwrite("HOLA",Cabe[0].Lfaciles,1,pfilebin);
      	fwrite("FARO",Cabe[0].Lfaciles,1,pfilebin);
      	fwrite("CUNA",Cabe[0].Lfaciles,1,pfilebin);
      	fwrite("PIEL",Cabe[0].Lfaciles,1,pfilebin);
      	fwrite("APIO",Cabe[0].Lfaciles,1,pfilebin);
      	fwrite("PELO",Cabe[0].Lfaciles,1,pfilebin);
      	fwrite("FILO",Cabe[0].Lfaciles,1,pfilebin);
      	fwrite("ROSA",Cabe[0].Lfaciles,1,pfilebin);
      	fwrite("ROMA",Cabe[0].Lfaciles,1,pfilebin);
      	fwrite("AMOR",Cabe[0].Lfaciles,1,pfilebin); 
      	fwrite("AMIGUERO",Cabe[0].Lmedias,1,pfilebin);
      	fwrite("DIALOGUE",Cabe[0].Lmedias,1,pfilebin);
      	fwrite("ECUALIZO",Cabe[0].Lmedias,1,pfilebin);
      	fwrite("EUTROFIA",Cabe[0].Lmedias,1,pfilebin);
      	fwrite("AUTOCINE",Cabe[0].Lmedias,1,pfilebin);
      	fwrite("AUTORICE",Cabe[0].Lmedias,1,pfilebin);
      	fwrite("OPUSIERA",Cabe[0].Lmedias,1,pfilebin);
      	fwrite("ACUIFERO",Cabe[0].Lmedias,1,pfilebin);
      	fwrite("AVERIGUO",Cabe[0].Lmedias,1,pfilebin);
      	fwrite("EUFORICA",Cabe[0].Lmedias,1,pfilebin);      
     	fwrite("MURCIELAGO",Cabe[0].Ldificiles,1,pfilebin);
      	fwrite("HIPOTENUSA",Cabe[0].Ldificiles,1,pfilebin);
      	fwrite("NEUROTICAS",Cabe[0].Ldificiles,1,pfilebin);
      	fwrite("DURACIONES",Cabe[0].Ldificiles,1,pfilebin);
      	fwrite("CUESTIONAR",Cabe[0].Ldificiles,1,pfilebin);
      	fwrite("ABRIGUEMOS",Cabe[0].Ldificiles,1,pfilebin);
      	fwrite("COMPUSIERA",Cabe[0].Ldificiles,1,pfilebin);
      	fwrite("ENSUCIARLO",Cabe[0].Ldificiles,1,pfilebin);
      	fwrite("ERUPCIONAS",Cabe[0].Ldificiles,1,pfilebin);          
      	fwrite("LICUAREMOS",Cabe[0].Ldificiles,1,pfilebin);

    	} 

	fclose(pfilebin); 

}




void *ReservarMemoria(int TotBytes){

void *pAux;
  
  if((pAux=malloc(TotBytes))==NULL) {
    cprintf("No pudo reservar memoria dinámica");
    getch( ); exit(1);
  }
  return(pAux);
}


char* PalabraAleatoria(int largo,int Aleatorio){

  char *palabra;
  int error,n,stat;
  FILE *pfile;
  TCabePala *Cabe;

  Cabe = (TCabePala*)ReservarMemoria(sizeof(TCabePala));
  palabra=(char*)ReservarMemoria(largo*sizeof(char));
  if((pfile=fopen(PALABRAS_BIN,"rb"))==NULL)
  {
    cprintf("Error al abrir el archivo");
    exit(1);
  }
  else{
        n = Aleatorio;  
        fread(Cabe,sizeof(TCabePala),1,pfile); //Leo la cabecera para sacar info de las palabras
        rewind(pfile);

        if(largo == 5){
          stat=fseek(pfile,sizeof(TCabePala)+(n)*(sizeof(char)*largo),0);
          if(stat!=0)
          {
            puts("Error, puntero del archivo movido fuera de este");
            exit(-1);
          }
	      fread(palabra,sizeof(char)*largo,1,pfile);
          fclose(pfile);
          cprintf("\r\n");
        }
        if(largo == 9){
          stat=fseek(pfile,sizeof(TCabePala)+((sizeof(char)*5)*Cabe->faciles) +(n)*(sizeof(char)*largo),0);
          if(stat!=0)
          {
            puts("Error, puntero del archivo movido fuera de este");
            exit(-1);
          }
          fread(palabra,sizeof(char)*largo,1,pfile);
          fclose(pfile);
          cprintf("\r\n"); 
        } 
        
        if(largo == 11){	//situo el puntero salteando la cabecera, las palabras faciles y las palabras medias
        	stat=fseek(pfile,(sizeof(TCabePala)+((sizeof(char)*5)*Cabe->faciles)+((sizeof(char)*9))*Cabe->medias) +(n)*(sizeof(char)*largo),0);
        	//comprueba que no haya error
        	if(stat!=0)
        	{
        		puts("Error, puntero del archivo movido fuera de este");
        		exit(-1);
        	}
        	fread(palabra,sizeof(char)*largo,1,pfile);
        	fclose(pfile);
        }

      }
	return palabra;
 
}