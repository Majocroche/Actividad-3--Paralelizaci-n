#include <stdio.h>
#include "header.h"
#include <omp.h>
#include <time.h>

#define NUM_THREADS 30

int main(){
    double t1, t2, tiempo;
    
    t1 = omp_get_wtime();
    omp_set_num_threads(NUM_THREADS);
    
    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            procesar_todas_imagenes("imagen1.bmp");
            #pragma omp section
            procesar_todas_imagenes("imagen2.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen3.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen4.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen5.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen6.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen7.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen8.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen9.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen10.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen11.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen12.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen13.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen14.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen15.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen16.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen17.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen18.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen19.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen20.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen21.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen22.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen23.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen24.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen25.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen26.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen27.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen28.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen29.bmp");
            
            #pragma omp section
            procesar_todas_imagenes("imagen30.bmp");
            
        }
    }
    
    t2 = omp_get_wtime();
    tiempo = t2 - t1;
    printf("Tiempo de ejecución: %lf segundos\n", tiempo);
    
    return 0;
}