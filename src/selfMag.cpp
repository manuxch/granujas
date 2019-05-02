/** \mainpage selfMag
 * \brief Simulación de granos magnéticos autopropulsados.
 * 
 * Programa que implementa la biblioteca Box2D para simular un sistema
 * de granos autopropulsados con dipolos magnéticos.
 *
 * \author Manuel Carlevaro <manuel@iflysib.unlp.edu.ar>
 * 
 * \version 1.0 Versión inicial 
 *
 * \date 2019.02.15
 *
 * */


#include <iostream>
using namespace std;
#include <Box2D/Box2D.h>
#include "globalSetup.hpp"
#include <string>
#include <cmath>
using std::sin; using std::cos; using std::fabs;
#include <iomanip>
#include <sstream>
#include <cstdlib>
using std::exit;
#include "sisAux.hpp"
#include "random.hpp"

#define PI 3.1415926

int main(int argc, char *argv[])
{
    cout << "# SiloMag" << endl;
    cout << "# v0.1 [2018.12.14]" << endl;
    string inputParFile(argv[1]);
    GlobalSetup *globalSetup = new GlobalSetup(inputParFile); 
    RandomGenerator rng(globalSetup->randomSeed);
    cout << "# Creación del sistema ..." << endl;

    // Definición del mundo
    //b2Vec2 gravedad(0.0f, globalSetup->g );
    b2Vec2 gravedad(0.0f, 0.0f);
    b2World world(gravedad);
    cout << "#\t- Objeto mundo creado." << endl;
    
    // Definición del contenedor
    b2BodyDef caja_def;
    caja_def.position.Set(0.0f, 0.0f);
    caja_def.type = b2_staticBody;
    b2Body* caja;
    caja = world.CreateBody(&caja_def);
    
    BodyData* cajaD = new BodyData;
    cajaD->isGrain = false;
    cajaD->nLados = globalSetup->caja.nVerts;
    cajaD->gID = -100;
    cajaD->m = 0.0f;
    caja->SetUserData(cajaD);
    
    int32 nverts = globalSetup->caja.nVerts;
    float32 xtmp, ytmp;
    b2Vec2 *verts = new b2Vec2[nverts];
    float32 angle = 0.0, delta_angle = 360.0/nverts;
    for (int i = 0; i < nverts; ++i) {
        xtmp = globalSetup->caja.R * cos(angle * b2_pi / 180.0f);
        ytmp = globalSetup->caja.R * sin(angle * b2_pi / 180.0f);
        verts[i].Set(xtmp, ytmp);
        angle += delta_angle;
    }
    b2ChainShape cajaShape;
    cajaShape.CreateLoop(verts, nverts);
    b2FixtureDef cajaFix;
    cajaFix.shape = &cajaShape;
    cajaFix.density = 0.0f;
    cajaFix.friction = globalSetup->caja.fric;
    caja->CreateFixture(&cajaFix);
    cout << "#\t- Silo creado." << endl;
    
    // Generación de granos.
    float32 xInf, xSup, yInf, ySup, x, y;
    float32 mg = 0.0;
    int noTotGranos = 0;
    xInf = 0.707107f * globalSetup->caja.R; // srtq(2) * R
    xSup = -0.707107f * globalSetup->caja.R;
    yInf = 0.707107f * globalSetup->caja.R;
    ySup = -0.707107f * globalSetup->caja.R;

    BodyData **gInfo;
    gInfo = new BodyData*[globalSetup->noTipoGranos];
    
    int contGid = 1;
    int *sumaTipo = new int[globalSetup->noTipoGranos] {};
    for (int i = 0; i < globalSetup->noTipoGranos; i++) { // Loop sobre tipos de granos.
        gInfo[i] = new BodyData[globalSetup->granos[i]->noGranos];
        for (int j = 0; j < globalSetup->granos[i]->noGranos; j++) { // Loop sobre el número de granos de cada tipo.
            x = rng.getAB(xInf, xSup);
            y = rng.getAB(yInf, ySup);
            gInfo[i][j].tipo = i;
            gInfo[i][j].isGrain = true;
            gInfo[i][j].isMag = (abs(globalSetup->granos[i]->m) > 1.0E-8 ? true : false);
            gInfo[i][j].m = globalSetup->granos[i]->m;
            gInfo[i][j].nLados = globalSetup->granos[i]->nLados;
            gInfo[i][j].f.Set(0.0f, 0.0f);
            gInfo[i][j].gID = contGid++;
            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.allowSleep = true;
            bd.bullet = globalSetup->isBullet;
            bd.position.Set(x,y);
            bd.angle = rng.getAB(-b2_pi,b2_pi);
            b2Body* grain = world.CreateBody(&bd);
            grain->SetUserData(&gInfo[i][j]);
            if (globalSetup->granos[i]->nLados == 1) {
                b2CircleShape circle;
                circle.m_radius = globalSetup->granos[i]->radio;
               //if (circle.m_radius > maxDiam) maxDiam = circle.m_radius;
                b2FixtureDef fixDef;
                fixDef.shape = &circle;
                fixDef.density = globalSetup->granos[i]->dens;
                fixDef.friction = globalSetup->granos[i]->fric;
                fixDef.restitution = globalSetup->granos[i]->rest;
                grain->CreateFixture(&fixDef);
                mg += grain->GetMass();
                if (j == 0) cout << "#\t- Grano de tipo " << i << " creado con masa " << grain->GetMass() << " kg." << endl;
            }
            else {
                b2PolygonShape poly;
                int32 vertexCount = globalSetup->granos[i]->nLados;
                b2Vec2 vertices[8];
                for (int k = 0; k < globalSetup->granos[i]->nLados; k++) 
                    vertices[k].Set(globalSetup->granos[i]->vertices[k][0],globalSetup->granos[i]->vertices[k][1]);
                poly.Set(vertices,vertexCount);
                b2FixtureDef fixDef;
                fixDef.shape = &poly;
                fixDef.density = globalSetup->granos[i]->dens;
                fixDef.friction = globalSetup->granos[i]->fric;
                fixDef.restitution = globalSetup->granos[i]->rest;
                grain->CreateFixture(&fixDef);
                mg += grain->GetMass();
                if (j == 0) cout << "#\t- Grano de tipo " << i << " creado con masa " << grain->GetMass() << " kg." << endl;
            }
            noTotGranos++;
        } // Fin loop sobre el número de granos de cada tipo.
    } // Fin loop sobre tipo de granos	
    cout << "#\t- Número total de granos = " << noTotGranos << endl;
    cout << "#\t- Masa total de granos = " << mg << " kg."<< endl;

    // Preparamos los parámetros de la simulación. 
    float32 timeStep = globalSetup->tStep;
    float32 timeS = 0.0;
    int32 pIterations = globalSetup->pIter;
    int32 vIterations = globalSetup->vIter;

    cout << "# Inicio de la simulación..." << endl;
    b2Vec2 pos, avec; 
    int paso = 0;
    bool saveFrm = (globalSetup->saveFrameFreq > 0 ? true : false);
    bool saveFlux = (globalSetup->fluxFreq > 0 ? true : false);
    std::ofstream fileFlux;
    if (saveFlux) {
        fileFlux.open((globalSetup->fluxFile).c_str());
        fileFlux << "# time  Grains" << " ";
        for (int i = 0; i < globalSetup->noTipoGranos; ++i) {
            fileFlux << " - Tipo " << i;
        }
        fileFlux << endl;
    }
    double noiseInt, noiseAng;
    std::ofstream fileF;
    BodyData *infGr;

    b2Vec2 vtmp;
    float32 vtmpM, fricBase;

    // Primeros pasos para satisfacer restricciones
    for (int i = 0; i < 5; ++i) world.Step(timeStep, pIterations, vIterations);
    
    // Guardo configuración inicial
    string foutName = globalSetup->preFrameFile+ "_" + n2s(paso) + ".xy";
    fileF.open(foutName.c_str());
    saveFrame(&fileF, &world);
    fileF.close();
    cout << "Frame " << paso << " guardado en " << timeS << endl;

    while (isActive(&world)) {

        // Si es necesario, aplicación de impulsos
        if (globalSetup->noiseFreq && !(paso % globalSetup->noiseFreq)) {
            noiseInt = globalSetup->noise;
            //noiseInt = rng.get01() * globalSetup->noise;
            for (b2Body *bd = world.GetBodyList(); bd; bd = bd->GetNext()) {
                infGr = (BodyData*) (bd->GetUserData());
                if (infGr->isGrain) {
                    //bd->ApplyForce(avec, bd->GetWorldCenter(), true);
                    noiseAng = bd->GetAngle();
                    avec.Set(noiseInt * cos(noiseAng), noiseInt * sin(noiseAng));
                    bd->ApplyLinearImpulseToCenter(avec, true);
                }
            }
        }


        // Cálculo y aplicación de fuerzas magnéticas y fricción con la base
        setMagneticForces(&world);
        for (b2Body *bd = world.GetBodyList(); bd; bd = bd->GetNext()) {
            infGr = (BodyData*) (bd->GetUserData());
            bd->ApplyForce(infGr->f, bd->GetWorldCenter(), true);
            vtmp = bd->GetLinearVelocity();
            vtmpM = vtmp.Length();
            if (vtmpM > 1.0e-3) {
                fricBase = globalSetup->caja.fric * 9.8f * bd->GetMass();
                vtmp.Normalize();
                vtmp = -vtmp;
                vtmp *= fricBase;
                bd->ApplyForceToCenter(vtmp, true);
            }
            else {
                bd->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                bd->SetAngularVelocity(0.0f);
            }
        }

        // Avance del sistema en el tiempo
        world.Step(timeStep,pIterations,vIterations);
        paso++;
        timeS += timeStep;

        // Si es necesario, guardo el frame para graficar
        if (saveFrm && !(paso % globalSetup->saveFrameFreq)) {
            string foutName = globalSetup->preFrameFile+ "_" + n2s(paso) + ".xy";
            fileF.open(foutName.c_str());
            saveFrame(&fileF, &world);
            fileF.close();
            cout << "Frame " << paso << " guardado en " << timeS << endl;
        }
        if (timeS > globalSetup->tMax) {
            cout << "# Máximo tiempo de simulación alcanzado." << endl;
            break;
        }
    }
    cout << "Simulación finalizada." << endl;

    
    // Clean up
    if (saveFlux) fileFlux.close();
    delete globalSetup;
    delete [] gInfo;
    delete [] verts;
    delete [] sumaTipo;

    return 0;
}	
