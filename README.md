# Simulador de Sistema de Archivos UNIX - CI3825

## 📖 Descripción
Este proyecto es una implementación de un **simulador de sistema de archivos UNIX** en C. Permite la creación, eliminación y navegación de archivos y directorios en una estructura jerárquica similar a la de UNIX. Se representa internamente mediante un **árbol binario**, donde cada nodo corresponde a un archivo o directorio.

Este proyecto es parte de la materia **CI3825 - Sistemas de Operación I** en la **Universidad Simón Bolívar**.

---

## 📂 Estructura del Proyecto

```
CI3825-Proyecto-1/
│── src/               # Código fuente
│   │── main.c         # Punto de entrada del programa
│   │── node.c         # Implementación de nodos del sistema de archivos
│   │── commands.c     # Implementación de los comandos UNIX
│   │── filesystem.c   # Funciones de gestión del sistema de archivos
│   ├── include/       # Archivos de cabecera
│   │   │── node.h
│   │   │── commands.h
│   │   │── filesystem.h
│── test/              # Pruebas
│── Makefile           # Archivo para compilar el proyecto
│── README.md          
```

---

## 🛠️ **Compilación**

Para compilar el proyecto, usa `make` en la terminal:

```sh
make
```

Para compilar en **modo depuración**, usa:

```sh
make DEBUG=1
```

Para limpiar archivos compilados:

```sh
make clean
```

---

## 🚀 **Ejecución**

Después de compilar, ejecuta el simulador con:

```sh
./simfs
```

Esto iniciará un intérprete de comandos donde se pueden ejecutar los siguientes comandos:

| Comando         | Descripción |
|----------------|------------|
| `touch <archivo>` | Crea un archivo en el directorio actual. |
| `mkdir <directorio>` | Crea un directorio en el directorio actual. |
| `rm <archivo>` | Elimina un archivo. |
| `rmdir <directorio>` | Elimina un directorio vacío. |
| `ls [-l]` | Lista los archivos y directorios. |
| `cd <directorio>` | Cambia al directorio indicado. |
| `pwd` | Muestra el directorio actual. |
| `wrts <archivo>` | Guarda la estructura del sistema de archivos en un archivo. |
| `help` | Muestra ayuda sobre los comandos disponibles. |
| `exit` | Cierra el programa. |

---


---

## ✍️ **Autores**
- **Eliezer Cario - 18-10605**
- **Miguel Salomon - 19-10274**

---
