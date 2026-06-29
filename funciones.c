#include "funciones.h"

/*=========================
        MENU
==========================*/

void menu()
{
    printf("\n=========================================\n");
    printf(" SISTEMA DE CONTROL DE CONTAMINACION\n");
    printf("=========================================\n");
    printf("1. Registrar zona\n");
    printf("2. Mostrar zonas\n");
    printf("3. Calcular contaminacion actual\n");
    printf("4. Calcular promedio historico\n");
    printf("5. Predecir contaminacion\n");
    printf("6. Mostrar alertas\n");
    printf("7. Mostrar recomendaciones\n");
    printf("8. Guardar datos\n");
    printf("9. Salir\n");
    printf("=========================================\n");
}


/*=========================
   VALIDAR SOLO LETRAS
==========================*/

int validarLetras(char texto[])
{
    int i = 0;

    while(texto[i] != '\0')
    {
        if(texto[i] == '\n')
        {
            i++;
            continue;
        }

        if(!((texto[i] >= 'A' && texto[i] <= 'Z') ||
             (texto[i] >= 'a' && texto[i] <= 'z') ||
              texto[i] == ' '))
        {
            return 0;
        }

        i++;
    }

    return 1;
}


/*=========================
 VALIDAR SOLO NUMEROS
==========================*/

int validarNumero(char texto[])
{
    int i = 0;
    int punto = 0;

    while(texto[i] != '\0')
    {
        if(texto[i] == '\n')
        {
            i++;
            continue;
        }

        if(texto[i] == '.')
        {
            punto++;

            if(punto > 1)
                return 0;
        }
        else if(texto[i] < '0' || texto[i] > '9')
        {
            return 0;
        }

        i++;
    }

    return 1;
}


/*=========================
 INGRESAR TEXTO
==========================*/

void ingresarTexto(char mensaje[], char texto[])
{
    do
    {
        printf("%s", mensaje);

        fgets(texto,50,stdin);

        texto[strcspn(texto,"\n")] = '\0';

        if(!validarLetras(texto))
        {
            printf("\nSolo se permiten letras.\n\n");
        }

    }while(!validarLetras(texto));
}


/*=========================
 INGRESAR FLOAT
==========================*/

float ingresarFloat(char mensaje[])
{
    char aux[30];

    do
    {
        printf("%s", mensaje);

        fgets(aux,30,stdin);

        aux[strcspn(aux,"\n")] = '\0';

        if(!validarNumero(aux))
        {
            printf("\nNumero invalido.\n\n");
        }

    }while(!validarNumero(aux));

    return atof(aux);
}


/*=========================
 GUARDAR DATOS
==========================*/

void guardarDatos(Zona zonas[], int cantidad)
{
    FILE *archivo;

    archivo = fopen("zonas.dat","wb");

    if(archivo == NULL)
    {
        printf("Error al crear archivo.\n");
        return;
    }

    fwrite(&cantidad,sizeof(int),1,archivo);

    fwrite(zonas,sizeof(Zona),cantidad,archivo);

    fclose(archivo);

    printf("\nDatos guardados correctamente.\n");
}


/*=========================
 CARGAR DATOS
==========================*/

int cargarDatos(Zona zonas[])
{
    FILE *archivo;

    int cantidad = 0;

    archivo = fopen("zonas.dat","rb");

    if(archivo == NULL)
    {
        return 0;
    }

    fread(&cantidad,sizeof(int),1,archivo);

    fread(zonas,sizeof(Zona),cantidad,archivo);

    fclose(archivo);

    return cantidad;
}
/*=========================
    REGISTRAR ZONA
=========================*/

void registrarZona(Zona zonas[], int *cantidad)
{
    if(*cantidad >= MAX_ZONAS)
    {
        printf("\nNo se pueden registrar mas zonas.\n");
        return;
    }

    printf("\n===== REGISTRO DE ZONA =====\n");

    ingresarTexto("Nombre de la zona: ", zonas[*cantidad].nombre);

    zonas[*cantidad].temperatura = ingresarFloat("Temperatura: ");
    zonas[*cantidad].viento = ingresarFloat("Velocidad del viento: ");
    zonas[*cantidad].humedad = ingresarFloat("Humedad: ");

    zonas[*cantidad].co2 = ingresarFloat("CO2: ");
    zonas[*cantidad].so2 = ingresarFloat("SO2: ");
    zonas[*cantidad].no2 = ingresarFloat("NO2: ");
    zonas[*cantidad].pm25 = ingresarFloat("PM2.5: ");

    zonas[*cantidad].promedio = 0;
    zonas[*cantidad].prediccion = 0;

    (*cantidad)++;

    printf("\nZona registrada correctamente.\n");
}


/*=========================
    MOSTRAR ZONAS
=========================*/

void mostrarZonas(Zona zonas[], int cantidad)
{
    int i;

    if(cantidad == 0)
    {
        printf("\nNo existen zonas registradas.\n");
        return;
    }

    printf("\n========== ZONAS ==========\n");

    for(i = 0; i < cantidad; i++)
    {
        printf("\nZona %d\n", i + 1);

        printf("Nombre: %s\n", zonas[i].nombre);

        printf("Temperatura: %.2f\n", zonas[i].temperatura);
        printf("Viento: %.2f\n", zonas[i].viento);
        printf("Humedad: %.2f\n", zonas[i].humedad);

        printf("CO2: %.2f\n", zonas[i].co2);
        printf("SO2: %.2f\n", zonas[i].so2);
        printf("NO2: %.2f\n", zonas[i].no2);
        printf("PM2.5: %.2f\n", zonas[i].pm25);

        printf("Contaminacion Actual: %.2f\n",
               calcularContaminacion(zonas[i]));
    }

}


/*=========================
 CALCULAR CONTAMINACION
=========================*/

float calcularContaminacion(Zona zona)
{
    float total;

    total = zona.co2 +
            zona.so2 +
            zona.no2 +
            zona.pm25;

    return total / 4;
}


/*=========================
 CALCULAR PROMEDIO
=========================*/

void calcularPromedio(Zona zonas[], int cantidad)
{
    int i;

    if(cantidad == 0)
    {
        printf("\nNo existen datos.\n");
        return;
    }

    for(i = 0; i < cantidad; i++)
    {
        zonas[i].promedio = calcularContaminacion(zonas[i]);
    }

    printf("\nPromedios calculados correctamente.\n");
}


/*=========================
 CALCULAR PREDICCION
=========================*/

void calcularPrediccion(Zona zonas[], int cantidad)
{
    int i;

    if(cantidad == 0)
    {
        printf("\nNo existen datos.\n");
        return;
    }

    for(i = 0; i < cantidad; i++)
    {
        zonas[i].prediccion =
        (calcularContaminacion(zonas[i]) * 0.70) +
        (zonas[i].promedio * 0.30);
    }

    printf("\nPrediccion realizada correctamente.\n");
}
/*=========================
    MOSTRAR ALERTAS
=========================*/

void mostrarAlertas(Zona zonas[], int cantidad)
{
    int i;
    float contaminacion;

    if(cantidad == 0)
    {
        printf("\nNo existen zonas registradas.\n");
        return;
    }

    printf("\n========== ALERTAS ==========\n");

    for(i = 0; i < cantidad; i++)
    {
        contaminacion = calcularContaminacion(zonas[i]);

        printf("\nZona: %s\n", zonas[i].nombre);

        if(contaminacion < 50)
        {
            printf("Estado: BUENO (%.2f)\n", contaminacion);
        }
        else if(contaminacion < 100)
        {
            printf("Estado: MODERADO (%.2f)\n", contaminacion);
        }
        else if(contaminacion < 150)
        {
            printf("Estado: MALO PARA GRUPOS SENSIBLES (%.2f)\n", contaminacion);
        }
        else if(contaminacion < 200)
        {
            printf("Estado: MALO (%.2f)\n", contaminacion);
        }
        else
        {
            printf("Estado: MUY MALO - PELIGROSO (%.2f)\n", contaminacion);
        }
    }
}


/*=========================
   RECOMENDACIONES
=========================*/

void recomendaciones(Zona zonas[], int cantidad)
{
    int i;
    float contaminacion;

    if(cantidad == 0)
    {
        printf("\nNo existen zonas registradas.\n");
        return;
    }

    printf("\n========== RECOMENDACIONES ==========\n");

    for(i = 0; i < cantidad; i++)
    {
        contaminacion = calcularContaminacion(zonas[i]);

        printf("\nZona: %s\n", zonas[i].nombre);

        if(contaminacion < 50)
        {
            printf("- Calidad del aire buena. No se requieren acciones.\n");
        }
        else if(contaminacion < 100)
        {
            printf("- Calidad moderada. Personas sensibles deben limitar actividad al aire libre.\n");
        }
        else if(contaminacion < 150)
        {
            printf("- Grupos sensibles deben evitar actividad prolongada al aire libre.\n");
            printf("- Use mascarilla si sale a la calle.\n");
        }
        else if(contaminacion < 200)
        {
            printf("- Evite actividades al aire libre.\n");
            printf("- Mantenga ventanas cerradas.\n");
            printf("- Use purificador de aire en interiores.\n");
        }
        else
        {
            printf("- ALERTA: Permanezca en interiores.\n");
            printf("- Evite cualquier exposicion al aire exterior.\n");
            printf("- Contacte a las autoridades sanitarias.\n");
        }
    }
}