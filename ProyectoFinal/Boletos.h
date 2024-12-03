#include <stdio.h>
#include <string.h>
#include "EstructurasPeliculas.h"

int indiceUltimaPeliculaComprada = 0;

struct boleto{
    char clave[51];
};

struct s_peliculaConBoletos{
    int indicePelicula, cantidadDeAsientos;
    char asientos[100], nombre[21];
    struct boleto boletos[200];
};

struct s_peliVista{
    int indicePelicula;
    char nombre[21];
};

struct registroUsuario{
    char nombreUsuario[31];
    int numPelisConBoletos, numPelisVistas;
    struct s_peliculaConBoletos peliculasConBoletos[50];
    struct s_peliVista pelisVistas[50]; 
};

int existePelicula(struct registroUsuario* usuario, int indicePelicula) {
    for (int i = 0; i < usuario->numPelisConBoletos; i++) {
        if (usuario->peliculasConBoletos[i].indicePelicula == indicePelicula) {
            indiceUltimaPeliculaComprada = i;
            return 1;
        }
    }
    return 0;
}

void agregarPelicula(struct registroUsuario* usuario, int indicePelicula) {
        struct s_peliculaConBoletos nuevaPelicula;
        nuevaPelicula.indicePelicula = indicePelicula;
        nuevaPelicula.cantidadDeAsientos = 0;
        strcpy(nuevaPelicula.asientos, "");

        // Leer la película desde el archivo
        struct pelicula peliculaDeseada;
        FILE* archivoPeliculas = fopen("peliculas.txt", "rb");
        if (archivoPeliculas != NULL) {
            fseek(archivoPeliculas, indicePelicula * sizeof(struct pelicula), SEEK_SET);
            fread(&peliculaDeseada, sizeof(struct pelicula), 1, archivoPeliculas);
            fclose(archivoPeliculas);

            strcpy(nuevaPelicula.nombre, peliculaDeseada.nombre);
        } else {
            printf("No se pudo abrir el archivo.\n");
        }

        usuario->peliculasConBoletos[usuario->numPelisConBoletos] = nuevaPelicula;
        indiceUltimaPeliculaComprada = usuario->numPelisConBoletos;
        usuario->numPelisConBoletos++;
}

void generarBoleto(char nickName[16], char clave[51], int indicePelicula, char nombreUsuario[31], char asiento[3]){
    printf("%s\n", clave);

    FILE* archivoRegistro = fopen(nickName, "rb");
    //Si no hay un registro para el usuario, crearlo
    if (archivoRegistro == NULL){
        archivoRegistro = fopen(nickName, "wb");
        struct registroUsuario nuevoRegistro;
        strcpy(nuevoRegistro.nombreUsuario, nombreUsuario);
        nuevoRegistro.numPelisConBoletos = 0;
        nuevoRegistro.numPelisVistas = 0;
        fwrite(&nuevoRegistro, sizeof(struct registroUsuario), 1, archivoRegistro);
        fclose(archivoRegistro);
        archivoRegistro = fopen(nickName, "rb");
    }
    struct registroUsuario usuario;
    fread(&usuario, sizeof(struct registroUsuario), 1, archivoRegistro);
    fclose(archivoRegistro);

    //Si el cliente no tiene mas boletos comprados para esa pelicula, crear el registro
    if (existePelicula(&usuario, indicePelicula)) {
    } else {
        agregarPelicula(&usuario, indicePelicula);
    }
    /*int i = 0;
    while (usuario.peliculasConBoletos[i].indicePelicula != indicePelicula){
        printf("DEBUG: %i == %i\n", i < usuario.numPelisConBoletos && usuario.peliculasConBoletos[i].indicePelicula, indicePelicula);
        if (i == usuario.numPelisConBoletos){
            printf("DEBUG: se creo nuevo registro de peli\n");
            usuario.numPelisConBoletos++;
            printf("DEBUG: PELIS CON BOLETOS = %i", usuario.numPelisConBoletos);
            usuario.peliculasConBoletos[i].indicePelicula = indicePelicula;
            usuario.peliculasConBoletos[i].cantidadDeAsientos = 0;
            strcpy(usuario.peliculasConBoletos[i].asientos, "");

            struct pelicula peliculaDeseada;
            FILE* archivoPeliculas = fopen("peliculas.txt", "rb");
            fseek(archivoPeliculas, indicePelicula * sizeof(struct pelicula), SEEK_SET);
            fread(&peliculaDeseada, sizeof(struct pelicula), 1, archivoPeliculas);
            fclose(archivoPeliculas);

            strcpy(usuario.peliculasConBoletos[i].nombre, peliculaDeseada.nombre);
            
            break;
        }
        i++;
    }*/
    strcpy(usuario.peliculasConBoletos[indiceUltimaPeliculaComprada].boletos[usuario.peliculasConBoletos[indiceUltimaPeliculaComprada].cantidadDeAsientos].clave, clave);
    usuario.peliculasConBoletos[indiceUltimaPeliculaComprada].cantidadDeAsientos++;
    strcat(usuario.peliculasConBoletos[indiceUltimaPeliculaComprada].asientos, asiento);
    strcat(usuario.peliculasConBoletos[indiceUltimaPeliculaComprada].asientos, " ");

    archivoRegistro = fopen(nickName, "wb");
    fwrite(&usuario, sizeof(struct registroUsuario), 1, archivoRegistro);
    fclose(archivoRegistro);
}

void verBoletosDeUsuario(char nickName[16]){
    
    struct registroUsuario registro;
    FILE* archivo = fopen(nickName, "rb");
    fread(&registro, sizeof(struct registroUsuario), 1, archivo);
    fclose(archivo);

    printf("\n\nNickname: %s\nNombre: %s\n\nPeliculas para las que ha comprado boletos:\n", nickName, registro.nombreUsuario);

    struct s_peliculaConBoletos peliculaLeida;
    for (int i = 0; i < registro.numPelisConBoletos; i++){
        peliculaLeida = registro.peliculasConBoletos[i];
        printf("\n -> %s \n\tCantidad de boletos: %i\n\tAsientos: %s\n", peliculaLeida.nombre, peliculaLeida.cantidadDeAsientos, peliculaLeida.asientos);
    }
}

void peliVista(char nickName[16], int indice, char nombre[21]){
    struct registroUsuario registro;
    FILE* archivo = fopen(nickName, "rb");
    fread(&registro, sizeof(struct registroUsuario), 1, archivo);
    fclose(archivo);

    registro.pelisVistas[registro.numPelisVistas].indicePelicula = indice;
    strcpy(registro.pelisVistas[registro.numPelisVistas].nombre, nombre);
    registro.numPelisVistas++;

    archivo = fopen(nickName, "wb");
    fwrite(&registro, sizeof(struct registroUsuario), 1, archivo);
    fclose(archivo);
}