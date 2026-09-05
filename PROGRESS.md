# 🚀 MyGit: Construyendo Git desde cero en C

> **Regla de oro del proyecto:**  
> Este es un proyecto de aprendizaje *hands-on*. **El usuario escribe todo el código** guiado paso a paso por el asistente (en rol de profesor/mentor), explicando conceptos de arquitectura de Git, lenguaje C, memoria y llamadas al sistema.

---

## 📌 Configuración y Compilación

- **Nombre del ejecutable:** `mygit`
- **Carpeta interna:** `.mygit/` (se utiliza `.mygit` para no interferir con el `.git` del repositorio de desarrollo).
- **Librerías externas:**
  - `OpenSSL` (`<openssl/sha.h>`, `-lssl -lcrypto`) para hashes SHA-1.
  - `zlib` (`<zlib.h>`, `-lz`) para compresión/descompresión de objetos.
- **Comando de compilación:**
  ```bash
  gcc main.c -o mygit -lssl -lcrypto -lz
  ```

---

## ✅ Avances Realizados (Lo que ya funciona)

### 1. `mygit init`
- [x] Validación de argumentos en `main(argc, argv)`.
- [x] Creación portable de directorios (`create_dir` para POSIX/Windows).
- [x] Creación de la estructura base del repositorio:
  - `.mygit/`
  - `.mygit/objects/`
  - `.mygit/refs/`
  - `.mygit/refs/heads/`
- [x] Creación del archivo `.mygit/HEAD` apuntando por defecto a `ref: refs/heads/main`.
- [x] Manejo de errores de sistema con `perror`.

### 2. `mygit hash-object <archivo>` (Cálculo del Hash)
- [x] Despacho del subcomando `hash-object` y verificación de `argc >= 3`.
- [x] Lectura de archivos binarios en C (`fopen` con modo `"rb"`).
- [x] Medición dinámica de tamaño con cursor de archivo (`fseek` a `SEEK_END`, `ftell`, `fseek` a `SEEK_SET`).
- [x] Gestión de memoria dinámica en RAM (`malloc` y `free`).
- [x] Construcción del encabezado de objeto Git: `"blob <tamaño>\0<contenido>"` con `sprintf` y `memcpy`.
- [x] Cálculo del hash SHA-1 de 20 bytes binarios usando OpenSSL.
- [x] Formateo e impresión del hash hexadecimal de 40 caracteres (`%02x`).
- [x] **Compatibilidad verificada:** Los hashes generados coinciden al 100% con `git hash-object` oficial.

---

## 📋 Lista de Tareas Pendientes (Roadmap de Objetivos)

### Fase 1: Completar Objetos y Blobs
- [x] **Escribir objetos en disco (`hash-object -w <archivo>`):**
  - [x] Comprimir `header + content` con `deflate` (usando `zlib`).
  - [x] Extraer los primeros 2 caracteres del hash para la subcarpeta `.mygit/objects/xx/`.
  - [x] Crear la subcarpeta si no existe y escribir los 38 caracteres restantes como nombre de archivo.
- [ ] **Leer objetos (`cat-file -p <hash>`):**
  - Localizar el archivo en `.mygit/objects/xx/yyyy...`.
  - Descomprimir con `inflate` (`zlib`).
  - Separar el encabezado (`blob <size>\0`) y mostrar el contenido original en pantalla.

---

### Fase 2: Directorios y Árboles (Trees)
- [ ] **Concepto del objeto `tree`:** Entender cómo Git representa carpetas y metadatos (permisos, tipo de objeto, hash SHA-1 del blob/sub-tree, nombre de archivo).
- [ ] **Comando `ls-tree <hash>`:** Inspeccionar el contenido estructurado de un objeto árbol.
- [ ] **Comando `write-tree`:** Empaquetar el directorio actual en objetos blobs y trees recursivamente.

---

### Fase 3: La Línea de Tiempo (Commits)
- [ ] **Concepto del objeto `commit`:** Árbol raíz (`tree`), commit padre (`parent`), autor, autor de confirmación y mensaje.
- [ ] **Comando `commit-tree <tree_hash> -p <parent_hash> -m "<mensaje>"`:** Generar el objeto commit con su SHA-1.
- [ ] **Actualización de ramas:** Escribir el nuevo SHA-1 del commit dentro de `.mygit/refs/heads/main`.

---

### Fase 4: La Zona de Preparación (Staging Area e Índice)
- [ ] **El archivo `.mygit/index`:** Formato binario del staging area de Git (cabecera DIRC, metadatos stat, hashes de blobs, rutas).
- [ ] **Comando `mygit add <archivo>`:** Hashear el archivo, guardarlo en `objects` y registrarlo en el índice.
- [ ] **Comando de alto nivel `mygit commit -m "<mensaje>"`:** Combina `write-tree`, `commit-tree` y actualiza la rama activa.

---

### Fase 5: Exploración y Navegación
- [ ] **Comando `mygit log`:** Recorrer la cadena de hashes de commit hacia atrás a través de los padres (`parent`).
- [ ] **Comando `mygit status`:** Comparar el directorio de trabajo, el índice y el último commit para detectar archivos modificados o sin seguimiento.
- [ ] **Ramas (`mygit branch` y cambio de rama con `checkout` básico).**
