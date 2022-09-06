/*! \file siloAux.hpp
 * \brief Archivo de cabecera para funciones auxiliares
 *
 * \author Manuel Carlevaro <manuel@iflysib.unlp.edu.ar>
 * \version 1.1
 * \date 2020.06.11 
 */

#include <iostream>
#include <iomanip>
using std::setw; using std::setprecision; using std::fixed;
#include <sstream>
#include <string>
#include <vector>
#include <box2d/box2d.h>
#include "globalSetup.hpp"

/*! Convierte un número en una string de ancho fijo
 * y rellena de ceros, para enumerar frames secuencialmente
 * \param int num
 * \return std::string
 */
std::string n2s(int num);

/*! Detecta si el sistema está activo
 * \param b2World* w 
 * \return bool
 */
bool isActive(b2World *w);

/*! Escribe en el archivo de salida las coordenadas de las partículas
 * \param std::ofstream* ff
 * \param b2World* w
 * \return void
 */
void savePart(std::ofstream *ff, b2World *w);

/*! Escribe todas las coordenadas necesarias para generar imágenes
 * y posteriores animaciones
 * \param std::ofstream* ff
 * \param b2World* w
 * \return void
 */
void saveFrame(std::ofstream *ff, b2World *w);

/*! Devuelve la cantidad de granos descargados
 * \param b2World* w
 * \param int* st (suma por tipo de granos)
 * \param int maxDiam (cuenta la descarga cuando pasa por -maxDiam)
 * \return int
 */
int countDesc(b2World *w, int *st, int maxD);

/*! Establece la fuerza magnética sobre los granos
 * \param b2World* w
 * \return void
 */
void setMagneticForces(b2World *w);

/*! Guarda las coordenadas, velocidades y contactos de todas los granos
 * \param std::ofstream* ff
 * \param b2World* w
 * \return void
 */
void saveXVCFile(std::ofstream *ff, b2World *w);

/*! \fn energyCalculation
 * \brief Calcula la energía cinética y potencial magnética del sistema
 * \param b2World* w
 * \return Energias
 */
Energias energyCalculation(b2World *w);

/*! \fn end_condition(globalSetup *gs, float timeS, int nTap)
 * \brief Determina la condición de finalización de la simulación
 * \param globalSetup* gs
 * \param float timeS
 * \param int nTap
 * \return bool
 */
bool end_condition(globalSetup *gs, float timeS, int nTap);
