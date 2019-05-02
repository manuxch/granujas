/*! \file siloAux.hpp
 * \brief Archivo de cabecera para funciones auxiliares
 *
 * \author Manuel Carlevaro <manuel@iflysib.unlp.edu.ar>
 * \version 1.0
 * \date 20018.12.14
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <Box2D/Box2D.h>
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
 * \param b2Word* w
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
