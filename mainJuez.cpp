#include <iostream>
#include <vector>
#include <queue>
#include <string>

using namespace std;

int INF = -1;

//Funcion que verifica si una poscion se sale de la matriz
//Pre: toma una posicion, la cnatidad de filas y la cantidad de columnas de la matriz
//Post: Devuelve bool que muestra si la posicion se encutra dentro del rango de la matriz
//Complejidad. O(1)
bool posValida(int x, int y, int cantFilas, int cantColumnas) {
    return x >= 0 && x < cantFilas && y >= 0 && y < cantColumnas;
}

// Función que verifica si hay una manifestación en una posición en un momento dado.
//Pre:Toma una posicon, una matriz con las manisfestaciones y el tiempo actual
//Post: Retorna true si hay una manifestación en la posición (x, y) en el momento tiempo,
//Da false si aun no hubo una manifestacion o si nunca va a haber una manifestacion
//Complejidad O(1)
bool hayManifestacion(int x, int y, vector<vector<int>> manifestaciones, int tiempo) {
    return manifestaciones[x][y] != 0 && manifestaciones[x][y] <= tiempo;
}

//Función que verifica si una posición ha sido visitada en la matriz de distancias
//Pre: Toma una posicion y la matriz generado del bfs
//Post verifica si la poscion ya ha sido visitado (O sea aun no es INF)
//Complejidad O(1)
bool visitado(int x, int y, vector<vector<int>> matriz) {
    return matriz[x][y] != INF;
}

//Función que calcula el tiempo utilizando el algoritmo BFS desde posOrigen hasta posDestino.
//Pre: requiere la matriz con las manisfestaciones, la posicion de origen (paciente u hospital), la posicion de destino (paciente u hospital)
//Y el tiempo inicial (0 si sale del hospital u otro numero si sale desde el paciente)
//Post: Retorna el tiempo mínimo requerido para llegar desde posOrigen hasta posDestino O INF en caso contrario.
//Complejidad: O(N*M) en el peor de los casos toda la matriz es accesible y la poscion origen y poscion destino se encuentran en las dos puntas del BFS
//El agotimo en si termina en el mejor caso cuando encuentra la poscion destino
int devolverTiempoBFS(vector<vector<int>> &manifestaciones, pair<int, int> posOrigen, pair<int, int> posDestino, int tiempoInicial) {

    //Se verifica de antemano si la posicon origen y poscion destino son igales. Si asi lo son no hace flata hacer BFS
    if (posOrigen == posDestino) {
        return 0;
    }

    //Se toma la cantidad de filas y columnas
    int n = manifestaciones.size();
    int m = manifestaciones[0].size();

    //Se crea una cola para poder hacer BFS a traves de la matriz
    //Cada elemento se compone la posicion en la matriz y el tiempo actual del BFS
    queue<pair<pair<int, int>, int>> nodosPendientes;
    nodosPendientes.push(make_pair(posOrigen, tiempoInicial));

    //Se crea una matriz con el mismo tamaño de la matriz manisfestaciones inicializada todo en inf, excepto el origen
    //Aca se va a realizar el registro del BFS
    vector<vector<int>> matrizDistancias(n, vector<int>(m, INF));
    matrizDistancias[posOrigen.first][posOrigen.second] = tiempoInicial;

    //Por cada coordenada en la matriz, se pueden hacer 4 movimientos (Arriba, izquierda, abajo, derecha)
    vector<pair<int, int>> movimientos = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    //Se para el algoritmo cuando no haya mas nodos en la cola o hasta encontrar la poscion destino
    while (!nodosPendientes.empty()) {

        //Siempre se toma el primer elemento de la cola
        pair<int, int> posActual = nodosPendientes.front().first;
        int tiempoActual = nodosPendientes.front().second;
        nodosPendientes.pop();

        //Se itera por cada movimeinto posible
        for (int i = 0; i < 4; i++) {
            int nuevoX = posActual.first + movimientos[i].first;
            int nuevaY = posActual.second + movimientos[i].second;

            //Se verifica si la nueva posicion dada se enucentra dentro del rango la matriz,
            //Aun no ha sido visitada en el BFS
            //y si se puede pasar (o sea no hay una manifestacion en curso)
            if (posValida(nuevoX, nuevaY, n, m) && !visitado(nuevoX, nuevaY, matrizDistancias)) {
                if (!hayManifestacion(nuevoX, nuevaY, manifestaciones, tiempoActual + 1)) {
                    if (posDestino == make_pair(nuevoX, nuevaY)) {
                        //Si la nueva poscion justo es el destino, se deuvle el tiempo que tendria en la matrizDistancias
                        return tiempoActual + 1;
                    }
                    //Si no es la poscion destino, entonces se sigue haciendo BFS y se la guarda en la cola
                    nodosPendientes.push(make_pair(make_pair(nuevoX, nuevaY), tiempoActual + 1));
                    matrizDistancias[nuevoX][nuevaY] = tiempoActual + 1;
                }
            }
        }
    }
    //En caso de que la cola no tenga mas elementos y no se llego a la posicon destino, se devuelve INFINITO
    return INF;
}


// Función que calcula el tiempo de ida y vuelta entre el hospital y el paciente.
//Pre: toma la matriz de las manifestaciones, la poscion del hospital, del paciente y un vector donde guardar los resultados finales
//Post: Devuelve el tiempo que tarda en llegar de ida y vuelta o imposible si no posible establecer el camino
//Complejidad: Se realizan dos BFS, asi que seria O (2*(N*M))
void calcularTiempo(vector<vector<int>> manifestaciones, pair<int, int> posHospital, pair<int, int> posPaciente, vector<string> &resultados) {
    int tiempoIDA = devolverTiempoBFS(manifestaciones, posHospital, posPaciente, 0);
    //En caso de que da INF, entonces no se puede llegar al paciente
    if (tiempoIDA == INF) {
        resultados.push_back("IMPOSIBLE");
    } else {
        int tiempoVuelta = devolverTiempoBFS(manifestaciones, posPaciente, posHospital, tiempoIDA);
        //En caso de que da INF, no es posible volver al hospital
        if (tiempoVuelta == INF) {
            resultados.push_back("IMPOSIBLE");
        } else {
            //Sino, es posible establecer camino de ida y de vuelta
            resultados.push_back(to_string(tiempoIDA) + " " + to_string(tiempoVuelta));
        }
    }
}


int main() {
    int cantTests;
    cin >> cantTests;

    //Se guarda en un vector cada resultado de cada test
    vector<string> resultados;

    //Se itera por cada test
    for (int test = 0; test < cantTests; test++) {
        int cantFilas, cantColumnas;
        cin >> cantFilas >> cantColumnas;

        //Se crea la matriz con las manisfestaciones
        vector<vector<int>> matrizManifestaciones(cantFilas, vector<int>(cantColumnas, 0));
        for (int i = 0; i < cantFilas; i++) {
            for (int j = 0; j < cantColumnas; j++) {
                cin >> matrizManifestaciones[i][j];
            }
        }

        //Se guarda la posicion del hospital y del paicente
        pair<int, int> posPaciente, posHospital;
        for (int i = 0; i < 2; i++) {
            int posX, posY;
            cin >> posX >> posY;
            if (i == 0) {
                posHospital = make_pair(posX, posY);
            } else {
                posPaciente = make_pair(posX, posY);
            }
        }

        //Se verifica de que la matriz es posible crearla y
        //de que la poscion del paciente y del hospital esta dentro de los rango la matriz
        if (cantColumnas <= 0 || cantFilas <= 0) {
            resultados.push_back("IMPOSIBLE");
        } else {
            if (!posValida(posPaciente.first, posPaciente.second, cantFilas, cantColumnas) || !posValida(posHospital.first, posHospital.second, cantFilas, cantColumnas)) {
                resultados.push_back("IMPOSIBLE");
            } else {
                calcularTiempo(matrizManifestaciones, posHospital, posPaciente, resultados);
            }
        }
    }

    for (const string &resultado : resultados) {
        cout << resultado << endl;
    }

    return 0;
}