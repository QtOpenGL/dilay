#include <assert.h>
#include <glm/glm.hpp>
#include "winged-vertex.hpp"
#include "winged-edge.hpp"
#include "winged-face.hpp"
#include "winged-mesh.hpp"
#include "util.hpp"
  
WingedEdge :: WingedEdge (WingedMesh& mesh) {
  this->_vertex1           = mesh.nullVertex (); 
  this->_vertex2           = mesh.nullVertex ();

  this->_leftFace          = mesh.nullFace ();
  this->_rightFace         = mesh.nullFace ();

  this->_leftPredecessor   = mesh.nullEdge ();
  this->_leftSuccessor     = mesh.nullEdge ();

  this->_rightPredecessor  = mesh.nullEdge ();
  this->_rightSuccessor    = mesh.nullEdge ();
}

WingedEdge :: WingedEdge ( LinkedVertex v1, LinkedVertex v2
                         , LinkedFace left, LinkedFace right
                         , LinkedEdge leftPred, LinkedEdge leftSucc
                         , LinkedEdge rightPred, LinkedEdge rightSucc) {

  this->set (v1,v2,left,right,leftPred,leftSucc,rightPred,rightSucc);
}

bool WingedEdge :: isLeftFace (const WingedFace& face) const {
  if (&face == &*this->_leftFace)
    return true;
  else if (&face == &*this->_rightFace)
    return false;
  else
    assert (false);
}

bool WingedEdge :: isRightFace (const WingedFace& face) const { 
  return ! this->isLeftFace (face); 
}

bool WingedEdge :: isVertex1 (const WingedVertex& vertex) const {
  if (&vertex == &*this->_vertex1)
    return true;
  else if (&vertex == &*this->_vertex2)
    return false;
  else
    assert (false);
}

bool WingedEdge :: isVertex2 (const WingedVertex& vertex) const { 
  return ! this->isVertex1 (vertex);
}

LinkedVertex WingedEdge :: firstVertex (const WingedFace& face) const {
  return this->isLeftFace (face) ? this->vertex1 () : this->vertex2 (); }

LinkedVertex WingedEdge :: secondVertex (const WingedFace& face) const {
  return this->isLeftFace (face) ? this->vertex2 () : this->vertex1 (); }

LinkedEdge WingedEdge :: predecessor (const WingedFace& face) const {
  return this->isLeftFace (face) ? this->leftPredecessor  () : this->rightPredecessor (); }

LinkedEdge WingedEdge :: successor (const WingedFace& face) const {
  return this->isLeftFace (face) ? this->leftSuccessor  () : this->rightSuccessor (); }

LinkedFace WingedEdge :: otherFace (const WingedFace& face) const {
  return this->isLeftFace (face) ? this->rightFace () : this->leftFace  (); }

void WingedEdge :: set ( LinkedVertex v1, LinkedVertex v2
                       , LinkedFace left, LinkedFace right
                       , LinkedEdge leftPred , LinkedEdge leftSucc
                       , LinkedEdge rightPred, LinkedEdge rightSucc) {
  this->setVertex1          (v1);
  this->setVertex2          (v2);
  this->setLeftFace         (left);
  this->setRightFace        (right);
  this->setLeftPredecessor  (leftPred);
  this->setLeftSuccessor    (leftSucc);
  this->setRightPredecessor (rightPred);
  this->setRightSuccessor   (rightSucc);
}

void WingedEdge :: setVertex1          (LinkedVertex v) { this->_vertex1          = v; }
void WingedEdge :: setVertex2          (LinkedVertex v) { this->_vertex2          = v; }
void WingedEdge :: setLeftFace         (LinkedFace f)   { this->_leftFace         = f; }
void WingedEdge :: setRightFace        (LinkedFace f)   { this->_rightFace        = f; }
void WingedEdge :: setLeftPredecessor  (LinkedEdge e)   { this->_leftPredecessor  = e; }
void WingedEdge :: setLeftSuccessor    (LinkedEdge e)   { this->_leftSuccessor    = e; }
void WingedEdge :: setRightPredecessor (LinkedEdge e)   { this->_rightPredecessor = e; }
void WingedEdge :: setRightSuccessor   (LinkedEdge e)   { this->_rightSuccessor   = e; }

void WingedEdge :: setFirstVertex (const WingedFace& face, LinkedVertex vertex) {
  if (this->isLeftFace (face)) this->setVertex1 (vertex);
  else                         this->setVertex2 (vertex);
}

void WingedEdge :: setSecondVertex (const WingedFace& face, LinkedVertex vertex) {
  if (this->isLeftFace (face)) this->setVertex2 (vertex);
  else                         this->setVertex1 (vertex);
}

void WingedEdge :: setFace (const WingedFace& face, LinkedFace newFace) {
  if (this->isLeftFace (face)) this->setLeftFace  (newFace);
  else                         this->setRightFace (newFace);
}

void WingedEdge :: setPredecessor (const WingedFace& face, LinkedEdge edge) {
  if (this->isLeftFace (face)) this->setLeftPredecessor  (edge);
  else                         this->setRightPredecessor (edge);
}

void WingedEdge :: setSuccessor (const WingedFace& face, LinkedEdge edge) {
  if (this->isLeftFace (face)) this->setLeftSuccessor  (edge);
  else                         this->setRightSuccessor (edge);
}

float WingedEdge :: lengthSqr (const WingedMesh& mesh) const {
  glm::vec3 a = this->_vertex1->vertex (mesh);
  glm::vec3 b = this->_vertex2->vertex (mesh);
  return glm::distance (a,b);
}

LinkedEdge WingedEdge :: successor (const WingedFace& face, unsigned int index) const {
  if (index == 0) return this->successor (face);
  else            return this->successor (face)->successor (face,index-1);
}

LinkedEdge WingedEdge :: predecessor (const WingedFace& face, unsigned int index) const {
  if (index == 0) return this->predecessor (face);
  else            return this->predecessor (face)->predecessor (face,index-1);
}

LinkedVertex WingedEdge :: vertex (const WingedFace& face, unsigned int index) const {
  if (index == 0) return this->firstVertex (face);
  else            return this->successor (face)->vertex (face,index-1);
}

glm::vec3 WingedEdge :: middle (const WingedMesh& mesh) const {
  return Util :: between ( this->vertex1 ()->vertex (mesh)
                         , this->vertex2 ()->vertex (mesh));
}
