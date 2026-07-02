#include "funciones.h"

int main()
{
    Zona zonas[MAX_ZONAS];

    int cantidad = 0;
    int opcion;

    cantidad = cargarDatos(zonas);

    int continuar = 1;

    while(continuar)
    {
        menu();

        printf("\nSeleccione una opcion: ");
        opcion = validarIntRango(1, 9);

        switch(opcion)
        {
            case 1:
                registrarZona(zonas, &cantidad);
                break;

            case 2:
                mostrarZonas(zonas, cantidad);
                break;

            case 3:
            {
                int i;

                if(cantidad == 0)
                {
                    printf("\nNo existen zonas registradas.\n");
                }
                else
                {
                    printf("\n====== CONTAMINACION ACTUAL ======\n");

                    for(i = 0; i < cantidad; i++)
                    {
                        printf("\nZona: %s\n", zonas[i].nombre);
                        printf("Nivel de contaminacion: %.2f\n",
                               calcularContaminacion(zonas[i]));
                    }
                }

                break;
            }

            case 4:
                calcularPromedio(zonas, cantidad);
                break;

            case 5:
                calcularPrediccion(zonas, cantidad);
                break;

            case 6:
                mostrarAlertas(zonas, cantidad);
                break;

            case 7:
                recomendaciones(zonas, cantidad);
                break;

            case 8:
                guardarDatos(zonas, cantidad);
                break;

            case 9:
                guardarDatos(zonas, cantidad);
                printf("\nDatos guardados correctamente.\n");
                printf("Gracias por utilizar el sistema.\n");
                continuar = 0;
                break;

            default:
                printf("\nOpcion incorrecta.\n");
        }

        if(continuar)
        {
            printf("\nDesea realizar alguna cosa mas 1.Si 0.No ");
            continuar = validarIntRango(0, 1);
        }
    }

    return 0;
}
