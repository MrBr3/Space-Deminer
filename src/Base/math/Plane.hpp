/* Space Deminer is a small 2D Arcade Game. Your task ist to eliminate
 * intelligent Mines created in the past galactic war.
 *
 * Copyright (C) 2010 the Space Deminer Development Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

class Plane
{
  Plane();

  void check()const
  {
    if(normal.get_square_length()==0.f)
      throw std::runtime_error("**void Math::Plane::check** zero length normal is not allowed");
  }
public:
  gfloat d; //> distance to origin
  Vector3 normal;

  /** \brief Defines the Plane using a normal and the distance to the origin
   *
   * \note the normal becomes automatically normalized
   * */
  Plane(const Vector3& normal, gfloat d)throw(std::invalid_argument)
  {
    set(normal, d);
  }

  /** \brief Defines the Plane using a normal and the distance to the origin
   *
   * \note the normal becomes automatically normalized
   * */
  void set(const Vector3& normal, gfloat d)throw(std::invalid_argument)
  {
    if(normal.get_square_length()==0.f)
      throw std::invalid_argument("**void Math::Plane::set(const Vector& normal, gfloat d)** zero length normal is not allowed");

    this->normal  = normal;
    this->d  = d;

    this->normal.normalize();
  }

  /** \brief Defines the Plane using a normal and one Point within the plane
   *
   * \note the normal becomes automatically normalized
   * */
  Plane(const Vector3& normal, const Vector3& p)throw(std::invalid_argument)
  {
    set(normal, p);
  }

  /** \brief Defines the Plane using a normal (not parallel) and one Point within the plane
   *
   * \note the normal becomes automatically normalized
   * */
  void set(Vector3 normal, const Vector3& p)throw(std::invalid_argument)
  {
    normal.normalize();

    set(normal, normal*p);
  }

  /** \brief Defines the Plane using two tangents (pointing to different directions) and one Point within the plane
   *
   * \note the normal becomes automatically normalized
   * */
  Plane(const Vector3& t1, const Vector3& t2, const Vector3& p)throw(std::invalid_argument)
  {
    set(t1, t2, p);
  }

  /** \brief Defines the Plane using two tangents (not parallel) and one Point within the plane
   *
   * \note the normal becomes automatically normalized
   * */
  void set(const Vector3& t1, const Vector3& t2, const Vector3& p)throw(std::invalid_argument)
  {
    Vector3 n(t1);

    n.set_cross(t2);

    if(n.get_square_length()==0.f)
      throw std::invalid_argument("**void Math::Plane::set(const Vector& t1, const Vector& t2, const Vector& p)** t1 and t2 are not allowed to be parallel");

    n.normalize();

    set(n, p);
  }

  /** @name Overlap tests
   * */
  //@{
    /** \brief Checks, whether a point is within the plane, if not on which side.
    *
    * \note for a valid result the normal must be normalized
    *
    * \return Collision one of \li \c BACKSIDE \li \c FORESIDE \li \c OUTSIDE
    */
    Overlap check_point(const Vector3& p, gfloat epsilon=1e-5)const
    {
      check();

      gfloat x  = normal * p;

      if(fabs(x-d)<=epsilon)
        return INSIDE;
      if(x<d)
        return BACKSIDE;
      return FORESIDE;
    }

    /** \brief Checks, whether a Sphere overlaps, of touches the plane and if not it checks the side
    *
    * \param sphere
    *
    * \return \li \c OVERLAPPING if the Sphere is overlapping/touching the plane
    *         \li \c FORESIDE if the plane is fully on the foreside of the plane
    *         \li \c BACKSIDE if the plane is fully on the backside of the plane
    * */
    Overlap check_sphere(const Sphere& sphere)const;

    /** \brief Checks whether and where the Ray intersects the plane.
    *
    * \note for a valid result the normal must be normalized
    *
    * \param ray
    * \param t if the direction vecto of the ray has been normalized, this reference will contain the
    *        distance from the rays origin to the intersectionpoint.
    *
    * \return false if there is no intersection
    **/
    bool check_ray(const Ray& ray, gfloat& t)const
    {
      gfloat plane_normal_dot_ray_dir = normal * ray.dir;

      if(plane_normal_dot_ray_dir==0.f)
      {
        if(check_point(ray.origin)==INSIDE)
        {
          t = 0.f;
          return true;
        }
        return false;
      }

      t = (d - normal*ray.origin)/plane_normal_dot_ray_dir;
      return true;
    }
  //@}

  /** @name Distance
   * */
  //@{
    /** \brief Gets the distance of a point to the plane
    *
    * \note for a valid result the normal must be normalized
    * */
    gfloat get_distance(const Vector3& p)const
    {
      check();

      return fabs(normal*p - d);
    }
  //@}

  /** @name Debugging
   * */
  //@{
  std::string str()const
  {
    return Glib::ustring::compose("%1*a + %2*b + %3 - %4 = 0", normal.x, normal.y, normal.z, d).c_str();
  }
  //@}
};