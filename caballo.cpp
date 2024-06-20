#include <cassert>
#include <iostream>
#include <climits>
#include "lista.cpp"
using namespace std;
int celdas = 8; // Tablero de ajedrez de 8x8

// verifica que la celda este dentro del tablero
bool esCoordenadaValida(int fila, int col)
{
	return fila >= 0 && fila < celdas && col >= 0 && col < celdas;
}

// verifica que no haya pasado por la celda
bool pasePorCelda(int fila, int col, int **camino)
{
	return camino[fila][col] > 0;
}

bool esSolucion(int filaActual, int colActual, int filaDestino, int colDestino) {
	return filaActual == filaDestino && colActual == colDestino;
}	

// imprime el camino del caballo, maneja los casos en que el número de movimientos es mayor a 9 para que no quede corrida la matriz
// como tiene 64 celdas el máximo número de movimientos es 64, por lo que no hay que preocuparse por el caso en que el número de movimientos sea mayor a 99
// si hay que preocuparse por el caso en que el número de movimientos sea mayor a 9, en ese caso si no se considera la matriz queda movida
void imprimirSolucion(int ** camino) {
	cout << "Solucion encontrada:" << endl;	
	for (int fila = 0; fila < celdas; fila++) {
		for (int col = 0; col < celdas; col++) {
			cout << "[";
			if (camino[fila][col] > 0 && camino[fila][col] < 10)
				cout << " " << camino[fila][col]; // caso en que el número de movimientos es menor a 10, para que no quede corrida la matriz se imprime un espacio mas
			else if (camino[fila][col] >= 10) cout << camino[fila][col];
			else cout << "  "; // caso en que no paso por la celda
			cout << "]";
		}
		cout << endl;
	}
}

// es valido si la celda esta dentro del tablero y no fue visitada
bool esMovimientoValido(int fila, int col, int ** camino) {
	return esCoordenadaValida(fila, col) && !pasePorCelda(fila, col, camino);
}

// queremos guardar el numero del paso dado
void realizarMovimiento(int fila, int col, int pasoActual, int ** camino) {
	camino[fila][col] = pasoActual;
}

// deshacemos el movimiento, ponemos en 0 la celda
void deshacerMovimiento(int fila, int col, int ** camino) {
	camino[fila][col] = 0;
}

// camino es una matriz de 8x8 que contiene el camino del caballo, todas las celdas comienzan en 0 y guarda en la celda por la que pasa 
// el caballo el número de movimiento
void caballoEnumeracion(int filaActual, int colActual, int filaDestino, int colDestino, int pasoActual, int **camino)
{
	if (esSolucion(filaActual, colActual, filaDestino, colDestino)) imprimirSolucion(camino); 
	else {
			// cout << "Running..." << endl; // para ver que el programa no se cuelga, imaginense tratar de debugguear!!!!
			// opciones de movimientos posibles, cada movimiento se representa con un valor de movimientosFila y movimientosCol en la misma ubicacion
			int movimientosFila[] = {-2, -1, 1, 2, 2, 1, -1, -2};
			int movimientosCol[] = {-1, -2, -2, -1, 1, 2, 2, 1};

			// exploramos las opciones posibles
			// el 8 no tiene nada que ver con la cantidad de celdas, es la cantidad de opciones de movimientos posibles
			for (int opcionMovimiento = 0; opcionMovimiento < 8; opcionMovimiento++) {
				int nuevaFila = filaActual + movimientosFila[opcionMovimiento];
				int nuevaCol = colActual + movimientosCol[opcionMovimiento];
				if (esMovimientoValido(nuevaFila, nuevaCol, camino))
				{
					realizarMovimiento(nuevaFila, nuevaCol, pasoActual, camino);
					caballoEnumeracion(nuevaFila, nuevaCol, filaDestino, colDestino, pasoActual + 1, camino);
					deshacerMovimiento(nuevaFila, nuevaCol, camino);
				}
			}
		}
}


void caballoDecision(int filaActual, int colActual, int filaDestino, int colDestino, int pasoActual, int **camino, bool &encontreSolucion)
{
	if (!encontreSolucion){

		if (esSolucion(filaActual, colActual, filaDestino, colDestino))
			//imprimirSolucion(camino);
			encontreSolucion = true;
		else
		{
			// cout << "Running..." << endl; // para ver que el programa no se cuelga, imaginense tratar de debugguear!!!!
			int movimientosFila[] = {-2, -1, 1, 2, 2, 1, -1, -2};
			int movimientosCol[] = {-1, -2, -2, -1, 1, 2, 2, 1};
			for (int opcionMovimiento = 0; opcionMovimiento < 8; opcionMovimiento++)
			{
				int nuevaFila = filaActual + movimientosFila[opcionMovimiento];
				int nuevaCol = colActual + movimientosCol[opcionMovimiento];
				if (esMovimientoValido(nuevaFila, nuevaCol, camino))
				{
					realizarMovimiento(nuevaFila, nuevaCol, pasoActual, camino);
					caballoDecision(nuevaFila, nuevaCol, filaDestino, colDestino, pasoActual + 1, camino, encontreSolucion);
					deshacerMovimiento(nuevaFila, nuevaCol, camino);
				}
			}
		}
	}
}


bool esMejorSolucion(int pasoActual, int mejorSolucion) {
	return pasoActual < mejorSolucion;
}

// Cuidado con podar de mas, podamos solamente si excede la mejor solucion
bool hayQuePodar(int pasoActual, int mejorSolucion) {
	return pasoActual > mejorSolucion;
}



// Cual es la metrica que usamos para decidir si es mejor solucion que la anterior?
void caballoOptimizacion(int filaActual, int colActual, int filaDestino, int colDestino, int pasoActual, int **camino, int &mejorSolucion)
{
	if(!hayQuePodar(pasoActual, mejorSolucion)) {
		if (esSolucion(filaActual, colActual, filaDestino, colDestino) && esMejorSolucion(pasoActual, mejorSolucion)) {
			imprimirSolucion(camino);
			mejorSolucion = pasoActual;
		}
		else
		{
			// cout << "Running..." << endl; // para ver que el programa no se cuelga, imaginense tratar de debugguear!!!
			int movimientosFila[] = {-2, -1, 1, 2, 2, 1, -1, -2};
			int movimientosCol[] = {-1, -2, -2, -1, 1, 2, 2, 1};
			for (int opcionMovimiento = 0; opcionMovimiento < 8; opcionMovimiento++)
			{
				int nuevaFila = filaActual + movimientosFila[opcionMovimiento];
				int nuevaCol = colActual + movimientosCol[opcionMovimiento];
				if (esMovimientoValido(nuevaFila, nuevaCol, camino))
				{
					realizarMovimiento(nuevaFila, nuevaCol, pasoActual, camino);
					caballoOptimizacion(nuevaFila, nuevaCol, filaDestino, colDestino, pasoActual + 1, camino, mejorSolucion);
					deshacerMovimiento(nuevaFila, nuevaCol, camino);
				}
			}
		}
	}
}

void copiarSolucion(int ** camino, int ** mejorCamino) {
	for (int fila = 0; fila < celdas; fila++) {
		for (int col = 0; col < celdas; col++) {
			mejorCamino[fila][col] = camino[fila][col];
		}
	}
}

// Cual es la metrica que usamos para decidir si es mejor solucion que la anterior?
void caballoOptimizacionConCopiaDeMejorCamino(int filaActual, int colActual, int filaDestino, int colDestino, int pasoActual, int **camino, int &mejorSolucion, int **mejorCamino)
{
	if (!hayQuePodar(pasoActual, mejorSolucion))
	{
		if (esSolucion(filaActual, colActual, filaDestino, colDestino) && esMejorSolucion(pasoActual, mejorSolucion))
		{
			copiarSolucion(camino, mejorCamino);
			mejorSolucion = pasoActual;
		}
		else
		{
			// cout << "Running..." << endl; // para ver que el programa no se cuelga, imaginense tratar de debugguear!!!!
			int movimientosFila[] = {-2, -1, 1, 2, 2, 1, -1, -2};
			int movimientosCol[] = {-1, -2, -2, -1, 1, 2, 2, 1};
			for (int opcionMovimiento = 0; opcionMovimiento < 8; opcionMovimiento++)
			{
				int nuevaFila = filaActual + movimientosFila[opcionMovimiento];
				int nuevaCol = colActual + movimientosCol[opcionMovimiento];
				if (esMovimientoValido(nuevaFila, nuevaCol, camino))
				{
					realizarMovimiento(nuevaFila, nuevaCol, pasoActual, camino);
					caballoOptimizacionConCopiaDeMejorCamino(nuevaFila, nuevaCol, filaDestino, colDestino, pasoActual + 1, camino, mejorSolucion, mejorCamino);
					deshacerMovimiento(nuevaFila, nuevaCol, camino);
				}
			}
		}
	}
}

bool pasePorObligatorias(int **camino, bool** celdasObligatorias) {
	for (int fila = 0; fila < celdas; fila++) {
		for (int col = 0; col < celdas; col++) {
			if (celdasObligatorias[fila][col] && !pasePorCelda(fila, col, camino)) return false;
		}
	}
	return true;
}

bool esSolucionRestriccion(int filaActual, int colActual, int filaDestino, int colDestino, int **camino, bool** celdasObligatorias) {
	return esSolucion(filaActual, colActual, filaDestino, colDestino) && pasePorObligatorias(camino, celdasObligatorias);
}


void caballoOptimizacionRestricciones(int filaActual, int colActual, int filaDestino, int colDestino, int pasoActual, int **camino, int &mejorSolucion, bool** celdasObligatorias)
{
	if (!hayQuePodar(pasoActual, mejorSolucion)) 
	{
		if (esSolucionRestriccion(filaActual, colActual, filaDestino, colDestino, camino, celdasObligatorias) && esMejorSolucion(pasoActual, mejorSolucion))
		{
			imprimirSolucion(camino);
			mejorSolucion = pasoActual;
		}
		else
		{
			// cout << "Running..." << endl; // para ver que el programa no se cuelga, imaginense tratar de debugguear!!!!
			int movimientosFila[] = {-2, -1, 1, 2, 2, 1, -1, -2};
			int movimientosCol[] = {-1, -2, -2, -1, 1, 2, 2, 1};
			for (int opcionMovimiento = 0; opcionMovimiento < 8; opcionMovimiento++)
			{
				int nuevaFila = filaActual + movimientosFila[opcionMovimiento];
				int nuevaCol = colActual + movimientosCol[opcionMovimiento];
				if (esMovimientoValido(nuevaFila, nuevaCol, camino))
				{
					realizarMovimiento(nuevaFila, nuevaCol, pasoActual, camino);
					caballoOptimizacionRestricciones(nuevaFila, nuevaCol, filaDestino, colDestino, pasoActual + 1, camino, mejorSolucion, celdasObligatorias);
					deshacerMovimiento(nuevaFila, nuevaCol, camino);
				}
			}
		}
	}
}

// puedo encontrar una mejor solucion o una solucion igual que la que tenemos hasta ahora
bool esMejorOIgualSolucion(int pasoActual, int mejorSolucion) {
	return pasoActual <= mejorSolucion;
}

// Ahora tenemos varias mejores soluciones
void caballoOptimizacionEnumeracion(int filaActual, int colActual, int filaDestino, int colDestino, int pasoActual, int **camino, int &mejorSolucion, Lista * mejoresCaminos)
{
	// si el podar incluye que pasoActual <= mejorSolucion, entonces no se encontraran mejores soluciones porque las que son iguales nunca entran en el if
	if (!hayQuePodar(pasoActual, mejorSolucion))
	{
		// si es solucion y es mejor o igual que la mejor solucion que tenemos hasta ahora
		if (esSolucion(filaActual, colActual, filaDestino, colDestino) && esMejorOIgualSolucion(pasoActual, mejorSolucion))
		{
			// si es mejor que la mejor solucion que tenemos hasta ahora
			if (esMejorSolucion(pasoActual, mejorSolucion)) {
				// borramos todas las que tenemos hasta ahora (otra alternativa puede ser crear una nueva lista cada vez que encontramos una mejor solucion, precisa referencia)
				while(mejoresCaminos->hayElementos()) mejoresCaminos->eliminarPrimera();
				mejorSolucion = pasoActual;
			}
			// agregamos la solucion a la lista, el agregar se encarga de copiar la solucion
			mejoresCaminos->agregar(camino);
		}
		else
		{
			int movimientosFila[] = {-2, -1, 1, 2, 2, 1, -1, -2};
			int movimientosCol[] = {-1, -2, -2, -1, 1, 2, 2, 1};
			for (int opcionMovimiento = 0; opcionMovimiento < 8; opcionMovimiento++)
			{
				int nuevaFila = filaActual + movimientosFila[opcionMovimiento];
				int nuevaCol = colActual + movimientosCol[opcionMovimiento];
				if (esMovimientoValido(nuevaFila, nuevaCol, camino))
				{
					realizarMovimiento(nuevaFila, nuevaCol, pasoActual, camino);
					caballoOptimizacionEnumeracion(nuevaFila, nuevaCol, filaDestino, colDestino, pasoActual + 1, camino, mejorSolucion, mejoresCaminos);
					deshacerMovimiento(nuevaFila, nuevaCol, camino);
				}
			}
		}
	}
}

int main(){
	// inicializar las estructuaras de datos y variables
	int ** camino = new int*[celdas];
	for (int fila = 0; fila < celdas; fila++) {
		camino[fila] = new int[celdas]();
	}
	int filaInicial = 0;
	int colInicial = 0;
	int pasoActual = 1;
	int filaDestino = 7;
	int colDestino = 7;
	realizarMovimiento(filaInicial, colInicial, pasoActual, camino);

	// Llamar a funcion de backtracking	


	// ENMERACION
	//caballoEnumeracion(filaInicial, colInicial, filaDestino, colDestino, pasoActual + 1, camino);

	// DECISION
	//bool encontreSolucion = false;
	//caballoDecision(filaInicial, colInicial, filaDestino, colDestino, pasoActual + 1, camino, encontreSolucion);
	//if (!encontreSolucion) cout << "No se encontro solucion" << endl;
	//else cout << "Se encontro solucion" << endl;

	// OPTIMIZACION
	//int mejorSolucion = INT_MAX;
	//caballoOptimizacion(filaInicial, colInicial, filaDestino, colDestino, pasoActual + 1, camino, mejorSolucion);
	//imprimirSolucion(camino); // no imprime la solucion encontrada porque sigue trabajando sobre el camino y termina saliendo en el estado en que entra a la funcion

	// OPTIMIZACION CON COPIA DE MEJOR CAMINO
	/*
	int mejorSolucion = INT_MAX;
	int ** mejorCamino = new int*[celdas];
	for (int fila = 0; fila < celdas; fila++) {
		mejorCamino[fila] = new int[celdas]();
	}
	caballoOptimizacionConCopiaDeMejorCamino(filaInicial, colInicial, filaDestino, colDestino, pasoActual + 1, camino, mejorSolucion, mejorCamino);
	imprimirSolucion(mejorCamino);*/


	// RESTRICCIONES 
	// Imponemos restricciones de que el caballo deba pasar por algunas celdas obligatoriamente.
	// Usamos una matriz del mismo tamaño para marcar las celdas obligatorias (matriz booleana)
	/*
	bool ** celdasObligatorias = new bool*[celdas];
	for (int fila = 0; fila < celdas; fila++) {
		celdasObligatorias[fila] = new bool[celdas]();
	}
	celdasObligatorias[5][2] = true;
	celdasObligatorias[3][2] = true;

	int mejorSolucion = INT_MAX;
	caballoOptimizacionRestricciones(filaInicial, colInicial, filaDestino, colDestino, pasoActual + 1, camino, mejorSolucion, celdasObligatorias);
	*/

	// Si tuvieramos celdas que no se pueden visitar, ¿donde impacta en el codigo? ---> en la funcion esMovimientoValido 


	// ENUMERACION OPTIMIZACION
	Lista * mejoresCaminos = new Lista(); 
	int mejorSolucion = INT_MAX;
	caballoOptimizacionEnumeracion(filaInicial, colInicial, filaDestino, colDestino, pasoActual + 1, camino, mejorSolucion, mejoresCaminos);
	if(mejoresCaminos != NULL) {
		cout << "Se encontraron " << mejoresCaminos->obtenerCantElementos() << " soluciones" << endl;
		cout << "Mejores soluciones encontradas:" << endl;
		while (mejoresCaminos->hayElementos()) {
			imprimirSolucion(mejoresCaminos->obtenerPrimera());
			mejoresCaminos->eliminarPrimera();
		}
	}
	else cout << "No se encontro solucion" << endl; // si luego de realizar backtracking es NULL, es porque no se encontro solucion
	return 0;
}