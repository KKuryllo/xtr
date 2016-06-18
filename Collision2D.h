#pragma once

#include <glm/glm.hpp>
#include <cmath>
//******************************************************************************
//   This program is a 'remote' 2D-collision detector for two balls on linear
//   trajectories and returns, if applicable, the location of the collision for
//   both balls as well as the new velocity vectors (assuming a partially elastic
//   collision as defined by the restitution coefficient).
//
//   In  'f' (free) mode no positions but only the initial velocities
//   and an impact angle are required.
//   All variables apart from 'mode' and 'error' are of Double Precision
//   Floating Point type.
//
//   The Parameters are:
//
//    mode  (char) (if='f' alpha must be supplied; otherwise arbitrary)
//    alpha (impact angle) only required in mode='f';
//                     should be between -PI/2 and PI/2 (0 = head-on collision))
//    R    (restitution coefficient)  between 0 and 1 (1=perfectly elastic collision)
//    m1   (mass of ball 1)
//    m2   (mass of ball 2)
//    r1   (radius of ball 1)        not needed for 'f' mode
//    r2   (radius of ball 2)                "
//  & x1   (x-coordinate of ball 1)          "
//  & y1   (y-coordinate of ball 1)          "
//  & x2   (x-coordinate of ball 2)          "
//  & y2   (y-coordinate of ball 2)          "
//  & vx1  (velocity x-component of ball 1)
//  & vy1  (velocity y-component of ball 1)
//  & vx2  (velocity x-component of ball 2)
//  & vy2  (velocity y-component of ball 2)
//  & error (int)  (0: no error
//                  1: balls do not collide
//                  2: initial positions impossible (balls overlap))
//
//   Note that the parameters with an ampersand (&) are passed by reference,
//   i.e. the corresponding arguments in the calling program will be updated;
//   however, the coordinates and velocities will only be updated if 'error'=0.
//
//   All variables should have the same data types in the calling program
//   and all should be initialized before calling the function even if
//   not required in the particular mode.
//
//   This program is free to use for everybody. However, you use it at your own
//   risk and I do not accept any liability resulting from incorrect behaviour.
//   I have tested the program for numerous cases and I could not see anything
//   wrong with it but I can not guarantee that it is bug-free under any
//   circumstances.
//
//   I would appreciate if you could report any problems to me
//   (for contact details see  http://www.plasmaphysics.org.uk/feedback.htm ).
//
//   Thomas Smid, January  2004
//                December 2005 (corrected faulty collision detection;
//                               a few minor changes to improve speed;
//                               added simplified code without collision detection)
//                December 2009 (generalization to partially inelastic collisions)
//*********************************************************************************

namespace xtr {
	namespace collision2D_origin {
		template<typename percision>
		inline void collision2D(char mode, percision alpha, percision R,
			percision m1, percision m2, percision r1, percision r2,
			percision& x1, percision& y1, percision& x2, percision& y2,
			percision& vx1, percision& vy1, percision& vx2, percision& vy2,
			int& error) {

			percision  r12, m21, d, gammav, gammaxy, dgamma, dr, dc, sqs, t,
				dvx2, a, x21, y21, vx21, vy21, pi2, vx_cm, vy_cm;

			//     ***initialize some variables ****
			pi2 = 2.0f * std::acos(percision(-1.0E0));
			error = 0;
			r12 = r1 + r2;
			m21 = m2 / m1;
			x21 = x2 - x1;
			y21 = y2 - y1;
			vx21 = vx2 - vx1;
			vy21 = vy2 - vy1;

			vx_cm = (m1*vx1 + m2*vx2) / (m1 + m2);
			vy_cm = (m1*vy1 + m2*vy2) / (m1 + m2);

			//     ****  return old positions and velocities if relative velocity =0 ****
			if (vx21 == 0 && vy21 == 0) { error = 1; return; }

			//     *** calculate relative velocity angle
			gammav = std::atan2(-vy21, -vx21);

			//******** this block only if initial positions are given *********
			if (mode != 'f') {

				d = std::sqrt(x21*x21 + y21*y21);

				//     **** return if distance between balls smaller than sum of radii ***
				if (d < r12) { error = 2; return; }

				//     *** calculate relative position angle and normalized impact parameter ***
				gammaxy = std::atan2(y21, x21);
				dgamma = gammaxy - gammav;
				if (dgamma > pi2) { dgamma = dgamma - pi2; }
				else if (dgamma<-pi2) { dgamma = dgamma + pi2; }
				dr = d*std::sin(dgamma) / r12;

				//     **** return old positions and velocities if balls do not collide ***
				if ((std::fabs(dgamma)>pi2 / 4 && std::fabs(dgamma) < 0.75*pi2) || std::fabs(dr) > 1)
				{
					error = 1; return;
				}

				//     **** calculate impact angle if balls do collide ***
				alpha = std::asin(dr);

				//     **** calculate time to collision ***
				dc = d*std::cos(dgamma);
				if (dc > 0) { sqs = 1.0; }
				else { sqs = -1.0; }
				t = (dc - sqs*r12*std::sqrt(1 - dr*dr)) / std::sqrt(vx21*vx21 + vy21*vy21);
				//    **** update positions ***
				x1 = x1 + vx1*t;
				y1 = y1 + vy1*t;
				x2 = x2 + vx2*t;
				y2 = y2 + vy2*t;
			}

			//******** END 'this block only if initial positions are given' *********

			//     ***  update velocities ***

			a = std::tan(gammav + alpha);

			dvx2 = -2 * (vx21 + a*vy21) / ((1 + a*a)*(1 + m21));

			vx2 = vx2 + dvx2;
			vy2 = vy2 + a*dvx2;
			vx1 = vx1 - m21*dvx2;
			vy1 = vy1 - a*m21*dvx2;

			//     ***  velocity correction for inelastic collisions ***

			vx1 = (vx1 - vx_cm)*R + vx_cm;
			vy1 = (vy1 - vy_cm)*R + vy_cm;
			vx2 = (vx2 - vx_cm)*R + vx_cm;
			vy2 = (vy2 - vy_cm)*R + vy_cm;

			return;
		}

		//******************************************************************************
		//  Simplified Version
		//  The advantage of the 'remote' collision detection in the program above is
		//  that one does not have to continuously track the balls to detect a collision.
		//  The program needs only to be called once for any two balls unless their
		//  velocity changes. However, if somebody wants to use a separate collision
		//  detection routine for whatever reason, below is a simplified version of the
		//  code which just calculates the new velocities, assuming the balls are already
		//  touching (this condition is important as otherwise the results will be incorrect)
		//****************************************************************************
		template<typename percision>
		void collision2Ds(percision m1, percision m2, percision R,
			percision x1, percision y1, percision x2, percision y2,
			percision& vx1, percision& vy1, percision& vx2, percision& vy2) {

			percision  m21, dvx2, a, x21, y21, vx21, vy21, fy21, sign, vx_cm, vy_cm;


			m21 = m2 / m1;
			x21 = x2 - x1;
			y21 = y2 - y1;
			vx21 = vx2 - vx1;
			vy21 = vy2 - vy1;

			vx_cm = (m1*vx1 + m2*vx2) / (m1 + m2);
			vy_cm = (m1*vy1 + m2*vy2) / (m1 + m2);


			//     *** return old velocities if balls are not approaching ***
			if ((vx21*x21 + vy21*y21) >= 0) return;


			//     *** I have inserted the following statements to avoid a zero divide;
			//         (for single precision calculations,
			//          1.0E-12 should be replaced by a larger value). **************

			fy21 = 0.00001f*fabs(y21);
			if (fabs(x21)<fy21) {
				if (x21<0) { sign = -1; }
				else { sign = 1; }
				x21 = fy21*sign;
			}

			//     ***  update velocities ***
			a = y21 / x21;
			dvx2 = -2 * (vx21 + a*vy21) / ((1 + a*a)*(1 + m21));
			vx2 = vx2 + dvx2;
			vy2 = vy2 + a*dvx2;
			vx1 = vx1 - m21*dvx2;
			vy1 = vy1 - a*m21*dvx2;

			//     ***  velocity correction for inelastic collisions ***
			vx1 = (vx1 - vx_cm)*R + vx_cm;
			vy1 = (vy1 - vy_cm)*R + vy_cm;
			vx2 = (vx2 - vx_cm)*R + vx_cm;
			vy2 = (vy2 - vy_cm)*R + vy_cm;

			return;
		}
	}

	// These are modified version of the above library for use with glm
	template<typename vec_type>
	inline void collision2Ds(const float mass_1, const float mass_2, const float restriction, const vec_type position_1, const vec_type position_2, vec_type& velocity_1, vec_type& velocity_2) {
		collision2D_origin::collision2Ds(mass_1, mass_2, restriction, position_1[0], position_1[1], position_2[0], position_2[1], velocity_1[0], velocity_1[1], velocity_2[0], velocity_2[1]);
	}

	template<typename vec_type>
	inline void collision2Dse(const float mass_1, const float mass_2, const vec_type position_1, const vec_type position_2, vec_type& velocity_1, vec_type& velocity_2) {
		collision2D_origin::collision2Ds(mass_1, mass_2, 1.0f, position_1[0], position_1[1], position_2[0], position_2[1], velocity_1[0], velocity_1[1], velocity_2[0], velocity_2[1]);
	}
}
