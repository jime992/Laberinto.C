#ifndef LISTAS_H_
#define LISTAS_H_

#include <stdio.h>
#include <stdlib.h>


struct Nodo {
	char modo;
	char nivel; 						
	int colum;
	int fila;
	struct Nodo *sig;
};

struct Nodo *crearLista(){ 					//2a) crea una lista vacia

	return NULL;

}

int esListaVacia(struct Nodo *lista){ 		//2b) pregunta si la lista esta vacia

	if (lista == NULL){
		return 1;
	}else{
		return 0;
	}

}

void mostrar(struct Nodo *lista){ 			//2c) muestra el contenido de los nodos de la lista

	while(esListaVacia(lista) != 1){
		printf("%c\t",lista->modo);
		printf("%c\t",lista->nivel);
		printf("%d\t",lista->colum);
		printf("%d\t",lista->fila);
		lista = lista->sig;
	}

}

void primerElemento(struct Nodo *lista){ 	//2d) retorna el primer elemento de la lista

	printf("\n\nEl primer elemento de la lista es:\nmodo: %c\nnivel: %c\ncolumna: %d\nfila: %d\n",lista->modo,lista->nivel,lista->colum,lista->fila);

}


struct Nodo *borrar(struct Nodo *lis){			//2f) borra el primer nodo de la lista
	
	struct Nodo *aux;
	aux = lis;
	lis = lis->sig;
	free(aux);
	
	return lis;
}

/*int SelecModo(struct Nodo *lis, int num) {		//2g) determina si un dato pertenece o no a la lista
	
	int cont=0;

	while (esListaVacia(lis)!=1){
		if (lis->dato == num){		
			cont = 1;
		}
		lis = lis->sig;
	}

	if (cont == 0){
		return 0;
	}else {
		return 1;
	}

}*/

#endif /* LISTAS_H_ */