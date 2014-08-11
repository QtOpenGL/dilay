#include <sstream>
#include <glm/glm.hpp>
#include "primitive/ray.hpp"
#include "util.hpp"

struct PrimRay::Impl {
  const glm::vec3 origin;
  const glm::vec3 direction;

  Impl (const glm::vec3& o, const glm::vec3& d) 
    : origin    (o)
    , direction (glm::normalize (d)) 
      {}

  Impl (const PrimRay& r, const glm::mat4x4& m) 
    : Impl ( Util::transformPosition  (m, r.origin    ())
           , Util::transformDirection (m, r.direction ())
           ) {}

  glm::vec3 pointAt (float t) const {
    return this->origin + (this->direction * glm::vec3 (t));
  }
};

DELEGATE2_BIG6        (PrimRay, const glm::vec3&, const glm::vec3&)
DELEGATE2_CONSTRUCTOR (PrimRay, const PrimRay&, const glm::mat4x4&)

GETTER_CONST    (const glm::vec3&, PrimRay, origin)
GETTER_CONST    (const glm::vec3&, PrimRay, direction)
DELEGATE1_CONST (glm::vec3       , PrimRay, pointAt, float)

std::ostream& operator<<(std::ostream& os, const PrimRay& ray) {
  os << "PrimRay { origin = "    << (ray.origin    ())
             << ", direction = " << (ray.direction ()) << " }";
  return os;
}
