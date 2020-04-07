/*! \file siloAux.cpp
 * \brief Archivo de implementación de funciones auxiliares
 *
 * \author Manuel Carlevaro <manuel@iflysib.unlp.edu.ar>
 *
 * \version 1.0 Versión inicial
 *
 * \date 2019.02.16
 */

#include "sisAux.hpp"

std::string n2s(int num) {
    std::ostringstream oss;
    oss << std::setfill('0') << std::setw(6) << num;
    return oss.str();
}

bool isActive(b2World *w) {
    BodyData* infGr;
    b2Vec2 posGr;
    for (b2Body *bd = w->GetBodyList(); bd; bd = bd->GetNext()) {
        infGr = (BodyData*) (bd->GetUserData());
        if (infGr->isGrain && bd->IsAwake()) return true;
    }
    return false;
}

void savePart(std::ofstream *ff, b2World *w) {
    BodyData* infGr;
    b2Vec2 p; float angle;
    for (b2Body *bd = w->GetBodyList(); bd; bd = bd->GetNext()) {
        infGr = (BodyData*) (bd->GetUserData());
        if (infGr->isGrain) {
            p = bd->GetPosition();
            angle = bd->GetAngle();
            *(ff) << infGr->gID << " " << p.x << " " << p.y << " " << angle << " " << endl;
        }
    }
    *(ff) << std::flush;
}

void saveFrame(std::ofstream *ff,  b2World *w) {
    b2Vec2 v2; float xtmp, ytmp;
    for ( b2Body* bd = w->GetBodyList(); bd; bd = bd->GetNext()) {
        BodyData* infGr = (BodyData*) (bd->GetUserData());
        *(ff) << infGr->gID << " ";
        if (infGr->isGrain) {
            if (infGr->nLados > 1) {    // Es un polígono
                b2Fixture* f = bd->GetFixtureList();
                b2Shape *shape = f->GetShape();
                b2PolygonShape *poly = (b2PolygonShape*) shape;
                int count = poly->m_count;
                *(ff) << count << " ";
                b2Vec2* verts = (b2Vec2*) poly->m_vertices;
                for (int i = 0; i < count; ++i) {
                    xtmp = bd->GetWorldPoint(verts[i]).x;
                    ytmp = bd->GetWorldPoint(verts[i]).y;
                    *(ff) << xtmp << " " << ytmp << " ";
                }
            }
            if (infGr->nLados == 1) {   // Es un círculo
                *(ff) << "1 ";
                b2Vec2 pos = bd->GetPosition();
                 b2Fixture* f = bd->GetFixtureList();
                 b2Shape* bs = (b2Shape*) f->GetShape();
                float radio = bs->m_radius;
                *(ff) << pos.x << " " << pos.y << " " << radio << " ";
                //v2 = bd->GetLinearVelocity();
                //angle = bd->GetAngularVelocity();
                //*(ff) << v2.x << " " << v2.y << " " << angle << " ";
            }
            if (infGr->isMag) {
                *(ff) << 1 << " ";
            }
            else {
                *(ff) << 0 << " ";
            }
            *(ff) << infGr->tipo << " ";
            *(ff) << endl;
        }
        else {  // Es la caja
             b2Fixture *f = bd->GetFixtureList();
             b2ChainShape *s = (b2ChainShape*) f->GetShape();
            b2Vec2 *verts = (b2Vec2*) s->m_vertices;
            *(ff) << s->m_count << " ";
            for (int i = 0; i < s->m_count; ++i) {
                verts[i] = bd->GetWorldPoint(verts[i]);
                *(ff) << verts[i].x << " " << verts[i].y << " ";
            }
            *(ff) << "BOX" << endl;
        }
    }
}

int countDesc(b2World *w, int *st, int maxDiam) {
    BodyData* infGr;
    b2Vec2 p;
    int nGranos = 0;
    for (b2Body *bd = w->GetBodyList(); bd; bd = bd->GetNext()) {
        infGr = (BodyData*) (bd->GetUserData());
        if (infGr->isGrain) {
            p = bd->GetPosition();
            if (p.y < -maxDiam) {
                nGranos++;
                st[infGr->tipo]++;
            }
        }
    }
    return nGranos;
}

void setMagneticForces(b2World *w) {
    BodyData *i1, *i2;
    b2Vec2 r1, r2, r12, sumF;
    float fB, r;
    for (b2Body *bd1 = w->GetBodyList(); bd1; bd1 = bd1->GetNext()) {
        i1 = (BodyData*) (bd1->GetUserData());
        if (!i1->isMag) continue;
        r1 = bd1->GetPosition();
        sumF.Set(0.0f, 0.0f);
        for (b2Body *bd2 = w->GetBodyList(); bd2; bd2 = bd2->GetNext()) {
            i2 = (BodyData*) (bd2->GetUserData());
            if (!i2->isMag) continue;
            if (i1->gID == i2->gID) continue;
            r2 = bd2->GetPosition();
            r12.Set(r2.x - r1.x, r2.y - r1.y);
            r = r12.Length();
            fB = 3.0E-7 * i1->m * i2->m / (r * r * r * r * r);
            r12.Normalize();
            r12 *= fB;
            sumF -= r12;
        }
        i1->f = sumF;
        //cout << "Grano " << i1->gID << " Fuerza " << i1->f.x << " , " << i1->f.y << endl;
    }
}
