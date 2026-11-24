# Informe del Proyecto Final: Sistema de Respaldo y Recuperación en xv6

## 1. Resumen Ejecutivo
Este proyecto implementa un sistema de respaldo y recuperación de archivos en el sistema operativo xv6. Se desarrollaron los comandos `backup` (para crear copias de seguridad con verificación de integridad) y `restore` (para recuperar archivos), junto con un programa de pruebas automatizadas (`test_backup.c`) que valida el correcto funcionamiento del sistema ante distintos escenarios, incluyendo errores controlados y archivos de distintos tamaños.

## 2. Planteamiento del Problema
La pérdida de información por eliminación accidental o fallos en el sistema es un riesgo constante. En xv6, no existe una herramienta nativa que permita crear copias de seguridad verificadas. Este proyecto busca resolver esa necesidad implementando un mecanismo que garantice que los archivos respaldados no se corrompan y que su restauración sea segura.

## 3. Diseño e Implementación
El sistema se divide en dos módulos principales y un script de pruebas:

### 3.1 Módulo de Backup (Pasos 1 y 3 unificados)
*Nota técnica: Aunque el proyecto se planificó por pasos, en el control de versiones (Git) el código de `backup.c` evolucionó sobre el mismo archivo. El código final incluye tanto la lógica básica del Paso 1 como las mejoras del Paso 3.*
- **Funcionalidad:** El comando toma un archivo de origen y lo copia a un destino.
- **Integridad:** Implementa la función `calculate_checksum` que suma los bytes del archivo para generar una firma de integridad.
- **Seguridad:** Utiliza la función `stat` para verificar si el archivo de destino ya existe, impidiendo sobrescribir archivos accidentalmente.

### 3.2 Módulo de Restore (Paso 2)
- **Funcionalidad:** Toma un archivo de respaldo (`.bak`) y lo escribe en un nuevo archivo en el destino especificado.
- **Manejo de errores:** Valida que el archivo de respaldo exista antes de intentar leerlo, imprimiendo mensajes claros al usuario.

## 4. Pruebas y Resultados (Paso 4)
Para garantizar la calidad, se desarrolló el programa `user/test_backup.c`, que ejecuta 8 casos de prueba automatizados (CP01 a CP08).

### Casos de prueba destacados:
- **CP01-CP03:** Flujo normal (crear backup, restaurar y verificar integridad).
- **CP04:** Intento de backup de un archivo inexistente (debe fallar controladamente).
- **CP05:** Intento de restauración de una copia inexistente (debe fallar controladamente).
- **CP07:** Manejo de archivos vacíos (0 bytes).
- **CP08:** Manejo de archivos extensos (2048 bytes).

*(AQUÍ DEBES INSERTAR LA IMAGEN DE LA TERMINAL DONDE SE VE EL RESULTADO DE LAS PRUEBAS CP01-CP08 CON LOS "OK")*

## 5. Conclusiones
El sistema de respaldo y recuperación implementado en xv6 cumple con los objetivos planteados. Las pruebas automatizadas demuestran que el código es robusto y capaz de manejar tanto operaciones exitosas como errores controlados (archivos inexistentes, archivos vacíos o archivos de gran tamaño). La inclusión de un checksum asegura la integridad de los datos respaldados.

## Anexos
### Comandos para ejecutar el proyecto:
```bash
# Compilar y ejecutar xv6
make qemu

# Dentro de xv6, ejecutar las pruebas
$ test_backup
