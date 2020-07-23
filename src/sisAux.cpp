/*! \file siloAux.cpp
 * \brief Archivo de implementación de funciones auxiliares
 *
 * \author Manuel Carlevaro <manuel@iflysib.unlp.edu.ar>
 *
 * \version 1.1 
 *
 * \date 2020.06.11
 */

#include "sisAux.hpp"

std::string n2s(int num) {
    std::ostringstream oss;
    oss << std::setfill('0') << setw(6) << num;
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
            *(ff) << infGr->gID << " " << p.x << " " << p.y << " " << angle 
                  << " " << endl;
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
        i1->f = b2Vec2(0.0, 0.0);
    }
    for (b2Body *bd1 = w->GetBodyList(); bd1; bd1 = bd1->GetNext()) {
        i1 = (BodyData*) (bd1->GetUserData());
        if (!i1->isMag) continue;
        r1 = bd1->GetPosition();
        for (b2Body *bd2 = bd1->GetNext(); bd2; bd2 = bd2->GetNext()) {
            i2 = (BodyData*) (bd2->GetUserData());
            if (!i2->isMag) continue;
            if (i1->gID == i2->gID) continue;
            r2 = bd2->GetPosition();
            r12.Set(r2.x - r1.x, r2.y - r1.y);
            r = r12.Length();
            fB = 3.0E-7 * i1->m * i2->m / (r * r * r * r);
            r12.Normalize();
            r12 *= fB;
            i1->f -= r12;
            i2->f += r12;
        }
    }
}

void saveXVCFile(std::ofstream *ff, b2World *w) {
    b2Vec2 p, v; 
    float ang, angv;
    (*ff) << setw(5) << "#id" << " "
        << setw(8) << "xc" << " "
        << setw(8) << "yc" << " "
        << setw(8) << "ang" << " "
        << setw(8) << "xv" << " "
        << setw(8) << "yv" << " "
        << setw(8) << "angV" << " "
        << setw(2) << "t" << " "
        << setw(2) << "nc" << " c1 c2 ..." << std::endl;

    for (b2Body* bd = w->GetBodyList(); bd; bd = bd->GetNext()) {
        BodyData* infGr = (BodyData*) (bd->GetUserData());
        if (!(infGr->isGrain)) continue; 
        p = bd->GetPosition();
        ang = bd->GetAngle();
        v = bd->GetLinearVelocity();
        angv = bd->GetAngularVelocity();
        vector<int> contacts;
        for (b2ContactEdge *e = bd->GetContactList(); e; e = e->next) {
            if(!(e->contact->IsTouching())) continue;
            b2Body *bb = e->other;
            BodyData *infGrB = (BodyData*) (bb->GetUserData());
            contacts.push_back(infGrB->gID);
        }
        (*ff) << setw(5) << infGr->gID  << " "
            << setw(8) << fixed << setprecision(3) << p.x << " "
            << setw(8) << fixed << setprecision(3) << p.y << " " 
            << setw(8) << fixed << setprecision(3) << ang << " "
            << setw(8) << fixed << setprecision(3) << v.x << " "
            << setw(8) << fixed << setprecision(3) << v.y << " "
            << setw(8) << fixed << setprecision(3) << angv << " "
            << setw(2) << infGr->tipo + 1 << " ";
        (*ff) << contacts.size() << " ";
        for (size_t i = 0; i < contacts.size(); ++i) {
            (*ff) << contacts[i] << " ";
        }
        (*ff) << std::endl;
    }
}

Energias energyCalculation(b2World *w){
    Energias eKU {0.0, 0.0};
    b2Vec2 pi, vi, pj, pij;
    float wi, mi, Ii, vim, pijm;
    float mui, muj;
    for (b2Body* bi = w->GetBodyList(); bi; bi = bi->GetNext()) {
        BodyData* igi = (BodyData*) (bi->GetUserData());
        if (!igi->isGrain) continue;
        pi = bi->GetPosition();
        vi = bi->GetLinearVelocity();
        vim = vi.Length();
        wi = bi->GetAngularVelocity();
        mi = bi->GetMass();
        mui = igi->m;
        Ii = bi->GetInertia();
        eKU.eKin += 0.5 * (mi * vim * vim + Ii * wi * wi);
        for (b2Body* bj = bi->GetNext(); bj; bj = bj->GetNext()) {
            BodyData* igj = (BodyData*) (bj->GetUserData());
            if (!igj->isGrain) continue;
            pj = bj->GetPosition();
            muj = igj->m;
            pij = pj - pi;
            pijm = pij.Length();
            // Ver 
            // https://en.wikipedia.org/wiki/Magnetic_dipole-dipole_interaction
            eKU.ePot += 1.0E-7 * mui * muj / (pijm * pijm * pijm);
        }
    }
    return eKU;
}
