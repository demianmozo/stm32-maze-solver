# stm32-maze-solver

🚗 Proyecto de robot autónomo que resuelve un laberinto mediante un algoritmo de llenado (*Flood Fill*), desarrollado sobre STM32 usando STM32CubeIDE.

---

## 🧭 Resumen del Proyecto

Este proyecto implementa un sistema de control embebido en un microcontrolador STM32 para un robot capaz de detectar obstáculos, ajustar su trayectoria y salir de un laberinto desconocido en el menor tiempo posible. La navegación está basada en un algoritmo de llenado de celdas, lo que permite optimizar el recorrido.

📄 [Ver documentación Doxygen](https://demianmozo.github.io/stm32-maze-solver/)

---

## ⚙️ Especificaciones Técnicas

- **Placa:** STM32F407G-DISC1 (Discovery)
- **Microcontrolador:** STM32F407VGT6  
- **IDE:** STM32CubeIDE  
- **Lenguaje:** C  
- **Periféricos utilizados:** ADC, GPIO, PWM, UART, DMA  

---

## 🎯 Objetivos del Proyecto

- Implementar un robot autónomo capaz de resolver un laberinto.  
- Diseñar subsistemas modulares: motores, sensores y lógica de decisión.  
- Optimizar el recorrido total y reducir el tiempo de salida.  

---

## 🧩 Subsistemas

- **Control de motores:**  
  Mantiene el desplazamiento recto y permite giros precisos de 90° y 180°.

- **Detección y escritura de muros:**  
  El sensor frontal detecta paredes y actualiza el mapa interno del laberinto.

- **Detección de cuadrículas:**  
  Un sensor de línea inferior cuenta celdas recorridas y permite ubicar al robot en la grilla.

- **Algoritmo de navegación (Flood Fill):**  
  Asigna peso a cada celda del laberinto y toma decisiones de movimiento basadas en ese mapa.

- **Control general:**  
  Coordina todos los subsistemas y ejecuta decisiones en tiempo real.

---

## 🏁 Estado del Proyecto

✅ Implementado
🔧 Mejoras en proceso

###### Asignatura Sistemas Digitales - Depto. de Electrónica y Computación - Facultad de Ingeniería - UNMdP 

