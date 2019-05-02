/*! \class GlobalSetup
	\brief Clase que contiene todas las variables de control
	\author Manuel Carlevaro <manuel@iflysib.unlp.edu.ar>  
	\version 0.1
	\date 2018.11.13
	*/

#ifndef _GLOBALSETUP_H
#define _GLOBALSETUP_H

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
#include <Box2D/Box2D.h>
#include <time.h>

/** \struct Contenedor
 * \brief Estructura que almacena la información relativa a un contenedor de granos
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
};
/** \struct tipoGrano
 * \brief Estructura que contiene información sobre un determinado tipo de granos
 * 
 * Estructura que contiene información sobre un determinado tipo de grano:
 * \li datos geométricos
 * \li información sobre el material que lo compone
 * */
struct tipoGrano {
	int noGranos;	/*!< Cantidad de granos de este tipo */
	double radio;	/*!< Radio */
	int nLados;		/*!< Número de lados (1: círculo) */
	double **vertices;	/*!< Array con coordenadas de los vértices (double x, double y) */
	double dens; /*!< Densidad de los granos */
	double fric; /*!< Coeficiente de fric de los granos */
	double rest; /*!< Coeficiente de restitución de los granos */
    double m; /*!< Momento dipolar magnético [0, 0, m] */
};

/** \struct bodyData
 * \brief Estructura que almacena datos asociados a cada grano.
 * */
struct BodyData {
    int tipo; /*!< Tipo de grano (en el orden en que aparecen en el .in */
	bool isGrain;	/*!< Variable lógica que identifica granos */
    bool isMag; /*!< Variable lógica que identifica granos magéticos */
	int nLados;	/*!< Número de lados que tiene el grano */
	int gID; /*!< Identificador del grano */
    double m; /*!< Momento magnético [0, 0, m] */
    b2Vec2 f; /*!< Fuerza acumulada sobre el grano (no gravitatoria) */
};


/** \class GlobalSetup
 * Clase que contiene toda la información necesaria para realizar una simulación
 * */
class GlobalSetup {
public:
	// Parámetros de objetos del modelo
	Contenedor caja; /*!< Recinto de contención */
	int noTipoGranos; /*!< Cantidad de tipos de granos distintos en la simulación */
	tipoGrano **granos; /*!< Array que contiene los distintos tipos de granos */
	bool isBullet; /*!< Define si los granos se tratan como bullet (T) o no (F) */
	// Parámetros de control de la simulación
	double tStep; /*!< Paso temporal de integración */
	double tMax; /*!< Límite temporal de la simulación */
	int pIter; /*!< Iteraciones para la satisfacción de restricciones de posición */
	int vIter; /*!< Iteraciones para la satisfacción de restricciones de velocidad */
	double g; /*!< Aceleración de la gravedad */
    double theta; /*!< Inclinación de la caja (ángulo del plano con la horizontal) */
    double noise; /*!< Intensidad del impulso de ruido (Ns) */
    int noiseFreq; /*!< Frecuencia de aplicación de ruido */
	
    // Parámetros de estadísticas y registros
    int saveFrameFreq; /*!< Frecuencia de guardado de frames */
    string preFrameFile; /*!< Prefijo del nombre del archivo de salida de frame/trayectoria */
    int fluxFreq; /*!< Frecuencia de observación del flujo */
    string fluxFile; /*!< Prefijo del nombre del archivo de salida de flujo */

    
	// Constructor & destructor
	GlobalSetup(string input);
	~GlobalSetup();
	
    // Info
	void printGlobalSetup();
	string inputFile; /*!< Nombre del archivo que contiene los parámetros de ejecución */
	//ModeOut modeOut; [>!< Define el modo de salida de la información <]
	unsigned int randomSeed; /*!< Semilla del generador de números aleatorios */

private:
	void load(string iFile);
};

#endif

