#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

// Función para invertir imagen en blanco y negro
void inv_img_bw(const char *mask, const char *path) {
    FILE *image, *outputImage;
    char add_char[100] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    
    image = fopen(path, "rb");
    if (image == NULL){
        printf("Error: No se pudo abrir %s\n", path);
        return;
    }
    
    outputImage = fopen(add_char, "wb");
    if (outputImage == NULL) {
        printf("Error: No se pudo crear %s\n", add_char);
        fclose(image);
        return;
    }
    
    unsigned char r, g, b;
    unsigned char xx[54];
    long ancho, tam, bpp, alto;
    
    // Copiar cabecera
    for(int i = 0; i < 54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);
    }
    
    tam = (long)xx[4]*65536 + (long)xx[3]*256 + (long)xx[2];
    bpp = (long)xx[29]*256 + (long)xx[28];
    ancho = (long)xx[20]*65536 + (long)xx[19]*256 + (long)xx[18];
    alto = (long)xx[24]*65536 + (long)xx[23]*256 + (long)xx[22];
    
    printf("\n=== IMAGEN INVERTIDA B/N ===\n");
    printf("Tamaño del archivo: %li bytes\n", tam);
    printf("Bits por pixel: %li\n", bpp);
    printf("Alto imagen: %li\n", alto);
    printf("Ancho imagen: %li\n", ancho);
    
    // Leer todos los pixeles RGB
    unsigned char* pixelData = (unsigned char*)malloc(ancho * alto * 3 * sizeof(unsigned char));
    if (pixelData == NULL) {
        printf("Error: No se pudo reservar memoria\n");
        fclose(image);
        fclose(outputImage);
        return;
    }
    
    long j = 0;
    while(!feof(image) && j < ancho * alto * 3) {
        pixelData[j] = fgetc(image);
        j++;
    }
    
    printf("Pixeles leídos: %li bytes\n", j);
    
    // Array para guardar en escala de grises
    unsigned char* arr_in = (unsigned char*)malloc(ancho * alto * sizeof(unsigned char));
    if (arr_in == NULL) {
        printf("Error: No se pudo reservar memoria\n");
        free(pixelData);
        fclose(image);
        fclose(outputImage);
        return;
    }
    
    // Convertir a escala de grises en paralelo
    long total_pixeles = ancho * alto;
    
    #pragma omp parallel
    {
        #pragma omp for
        for(long i = 0; i < total_pixeles; i++) {
            long index = i * 3;
            unsigned char b = pixelData[index];
            unsigned char g = pixelData[index + 1];
            unsigned char r = pixelData[index + 2];
            
            unsigned char pixel = (unsigned char)(0.299*r + 0.587*g + 0.114*b);
            arr_in[i] = pixel;
        }
    }
    
    printf("Pixeles procesados: %li\n", total_pixeles);
    
    // Escribir invertido (de atrás hacia adelante)
    for(long i = 0; i < total_pixeles; i++) {
        long indice = total_pixeles - 1 - i;
        fputc(arr_in[indice], outputImage);
        fputc(arr_in[indice], outputImage);
        fputc(arr_in[indice], outputImage);
    }
    
    free(arr_in);
    free(pixelData);
    fclose(image);
    fclose(outputImage);
    
    printf("Procesamiento completado exitosamente\n");
}
// Función para convertir imagen a escala de grises
void img_to_grayscale(const char *mask, const char *path) {
    FILE *image, *outputImage;
    char add_char[100] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    
    image = fopen(path, "rb");
    if (image == NULL){
        printf("Error: No se pudo abrir %s\n", path);
        return;
    }
    
    outputImage = fopen(add_char, "wb");
    if (outputImage == NULL) {
        printf("Error: No se pudo crear %s\n", add_char);
        fclose(image);
        return;
    }
    
    unsigned char r, g, b;
    unsigned char xx[54];
    long ancho, tam, bpp, alto;
    
    // Copiar cabecera
    for(int i = 0; i < 54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);
    }
    
    // Leer información del header
    tam = (long)xx[4]*65536 + (long)xx[3]*256 + (long)xx[2];
    bpp = (long)xx[29]*256 + (long)xx[28];
    ancho = (long)xx[20]*65536 + (long)xx[19]*256 + (long)xx[18];
    alto = (long)xx[24]*65536 + (long)xx[23]*256 + (long)xx[22];
    
    printf("\n=== IMAGEN EN ESCALA DE GRISES ===\n");
    printf("Tamaño del archivo: %li bytes\n", tam);
    printf("Bits por pixel: %li\n", bpp);
    printf("Alto imagen: %li\n", alto);
    printf("Ancho imagen: %li\n", ancho);
    
    // Leer todos los pixeles RGB
    unsigned char* pixelData = (unsigned char*)malloc(ancho * alto * 3 * sizeof(unsigned char));
    if (pixelData == NULL) {
        printf("Error: No se pudo reservar memoria\n");
        fclose(image);
        fclose(outputImage);
        return;
    }
    
    long j = 0;
    while(!feof(image) && j < ancho * alto * 3) {
        pixelData[j] = fgetc(image);
        j++;
    }
    
    printf("Pixeles leídos: %li bytes\n", j);
    
    // Array para guardar en escala de grises (salida)
    unsigned char* grayData = (unsigned char*)malloc(ancho * alto * 3 * sizeof(unsigned char));
    if (grayData == NULL) {
        printf("Error: No se pudo reservar memoria para grayData\n");
        free(pixelData);
        fclose(image);
        fclose(outputImage);
        return;
    }
    
    // Convertir a escala de grises en paralelo
    long total_pixeles = ancho * alto;
    
    #pragma omp parallel
    {
        #pragma omp for
        for(long i = 0; i < total_pixeles; i++) {
            long index = i * 3;
            unsigned char b = pixelData[index];
            unsigned char g = pixelData[index + 1];
            unsigned char r = pixelData[index + 2];
            
            // Fórmula de conversión a escala de grises
            unsigned char pixel = (unsigned char)(0.21*r + 0.72*g + 0.07*b);
            
            grayData[index] = pixel;
            grayData[index + 1] = pixel;
            grayData[index + 2] = pixel;
        }
    }
    
    printf("Pixeles procesados: %li\n", total_pixeles);
    
    // Escribir imagen en escala de grises
    for(long i = 0; i < ancho * alto * 3; i++) {
        fputc(grayData[i], outputImage);
    }
    
    free(pixelData);
    free(grayData);
    fclose(image);
    fclose(outputImage);
    
    printf("Procesamiento completado exitosamente\n");
}

// Función para invertir imagen horizontalmente (espejo)
void inv_img_horizontal(const char *mask, const char *path) {
    FILE *image, *outputImage;
    char add_char[100] = "./img/";
    strcat(add_char, mask);
    strcat(add_char, ".bmp");
    
    image = fopen(path, "rb");
    if (image == NULL){
        printf("Error: No se pudo abrir %s\n", path);
        return;
    }
    
    outputImage = fopen(add_char, "wb");
    if (outputImage == NULL) {
        printf("Error: No se pudo crear %s\n", add_char);
        fclose(image);
        return;
    }
    
    unsigned char r, g, b;
    unsigned char xx[54];
    long ancho, tam, bpp, alto;
    
    // Copiar cabecera
    for(int i = 0; i < 54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);
    }
    
    // Leer información del header
    tam = (long)xx[4]*65536 + (long)xx[3]*256 + (long)xx[2];
    bpp = (long)xx[29]*256 + (long)xx[28];
    ancho = (long)xx[20]*65536 + (long)xx[19]*256 + (long)xx[18];
    alto = (long)xx[24]*65536 + (long)xx[23]*256 + (long)xx[22];
    
    printf("\n=== IMAGEN INVERTIDA HORIZONTALMENTE ===\n");
    printf("Tamaño del archivo: %li bytes\n", tam);
    printf("Bits por pixel: %li\n", bpp);
    printf("Alto imagen: %li\n", alto);
    printf("Ancho imagen: %li\n", ancho);
    
    // Leer todos los pixeles RGB
    unsigned char* pixelData = (unsigned char*)malloc(ancho * alto * 3 * sizeof(unsigned char));
    if (pixelData == NULL) {
        printf("Error: No se pudo reservar memoria\n");
        fclose(image);
        fclose(outputImage);
        return;
    }
    
    long j = 0;
    while(!feof(image) && j < ancho * alto * 3) {
        pixelData[j] = fgetc(image);
        j++;
    }
    
    printf("Pixeles leídos: %li bytes\n", j);
    
    // Array para guardar la imagen invertida horizontalmente
    unsigned char* mirrorData = (unsigned char*)malloc(ancho * alto * 3 * sizeof(unsigned char));
    if (mirrorData == NULL) {
        printf("Error: No se pudo reservar memoria para mirrorData\n");
        free(pixelData);
        fclose(image);
        fclose(outputImage);
        return;
    }
    
    // Invertir horizontalmente en paralelo (fila por fila)
    #pragma omp parallel
    {
        #pragma omp for
        for(long y = 0; y < alto; y++) {
            // Para cada fila, invertir los pixeles de izquierda a derecha
            for(long x = 0; x < ancho; x++) {
                long src_index = (y * ancho + x) * 3;
                long dst_index = (y * ancho + (ancho - 1 - x)) * 3;
                
                // Copiar BGR del pixel original a su posición espejo
                mirrorData[dst_index] = pixelData[src_index];         // B
                mirrorData[dst_index + 1] = pixelData[src_index + 1]; // G
                mirrorData[dst_index + 2] = pixelData[src_index + 2]; // R
            }
        }
    }
    
    printf("Filas procesadas: %li\n", alto);
    printf("Total pixeles: %li\n", alto * ancho);
    
    // Escribir imagen invertida
    for(long i = 0; i < ancho * alto * 3; i++) {
        fputc(mirrorData[i], outputImage);
    }
    
    free(pixelData);
    free(mirrorData);
    fclose(image);
    fclose(outputImage);
    
    printf("Procesamiento completado exitosamente\n");
}
void blur_img(int mm, const char *mask, const char *path) {
    FILE *image = fopen(path, "rb");
    
    if (image == NULL) {
        printf("Error: No se pudo abrir %s\n", path);
        return;
    }
    
    char add_char[80] = "./img/";
    char mma[20];
    
    strcat(add_char, mask);
    sprintf(mma, "%d", mm);
    strcat(add_char, mma);
    strcat(add_char, ".bmp");
    
    FILE *outputImage = fopen(add_char, "wb");
    if (outputImage == NULL) {
        printf("Error: No se pudo crear %s\n", add_char);
        fclose(image);
        return;
    }
    
    long ancho, alto, tam;
    unsigned char xx[54];
    
    // Copiar cabecera
    for(int i = 0; i < 54; i++) {
        xx[i] = fgetc(image);
        fputc(xx[i], outputImage);
    }
    
    tam = (long)xx[4]*65536 + (long)xx[3]*256 + (long)xx[2];
    ancho = (long)xx[20]*65536 + (long)xx[19]*256 + (long)xx[18];
    alto = (long)xx[24]*65536 + (long)xx[23]*256 + (long)xx[22];
    
    printf("\n=== APLICANDO BLUR ===\n");
    printf("Tamaño kernel: %dx%d\n", mm, mm);
    printf("Largo imagen: %li\n", alto);
    printf("Ancho imagen: %li\n", ancho);
    
    // Extraer array de pixeles
    unsigned char *pixelData = (unsigned char *)malloc(ancho * alto * 3 * sizeof(unsigned char));
    if (pixelData == NULL) {
        printf("Error: No se pudo reservar memoria para pixelData\n");
        fclose(image);
        fclose(outputImage);
        return;
    }
    
    // Array para la imagen con blur
    unsigned char *blurData = (unsigned char *)malloc(ancho * alto * 3 * sizeof(unsigned char));
    if (blurData == NULL) {
        printf("Error: No se pudo reservar memoria para blurData\n");
        free(pixelData);
        fclose(image);
        fclose(outputImage);
        return;
    }
    
    long j = 0;
    while(!feof(image) && j < ancho * alto * 3) {
        pixelData[j] = fgetc(image);
        j++;
    }
    
    printf("Pixeles leídos: %ld bytes\n", j);
    printf("Iniciando procesamiento...\n");
    
    int kernelRadius = (mm - 1) / 2;
    
    #pragma omp parallel
    {
        #pragma omp for
        for (long y = 0; y < alto; y++) {
            if (y % 100 == 0) {
                printf("\rProgreso: %.1f%%", ((float)y / (float)alto) * 100);
                fflush(stdout);
            }
            
            for (long x = 0; x < ancho; x++) {
                unsigned int sumB = 0, sumG = 0, sumR = 0;
                int pond = 0;
                
                // Aplicar kernel de blur
                for (int ky = -kernelRadius; ky <= kernelRadius; ky++) {
                    for (int kx = -kernelRadius; kx <= kernelRadius; kx++) {
                        long ny = y + ky;
                        long nx = x + kx;
                        
                        // Verificar límites
                        if (ny < 0 || ny >= alto || nx < 0 || nx >= ancho)
                            continue;
                        
                        // CORRECCIÓN: índice sin padding
                        long index = (ny * ancho + nx) * 3;
                        
                        sumB += pixelData[index];
                        sumG += pixelData[index + 1];
                        sumR += pixelData[index + 2];
                        pond++;
                    }
                }
                
                // Promediar y escribir en blurData
                long index = (y * ancho + x) * 3;
                blurData[index] = sumB / pond;
                blurData[index + 1] = sumG / pond;
                blurData[index + 2] = sumR / pond;
            }
        }
    }
    
    printf("\rProgreso: 100.0%%\n");
    printf("Fin procesamiento\n");
    
    // Escribir imagen procesada
    for(long i = 0; i < ancho * alto * 3; i++) {
        fputc(blurData[i], outputImage);
    }
    
    free(pixelData);
    free(blurData);
    fclose(image);
    fclose(outputImage);
}

void procesar_todas_imagenes(const char *imagen_path) {
    // Extraer el nombre base de la imagen (sin extensión)
    char nombre_base[100];
    strcpy(nombre_base, imagen_path);
    
    // Remover la extensión .bmp
    char *punto = strrchr(nombre_base, '.');
    if (punto != NULL) {
        *punto = '\0';
    }
    
    // Crear nombres únicos para cada transformación
    char mask_bn[150];
    char mask_grises[150];
    char mask_espejo[150];
    char mask_blur[150];
    
    sprintf(mask_bn, "%s_invertida_bn", nombre_base);
    sprintf(mask_grises, "%s_grises", nombre_base);
    sprintf(mask_espejo, "%s_espejo", nombre_base);
    sprintf(mask_blur, "%s_blur_", nombre_base);
    
    printf("\n========================================\n");
    printf("INICIANDO PROCESAMIENTO DE IMAGEN: %s\n", imagen_path);
    printf("========================================\n");
    
    // Invertir en blanco y negro
    inv_img_bw(mask_bn, imagen_path);
    
    // Convertir a escala de grises
    img_to_grayscale(mask_grises, imagen_path);
    
    // Invertir horizontalmente (espejo)
    inv_img_horizontal(mask_espejo, imagen_path);
    
    // Blur con kernel de 57x57
    blur_img(57, mask_blur, imagen_path);
    
}
#endif