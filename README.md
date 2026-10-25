### Información del Equipo
- **Integrantes:**
  - Sebastian Cáceres - Rol 1
  - Maximiliano Riffo- Rol 2
  - Santiago Marín - Rol 3
  - Joaquín Paiva - Rol 4
  - Mariano Pedrosa - Rol 5
  
- **ODS Seleccionado:** El proyecto se vincula con el Objetivo de Desarrollo Sostenible (ODS) 11: Ciudades y comunidades sostenibles, al proponer una solución tecnológica orientada a mejorar la movilidad, optimizar el uso de la infraestructura y contribuir a la reducción de recorridos vehiculares innecesarios.

- **Problema a resolver:**
En la Universidad Adolfo Ibáñez, Campus Peñalolén, los estudiantes que se trasladan en automóvil enfrentan dificultades para encontrar estacionamientos disponibles, especialmente durante los horarios de mayor demanda. Esta situación genera recorridos innecesarios dentro del campus, aumenta los tiempos de búsqueda y contribuye a la congestión vehicular.

El problema se debe, principalmente, a la falta de información en tiempo real sobre la disponibilidad de estacionamientos, lo que obliga a los conductores a recorrer distintos sectores sin saber dónde existen espacios libres.

Por ello, surge la necesidad de implementar una solución tecnológica que permita identificar y comunicar la disponibilidad de estacionamientos, con el objetivo de reducir los tiempos de búsqueda, optimizar el uso de los espacios y contribuir a una movilidad más eficiente y sostenible dentro de la universidad.

### Descripción del Proyecto
Nuestro proyecto consiste en el desarrollo de un sistema de estacionamiento inteligente que busca facilitar la identificación de espacios disponibles en la Universidad Adolfo Ibáñez, Campus Peñalolén, reduciendo los tiempos de búsqueda y los recorridos innecesarios de los conductores.

Para ello, desarrollamos un prototipo funcional que utiliza un sensor ultrasónico HC-SR04 para medir la distancia y detectar la presencia de un vehículo en un espacio de estacionamiento. Esta información es procesada por un microcontrolador ESP32, programado mediante Arduino IDE, que determina si el espacio se encuentra libre u ocupado según un umbral de distancia previamente establecido.

El estado del estacionamiento se refleja automáticamente en un display de siete segmentos, que muestra la cantidad de espacios ocupados. De esta manera, el sistema permite visualizar los cambios de ocupación en tiempo real, sin necesidad de intervención manual.

Actualmente, el prototipo permite monitorear un estacionamiento a pequeña escala. Como proyección futura, se contempla incorporar conectividad Wi-Fi y ampliar el sistema a múltiples espacios, permitiendo consultar su disponibilidad de manera remota.

### Estado del Proyecto
- **Versión actual:** v1.0
- **Última actualización:** 05/10
- **Estado:** Prototipo Inicial

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
- Arduino IDE 2.x
- Fusion 360
- [Otras herramientas necesarias]

### Instalación
1. Clonar el repositorio
2. Instalar librerías (ver `software/librerias/`)
3. Cargar código en ESP32/Arduino
4. [Pasos adicionales]

---

##  Checklist de Entrega

### Hardware ✓
- [ ] Esquema del circuito (Fritzing/Wokwi)
- [ ] BOM completo
- [ ] Fotos de alta resolución

### Software ✓
- [ ] Código comentado
- [ ] Librerías documentadas
- [ ] Manual de instalación

### Diseño 3D ✓
- [ ] Archivos Fusion 360 (.f3d) con TODOS los componentes
- [ ] Renders de alta calidad (1+ ángulos)
- [ ] Planos técnicos

### Testing ✓
- [ ] Reporte de testing con usuarios (mín. 5)
- [ ] Evidencias fotográficas/videos
- [ ] Datos cuantitativos

### Documentación ✓
- [ ] Reporte final (máx. 15 páginas)
- [ ] Presentación (10 min)

---


---

##  Licencia
Este proyecto es desarrollado como parte del curso TEI201 - Facultad de Ingenieria y Ciencias - Universidad Adolfo Ibañez
