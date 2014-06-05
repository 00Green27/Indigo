/****************************************************************************
 * Copyright (C) 2009-2013 GGA Software Services LLC
 * 
 * This file is part of Indigo toolkit.
 * 
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 3 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in the
 * packaging of this file.
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 ***************************************************************************/

#include "math/algebra.h"

using namespace indigo;

IMPL_ERROR(Vec2f, "Vec2f");

bool Vec2f::normalize ()
{
   float l = lengthSqr();

   if (l < EPSILON * EPSILON) {
      return false;
   }

   l = (float)sqrt(l);

   x /= l;
   y /= l;

   return true;
}

bool Vec2f::normalization (const Vec2f &v)
{
   float l = v.lengthSqr();

   if (l < EPSILON * EPSILON)
      return false;

   l = (float)sqrt(l);

   x = v.x / l;
   y = v.y / l;

   return true;
}

void Vec2f::rotate (float angle)
{
   rotate(sin(angle), cos(angle));
}

void Vec2f::rotate (float si, float co)
{
   Vec2f a(*this);

   x = co * a.x - si * a.y;
   y = si * a.x + co * a.y;
}

void Vec2f::rotate(Vec2f vec)
{
   rotate(vec.y, vec.x);
}

void Vec2f::rotateL(Vec2f vec)
{
   rotateL(vec.y, vec.x);
}

void Vec2f::rotateL(float angle)
{
   rotateL(sin(angle), cos(angle));
}

void Vec2f::rotateL (float si, float co)
{
   rotate(-si, co);
}

void Vec2f::rotateAroundSegmentEnd (const Vec2f &a, const Vec2f &b, float angle)
{
   Vec2f c;

   c.diff(a, b);
   c.rotate(angle);

   sum(b, c);
}

float Vec2f::tiltAngle ()
{
   float l = length();

   if (l < EPSILON)
      throw Error("zero length");

   if (y >= 0) 
      return acos(x / l);
   return -acos(x / l);
}

float Vec2f::tiltAngle2 ()
{
   float l = length();

   if (l < EPSILON)
      throw Error("zero length");

   if (y >= 0) 
      return acos(x / l);
   return 2 * PI - acos(x / l);
}

float Vec2f::distSqr (const Vec2f &a, const Vec2f &b)
{
   float dx = b.x - a.x;
   float dy = b.y - a.y;

   return dx * dx + dy * dy;
}

float Vec2f::dist (const Vec2f &a, const Vec2f &b)
{
   return (float)sqrt(distSqr(a, b));
}

float Vec2f::dot (const Vec2f &a, const Vec2f &b)
{
   return a.x * b.x + a.y * b.y;
}

float Vec2f::cross (const Vec2f &a, const Vec2f &b)
{
   return a.x * b.y - a.y * b.x;
}

void Vec2f::projectZ (Vec2f& v2, const Vec3f& v3)
{
   v2.x = v3.x;
   v2.y = v3.y;
}

// two edges:
//    x = x1_1 + (x1_2 - x1_1)t1;
//    y = y1_1 + (y1_2 - y1_1)t1;
// and
//    x = x2_1 + (x2_2 - x2_1)t2;
//    y = y2_1 + (y2_2 - y2_1)t2;
// then
//       (x2_2 - x2_1)(y2_1 - y1_1) - (x2_1 - x1_1)(y2_2 - y2_1)   a2 * b12 - a12 * b2
// t1 =  ------------------------------------------------------- = -------------------
//       (x2_2 - x2_1)(y1_2 - y1_1) - (x1_2 - x1_1)(y2_2 - y2_1)    a2 * b1 - a1 * b2
//
//       (x1_2 - x1_1)(y2_1 - y1_1) - (x2_1 - x1_1)(y1_2 - y1_1)   a1 * b12 - a12 * b1
// t2 =  ------------------------------------------------------- = -------------------
//       (x2_2 - x2_1)(y1_2 - y1_1) - (x1_2 - x1_1)(y2_2 - y2_1)    a2 * b1 - a1 * b2
bool Vec2f::intersection (const Vec2f &v1_1, const Vec2f &v1_2, const Vec2f &v2_1, const Vec2f &v2_2, Vec2f &p)
{
   float a1,  a12,  b12,  a2,  b1,  b2;
   float delta,  delta1,  delta2,  t1,  t2;

   a1 = v1_2.x - v1_1.x;
   b1 = v1_2.y - v1_1.y;
   a12 = v2_1.x - v1_1.x;
   b12 = v2_1.y - v1_1.y;
   a2 = v2_2.x - v2_1.x;
   b2 = v2_2.y - v2_1.y;

   delta  = a2 * b1  - a1  * b2;
   delta1 = a2 * b12 - a12 * b2;
   delta2 = a1 * b12 - a12 * b1;

   if (fabs(delta) < EPSILON)
      return false;

   t1 = delta1 / delta;
   t2 = delta2 / delta;

   if (fabs(t1) < EPSILON || fabs(t1 - 1.f) < EPSILON || fabs(t2) < EPSILON || fabs(t2 - 1.f) < EPSILON)
      return false;

   if (t1 < 0.f || t1 > 1.f || t2 < 0.f || t2 > 1.f)
      return false;

   p.x = v1_1.x + (v1_2.x - v1_1.x) * t1;
   p.y = v1_1.y + (v1_2.y - v1_1.y) * t1;

   return true;
}

float Vec2f::triangleArea (const Vec2f &a, const Vec2f &b, const Vec2f &c) {
   return (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
}

bool Vec2f::segmentsIntersect (const Vec2f &a0, const Vec2f &a1, const Vec2f &b0, const Vec2f &b1) {

   float maxax = __max(a0.x, a1.x);
   float maxay = __max(a0.y, a1.y);
   float maxbx = __max(b0.x, b1.x);
   float maxby = __max(b0.y, b1.y);
   float minax = __min(a0.x, a1.x);
   float minay = __min(a0.y, a1.y);
   float minbx = __min(b0.x, b1.x);
   float minby = __min(b0.y, b1.y);

   float big_eps = 0.001;

   if (maxax + big_eps < minbx || maxbx + big_eps < minax || maxay + big_eps < minby || maxby + big_eps < minay) return false;

   // regular check
   return triangleArea(a0, a1, b0) * triangleArea(a0, a1, b1) < EPSILON
      && triangleArea(b0, b1, a0) * triangleArea(b0, b1, a1) < EPSILON;
}

bool Vec2f::segmentsIntersectInternal (const Vec2f &a0, const Vec2f &a1, const Vec2f &b0, const Vec2f &b1) {

   float maxax = __max(a0.x, a1.x);
   float maxay = __max(a0.y, a1.y);
   float maxbx = __max(b0.x, b1.x);
   float maxby = __max(b0.y, b1.y);
   float minax = __min(a0.x, a1.x);
   float minay = __min(a0.y, a1.y);
   float minbx = __min(b0.x, b1.x);
   float minby = __min(b0.y, b1.y);

   float big_eps = 0.001;

   if (maxax < minbx + big_eps || maxbx < minax + big_eps || maxay < minby + big_eps || maxby < minay + big_eps) return false;

   // regular check
   return triangleArea(a0, a1, b0) * triangleArea(a0, a1, b1) < - EPSILON
      && triangleArea(b0, b1, a0) * triangleArea(b0, b1, a1) < - EPSILON;
}

double Vec2f::distPointSegment(Vec2f p, Vec2f q, Vec2f r) {
   if (dot(p - q, r - q) <= 0) return dist(p, q);
   if (dot(p - r, q - r) <= 0) return dist(p, r);
   
   Vec2f normal = r - q;
   normal.rotate(PI/2);
   double c = cross(q, r);
   double s = normal.length();

   double t = -c - dot(normal, p);

   return fabs(t / s);
}

double Vec2f::distSegmentSegment(Vec2f p, Vec2f q, Vec2f r, Vec2f s) {
   if (Vec2f::segmentsIntersect(p, q, r, s)) return 0;

   return __min( __min(distPointSegment(p, r, s), distPointSegment(q, r, s)),
      __min(distPointSegment(r, p, q), distPointSegment(s, p, q)));
}

Vec2f Vec2f::get_circle_center(Vec2f p, Vec2f q, double angle) {

   Vec2f vec(q - p);

   return (p + q) / 2 + vec / tan((PI - angle)/2);
}