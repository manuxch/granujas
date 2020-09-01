#include "globalSetup.hpp"

/*! \fn GlobalSetup::GlobalSetup(string iFile)
    Constructor de la clase GlobalSetup */
GlobalSetup::GlobalSetup(string iFile): inputFile(iFile)  {
        std::srand(time(NULL));
    load(iFile);
    printGlobalSetup();
}

/*! \fn ~GlobalSetup::GlobalSetup()
    Destructor de la clase GlobalSetup
    */
GlobalSetup::~GlobalSetup() {
    for (int i = 0; i < noTipoGranos; i++) {
        if (granos[i]->nLados > 1) {
            for (int j = 0; j < granos[i]->nLados; j++) {
                delete [] granos[i]->vertices[j];
            }
            delete granos[i]->vertices;
        }
    }
    delete [] granos;
}


void GlobalSetup::load(string inputFile){
    ifstream fin(inputFile.c_str());
    string ident;
    
    if (!fin.is_open()) {
        cout << "ERROR: No se puede abrir el archivo " << inputFile << endl;
        exit(1);
    }
    while (!fin.eof()) {
        fin >> ident;

        // Lectura de parámetros del contenedor
        //
        if (ident == "Radio_caja:") {
            fin >> caja.R;
            if (caja.R <= 0) {
                cout << "ERROR: El radio de la caja debe ser > 0." << endl;
                exit(1);
            }
        }
        if (ident == "n_vertices:") {
            fin >> caja.nVerts;
            if (caja.nVerts <= 2) {
                cout << "ERROR: La caja debe tener más de 2 vé®tices." << endl;
                exit(1);
            }
        }
        if (ident == "Caja_fric:") {
            fin >> caja.fric;
            if (caja.fric<= 0) {
                cout << "El coeficiente de fricción de la caja debe ser > 0." 
                    << endl;
                exit(1);
            }
        }
        if (ident == "Caja_rest:") {
            fin >> caja.rest;
            if (caja.rest <= 0) {
                cout << "ERROR: El coeficiente de restitución debe ser > 0." 
                    << endl;
                exit(1);
            }
        }
        if (ident == "fricGB_static:") {
            fin >> caja.fgb_stat;
            if (caja.fgb_stat <= 0) {
                cout << "ERROR: El coeficiente de fricción estático "
                    << "grano-base debe ser > 0." 
                    << endl;
                exit(1);
            }
        }
        if (ident == "fricGB_dynamic:") {
            fin >> caja.fgb_dyn;
            if (caja.fgb_stat <= 0) {
                cout << "ERROR: El coeficiente de fricción dinámico "
                    << "grano-base debe ser > 0." 
                    << endl;
                exit(1);
            }
        }
        // Fin parámetros del contenedor
        //
        // Lectura de parámetros de los granos
        //
        if (ident == "noTipoGranos:") {
            fin >> noTipoGranos;
            if (noTipoGranos <= 0) {
                cout << "ERROR: El número de tipos granos debe ser > 0." 
                    << endl;
                exit(1);
            }
            granos = new tipoGrano*[noTipoGranos];
            for (int i = 0; i < noTipoGranos; i++) {
                granos[i] = new tipoGrano;
                fin >> granos[i]->noGranos;
                fin >> granos[i]->radio;
                fin >> granos[i]->nLados;
                fin >> granos[i]->dens;
                fin >> granos[i]->fric;
                fin >> granos[i]->rest;
                fin >> granos[i]->m;
                fin >> granos[i]->r;
                if (granos[i]->noGranos < 1) {
                cout << "ERROR: El número de granos debe ser > 0." << endl;
                exit(1);
                }
                if (granos[i]->radio < 0.01) {
                cout << "ERROR: El radio de los granos debe ser > 0.01" << endl;
                exit(1);
                }
                if ((granos[i]->nLados != 1) && (granos[i]->nLados < 1 
                            || granos[i]->nLados > 8)) {
                cout << "ERROR: El número de lados debe ser 1 o 2 < nLados < 8." 
                    << endl;
                exit(1);
                }
                if (granos[i]->dens <= 0) {
                    cout << "ERROR: La densidad de los granos debe ser > 0." 
                        << endl;
                    exit(1);
                }
                if (granos[i]->fric < 0.0) {
                    cout << "ERROR: El coeficiente de rozamiento debe ser 0 " 
                        << " <= rozam." << endl;
                    exit(1);
                }
                if (granos[i]->rest < 0.0 || granos[i]->rest > 1.0) {
                    cout << "ERROR: El coeficiente de restitución debe ser 0 "
                        << "<= rest <= 1.0." << endl;
                    exit(1);
                }
                if (granos[i]->nLados > 1) {
                    granos[i]->vertices = new double*[granos[i]->nLados];
                    double x,y,theta;
                    theta = 2.0*3.141592653589793/granos[i]->nLados;
                    for (int j = 0; j < granos[i]->nLados; j++) {
                        x = granos[i]->radio*cos(j*theta); 
                        y = granos[i]->radio*sin(j*theta); 
                        granos[i]->vertices[j] = new double[2];
                        granos[i]->vertices[j][0] = x;
                        granos[i]->vertices[j][1] = y;
                    }
                }
            }
        }
        // Fin parámetros de granos
        //
        // Lectura de parámetros de control
        //
        if (ident == "timeStep:") {
            fin >> tStep;
            if (tStep < 0.0) {
                cout << "ERROR: El paso de integración debe ser >= 0." << endl;
                exit(1);
            }
        }
        if (ident == "tMax:") {
            fin >> tMax;
            if (tMax < 0.0) {
                cout << "ERROR: el tiempo máximo de simulación debe ser > 0." 
                    << endl;
                exit(1);
            }
        }
        if (ident == "finEkin:") {
            fin >> EkStop;
        }
        if (ident == "pIter:") {
            fin >> pIter;
            if (pIter < 0) {
                cout << "ERROR: El número de iteraciones de posicion debe ser "
                    << "> 0." << endl;
                exit(1);
            }
        }
        if (ident == "vIter:") {
            fin >> vIter;
            if (vIter < 0) {
                cout << "ERROR: El número de iteraciones de velocidad debe ser "
                    << "> 0." << endl;
                exit(1);
            }
        }
        if (ident == "g:") {
            fin >> g;
        }
        if (ident == "noise:") {
            fin >> noise;
            if (noise < 0.0) {
                cout << "Error: noise debe ser >= 0." << endl;
                exit(1);
            }
        }
        if (ident == "noiseFreq:") {
            fin >> noiseFreq;
            if (noise < 0) {
                cout << "Error: la frecuencia del ruido  ser >= 0." << endl;
                exit(1);
            }
        }
        if (ident == "tNoiseOff:") {
            fin >> tNoiseOff;
            if (tMax < 0.0) {
                cout << "ERROR: el tiempo de apagado del ruido debe ser > 0." 
                    << endl;
                exit(1);
            }
        }
        if (ident == "Bullets:") {
            string bull;
            fin >> bull;
            isBullet = (bull == "T" ? true : false);
        }
        if (ident == "RandomSeed:") {
            fin >> randomSeed;
        }
        if (ident == "preFrameFile:") {
            fin >> preFrameFile;
        }
        if (ident == "saveFrameFreq:") {
            fin >> saveFrameFreq;
            if (saveFrameFreq < 0) {
            cout << "ERROR: la frecuencia de guardado debe ser >= 0." << endl;
            exit(1);
            }
        }
        if (ident == "xvc_File:") {
            fin >> xvcFile;
        }
        if (ident == "xvc_Freq:") {
            fin >> xvcFreq;
            if (xvcFreq < 0) {
                cout << "ERROR: la frecuencia de guardado de coordenadas, "
                     << "velocidades y contactos debe ser >= 0." << endl;
                exit(1);
            }
        }
        if (ident == "atenuacion_rot:") {
            fin >> atRot;
            if (atRot < 0 || atRot > 1.0) {
                cout << "ERROR: el coeficiente de atenuación de rotaciones "
                    << "debe ser 0 <= coef <= 1." << endl;
                exit(1);
            }
        }
        if (ident == "enerFreq:") {
            fin >> enerFreq;
            if (enerFreq < 0) {
                cout << "ERROR: la frecuencia de guardado de energías debe ser"
                    << " >= 0." << endl;
                exit(1);
            }
        }
        if (ident == "enerFile:") {
            fin >> enerFile;
        }
        if (ident == "finXVCFile:") {
            fin >> finXVCFile;
        }
    } //fin bucle de lectura de inputFile
} // Fin función load()

 void GlobalSetup::printGlobalSetup() {
    cout << "# Lectura de los parámetros de entrada ..." << endl;
    cout << "#  Archivo de parámetros: " << inputFile << endl;
    cout << "# Parámetros de los objetos" << endl;
    cout << "# \t Radio de la caja: " << caja.R << " [m]" << endl;
    cout << "# \t Número de vértices de la caja: " << caja.nVerts << endl;
    cout << "# \t Coeficiente de fricción de la caja: " << caja.fric << endl;
    cout << "# \t Coeficiente de restitución de la caja: " << caja.rest 
        << endl;
    cout << "# \t Coeficiente de fricción estático grano-base: " 
        << caja.fgb_stat << endl;
    cout << "# \t Coeficiente de fricción dinámico grano-base: " 
        << caja.fgb_stat << endl;
    cout << "#  Parámetros de los granos:" << endl;
    cout << "# \t Número de tipos de granos: " << noTipoGranos << endl;
    for (int i = 0; i < noTipoGranos; i++) {
        cout << "# \t Grano tipo " << i + 1 << ":" << endl;
        cout << "# \t   Número de granos: " << granos[i]->noGranos << endl;
        cout << "# \t   Radio = " << granos[i]->radio << " [m]" << endl;
        cout << "# \t   Densidad = " << granos[i]->dens << " [kg/m²]" << endl;
        cout << "# \t   Coeficiente de fricción = " << granos[i]->fric << endl;
        cout << "# \t   Coeficiente de restitución = " << granos[i]->rest 
            << endl;
        cout << "# \t   Geometría: ";
        if (granos[i]->nLados == 1) cout << "Disco." << endl;
        else {
            cout << "Polígono de " << granos[i]->nLados << " lados." << endl;
            cout <<"# \t   Vértices: " << endl;
            for (int j = 0; j < granos[i]->nLados; j++) {
                cout << "# \t\t(" << fixed << setw(4) 
                    << granos[i]->vertices[j][0] << ", " << fixed << setw(4) 
                    << granos[i]->vertices[j][1] << "), " << endl;
            }
        }
        cout << "# \t   Momento dipolar magético: " << granos[i]->m << endl;
        cout << "# \t   Sentido del movimiento: " 
            << (granos[i]->r ? "atrás" : "adelante.") << endl;
    }
    cout << "# Parámetros de control:" << endl;
    cout << "# \t Paso de integración: " << tStep << " s."<< endl;
    cout << "# \t Tiempo máximo de simulación: " << tMax << " s." << endl;
    cout << "# \t Energía cinética de finalización: " << EkStop << " J." << endl;
    cout << "# \t Iteraciones para restricciones de posición: " << pIter 
        << endl;
    cout << "# \t Iteraciones para restricciones de velocidad: " << vIter 
        << endl;
    cout << "# \t Aceleración de la gravedad: " << g << " m/s²." << endl;
    cout << "# \t Intensidad de la vibración: " << noise << " Ns." << endl; 
    cout << "# \t Frecuencia de la vibración: " << noiseFreq << " pasos." 
        << endl; 
    cout << "# \t Coeficiente de atenuación de rotaciones: " << atRot << endl; 
    cout << "# \t Tiempo de apagado de la vibración: " << tNoiseOff << " s." 
        << endl; 
    cout << "# \t Granos considerados bullet? " << (isBullet ? "Si." : "No.") 
        << endl;
    cout << "# \t Semilla del generador de números aleatorios: " << randomSeed 
        << endl;
    cout << "# Parámetros de estadísticas y registros:" << endl;
    cout << "# \t Prefijo de archivos de frames: " << preFrameFile << endl;
    cout << "# \t Frecuencia de guardado de frames: " << saveFrameFreq 
        << " pasos" << endl;
    cout << "# \t Frecuencia de guardado de coordenadas, velocidades y "
         << "contactos: " << xvcFreq << " pasos" << endl;
    cout << "# \t Prefijo de archivo de coordenadas, velocidades y contactos: " 
         << xvcFile << endl;
    cout << "# \t Archivo final xvc : " << finXVCFile << endl;
    cout << "# \t Frecuencia de guardado de energías: " << enerFreq << " pasos"
        << endl;
    cout << "# \t Archivo de registro de energías: " << enerFile << endl;
    cout << "# ... lectura de parámetros de entrada finalizada." << endl;
}
