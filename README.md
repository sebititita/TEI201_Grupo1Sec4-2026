### Información del Equipo
- **Integrantes:**
  - Sebastian Cáceres - Rol 1
  - Maximiliano Riffo- Rol 2
  - Santiago Marín - Rol 3
  - Joaquín Paiva - Rol 4
  - Mariano Pedrosa - Rol 5
  
- **ODS Seleccionado:** ODS 11 Ciudades y comunidades sostenibles
- **Problema a resolver:**
En la Universidad Adolfo Ibáñez, Campus Peñalolén, los estudiantes que se trasladan en automóvil enfrentan dificultades para encontrar estacionamientos disponibles, especialmente durante los horarios de mayor demanda. Esta situación genera recorridos innecesarios dentro del campus, aumenta los tiempos de búsqueda y contribuye a la congestión vehicular.

El problema se debe, principalmente, a la falta de información en tiempo real sobre la disponibilidad de estacionamientos, lo que obliga a los conductores a recorrer distintos sectores sin saber dónde existen espacios libres.

Por ello, surge la necesidad de implementar una solución tecnológica que permita identificar y comunicar la disponibilidad de estacionamientos, con el objetivo de reducir los tiempos de búsqueda, optimizar el uso de los espacios y contribuir a una movilidad más eficiente y sostenible dentro de la universidad.

### Descripción del Proyecto
Sistema IoT que detecta si un estacionamiento está **ocupado o libre** y lo informa en tiempo real. Un sensor ultrasónico HC-SR04 mide la distancia al vehículo; un ESP32 procesa la lectura (ocupado si la distancia es menor a 15 cm en la maqueta), enciende un indicador en un display de 7 segmentos y publica el estado en una **página web** a la que se accede desde el celular mediante una **red WiFi propia creada por el ESP32**.

En una implementación real, cada nodo (ESP32 + sensores) cubriría un grupo de espacios y se conectaría a la red WiFi del campus, enviando sus datos a una página central con el total de espacios disponibles. Para agregar espacios basta con instalar más nodos, sin cablear hasta un computador.

### Estado del Proyecto
- **Versión actual:** v2.0 — Prototipo Alpha IoT (Avance #2)
- **Última actualización:** 25/10/2026
- **Estado:** Prototipo funcional de 1 espacio con conectividad WiFi; encapsulado conceptualizado en 3D

---

##  Estructura del Repositorio
```
├── hardware/          # Circuitos, esquemas y BOM
├── software/          # Código Arduino y librerías
├── diseno_3d/        # Modelos Fusion 360 y renders
├── testing/          # Validación con usuarios
├── documentacion/    # Reportes y presentación
└── iteraciones/      # Historial de versiones
```

---

##  Inicio Rápido

### Requisitos Previos
- Arduino IDE 2.x con el paquete de placas **ESP32 (Espressif)**
- Fusion 360 (para abrir el modelo 3D `.step`)
- Celular con WiFi (para ver la página del estacionamiento)

### Instalación
1. Clonar el repositorio
2. Instalar librerías (ver `software/librerias/`; solo se usan las incluidas en el paquete ESP32)
3. Cargar `software/src/codigosensorwifi/codigosensorwifi.ino` en el ESP32 (placa: *ESP32 Dev Module*)
4. Conectar el celular a la red WiFi **Estacionamiento_UAI** (clave `parking123`) y abrir **http://192.168.4.1**

---

##  Checklist de Entrega

### Avance #2 (esta entrega)
- [x] Prototipo funcional: sensor → ESP32 → indicador + página web por WiFi
- [x] Código comentado (`software/src/`)
- [x] Diagrama de conexiones con pines (`hardware/esquemas/`)
- [x] Lista de materiales (`hardware/bom/`)
- [x] Fotos del prototipo (`hardware/fotos/`)
- [x] Modelo 3D conceptual del encapsulado + renders (`diseno_3d/`)
- [x] Notas de iteración v1 y v2 (`iteraciones/`)
- [x] Evidencia de funcionamiento (`testing/evidencias/`)

### Pendiente para el Avance #3 (entrega final)
- [ ] Segundo sensor integrado y conteo de espacios disponibles
- [ ] Archivo Fusion 360 (.f3d) final y planos técnicos
- [ ] Reporte de testing con usuarios (mín. 5) y datos cuantitativos
- [ ] Reporte final (máx. 15 páginas) y presentación (10 min)

---

##  Licencia
Este proyecto es desarrollado como parte del curso TEI201 - Facultad de Ingenieria y Ciencias - Universidad Adolfo Ibañez
