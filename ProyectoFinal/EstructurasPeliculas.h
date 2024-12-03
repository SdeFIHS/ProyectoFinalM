//Estructura para almacenar los estados de los asientos de una sala en cada funcion
struct salas{
    char salaA[10][10],
    salaB[10][10],
    salaC[8][6];
};

//Estructura para definir las caracteristicas de una funcion
struct funcion{
    int existe; //Si es 0 la funcion no existe
    char fecha[9],
    horario[5];
    struct salas sala;
};

//Estructura para definir las caracteristicas de una pelicula
struct pelicula{
    int indice, numeroDeComentarios;
    char nombre[21],
    sinopsis[201],
    duracion[6],
    sala;
    struct funcion funciones[20];
};