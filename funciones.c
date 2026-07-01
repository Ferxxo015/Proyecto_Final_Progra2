#include "funciones.h"



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




void leerCadena(char *cadena, int n)
{
    int len;

    fgets(cadena, n, stdin);

    len = strlen(cadena) - 1;
    if (len >= 0 && cadena[len] == '\n')
        cadena[len] = '\0';
}




float validarFloatRango(float a, float b)
{
    int aux;
    float n;

    do
    {
        aux = scanf("%f", &n);
        while (getchar() != '\n');

        if (aux != 1 || n < a || n > b)
        {
            printf("Error: valor ingresado incorrecto\n");
            printf(">> ");
        }
    } while (aux != 1 || n < a || n > b);

    return n;
}




int validarIntRango(int a, int b)
{
    int aux;
    int n;

    do
    {
        aux = scanf("%d", &n);
        while (getchar() != '\n');

        if (aux != 1 || n < a || n > b)
        {
            printf("Error: valor ingresado incorrecto\n");
            printf(">> ");
        }
    } while (aux != 1 || n < a || n > b);

    return n;
}



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


void registrarZona(Zona zonas[], int *cantidad)
{
    if(*cantidad >= MAX_ZONAS)
    {
        printf("\nNo se pueden registrar mas zonas.\n");
        return;
    }

    printf("\n===== REGISTRO DE ZONA =====\n");

    printf("Nombre de la zona: ");
    leerCadena(zonas[*cantidad].nombre, 50);

    printf("Temperatura: ");
    zonas[*cantidad].temperatura = validarFloatRango(-56, 56);

    printf("Velocidad del viento: ");
    zonas[*cantidad].viento = validarFloatRango(1, 999);

    printf("Humedad: ");
    zonas[*cantidad].humedad = validarFloatRango(1, 999);

    printf("CO2: ");
    zonas[*cantidad].co2 = validarFloatRango(1, 999);

    printf("SO2: ");
    zonas[*cantidad].so2 = validarFloatRango(1, 999);

    printf("NO2: ");
    zonas[*cantidad].no2 = validarFloatRango(1, 999);

    printf("PM2.5: ");
    zonas[*cantidad].pm25 = validarFloatRango(1, 999);

    zonas[*cantidad].promedio = 0;
    zonas[*cantidad].prediccion = 0;

    (*cantidad)++;

    printf("\nZona registrada correctamente.\n");
}




void mostrarZonas(Zona zonas[], int cantidad)
{
    int i;

    if(cantidad == 0)
    {
        printf("\nNo existen zonas registradas.\n");
        return;
    }

    printf("\n");
    printf("%-20s %8s %8s %8s %8s %8s %8s %8s %12s %10s %10s\n",
           "Zona", "Temp", "Viento", "Humedad",
           "CO2", "SO2", "NO2", "PM2.5",
           "Contam.Act", "Promedio", "Prediccion");

    printf("%-20s %8s %8s %8s %8s %8s %8s %8s %12s %10s %10s\n",
           

    for(i = 0; i < cantidad; i++)
    {
        printf("%-20s %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f %8.2f %12.2f",
               zonas[i].nombre,
               zonas[i].temperatura,
               zonas[i].viento,
               zonas[i].humedad,
               zonas[i].co2,
               zonas[i].so2,
               zonas[i].no2,
               zonas[i].pm25,
               calcularContaminacion(zonas[i]));

        if(zonas[i].promedio > 0)
            printf(" %10.2f", zonas[i].promedio);
        else
            printf(" %10s", "N/A");

        if(zonas[i].prediccion > 0)
            printf(" %10.2f", zonas[i].prediccion);
        else
            printf(" %10s", "N/A");

        printf("\n");
    }
}




float calcularContaminacion(Zona zona)
{
    float total;

    total = zona.co2 +
            zona.so2 +
            zona.no2 +
            zona.pm25;

    return total / 4;
}




void calcularPromedio(Zona zonas[], int cantidad)
{
    int i;

    if(cantidad == 0)
    {
        printf("\nNo existen datos.\n");
        return;
    }

    printf("\n====== PROMEDIO HISTORICO ======\n");
    printf("Ingrese el promedio de los ultimos 30 dias por zona.\n");

    for(i = 0; i < cantidad; i++)
    {
        printf("\nZona %d: %s\n", i + 1, zonas[i].nombre);
        printf("Contaminacion actual: %.2f\n", calcularContaminacion(zonas[i]));
        printf("Promedio historico [0 a 999]: ");
        zonas[i].promedio = validarFloatRango(0, 999);
    }

    printf("\nPromedios ingresados correctamente.\n");
}




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
        if(zonas[i].promedio == 0)
        {
            printf("\nError: la zona '%s' no tiene promedio historico.\n", zonas[i].nombre);
            printf("Ejecute primero la opcion 4.\n");
            return;
        }
    }

    printf("\n====== PREDICCION DE CONTAMINACION ======\n");

    for(i = 0; i < cantidad; i++)
    {
        zonas[i].prediccion =
        (calcularContaminacion(zonas[i]) * 0.70) +
        (zonas[i].promedio * 0.30);

        printf("\nZona: %s\n", zonas[i].nombre);
        printf("Contaminacion actual:  %.2f\n", calcularContaminacion(zonas[i]));
        printf("Promedio historico:    %.2f\n", zonas[i].promedio);
        printf("Prediccion (24 horas): %.2f\n", zonas[i].prediccion);
    }

    printf("\nPrediccion realizada correctamente.\n");
}


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
    }
}
