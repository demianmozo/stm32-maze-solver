/**
 * @file navegacion.c
 * @brief Implementación del módulo de navegación
 */

 #include "navegacion.h"

 brujula calcular_mejor_direccion(uint8_t fila_actual, uint8_t columna_actual) //nos devuelve direccion en TIPO BRUJULA gracias colo
 {
     uint8_t peso_minimo = PESO_MAXIMO;
     brujula mejor_direccion = norte; // Dirección por defecto
     
     // Evaluar las 4 direcciones posibles
     for (brujula direccion = norte; direccion <= oeste; direccion++) {
         
         // 1. ¿Hay muro en esta dirección?
         if (laberinto_hay_muro(fila_actual, columna_actual, direccion)) {
             continue; // Saltar si hay muro
         }
         
         // 2. Obtener posición adyacente
         posicion_t pos_adyacente = laberinto_get_posicion_adyacente(
             (posicion_t){fila_actual, columna_actual}, direccion);
         
         // 3. ¿Es válida? (no salirse del laberinto)
         if (!laberinto_posicion_valida(pos_adyacente.fila, pos_adyacente.columna)) {
             continue; // Saltar si está fuera del laberinto
         }
         
         // 4. Obtener peso de la casilla adyacente
         uint8_t peso_adyacente = laberinto_get_peso(pos_adyacente.fila, pos_adyacente.columna);
         
         // 5. ¿Es el mejor peso hasta ahora?
         if (peso_adyacente < peso_minimo) {
             peso_minimo = peso_adyacente;
             mejor_direccion = direccion;
         }
     }
     
     return mejor_direccion;
 }

 brujula ejecutar_movimiento(brujula sentido_actual, brujula sentido_deseado)
 {
     // ¿Ya está orientado correctamente?
     if (sentido_actual == sentido_deseado)
     {
         avanza(); // Solo avanzar
         return sentido_actual;
     }

     // Calcular diferencia usando aritmética xq es un tipo brujula
     // norte=0, este=1, sur=2, oeste=3
     int diferencia = (sentido_deseado - sentido_actual + 4) % 4;

     switch (diferencia)
     {
     case 1: // Girar 90° a la derecha
         return gira90der(sentido_actual);

     case 2: // Girar 180° (media vuelta)
         return gira180(sentido_actual);

     case 3: // Girar 90° a la izquierda
         return gira90izq(sentido_actual);

     default: // No debería pasar (caso 0)
         avanza();
         return sentido_actual;
     }
 }