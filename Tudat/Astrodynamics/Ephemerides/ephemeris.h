/*    Copyright (c) 2010-2012, Delft University of Technology
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without modification, are
 *    permitted provided that the following conditions are met:
 *      - Redistributions of source code must retain the above copyright notice, this list of
 *        conditions and the following disclaimer.
 *      - Redistributions in binary form must reproduce the above copyright notice, this list of
 *        conditions and the following disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *      - Neither the name of the Delft University of Technology nor the names of its contributors
 *        may be used to endorse or promote products derived from this software without specific
 *        prior written permission.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS
 *    OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *    MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *    COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 *    GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 *    OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *    Changelog
 *      YYMMDD    Author            Comment
 *      110128    K. Kumar          Creation of code.
 *      110221    K. Kumar          Updated code to work as base class for derived ephemeris
 *                                  classes.
 *      120322    D. Dirkx          Modified to new Ephemeris interfaces.
 *
 *    References
 *
 */

#ifndef TUDAT_EPHEMERIS_H
#define TUDAT_EPHEMERIS_H

#include <sstream>

#include <TudatCore/Mathematics/BasicMathematics/linearAlgebra.h>

namespace tudat
{
namespace ephemerides
{

//! Ephemeris base class.
/*!
 * Ephemeris base class.
 */
class Ephemeris
{
public:

    //! Default destructor.
    /*!
     * Default destructor.
     */
    virtual ~Ephemeris( ) { }

    //! Get state from ephemeris.
    /*!
     * Returns state from ephemeris at given Julian date.
     * \param julianDate Julian date given in Julian days.
     * \return State from ephemeris.
     */
    virtual Eigen::VectorXd getCartesianStateFromEphemeris( const double julianDate ) = 0;

protected:

private:
};

} // namespace ephemerides
} // namespace tudat

#endif // TUDAT_EPHEMERIS_H