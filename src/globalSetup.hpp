/*! \class GlobalSetup
    \brief Clase que contiene todas las variables de control
    \author Manuel Carlevaro <manuel@iflysib.unlp.edu.ar>  
    \version 0.1
    \date 2018.11.13
    */

#ifndef _GLOBALSETUP_H
#define _GLOBALSETUP_H

#define INFO(msg) \
    fprintf(stderr, "info: %s:%d: ", __FILE__, __LINE__); \
    fprintf(stderr, "%s\n", msg);

#include <cstdlib>
using std::exit;
#include <iostream>
using std::cout; using std::endl;
#include <fstream>
using std::ifstream;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <cmath>
using std::sin; using std::cos;
#include <iomanip>
using std::fixed; using std::setw;
#include <box2d/box2d.h>
#include <time.h>

/** \struct Contenedor
 * \brief Estructura que almacena la información relativa a un contenedor de 
 * granos
 * 
 * Estructura que almacena la información relativa a un contenedor de granos:
 * \li datos geométricos
 * \li información sobre el material
 * */
struct Contenedor {
    double R; /*!< Radio del contenedor */
    double nVerts; /*!< Cantidad de vértices del contenedor */
    double fric; /*!< Coeficiente de fricción del contenedor */
    double rest; /*!< Coeficiente de restitución del contenedor */
    double fgb_stat; /*!< Coeficiente de fricción estático grano-base */
    double fgb_dyn; /*!< Coeficiente de fricción estático grano-base */
};
/** \struct tipoGrano
 * \brief Estructura que contiene información sobre un determinado tipo de 
 * granos
 * 
 * Estructura que contiene información sobre un determinado tipo de grano:
 * \li datos geométricos
 * \li información sobre el material que lo compone
 * */
struct tipoGrano {
    int noGranos;   /*!< Cantidad de granos de este tipo */
    double radio;   /*!< Radio */
    int nLados;     /*!< Número de lados (1: círculo) */
    double **vertices;  /*!< Array con coordenadas de los vértices 
                          (double x, double y) */
    double dens; /*!< Densidad de los granos */
    double fric; /*!< Coeficiente de fric de los granos */
    double rest; /*!< Coeficiente de restitución de los granos */
    double m; /*!< Momento dipolar magnético [0, 0, m] */
    int r; /*!< Sentido de avance: 0 -> adelante, 1 -> atrás */
};

/** \struct bodyData
 * \brief Estructura que almacena datos asociados a cada grano.
 * */
struct BodyData {
    int tipo; /*!< Tipo de grano (en el orden en que aparecen en el .in */
    bool isGrain;   /*!< Variable lógica que identifica granos */
    bool isMag; /*!< Variable lógica que identifica granos magéticos */
    int nLados; /*!< Número de lados que tiene el grano */
    int gID; /*!< Identificador del grano */
    double m; /*!< Momento magnético [0, 0, m] */
    int r; /*!< Sentido de avance: 0 -> adelante, 1 -> atrás */
    b2Vec2 f; /*!< Fuerza acumulada sobre el grano (no gravitatoria) */
};

/** \struct Energias
 * \brief Contiene la energía cinética y potencial magnética del sistema
 */
struct Energias {
    double eKin; /*!< Energía cinética */
    double ePot; /*!< Energía potencial magnética */
};

/** \class GlobalSetup
 * Clase que contiene toda la información necesaria para realizar una simulación
 * */
class GlobalSetup {
public:
    // Parámetros de objetos del modelo
    Contenedor caja; /*!< Recinto de contención */
    int noTipoGranos; /*!< Cantidad de tipos de granos distintos en la 
                        simulación */
    tipoGrano **granos; /*!< Array que contiene los distintos tipos de granos */
    bool isBullet; /*!< Define si los granos se tratan como bullet (T) o 
                     no (F) */
    // Parámetros de control de la simulación
    double tStep; /*!< Paso temporal de integración */
    double tMax; /*!< Límite temporal de la simulación */
    int pIter; /*!< Iteraciones para la satisfacción de restricciones de 
                 posición */
    int vIter; /*!< Iteraciones para la satisfacción de restricciones de 
                 velocidad */
    double g; /*!< Aceleración de la gravedad */
    double noise; /*!< Intensidad del impulso de ruido (Ns) */
    int noiseFreq; /*!< Frecuencia de aplicación de ruido */
    double tNoiseOff; /*!< Tiempo de apagado del ruido */
    double atRot; /*!< Coeficiente de atenuación de rotaciones */
    
    // Parámetros de estadísticas y registros
    int saveFrameFreq; /*!< Frecuencia de guardado de frames */
    string preFrameFile; /*!< Prefijo del nombre del archivo de salida de 
                           frame/trayectoria */
    int xvcFreq; /*!< Frecuencia de guardado de coordenadas, velocidades y
                        contactos */
    string xvcFile; /*!< Prefijo del nombre del archivo de salida de 
                      coordenadas, velocidades y contactos */
    int enerFreq; /*!< Frecuencia de guardado de energías */
    string enerFile; /*!< Archivo de guardado de energías */
    string finXVCFile; /*!< Archivo de guardadoi final de X, V, C */

    
    // Constructor & destructor
    GlobalSetup(string input);
    ~GlobalSetup();
    
    // Info
    /*! \fn GlobalSetup::printGlobalSetup()
        Función que imprime las variables contenidas en GlobalSetup
    */
    void printGlobalSetup();

    string inputFile; /*!< Nombre del archivo que contiene los parámetros de 
                        ejecución */
    
    unsigned int randomSeed; /*!< Semilla del generador de números aleatorios */

private:
    void load(string iFile);
};

#endif

