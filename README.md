# autoPropMag

Simulación de granos autopropulsados con interacciones magnéticas.


Simulación con Box2D de la dinámica de un contenedor con granos magnéticos.
Los parámetros de la simulación se establecen en un archivo de texto como 
el adjunto (params.ini). 

# Requerimientos:
1. Box2D
2. Matplotlib
3. ffmpeg

# Compilación
    make
Es necesario indicar en el Makefile la ruta hacia la librería libBox2D.a y
también el camino hacia Box2D.h

# Documentación
La documentación del código se genera en el directorio raíz con:

    doxygen Doxyfile

Esto genera la documentación en html en el directorio /doc.

# Uso:
    ./siloMag params.ini &> salida.out

# Generación de animación:
Este proceso consiste en dos pasos: 1) Generar los archivos de imágenes, 
2) concatenar las imágenes en un video.

1. Generación de imágenes:

    ./makePreMovie.py -f [prefijo de nombre de archivos .xy]

Ejemplo:

    ./makePreMovie.py -f frm

2. Generación de video:

    ./dovideo.sh [nombre] [framerate]

Ejemplo:

    ./dovideo.sh dd-100-20 10

3. Gráfico de energías:

    ./plotEnergias.py -f [archivo de energías]

Ejemplo:

    ./plotEnergias.py -f energias.dat
