#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ZONAS 10

typedef struct
{
    char nombre[50];

    float temperatura;
    float viento;
    float humedad;

    float co2;
    float so2;
    float no2;
    float pm25;

    float promedio;
    float prediccion;

} Zona;




int validarLetras(char texto[]);
int validarNumero(char texto[]);

void ingresarTexto(char mensaje[], char texto[]);
float ingresarFloat(char mensaje[]);




void guardarDatos(Zona zonas[], int cantidad);
int cargarDatos(Zona zonas[]);




void registrarZona(Zona zonas[], int *cantidad);
void mostrarZonas(Zona zonas[], int cantidad);




float calcularContaminacion(Zona zona);
void calcularPromedio(Zona zonas[], int cantidad);
void calcularPrediccion(Zona zonas[], int cantidad);




void mostrarAlertas(Zona zonas[], int cantidad);
void recomendaciones(Zona zonas[], int cantidad);




void menu();
