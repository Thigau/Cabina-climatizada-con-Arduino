# Cabina Climatizada
Diseñado por estudiantes, este proyecto esta pensado para visualizar como se haría una cabina climatizada en codigo C (Arduino).

## Objetivos

- Elaborar el croquis del proceso industrial (investigación).
- Desarrollar la aplicación en lenguaje C para Arduino.

## Partes del Sistema

- **Panel de control:** Incluye un panel de fallas.
- **Pulsadores:**
  - Pulsador de marcha del proceso (NA).
  - Pulsador de parada del proceso (NA).
  - Pulsador "Silenciar alarma" (NA).
- **Alarmas:**
  - Alarma de 12V, 0.5A CC.
- **Sensores:**
  - Sensor de temperatura con salida de 0-5V proporcional (0V = 0°C; 5V = 500°C).
  - Higrómetro con salida de 0-5V proporcional (0V = 0%; 5V = 100%).
- **Actuadores:**
  - Ventilador accionado por motor de 3 HP trifásico a 750 rpm.
  - Extractor accionado por motor de 3 HP trifásico a 1500 rpm.
  - Calentador de 5000W trifásico.
  - Humidificador de 1000W a 220V monofásico.

## Condiciones Normales

En esta industria, las piezas deben permanecer durante 10 horas en un recinto con temperatura y humedad controladas:

- **Temperatura (T):** Entre 80°C y 85°C.
- **Humedad relativa (φ):** Mayor al 55%.

### Detalles del Proceso

- La colocación y retiro de las piezas, así como el control del tiempo, están a cargo de los operadores.
- Las condiciones ambientales en la fábrica son: **T < 30°C** y **φ < 52%**.
- La climatización comienza al presionar el pulsador de marcha y se detiene al presionar el pulsador de parada.
- El ventilador permanece encendido mientras el proceso esté en marcha, para garantizar condiciones homogéneas.
- Se monitorean la temperatura y la humedad constantemente, mostrando los valores en un display cada 5 segundos.

### Control de Temperatura

- Si la temperatura supera los 85°C, se activa el extractor hasta que descienda a 82°C.
- Si la temperatura baja de 80°C, se activa el calentador hasta que alcance los 83°C.

### Control de Humedad

- Si la humedad es menor al 53%, se activa el humidificador hasta que alcance el 60% y se desactiva.

## Condiciones de Falla

- **Protección térmica:** De cualquiera de los motores, con indicadores separados.
- **Tiempo excesivo:** Si después de 35 minutos de intentar subir/bajar la temperatura o humidificar, no se alcanzan los valores requeridos. Cada situación debe tener un indicador específico.
- **Acciones en caso de falla:**
  - Se desactivan todos los actuadores del proceso.
  - Se activa una alarma sonora intermitente, que puede desactivarse con el pulsador "Silenciar alarma".
  - Se enciende un indicador luminoso rojo "EN FALLA" (no intermitente).
  - Los indicadores de falla permanecen activos hasta que las fallas sean resueltas y se presione dos veces el pulsador de parada.

---
