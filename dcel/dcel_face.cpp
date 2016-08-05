/*
 * @author    Alessandro Muntoni (muntoni.alessandro@gmail.com)
 * @copyright Alessandro Muntoni 2016.
 */

#include "dcel_face_iterators.h"
#include "dcel_vertex_iterators.h"
#include "../common/utils.h"
#ifdef CGAL_DEFINED
#include "../cgal/cgalinterface.h"
#endif

/****************
 * Constructors *
 ****************/
/**
 * \~Italian
 * @brief Costruttore di default.
 *
 * Crea una faccia con:
 * - outer half edge settato a nullptr;
 * - nessun inner half edge;
 * - normale pari a (0, 0, 0);
 * - area pari a 0;
 * - id pari a 0;
 * - flag pari a 0.
 */
Dcel::Face::Face() : outerHalfEdge(nullptr), area(0), id(0), flag(0){
    innerHalfEdges.clear();
}

/**
 * \~Italian
 * @brief Costruttore.
 *
 * Crea una faccia con:
 * - outer half edge pari al parametro in input outer;
 * - nessun inner half edge;
 * - normale pari a (0, 0, 0);
 * - area pari a 0;
 * - id pari a 0;
 * - flag pari a 0.
 *
 *
 * @param[in] outer: puntatore all'outer half edge settato alla faccia
 */
Dcel::Face::Face(Dcel::HalfEdge* outer) : outerHalfEdge(outer), area(0), id(0), flag(0) {
    innerHalfEdges.clear();
}

/**
 * \~Italian
 * @brief Costruttore.
 *
 * Crea una faccia con:
 * - outer half edge pari al parametro in input outer;
 * - nessun inner half edge;
 * - normale pari al parametro in input normal;
 * - area pari a 0;
 * - id pari a 0;
 * - flag pari a 0;
 *
 *
 * @param[in] outer: puntatore all'outer half edge settato alla faccia
 * @param[in] normal: vettore normale assegnato alla faccia
 */
Dcel::Face::Face(Dcel::HalfEdge* outer, const Vec3& normal) : outerHalfEdge(outer), normal(normal), area(0), id(0), flag(0) {
    innerHalfEdges.clear();
}

/**
 * \~Italian
 * @brief Distruttore vuoto.
 *
 * La classe Dcel dovrà occuparsi di eliminare tutti i riferimenti in essa contenuti (e quindi contenuti di conseguenza anche nella classe Dcel::Face).
 */
Dcel::Face::~Face(void) {}


/******************
 * Public Methods *
 ******************/

const Dcel::Vertex*Dcel::Face::getVertex1() const {
    return outerHalfEdge->getFromVertex();
}

const Dcel::Vertex*Dcel::Face::getVertex2() const {
    return outerHalfEdge->getToVertex();
}

const Dcel::Vertex*Dcel::Face::getVertex3() const {
    return outerHalfEdge->getNext()->getToVertex();
}

/**
 * \~Italian
 * @brief Funzione che verifica se la faccia è un triangolo
 * @warning utilizza Dcel::Face::ConstIncidentVertexIterator
 * @return True se la faccia è un triangolo, false altrimenti
 */
bool Dcel::Face::isTriangle() const {
    Dcel::Face::ConstIncidentVertexIterator v = incidentVertexBegin();
    ++v; ++v; ++v;
    return (v == incidentVertexEnd());
}

/**
 * \~Italian
 * @brief Restituisce il numero di vertici incidenti alla faccia
 * @warning Utilizza Dcel::Face::ConstIncidentVertexIterator
 * @return Il numero di vertici incidenti alla faccia
 */
int Dcel::Face::getNumberIncidentVertices() const {
    ConstIncidentVertexIterator vi;
    int n = 0;
    for (vi = incidentVertexBegin(); vi != incidentVertexEnd(); ++vi) n++;
    return n;
}

/**
 * \~Italian
 * @brief Restituisce il numero di half edge incidenti alla faccia
 * @warning Utilizza Dcel::Face::ConstIncidentHalfEdgeIterator
 * @return Il numero di half edge incidenti alla faccia
 */
int Dcel::Face::getNumberIncidentHalfEdges() const {
    ConstIncidentHalfEdgeIterator hei;
    int n = 0;
    for (hei = incidentHalfEdgeBegin(); hei != incidentHalfEdgeEnd(); ++hei) n++;
    return n;
}

/**
 * \~Italian
 * @brief Calcola e restituisce il baricentro della faccia.
 *
 * Il baricentro è calcolato come media dei vertici incidenti alla faccia.
 *
 * @warning Utilizza Dcel::Face::ConstIncidentVertexIterator
 * @return Il baricentro della faccia.
 */
Pointd Dcel::Face::getBarycentre() const {
    int n = 0;
    Pointd p;
    for (ConstIncidentVertexIterator vit = incidentVertexBegin(); vit != incidentVertexEnd(); ++vit){
        p += (*vit)->getCoordinate();
        n++;
    }
    p /= n;
    return p;
}

/**
 * \~Italian
 * @brief Funzione toString di una faccia
 * @return Una stringa rappresentativa della faccia
 * @todo Da aggiornare
 */
std::string Dcel::Face::toString() const {
    std::stringstream ss;

    ss << "ID: " << id << "; Normal: " << normal.toString() << "; Outer Component: ";
    if (outerHalfEdge != nullptr) ss << outerHalfEdge->getId();
    else ss << "nullptr";
    ss << "; N Inner Components: " << innerHalfEdges.size() << "; Inner Components: "
       << innerComponentsToString() << ".";
    std::string s1 = ss.str();
    return s1;
}

Dcel::Face::ConstAdjacentFaceIterator Dcel::Face::adjacentFaceBegin() const {
    return std::move(ConstAdjacentFaceIterator(outerHalfEdge, outerHalfEdge, this));
}

Dcel::Face::ConstAdjacentFaceIterator Dcel::Face::adjacentFaceEnd() const {
    return std::move(ConstAdjacentFaceIterator(nullptr, nullptr, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::ConstInnerHalfEdgeIterator
 * @return Un iteratore che punta al primo inner half edge della faccia
 */
Dcel::Face::ConstInnerHalfEdgeIterator Dcel::Face::innerHalfEdgeBegin() const {
    return std::move(innerHalfEdges.begin());
}

/**
 * \~Italian
 * @brief Funzione di finalizzazione di Dcel::Face::ConstInnerHalfEdgeIterator
 * @return Un iteratore che punta all'ultimo inner half edge della faccia
 */
Dcel::Face::ConstInnerHalfEdgeIterator Dcel::Face::innerHalfEdgeEnd() const {
    return std::move(innerHalfEdges.end());
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::ConstIncidentHalfEdgeIterator.
 *
 * Permette di ciclare sugli half edge incidenti alla faccia, partendo dall'outer half edge.
 *
 * @return Un iteratore che punta all'outer half edge della faccia
 */
Dcel::Face::ConstIncidentHalfEdgeIterator Dcel::Face::incidentHalfEdgeBegin() const {
    return std::move(ConstIncidentHalfEdgeIterator(outerHalfEdge, outerHalfEdge, this));
}

/**
 * \~Italian
 * @brief Funzione di finalizzazione di Dcel::Face::ConstIncidentHalfEdgeIterator
 * @warning L'iteratore restituito non è un iteratore valido, e su di esso le operazioni di
 *          incremento e decremento sono inutili. Questa funzione è da utilizzarsi solamente per
 *          il \b confronto \b con \b un \b altro \b iteratore \b valido
 * @return Un iteratore di finalizzazione
 */
Dcel::Face::ConstIncidentHalfEdgeIterator Dcel::Face::incidentHalfEdgeEnd() const {
    return std::move(ConstIncidentHalfEdgeIterator(nullptr, nullptr, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::ConstIncidentHalfEdgeIterator.
 *
 * Permette di ciclare sugli half edge incidenti alla faccia, partendo dall'half edge start.
 *
 * @param[in] start: half edge di partenza
 * @warning Se start non risulta essere incidente alla faccia, viene lanciata un'asserzione e il programma termina
 * @return Un iteratore che punta all'half edge start
 */
Dcel::Face::ConstIncidentHalfEdgeIterator Dcel::Face::incidentHalfEdgeBegin(const Dcel::HalfEdge* start) const {
    #ifdef DEBUG
    if (start->getFace() != this){
        std::cerr << "ERROR: start half edge " << start->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    #endif
    return std::move(ConstIncidentHalfEdgeIterator(start, start, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::ConstIncidentHalfEdgeIterator.
 *
 * Permette di ciclare sugli half edge incidenti alla faccia, partendo dall'half edge start e fino all'half edge end.
 *
 * @param[in] start: half edge di partenza
 * @param[in] end: half edge di arrivo, \b non \b compreso
 * @warning Se start e end non risultano essere incidenti alla faccia, viene lanciata un'asserzione e il programma termina
 * @return Un iteratore che punta all'half edge start
 */
Dcel::Face::ConstIncidentHalfEdgeIterator Dcel::Face::incidentHalfEdgeBegin(const Dcel::HalfEdge* start, const Dcel::HalfEdge* end) const {
    #ifdef DEBUG
    if (start->getFace() != this){
        std::cerr << "ERROR: start half edge " << start->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    if (end->getFace() != this){
        std::cerr << "ERROR: end half edge " << end->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    #endif
    return std::move(ConstIncidentHalfEdgeIterator(start, end, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::ConstIncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal toVertex dell'outer half edge.
 *
 * @return Un iteratore che punta al toVertex dell'outer half edge della faccia
 */
Dcel::Face::ConstIncidentVertexIterator Dcel::Face::incidentVertexBegin() const {
    return std::move(ConstIncidentVertexIterator(outerHalfEdge, outerHalfEdge, this));
}

/**
 * \~Italian
 * @brief Funzione di finalizzazione di Dcel::Face::ConstIncidentVertexIterator
 * @warning L'iteratore restituito non è un iteratore valido, e su di esso le operazioni di
 *          incremento e decremento sono inutili. Questa funzione è da utilizzarsi solamente per
 *          il \b confronto \b con \b un \b altro \b iteratore \b valido
 * @return Un iteratore di finalizzazione
 */
Dcel::Face::ConstIncidentVertexIterator Dcel::Face::incidentVertexEnd() const {
    return std::move(ConstIncidentVertexIterator(nullptr, nullptr, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::ConstIncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal toVertex dell'half edge start.
 *
 * @param[in] start: half edge di partenza
 * @warning Se start non risulta essere incidente alla faccia, viene lanciata un'asserzione e il programma termina
 * @return Un iteratore che punta al toVertex dell'half edge start
 */
Dcel::Face::ConstIncidentVertexIterator Dcel::Face::incidentVertexBegin(const Dcel::HalfEdge* start) const {
    #ifdef DEBUG
    if (start->getFace() != this) {
        std::cerr << "ERROR: start half edge " << start->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    #endif
    return std::move(ConstIncidentVertexIterator(start, start, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::ConstIncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal toVertex dell'half edge start e fino al toVertex dell'half edge end.
 *
 * @param[in] start: half edge di partenza
 * @param[in] end: half edge di arrivo, il cui toVertex \b non \b è \b compreso
 * @warning Se start e end non risultano essere incidenti alla faccia, viene lanciata un'asserzione e il programma termina
 * @return Un iteratore che punta al toVertex dell'half edge start
 */
Dcel::Face::ConstIncidentVertexIterator Dcel::Face::incidentVertexBegin(const Dcel::HalfEdge* start, const Dcel::HalfEdge* end) const {
    #ifdef DEBUG
    if (start->getFace() != this){
        std::cerr << "ERROR: start half edge " << start->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    if (end->getFace() != this){
        std::cerr << "ERROR: end half edge " << end->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    #endif
    return std::move(ConstIncidentVertexIterator(start, end, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::ConstIncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal vertice start. \n
 * È meno efficiente rispetto a Dcel::Face::constIncidentVertexBegin(const Dcel::HalfEdge* start).
 *
 * @param[in] start: vertice di partenza
 * @warning Se start non risulta essere incidente alla faccia (ossia non possiede un half edge incidente alla faccia),
 *          viene lanciata un'asserzione e il programma termina
 * @warning Utilizza Dcel::Vertex::ConstIncomingHalfEdgeIterator
 * @return Un iteratore che punta al vertice start
 */
Dcel::Face::ConstIncidentVertexIterator Dcel::Face::incidentVertexBegin(const Dcel::Vertex* start) const {
    for (Dcel::Vertex::ConstIncomingHalfEdgeIterator heit = start->incomingHalfEdgeBegin();
         heit!= start->incomingHalfEdgeEnd();
         ++heit)
    {
        if ((*heit)->getFace() == this) return std::move(ConstIncidentVertexIterator(*heit, *heit, this));
    }
    std::cerr << "ERROR: start vertex " << start->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
    assert(0);
    return std::move(ConstIncidentVertexIterator());
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::ConstIncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal vertice start e fino al vertice end. \n
 * È meno efficiente rispetto a Dcel::Face::constIncidentVertexBegin(const Dcel::HalfEdge* start, const Dcel::HalfEdge* end).
 *
 * @param[in] start: vertice di partenza
 * @param[in] end: vertice di arrivo, \b non \b compreso
 * @warning Se start e end non risultano essere incidenti alla faccia (ossia non possiedono un half edge incidente alla faccia),
 *          viene lanciata un'asserzione e il programma termina
 * @warning Utilizza Dcel::Vertex::ConstIncomingHalfEdgeIterator
 * @return Un iteratore che punta al vertice start
 */
Dcel::Face::ConstIncidentVertexIterator Dcel::Face::incidentVertexBegin(const Dcel::Vertex* start, const Dcel::Vertex* end) const {
    Vertex::ConstIncomingHalfEdgeIterator heit = start->incomingHalfEdgeBegin();
    while (heit!= start->incomingHalfEdgeEnd() && ((*heit)->getFace() != this)) ++heit;
    #ifdef DEBUG
    if ((*heit)->getFace() != this) {
        std::cerr << "ERROR: start vertex " << start->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    #endif
    const HalfEdge* s = *heit;
    for (heit= end->incomingHalfEdgeBegin(); heit!= end->incomingHalfEdgeEnd(); ++heit){
        if ((*heit)->getFace() == this) return std::move(ConstIncidentVertexIterator(s, *heit, this));
    }
    std::cerr << "ERROR: end vertex " << end->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
    assert(0);
    return std::move(ConstIncidentVertexIterator());
}

Dcel::Vertex*Dcel::Face::getVertex1() {
    return outerHalfEdge->getFromVertex();
}

Dcel::Vertex*Dcel::Face::getVertex2() {
    return outerHalfEdge->getToVertex();
}

Dcel::Vertex*Dcel::Face::getVertex3() {
    return outerHalfEdge->getNext()->getToVertex();
}

/**
 * \~Italian
 * @brief Funzione che aggiorna la normale alla faccia
 * @warning Funziona se e solo se la faccia è un triangolo
 * @warning Utilizza Dcel::Face::ConstIncidentVertexIterator
 * @return La normale alla faccia aggiornata
 */
Vec3 Dcel::Face::updateNormal() {
    Vertex* a = outerHalfEdge->getFromVertex();
    Vertex* b = outerHalfEdge->getToVertex();
    Vertex* c = outerHalfEdge->getNext()->getToVertex();
    normal = (b->getCoordinate() - a->getCoordinate()).cross(c->getCoordinate() - a->getCoordinate());
    normal.normalize();
    if (outerHalfEdge->getNext()->getNext()->getToVertex() != a){
        Vec3 zAxis(0,0,1);
        Vec3 v = -(normal.cross(zAxis));
        v.normalize();
        double dot = normal.dot(zAxis);
        double angle = acos(dot);

        double r[3][3] = {0};
        if (normal != zAxis){
            if (normal == -zAxis){
                v = Vec3(1,0,0);
            }
            getRotationMatrix(v, angle, r);
        }
        else {
            r[0][0] = r[1][1] = r[2][2] = 1;
        }

        std::vector<Pointd> points;
        for (ConstIncidentVertexIterator vit = incidentVertexBegin(); vit != incidentVertexEnd(); ++vit){
            Pointd p = (*vit)->getCoordinate();
            Pointd pr(p.x() * r[0][0] + p.y() * r[1][0] +p.z() * r[2][0], p.x() * r[0][1] + p.y() * r[1][1] +p.z() * r[2][1], p.x() * r[0][2] + p.y() * r[1][2] +p.z() * r[2][2]);
            points.push_back(pr);
        }
        double sum = 0;
        for (unsigned int i = 0; i < points.size(); i++){
            Pointd p1 = points[i];
            Pointd p2 = points[(i+1)%points.size()];
            sum += (p2.x() - p1.x()) * (p2.y()+p1.y());
        }
        if (sum > 0)
            normal = -normal;
    }
    return normal;
}

/**
 * \~Italian
 * @brief Funzione che aggiorna l'area della faccia
 * @warning Funziona se e solo se la faccia è un triangolo
 * @warning Utilizza Dcel::Face::ConstIncidentVertexIterator
 * @return L'area della faccia aggiornata
 */
double Dcel::Face::updateArea() {
    updateNormal();
    if (isTriangle()) {
        Pointd v1 = outerHalfEdge->getFromVertex()->getCoordinate();
        Pointd v2 = outerHalfEdge->getToVertex()->getCoordinate();
        Pointd v3 = outerHalfEdge->getPrev()->getFromVertex()->getCoordinate();
        area = (((v3 - v1).cross(v2 - v1)).getLength() / 2);
    }
    #ifdef CGAL_DEFINED
    else {
        area = 0;
        std::vector<std::array<const Dcel::Vertex*, 3> > t;

        getTriangulation(t);
        for (unsigned int i = 0; i <t.size(); ++i){
            std::array<const Dcel::Vertex*, 3> tr =  t[i];
            Pointd v1 = tr[0]->getCoordinate();
            Pointd v2 = tr[1]->getCoordinate();
            Pointd v3 = tr[2]->getCoordinate();
            area += (((v3 - v1).cross(v2 - v1)).getLength() / 2);
        }
    }
    #endif
    return area;
}

/**
 * \~Italian
 * @brief Funzione che rimuove un inner half edge dalla faccia
 * @param[in] iterator: iteratore che punta all'inner half edge da eliminare
 */
void Dcel::Face::removeInnerHalfEdge(const Face::InnerHalfEdgeIterator& iterator) {
    innerHalfEdges.erase(iterator);
}

/**
 * \~Italian
 * @brief Funzione che rimuove un inner half edge dalla faccia
 *
 * È meno efficiente rispetto a Dcel::Face::removeInnerHalfEdge(const Face::innerHalfEdgeIterator &ihe).
 *
 * @param[in] halfEdge: inner half edge da eliminare
 * @return True se la rimozione è andata a buon fine, false altrimenti.
 */
bool Dcel::Face::removeInnerHalfEdge(const Dcel::HalfEdge* halfEdge) {
    InnerHalfEdgeIterator i = std::find(innerHalfEdges.begin(), innerHalfEdges.end(), halfEdge);
    if (i != innerHalfEdges.end()){
        innerHalfEdges.erase(i);
        return true;
    }
    return false;
}

void Dcel::Face::removeAllInnerHalfEdges() {
    innerHalfEdges.clear();
}

Dcel::Face::AdjacentFaceIterator Dcel::Face::adjacentFaceBegin() {
    return std::move(AdjacentFaceIterator(outerHalfEdge, outerHalfEdge, this));
}

Dcel::Face::AdjacentFaceIterator Dcel::Face::adjacentFaceEnd() {
    return std::move(AdjacentFaceIterator(nullptr, nullptr, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::InnerHalfEdgeIterator
 * @return Un iteratore che punta al primo inner half edge della faccia
 */
Dcel::Face::InnerHalfEdgeIterator Dcel::Face::innerHalfEdgeBegin() {
    return std::move(innerHalfEdges.begin());
}

/**
 * \~Italian
 * @brief Funzione di finalizzazione di Dcel::Face::InnerHalfEdgeIterator
 * @return Un iteratore che punta all'ultimo inner half edge della faccia
 */
Dcel::Face::InnerHalfEdgeIterator Dcel::Face::innerHalfEdgeEnd() {
    return std::move(innerHalfEdges.end());
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::IncidentHalfEdgeIterator.
 *
 * Permette di ciclare sugli half edge incidenti alla faccia, partendo dall'outer half edge.
 *
 * @return Un iteratore che punta all'outer half edge della faccia
 */
Dcel::Face::IncidentHalfEdgeIterator Dcel::Face::incidentHalfEdgeBegin() {
    return std::move(IncidentHalfEdgeIterator(outerHalfEdge, outerHalfEdge, this));
}

/**
 * \~Italian
 * @brief Funzione di finalizzazione di Dcel::Face::IncidentHalfEdgeIterator
 * @warning L'iteratore restituito non è un iteratore valido, e su di esso le operazioni di
 *          incremento e decremento sono inutili. Questa funzione è da utilizzarsi solamente per
 *          il \b confronto \b con \b un \b altro \b iteratore \b valido
 * @return Un iteratore di finalizzazione
 */
Dcel::Face::IncidentHalfEdgeIterator Dcel::Face::incidentHalfEdgeEnd() {
    return std::move(IncidentHalfEdgeIterator(nullptr, nullptr, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::IncidentHalfEdgeIterator.
 *
 * Permette di ciclare sugli half edge incidenti alla faccia, partendo dall'half edge start.
 *
 * @param[in] start: half edge di partenza
 * @warning Se start non risulta essere incidente alla faccia, viene lanciata un'asserzione e il programma termina
 * @return Un iteratore che punta all'half edge start
 */
Dcel::Face::IncidentHalfEdgeIterator Dcel::Face::incidentHalfEdgeBegin(Dcel::HalfEdge* start) {
    #ifdef DEBUG
    if (start->getFace() != this){
        std::cerr << "ERROR: start half edge " << start->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    #endif
    return std::move(IncidentHalfEdgeIterator(start, start, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::IncidentHalfEdgeIterator.
 *
 * Permette di ciclare sugli half edge incidenti alla faccia, partendo dall'half edge start e fino all'half edge end.
 *
 * @param[in] start: half edge di partenza
 * @param[in] end: half edge di arrivo, \b non \b compreso
 * @warning Se start e end non risultano essere incidenti alla faccia, viene lanciata un'asserzione e il programma termina
 * @return Un iteratore che punta all'half edge start
 */
Dcel::Face::IncidentHalfEdgeIterator Dcel::Face::incidentHalfEdgeBegin(Dcel::HalfEdge* start, Dcel::HalfEdge* end) {
    #ifdef DEBUG
    if (start->getFace() != this){
        std::cerr << "ERROR: start half edge " << start->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    if (end->getFace() != this){
        std::cerr << "ERROR: end half edge " << end->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    #endif
    return std::move(IncidentHalfEdgeIterator(start, end, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::IncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal toVertex dell'outer half edge.
 *
 * @return Un iteratore che punta al toVertex dell'outer half edge della faccia
 */
Dcel::Face::IncidentVertexIterator Dcel::Face::incidentVertexBegin() {
    return std::move(IncidentVertexIterator(outerHalfEdge, outerHalfEdge, this));
}

/**
 * \~Italian
 * @brief Funzione di finalizzazione di Dcel::Face::IncidentVertexIterator
 * @warning L'iteratore restituito non è un iteratore valido, e su di esso le operazioni di
 *          incremento e decremento sono inutili. Questa funzione è da utilizzarsi solamente per
 *          il \b confronto \b con \b un \b altro \b iteratore \b valido
 * @return Un iteratore di finalizzazione
 */
Dcel::Face::IncidentVertexIterator Dcel::Face::incidentVertexEnd() {
    return std::move(IncidentVertexIterator(nullptr, nullptr, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::IncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal toVertex dell'half edge start.
 *
 * @param[in] start: half edge di partenza
 * @warning Se start non risulta essere incidente alla faccia, viene lanciata un'asserzione e il programma termina
 * @return Un iteratore che punta al toVertex dell'half edge start
 */
Dcel::Face::IncidentVertexIterator Dcel::Face::incidentVertexBegin(Dcel::HalfEdge* start) {
    #ifdef DEBUG
    if (start->getFace() != this){
        std::cerr << "ERROR: start half edge " << start->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    #endif
    return std::move(IncidentVertexIterator(start, start, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::IncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal toVertex dell'half edge start e fino al toVertex dell'half edge end.
 *
 * @param[in] start: half edge di partenza
 * @param[in] end: half edge di arrivo, il cui toVertex \b non \b è \b compreso
 * @warning Se start e end non risultano essere incidenti alla faccia, viene lanciata un'asserzione e il programma termina
 * @return Un iteratore che punta al toVertex dell'half edge start
 */
Dcel::Face::IncidentVertexIterator Dcel::Face::incidentVertexBegin(Dcel::HalfEdge* start, Dcel::HalfEdge* end) {
    #ifdef DEBUG
    if (start->getFace() != this){
        std::cerr << "ERROR: start half edge " << start->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    if (end->getFace() != this){
        std::cerr << "ERROR: end half edge " << end->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    #endif
    return std::move(IncidentVertexIterator(start, end, this));
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::IncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal vertice start. \n
 * È meno efficiente rispetto a Dcel::Face::incidentVertexBegin(const Dcel::HalfEdge* start).
 *
 * @param[in] start: vertice di partenza
 * @warning Se start non risulta essere incidente alla faccia (ossia non possiede un half edge incidente alla faccia),
 *          viene lanciata un'asserzione e il programma termina
 * @warning Utilizza Dcel::Vertex::ConstIncomingHalfEdgeIterator
 * @return Un iteratore che punta al vertice start
 */
Dcel::Face::IncidentVertexIterator Dcel::Face::incidentVertexBegin(Dcel::Vertex* start) {
    for (Dcel::Vertex::IncomingHalfEdgeIterator heit = start->incomingHalfEdgeBegin();
         heit!= start->incomingHalfEdgeEnd();
         ++heit)
    {
        if ((*heit)->getFace() == this) return std::move(IncidentVertexIterator(*heit, *heit, this));
    }
    std::cerr << "ERROR: start vertex " << start->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
    assert(0);
    return std::move(IncidentVertexIterator());
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::Face::IncidentVertexIterator.
 *
 * Permette di ciclare sui vertici incidenti alla faccia, partendo dal vertice start e fino al vertice end. \n
 * È meno efficiente rispetto a Dcel::Face::incidentVertexBegin(const Dcel::HalfEdge* start, const Dcel::HalfEdge* end).
 *
 * @param[in] start: vertice di partenza
 * @param[in] end: vertice di arrivo, \b non \b compreso
 * @warning Se start e end non risultano essere incidenti alla faccia (ossia non possiedono un half edge incidente alla faccia),
 *          viene lanciata un'asserzione e il programma termina
 * @warning Utilizza Dcel::Vertex::ConstIncomingHalfEdgeIterator
 * @return Un iteratore che punta al vertice start
 */
Dcel::Face::IncidentVertexIterator Dcel::Face::incidentVertexBegin(Dcel::Vertex* start, Dcel::Vertex* end) {
    Vertex::IncomingHalfEdgeIterator heit = start->incomingHalfEdgeBegin();
    while (heit!= start->incomingHalfEdgeEnd() && ((*heit)->getFace() != this)) ++heit;
    #ifdef DEBUG
    if ((*heit)->getFace() != this) {
        std::cerr << "ERROR: start vertex " << start->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
        assert(0);
    }
    #endif
    HalfEdge* s = *heit;
    for (heit= end->incomingHalfEdgeBegin(); heit!= end->incomingHalfEdgeEnd(); ++heit){
        if ((*heit)->getFace() == this) return std::move(IncidentVertexIterator(s, *heit, this));
    }
    std::cerr << "ERROR: end vertex " << end->getId() << " hasn't this face " << this->getId() << " as incident face.\n";
    assert(0);
    return std::move(IncidentVertexIterator());
}

#ifdef CGAL_DEFINED
/**
 * \~Italian
 * @brief Dcel::Face::getTriangulation
 * @param triangles
 */
void Dcel::Face::getTriangulation(std::vector<std::array<const Dcel::Vertex*, 3> > &triangles) const {
    // Taking all the coordinates on vectors
    std::vector<Pointd> borderCoordinates;
    std::vector< std::vector<Pointd> > innerBorderCoordinates;
    std::map<Pointd, const Dcel::Vertex*> pointsVerticesMap;
    for (Dcel::Face::ConstIncidentHalfEdgeIterator heit = incidentHalfEdgeBegin(); heit != incidentHalfEdgeEnd(); ++heit){
        borderCoordinates.push_back((*heit)->getFromVertex()->getCoordinate());
        std::pair<const Dcel::Vertex*, const Dcel::Vertex*> pp;
        pp.first = (*heit)->getFromVertex();
        pp.second = (*heit)->getToVertex();
        pointsVerticesMap[(*heit)->getFromVertex()->getCoordinate()] = (*heit)->getFromVertex();
    }

    if (hasHoles()){
        innerBorderCoordinates.reserve(getNumberInnerHalfEdges());
        int i = 0;
        for (Dcel::Face::ConstInnerHalfEdgeIterator ihe = innerHalfEdgeBegin(); ihe != innerHalfEdgeEnd(); ++ihe, ++i){
            const Dcel::HalfEdge* he = *ihe;
            std::vector<Pointd> inner;
            for (Dcel::Face::ConstIncidentHalfEdgeIterator heit = incidentHalfEdgeBegin(he); heit != incidentHalfEdgeEnd(); ++heit){
                inner.push_back((*heit)->getFromVertex()->getCoordinate());
                std::pair<const Dcel::Vertex*, const Dcel::Vertex*> pp;
                pp.first = (*heit)->getFromVertex();
                pp.second = (*heit)->getToVertex();
                pointsVerticesMap[(*heit)->getFromVertex()->getCoordinate()] = (*heit)->getFromVertex();
            }
            innerBorderCoordinates.push_back(inner);
        }
    }

    std::vector<std::array<Pointd, 3> > trianglesP;
    CGALInterface::Triangulation::triangulate(trianglesP, normal, borderCoordinates, innerBorderCoordinates);

    triangles.clear();
    for (unsigned int i = 0; i < trianglesP.size(); ++i) {

            std::array<Pointd, 3> triangle = trianglesP[i];

            Pointd p1 = triangle[0];
            Pointd p2 = triangle[1];
            Pointd p3 = triangle[2];
            assert(pointsVerticesMap.find(p1) != pointsVerticesMap.end());
            assert(pointsVerticesMap.find(p2) != pointsVerticesMap.end());
            assert(pointsVerticesMap.find(p3) != pointsVerticesMap.end());
            const Dcel::Vertex* a = pointsVerticesMap[p1];
            const Dcel::Vertex* b = pointsVerticesMap[p2];
            const Dcel::Vertex* c = pointsVerticesMap[p3];
            std::array<const Dcel::Vertex*, 3> tuple = {a, b, c};
            triangles.push_back(tuple);
    }
}
#endif


/*********************
 * Protected Methods *
 *********************/

/**
 * \~Italian
 * @brief Funzione che restituisce una stringa degli inner half edge
 * @return Una stringa rappresentativa degli inner half edge della faccia
 */
std::string Dcel::Face::innerComponentsToString() const {
    std::stringstream ss;
    ss << "(";
    for (unsigned int i=0; i<innerHalfEdges.size(); i++){
        if (innerHalfEdges[i] != nullptr) {
            if (i != 0) ss << "; ";
            ss << innerHalfEdges[i]->getId();
        }
        else ss << "nullptr";
    }
    ss << ")";
    std::string s1 = ss.str();
    return s1;
}

Dcel::Face::ConstAdjacentFaceIterator Dcel::Face::ConstAdjacentFaceRangeBasedIterator::begin() const {
    return std::move(f->adjacentFaceBegin());
}

Dcel::Face::ConstAdjacentFaceIterator Dcel::Face::ConstAdjacentFaceRangeBasedIterator::end() const {
    return std::move(f->adjacentFaceEnd());
}

Dcel::Face::ConstIncidentHalfEdgeIterator Dcel::Face::ConstIncidentHalfEdgeRangeBasedIterator::begin() const {
    return std::move(f->incidentHalfEdgeBegin());
}

Dcel::Face::ConstIncidentHalfEdgeIterator Dcel::Face::ConstIncidentHalfEdgeRangeBasedIterator::end() const {
    return std::move(f->incidentHalfEdgeEnd());
}

Dcel::Face::ConstIncidentVertexIterator Dcel::Face::ConstIncidentVertexRangeBasedIterator::begin() const {
    return std::move(f->incidentVertexBegin());
}

Dcel::Face::ConstIncidentVertexIterator Dcel::Face::ConstIncidentVertexRangeBasedIterator::end() const {
    return std::move(f->incidentVertexEnd());
}

Dcel::Face::AdjacentFaceIterator Dcel::Face::AdjacentFaceRangeBasedIterator::begin() {
    return std::move(f->adjacentFaceBegin());
}

Dcel::Face::AdjacentFaceIterator Dcel::Face::AdjacentFaceRangeBasedIterator::end() {
    return std::move(f->adjacentFaceEnd());
}

Dcel::Face::IncidentHalfEdgeIterator Dcel::Face::IncidentHalfEdgeRangeBasedIterator::begin() {
    return std::move(f->incidentHalfEdgeBegin());
}

Dcel::Face::IncidentHalfEdgeIterator Dcel::Face::IncidentHalfEdgeRangeBasedIterator::end() {
    return std::move(f->incidentHalfEdgeEnd());
}

Dcel::Face::IncidentVertexIterator Dcel::Face::IncidentVertexRangeBasedIterator::begin() {
    return std::move(f->incidentVertexBegin());
}

Dcel::Face::IncidentVertexIterator Dcel::Face::IncidentVertexRangeBasedIterator::end(){
    return std::move(f->incidentVertexEnd());
}
