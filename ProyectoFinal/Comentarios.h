#include "Peliculas.h"

struct comentario{
    char nickName[16], comentario[201];
};

struct comentariosPelicula{
    int totalCalificaciones;
    float sumaCalificaciones, promedio;
    struct comentario comentarios[20];
};

void mostrarPeliculasConBoletos(char nickName[16]){
    struct registroUsuario registro;
    FILE* archivo = fopen(nickName, "rb");
    fread(&registro, sizeof(struct registroUsuario), 1, archivo);
    fclose(archivo);

    printf("Peliculas con boletos comprados:\n");
    for (int i = 0; i < registro.numPelisConBoletos; i++){
        printf("\n %i.- %s\n", registro.peliculasConBoletos[i].indicePelicula, registro.peliculasConBoletos[i].nombre);
    }
}

void mostrarPelisVistas(char nickName[16]){
    struct registroUsuario registro;
    FILE* archivo = fopen(nickName, "rb");
    fread(&registro, sizeof(struct registroUsuario), 1, archivo);
    fclose(archivo);

    for (int i = 0; i < registro.numPelisVistas; i++){
        printf("\n %i.- %s\n", registro.pelisVistas[i].indicePelicula, registro.pelisVistas[i].nombre);
    }
}

void agregarComentario(char nickName[16]){

    mostrarPeliculasConBoletos(nickName);

    printf("\nSeleccione el numero de la pelicula para la que desea dejar un comentario: ");
    int indicePelicula = 0;
    scanf("%i", &indicePelicula);
    fflush(stdin);
    
    struct pelicula s_pelicula;
    int numeroDeComentarios;
    FILE* archivoPeliculas = fopen("peliculas.txt", "rb");
    fseek(archivoPeliculas, indicePelicula * sizeof(struct pelicula), SEEK_SET);
    fread(&s_pelicula, sizeof(struct pelicula), 1, archivoPeliculas);
    numeroDeComentarios = s_pelicula.numeroDeComentarios++;
    fclose(archivoPeliculas);
    archivoPeliculas = fopen("peliculas.txt", "r+b");
    fseek(archivoPeliculas, indicePelicula * sizeof(struct pelicula), SEEK_SET);
    fwrite(&s_pelicula, sizeof(struct pelicula), 1, archivoPeliculas);

    char yaLaVio;
    printf("\nSeleccione una opcion:\n1.- Ya vi la pelicula\n2.- Estoy por ver la pelicula\n\nIngresar 1 o 2: ");
    scanf("%c", &yaLaVio);
    fflush(stdin);

    if (yaLaVio == '1') 
        peliVista(nickName, indicePelicula, s_pelicula.nombre);

    char comentario[202];

    do{
        printf("Escriba su comentario:\n");
        fgets(comentario, 202, stdin);
        fflush(stdin);
        comentario[201] = 0;
    }while (strlen(comentario) > 200);

    struct comentario nuevoComentario;
    strcpy(nuevoComentario.nickName, nickName);
    strcpy(nuevoComentario.comentario, comentario);

    struct comentariosPelicula registroComentarios;

    if (!numeroDeComentarios){
        registroComentarios.sumaCalificaciones = 0;
        registroComentarios.totalCalificaciones = 0;
    }

    FILE *archivoComentarios = fopen("comentarios.txt", "r+b");
    if (archivoComentarios == NULL){
        archivoComentarios = fopen("comentarios.txt", "wb");
        fclose(archivoComentarios);
        archivoComentarios = fopen("comentarios.txt", "r+b");
    }

    fseek(archivoComentarios, indicePelicula * sizeof(struct comentariosPelicula), SEEK_SET);
    fread(&registroComentarios, sizeof(struct comentariosPelicula), 1, archivoComentarios);

    registroComentarios.comentarios[numeroDeComentarios] = nuevoComentario;

    archivoComentarios = fopen("comentarios.txt", "r+b");
    fseek(archivoComentarios, indicePelicula * sizeof(struct comentariosPelicula), SEEK_SET);
    fwrite(&registroComentarios, sizeof(struct comentariosPelicula), 1, archivoComentarios);
}

void verComentarios(){
    mostrarPeliculas();
    printf("Ingrese el numero de la pelicula que desee ver los comentarios: ");
    int indicePelicula = 0;
    scanf("%i", &indicePelicula);
    fflush(stdin);
    
    struct pelicula s_pelicula;
    int numeroDeComentarios;
    FILE* archivoPeliculas = fopen("peliculas.txt", "rb");
    fseek(archivoPeliculas, indicePelicula * sizeof(struct pelicula), SEEK_SET);
    fread(&s_pelicula, sizeof(struct pelicula), 1, archivoPeliculas);
    numeroDeComentarios = s_pelicula.numeroDeComentarios;
    fclose(archivoPeliculas);

    if (!numeroDeComentarios){
        printf("No hay comentarios para esa pelicula");
        return;
    }
    
    struct comentariosPelicula registroComentarios;
    FILE *archivoComentarios = fopen("comentarios.txt", "r+b");
    if (archivoComentarios == NULL){
        archivoComentarios = fopen("comentarios.txt", "wb");
        fclose(archivoComentarios);
        archivoComentarios = fopen("comentarios.txt", "r+b");
    }
    fseek(archivoComentarios, indicePelicula * sizeof(struct comentariosPelicula), SEEK_SET);
    fread(&registroComentarios, sizeof(struct comentariosPelicula), 1, archivoComentarios);

    for (int i = 0; i < numeroDeComentarios; i++){
        printf("\n%s:\n%s\n", registroComentarios.comentarios[i].nickName, registroComentarios.comentarios[i].comentario);
    }
}

void calificarPelicula(char nickName[16]){
    printf("Peliculas vistas:\n");
    mostrarPelisVistas(nickName);
    printf("Ingrese el numero de la pelicula que va a calificar: ");
    int indicePelicula = 0;
    scanf("%i", &indicePelicula);
    fflush(stdin);

    struct comentariosPelicula registroComentarios;
    FILE *archivoComentarios = fopen("comentarios.txt", "rb");
    fseek(archivoComentarios, indicePelicula * sizeof(struct comentariosPelicula), SEEK_SET);
    fread(&registroComentarios, sizeof(struct comentariosPelicula), 1, archivoComentarios);
    fclose(archivoComentarios);

    float calificacion = 0;
    do{
        printf("Ingrese una calificacion de una a cinco estrellas: ");
        scanf("%f", &calificacion);}
    while (calificacion < 0 || calificacion > 5);
    registroComentarios.sumaCalificaciones += calificacion;
    registroComentarios.totalCalificaciones++;
    registroComentarios.promedio = registroComentarios.sumaCalificaciones / registroComentarios.totalCalificaciones;

    printf("Promedio actual: %.2f", registroComentarios.promedio);

    archivoComentarios = fopen("comentarios.txt", "r+b");
    fseek(archivoComentarios, indicePelicula * sizeof(struct comentariosPelicula), SEEK_SET);
    fwrite(&registroComentarios, sizeof(struct comentariosPelicula), 1, archivoComentarios);
}

void verTodasLasCalificaciones(){
    struct pelicula peliculaLeida;
    struct comentariosPelicula registroComentarios;
    FILE* archivoPeliculas = fopen("peliculas.txt", "rb");
    FILE* archivoComentarios = fopen("comentarios.txt", "rb");

    while (fread(&peliculaLeida, sizeof(struct pelicula), 1, archivoPeliculas)){
        fread(&registroComentarios, sizeof(struct comentariosPelicula), 1, archivoComentarios);

        if (!peliculaLeida.numeroDeComentarios)
            registroComentarios.promedio = 0;

        printf("\n -> %scalificacion: %.2f\n", peliculaLeida.nombre, registroComentarios.promedio);
    }
    fclose(archivoPeliculas);
}