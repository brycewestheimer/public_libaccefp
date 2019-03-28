/*
 * efmo.hpp
 * 
 * Copied: Mar 28, 2019
 * Author: Bryce M. Westheimer
 * 
 * Description: Effective Fragment Molecular Orbital class template and related functionality
 * 
 */

#ifndef LIBACCEFP_INCLUDE_METHODS_EFMO_HPP_
#define LIBACCEFP_INCLUDE_METHODS_EFMO_HPP_

#include <libaccqc.hpp> // WavefnType classes

namespace libaccefp {

  namespace methods {

    // Default Efmo class instance. Uses same multipole
    // order as GAMESS default
    template<class libaccqc::wavefunctions::WavefnType>
    class Efmo : public Efp<libaccqc::wavefunctions::WavefnType> {

      public:

      /***** Constructor(s) and Destructor(s) *****/

        Efmo();

        // TODO: Etc.

        ~Efmo();

      /***** Public Member Functions *****/

        // TODO: Copy over when possible

      /***** Public Member Variables *****/

        // TODO: Copy over when possible
        
      /***** Operator Overloads *****/

        // TODO: Copy over when possible

      protected:

      /***** Protected Member Functions *****/

        // TODO: Copy over when possible

      /***** Protected Member Variables *****/

        // TODO: Copy over when possible

      private:

      /***** Protected Member Functions *****/

        // TODO: Copy over when possible

      /***** Protected Member Variables *****/

        // TODO: Copy over as possible

    };

  } // namespace libaccefp::methods

} // namespace libaccefp

#endif /* LIBACCEFP_INCLUDE_METHODS_EFMO_HPP_ */