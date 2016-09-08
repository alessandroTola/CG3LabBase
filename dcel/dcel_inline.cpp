//#ifndef DCEL_INLINES_H
//#define DCEL_INLINES_H

#include "dcel_struct.h"
#include "dcel_iterators.h"

/**
 * \~Italian
 * @brief Restituisce il numero di vertici presenti nella Dcel.
 * @return Numero di vertici.
 */
inline unsigned int Dcel::getNumberVertices () const {
    return nVertices;
}

/**
 * \~Italian
 * @brief Restituisce il numero di half edge presenti nella Dcel.
 * @return Numero di half edge.
 */
inline unsigned int Dcel::getNumberHalfEdges () const {
    return nHalfEdges;
}

/**
 * \~Italian
 * @brief Restituisce il numero di facce presenti nella Dcel.
 * @return Numero di facce.
 */
inline unsigned int Dcel::getNumberFaces () const {
    return nFaces;
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::ConstVertexIterator
 * @return Un iteratore che punta al primo vertice della Dcel
 */
inline Dcel::ConstVertexIterator Dcel::vertexBegin() const {
    unsigned int i = 0;
    while (i < vertices.size() && vertices[i] == nullptr) ++i;
    return ConstVertexIterator(i, vertices);
}

/**
 * \~Italian
 * @brief Funzione di finalizzazione di Dcel::ConstVertexIterator
 * @return Un iteratore che punta all'ultimo vertice della Dcel
 */
inline Dcel::ConstVertexIterator Dcel::vertexEnd() const {
    return ConstVertexIterator(vertices.size(), vertices);
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::ConstHalfEdgeIterator
 * @return Un iteratore che punta al primo half edge della Dcel
 */
inline Dcel::ConstHalfEdgeIterator Dcel::halfEdgeBegin() const {
    unsigned int i = 0;
    while (i < halfEdges.size() && halfEdges[i] == nullptr) ++i;
    return ConstHalfEdgeIterator(i, halfEdges);
}

/**
 * \~Italian
 * @brief Funzione di finalizzazione di Dcel::ConstHalfEdgeIterator
 * @return Un iteratore che punta all'ultimo half edge della Dcel
 */
inline Dcel::ConstHalfEdgeIterator Dcel::halfEdgeEnd() const {
    return ConstHalfEdgeIterator(halfEdges.size(), halfEdges);
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::ConstFaceIterator
 * @return Un iteratore che punta alla prima faccia della Dcel
 */
inline Dcel::ConstFaceIterator Dcel::faceBegin() const {
    unsigned int i = 0;
    while (i < faces.size() && faces[i] == nullptr) ++i;
    return ConstFaceIterator(i, faces);
}

/**
 * @brief Funzione di finalizzazione di Dcel::ConstFaceIterator
 * @return Un iteratore che punta all'ultima faccia della Dcel
 */
inline Dcel::ConstFaceIterator Dcel::faceEnd() const {
    return ConstFaceIterator(faces.size(), faces);
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::VertexIterator
 * @return Un iteratore che punta al primo vertice della Dcel
 */
inline Dcel::VertexIterator Dcel::vertexBegin() {
    unsigned int i = 0;
    while (i < vertices.size() && vertices[i] == nullptr) ++i;
    return VertexIterator(i, vertices);
}

/**
 * \~Italian
 * @brief Funzione di finalizzazione di Dcel::VertexIterator
 * @return Un iteratore che punta all'ultimo vertice della Dcel
 */
inline Dcel::VertexIterator Dcel::vertexEnd() {
    return VertexIterator(vertices.size(), vertices);
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::HalfEdgeIterator
 * @return Un iteratore che punta al primo half edge della Dcel
 */
inline Dcel::HalfEdgeIterator Dcel::halfEdgeBegin() {
    unsigned int i = 0;
    while (i < halfEdges.size() && halfEdges[i] == nullptr) ++i;
    return HalfEdgeIterator(i, halfEdges);
}

/**
 * \~Italian
 * @brief Funzione di finalizzazione di Dcel::HalfEdgeIterator
 * @return Un iteratore che punta all'ultimo half edge della Dcel
 */
inline Dcel::HalfEdgeIterator Dcel::halfEdgeEnd() {
    return HalfEdgeIterator(halfEdges.size(), halfEdges);
}

/**
 * \~Italian
 * @brief Funzione di inizializzazione di Dcel::FaceIterator
 * @return Un iteratore che punta alla prima faccia della Dcel
 */
inline Dcel::FaceIterator Dcel::faceBegin() {
    unsigned int i = 0;
    while (i < faces.size() && faces[i] == nullptr) ++i;
    return FaceIterator(i, faces);
}

/**
 * \~Italian
 * @brief Funzione di finalizzazione di Dcel::FaceIterator
 * @return Un iteratore che punta all'ultima faccia della Dcel
 */
inline Dcel::FaceIterator Dcel::faceEnd() {
    return FaceIterator(faces.size(), faces);
}

inline const Dcel::ConstVertexRangeBasedIterator Dcel::vertexIterator() const {
    return ConstVertexRangeBasedIterator(this);
}

inline Dcel::VertexRangeBasedIterator Dcel::vertexIterator() {
    return VertexRangeBasedIterator(this);
}

inline const Dcel::ConstHalfEdgeRangeBasedIterator Dcel::halfEdgeIterator() const {
    return ConstHalfEdgeRangeBasedIterator(this);
}

inline Dcel::HalfEdgeRangeBasedIterator Dcel::halfEdgeIterator() {
    return HalfEdgeRangeBasedIterator(this);
}

inline const Dcel::ConstFaceRangeBasedIterator Dcel::faceIterator() const {
    return ConstFaceRangeBasedIterator(this);
}

inline Dcel::FaceRangeBasedIterator Dcel::faceIterator() {
    return FaceRangeBasedIterator(this);
}

//#endif // DCEL_INLINES_H
