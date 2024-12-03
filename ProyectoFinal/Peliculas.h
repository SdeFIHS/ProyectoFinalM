#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>
#include "Boletos.h"

int numeroDePeliculas = 0;

void InicializarSalaA(struct salas *sala){
    for (int i = 0 ; i < 10 ; i++){
        for (int j = 0 ; j < 10 ; j++){
            sala->salaA[i][j] = '0';
        }
    }
}

void InicializarSalaB(struct salas *sala){
    for (int i = 0 ; i < 10 ; i++){
        for (int j = 0 ; j < 10 ; j++){
            if ((i < 4 && j < 7) || (i >= 4)){
                sala->salaB[i][j] = '0';
            } else {
                sala->salaB[i][j] = ' ';
            }
        }
    }
}

void InicializarSalaC(struct salas *sala){
    for (int i = 0 ; i < 8 ; i++){
        for (int j = 0 ; j < 6 ; j++){
            sala->salaC[i][j] = '0';
        }
    }
}


//Pide los datos necesarios, y agrega la pelicula al registro
void darDeAltaPelicula(){
    struct pelicula nuevaPelicula;

    nuevaPelicula.numeroDeComentarios = 0;
    //Pedir datos
    printf("Nombre: ");
    fgets(nuevaPelicula.nombre, 20, stdin);
    fflush(stdin);
    printf("Sinopsis: ");
    fgets(nuevaPelicula.sinopsis, 200, stdin);
    fflush(stdin);
    printf("Duracion (En formato hh:mm): ");
    scanf("%s", &nuevaPelicula.duracion);
    fflush(stdin);
    printf("Sala en que se exhibira (A, B o C): ");
    scanf("%c", &nuevaPelicula.sala);
    fflush(stdin);
    char deseaOtraFuncion = '\0';
    int indiceFuncion = 0;
    do{
        printf("\n FUNCION %i \n", indiceFuncion);
        printf("\nFecha (En formato AAAAMMDD): ");
        scanf("%s", &nuevaPelicula.funciones[indiceFuncion].fecha);
        fflush(stdin);
        printf("\nHora (En formato hhmm): ");
        scanf("%s", &nuevaPelicula.funciones[indiceFuncion].horario);
        fflush(stdin);
        printf("\nDesea agregar otra funcion? (S/N)\n");
        scanf("%c", &deseaOtraFuncion);
        fflush(stdin);

        switch(nuevaPelicula.sala){
            case 'A':
                InicializarSalaA(&nuevaPelicula.funciones[indiceFuncion].sala);
                break;
            case 'B':
                InicializarSalaB(&nuevaPelicula.funciones[indiceFuncion].sala);
                break;
            case 'C':
                InicializarSalaC(&nuevaPelicula.funciones[indiceFuncion].sala);
                break;
            default:
                printf("No se encontro la sala %c", nuevaPelicula.sala);
                return;
        }

        nuevaPelicula.funciones[indiceFuncion].existe = 1;
        indiceFuncion++;
    }
    while (deseaOtraFuncion == 'S' && indiceFuncion <= 20);
    nuevaPelicula.funciones[indiceFuncion].existe = 0;

    //Agregar pelicula al registro
    FILE* archivoPeliculas = fopen("peliculas.txt", "ab");
    if (archivoPeliculas != NULL) {
        nuevaPelicula.indice = numeroDePeliculas++;
        fwrite(&nuevaPelicula, sizeof(struct pelicula), 1, archivoPeliculas);
        fclose(archivoPeliculas);
    }
    else printf("No se pudo registrar la pelicula");
}

//Imprime la lista de peliculas programadas
void mostrarPeliculas(){
    struct pelicula peliculaLeida;
    FILE* archivoPeliculas = fopen("peliculas.txt", "rb");

    while (fread(&peliculaLeida, sizeof(struct pelicula), 1, archivoPeliculas)){
        printf("\n %i.- %s\n", peliculaLeida.indice, peliculaLeida.nombre);
    }
    fclose(archivoPeliculas);
}

//Muestra la informacion y funciones de una pelicula
void detallesDePelicula(struct pelicula peliculaDeseada){
    printf("\n\n%s\n\n%s\nDuracion:%s\nSala %c", peliculaDeseada.nombre, peliculaDeseada.sinopsis, peliculaDeseada.duracion, peliculaDeseada.sala);
    printf("\n\nFunciones:\n\n");
    int i = 0;
    while (peliculaDeseada.funciones[i].existe){
        printf(" - Funcion %i\nHora: %s\tFecha: %s\n\n", i, peliculaDeseada.funciones[i].horario, peliculaDeseada.funciones[i].fecha);
        i++;
    }
}

char asientosGlobal[30] = {0}; //Se pueden comprar hasta 10 lugares a la vez

void eliminarAsiento(char *asiento) {
    char *pos;
    int len_asiento = strlen(asiento);

    //Guardar posicion en el arreglo del asiento por eliminar
    if ((pos = strstr(asientosGlobal, asiento)) != NULL) {
        // Mover el resto de caracteres hacia atrás para eliminar el asiento
        memmove(pos, pos + len_asiento, strlen(pos + len_asiento) + 1);
    }
}

//Comprueba si los lugares son validos y reserva todos los lugares validos seleccionados, devuelve la cantidad de asientos comprados
int reservarLugaresA(struct salas *salas){
    int asientosComprados = 0;
    fgets(asientosGlobal, 30, stdin);
    asientosGlobal[strlen(asientosGlobal)-1] = 0;
    char asientos[30] = "";
    strcpy(asientos, asientosGlobal);
    char *asiento = strtok(asientos, " ");
    while (asiento != NULL) {
        int i = asiento[0] - 'A';
        int j = asiento[1] - '0';
        if (salas->salaA[i][j] == '0'){
            asientosComprados++;
            salas->salaA[i][j] = 'X';
            printf("El asiento %s es valido\n", asiento);
        }
        else {
            printf("El asiento %s NO es valido\n", asiento);
            eliminarAsiento(asiento);
        }
        asiento = strtok(NULL, " ");
    }
    return asientosComprados;
}

int reservarLugaresB(struct salas *salas){
    int asientosComprados = 0;
    fgets(asientosGlobal, 30, stdin);
    asientosGlobal[strlen(asientosGlobal)-1] = 0;
    char asientos[30] = "";
    strcpy(asientos, asientosGlobal);
    char *asiento = strtok(asientos, " ");
    while (asiento != NULL) {
        int i = asiento[0] - 'A';
        int j = asiento[1] - '0';
        if (salas->salaB[i][j] == '0'){
            asientosComprados++;
            salas->salaB[i][j] = 'X';
            printf("El asiento %s es valido\n", asiento);
        }
        else {
            printf("El asiento %s NO es valido\n", asiento);
            eliminarAsiento(asiento);
        }
        asiento = strtok(NULL, " ");
    }
    return asientosComprados;
}

int reservarLugaresC(struct salas *salas){
    int asientosComprados = 0;
    fgets(asientosGlobal, 30, stdin);
    asientosGlobal[strlen(asientosGlobal)-1] = 0;
    char asientos[30] = "";
    strcpy(asientos, asientosGlobal);
    char *asiento = strtok(asientos, " ");
    while (asiento != NULL) {
        int i = asiento[0] - 'A';
        int j = asiento[1] - '0';
        if (salas->salaC[i][j] == '0'){
            asientosComprados++;
            salas->salaC[i][j] = 'X';
            printf("El asiento %s es valido\n", asiento);
        }
        else {
            printf("El asiento %s NO es valido\n", asiento);
            eliminarAsiento(asiento);
        }
        asiento = strtok(NULL, " ");
    }
    return asientosComprados;
}

//Pantalla de carga y se llama funcion generarBoleto()
void cargo(){
    printf("\nSe hara el cargo a la tarjeta de credito registrada\n");
    char barra[11] = {176, 176, 176, 176, 176, 176, 176, 176, 176, 176};
    int tiempo = ((rand() % 5) + 3)*100; //Pueden decir lo que quieran del resto de mi codigo todo feo, pero no se metan con mi maravillosa linea que escoge un tiempo al azar
    //quedo bien padre apoco no
    for (int i = 0; i < 10; i++){
        Sleep(tiempo);
        printf("\33[2K\r");
        barra[i] = 178;
        printf("%s", barra);
    }
    printf("\33[2K\r");
}

void AsientosSalaA(struct salas *salas){
    printf("Bienvenido a la Sala A\n\n");
    printf("   ");
    for (int i = 0; i < 10; i++){
        printf("%i  ", i);
    }
    for (int i = 0 ; i < 10 ; i++){
        printf("\n");
        printf("%c: ",'A' + i);
        for(int j=0;j<10;j++){
            printf("%c  ", salas->salaA[i][j]);
        }
    }
    do{
        printf("\n\nSeleccione los asientos que desea, separados por espacio (Por ejemplo: H7 H8 H9): ");
    }while (!reservarLugaresA(salas));
}

void AsientosSalaB(struct salas *salas){
    printf("Bienvenido a la Sala B\n\n");
    printf("   ");
    for (int i = 0; i < 10; i++){
        printf("%i  ", i);
    }
    for (int i = 0 ; i < 10 ; i++){
        printf("\n");
        printf("%c: ",'A' + i);
        for(int j=0;j<10;j++){
            printf("%c  ", salas->salaB[i][j]);
        }
    }
    do{
        printf("\n\nSeleccione los asientos que desea, separados por espacio (Por ejemplo: H7 H8 H9): ");
    }while (!reservarLugaresB(salas));
}

void AsientosSalaC(struct salas *salas){
    printf("Bienvenido a la Sala C\n\n");
    printf("   ");
    for (int i = 0; i < 6; i++){
        printf("%i  ", i);
    }
    for (int i = 0 ; i < 8 ; i++){
        printf("\n");
        printf("%c: ",'A' + i);
        for(int j=0;j<6;j++){
            printf("%c  ", salas->salaC[i][j]);
        }
    }
    do{
        printf("\n\nSeleccione los asientos que desea, separados por espacio (Por ejemplo: H7 H8 H9): ");
    }while (!reservarLugaresC(salas));
}

void seleccionarAsientos(char sala, struct salas *salas){
    switch(sala){
        case 'A':
            AsientosSalaA(salas);
            break;
        case 'B':
            AsientosSalaB(salas);
            break;
        case 'C':
            AsientosSalaC(salas);
            break;
        default:
            printf("No se encontro la sala %c", sala);
    }
}

char iniciales[10] = "";
void obtenerIniciales(char nombre[21]){
    strcpy(iniciales, "");
    char *palabra = strtok(nombre, " ");
    int i = 0;
    while (palabra != NULL) {
        iniciales[i++] = palabra[0];
        palabra = strtok(NULL, " ");
    }
}

void comprarBoletos(char nickName[16], char nombreUsuario[31]){
    mostrarPeliculas();
    printf("Ingrese el numero de la pelicula de la que desea comprar boletos: ");
    int indicePeliculaDeseada = 0;
    scanf("%i", &indicePeliculaDeseada);
    fflush(stdin);

    struct pelicula peliculaDeseada;
    FILE* archivoPeliculas = fopen("peliculas.txt", "rb");
    fseek(archivoPeliculas, indicePeliculaDeseada * sizeof(struct pelicula), SEEK_SET);
    fread(&peliculaDeseada, sizeof(struct pelicula), 1, archivoPeliculas);
    fclose(archivoPeliculas);

    detallesDePelicula(peliculaDeseada);
    printf("Seleccione el numero de la funcion para la cual quiere comprar boletos: ");
    int indiceFuncionDeseada = 0;
    scanf("%i", &indiceFuncionDeseada);
    fflush(stdin);
    struct funcion *funcionDeseada = &peliculaDeseada.funciones[indiceFuncionDeseada];
    seleccionarAsientos(peliculaDeseada.sala, &funcionDeseada->sala);

    //Actualizar valores en archivo
    archivoPeliculas = fopen("peliculas.txt", "r+b");
    fseek(archivoPeliculas, indicePeliculaDeseada * sizeof(struct pelicula), SEEK_SET);
    fwrite(&peliculaDeseada, sizeof(struct pelicula), 1, archivoPeliculas);
    fclose(archivoPeliculas);

    cargo();

    printf("Resumen de la compra:\nPelicula: %s\nFecha: %s\nHorario: %s\nAsientos: %s\n\n", peliculaDeseada.nombre, funcionDeseada->fecha, funcionDeseada->horario, asientosGlobal);

    obtenerIniciales(peliculaDeseada.nombre);
    printf("Boletos:\n");

    char salaArreglo[2] = {peliculaDeseada.sala, 0};
    char *asiento = strtok(asientosGlobal, " ");
    while (asiento != NULL) {
        char clave[51] = "";
        strcpy(clave, iniciales);
        strcat(clave, ":");
        strcat(clave, funcionDeseada->fecha);
        strcat(clave, ":");
        strcat(clave, funcionDeseada->horario);
        strcat(clave, ":");
        strcat(clave, salaArreglo);
        strcat(clave, ":");
        strcat(clave, asiento);
        generarBoleto(nickName, clave, peliculaDeseada.indice, nombreUsuario, asiento);
        asiento = strtok(NULL, " ");
    }
}