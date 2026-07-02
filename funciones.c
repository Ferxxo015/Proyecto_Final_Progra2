#include "funciones.h"



void menu()
{
    printf("\n=========================================\n");
    printf(" SISTEMA DE CONTROL DE CONTAMINACION\n");
    printf("=========================================\n");
    printf("1. Registrar zona\n");
    printf("2. Mostrar zonas\n");
    printf("3. Calcular contaminacion actual\n");
    printf("4. Promedio historico\n");
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

    printf("\n========== ZONAS ==========\n");
    printf("%-3s %-20s %10s %10s %10s %10s %10s %10s %10s %15s\n",
           "No", "Nombre", "Temperatura", "Viento", "Humedad", "CO2", "SO2", "NO2", "PM2.5", "ContActual");
    printf("%-3s %-20s %10s %10s %10s %10s %10s %10s %10s %15s\n",
           "--", "--------------------", "----------", "----------", "----------", "----------", "----------", "----------", "----------", "---------------");

    for(i = 0; i < cantidad; i++)
    {
        printf("%-3d %-20s %10.2f %10.2f %10.2f %10.2f %10.2f %10.2f %10.2f %15.2f\n",
               i + 1,
               zonas[i].nombre,
               zonas[i].temperatura,
               zonas[i].viento,
               zonas[i].humedad,
               zonas[i].co2,
               zonas[i].so2,
               zonas[i].no2,
               zonas[i].pm25,
               calcularContaminacion(zonas[i]));
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
    if (cantidad == 0)
    {
        printf("\nNo existen zonas registradas para cargar el historial.\n");
        return;
    }

    printf("\n======================================================================\n");
    printf("  CARGANDO DATOS HISTORICOS (30 DIAS) - ESTANDARES OMS / EPA (TABLA)  \n");
    printf("======================================================================\n");
    printf("%-25s %-10s %-10s %-10s %-10s %-12s\n", "Zona", "PM2.5", "NO2", "SO2", "CO2", "Ponderado");
    printf("----------------------------------------------------------------------\n");

    for (i = 0; i < cantidad; i++)
    {
        float pm25_hist = 0.0;
        float no2_hist = 0.0;
        float so2_hist = 0.0;
        float co2_hist = 0.0;

        
        switch (i % 5)
        {
            case 0: 
                pm25_hist = 12.0; no2_hist = 15.0; so2_hist = 8.0; co2_hist = 410.0;
                break;
            case 1: 
                pm25_hist = 32.0; no2_hist = 45.0; so2_hist = 22.0; co2_hist = 450.0;
                break;
            case 2:
                pm25_hist = 52.0; no2_hist = 75.0; so2_hist = 42.0; co2_hist = 520.0;
                break;
            case 3: 
                pm25_hist = 85.0; no2_hist = 110.0; so2_hist = 65.0; co2_hist = 580.0;
                break;
            case 4: 
                pm25_hist = 24.0; no2_hist = 35.0; so2_hist = 15.0; co2_hist = 435.0;
                break;
        }

        
        zonas[i].promedio = (pm25_hist * 0.50) + 
                            (no2_hist * 0.20) + 
                            (so2_hist * 0.20) + 
                            ((co2_hist / 10.0) * 0.10);

        // Imprime la fila correspondiente en la tabla de la consola
        printf("%-25s %-10.1f %-10.1f %-10.1f %-10.1f %-12.2f\n", 
               zonas[i].nombre, pm25_hist, no2_hist, so2_hist, co2_hist, zonas[i].promedio);
    }
    
    printf("======================================================================\n");
    printf("¡Carga masiva completada! Valores guardados en el registro historico.\n");
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
    printf("%-3s %-20s %-35s %15s\n", "No", "Nombre", "Estado", "Contaminacion");
    printf("%-3s %-20s %-35s %15s\n", "--", "--------------------", "-----------------------------------", "---------------");

    for(i = 0; i < cantidad; i++)
    {
        contaminacion = calcularContaminacion(zonas[i]);
        const char *estado;

        if(contaminacion < 50)
        {
            estado = "BUENO";
        }
        else if(contaminacion < 100)
        {
            estado = "MODERADO";
        }
        else if(contaminacion < 150)
        {
            estado = "MALO PARA GRUPOS SENSIBLES";
        }
        else if(contaminacion < 200)
        {
            estado = "MALO";
        }
        else
        {
            estado = "MUY MALO - PELIGROSO";
        }

        printf("%-3d %-20s %-35s %15.2f\n",
               i + 1,
               zonas[i].nombre,
               estado,
               contaminacion);
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
    printf("%-3s %-20s %-70s %15s\n", "No", "Nombre", "Recomendacion", "Contaminacion");
    printf("%-3s %-20s %-70s %15s\n", "--", "--------------------", "----------------------------------------------------------------------", "---------------");

    for(i = 0; i < cantidad; i++)
    {
        contaminacion = calcularContaminacion(zonas[i]);
        const char *recomendacion;

        if(contaminacion < 50)
        {
            recomendacion = "Calidad del aire buena. No se requieren acciones.";
        }
        else if(contaminacion < 100)
        {
            recomendacion = "Calidad moderada. Personas sensibles deben limitar actividad al aire libre.";
        }
        else if(contaminacion < 150)
        {
            recomendacion = "Grupos sensibles deben evitar actividad prolongada al aire libre. Use mascarilla.";
        }
        else if(contaminacion < 200)
        {
            recomendacion = "Evite actividades al aire libre. Mantenga ventanas cerradas. Use purificador en interiores.";
        }
        else
        {
            recomendacion = "ALERTA: Permanezca en interiores. Evite exposicion al aire exterior. Contacte autoridades.";
        }

        printf("%-3d %-20s %-70s %15.2f\n",
               i + 1,
               zonas[i].nombre,
               recomendacion,
               contaminacion);
    }
}
