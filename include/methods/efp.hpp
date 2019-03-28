/*
 * efp.hpp
 * 
 * Copied: Mar 28, 2019
 * Author: Bryce M. Westheimer
 * 
 * Description: Effective Fragment Potential class template and related functionality
 * 
 */

#ifndef LIBACCEFP_INCLUDE_METHODS_EFP_HPP_
#define LIBACCEFP_INCLUDE_METHODS_EFP_HPP_

#include <dtl.hpp>        // Distributed vectors and matrices
#include <libaccqc.hpp>   // WavefnType classes
#include <libcfrag.hpp>   // Fragment class

#include <array>
#include <functional>     // reference_wrapper<>

namespace libaccefp {

  namespace methods {

    /*! \enum LocalizationMethod
     *  \ingroup TODO:ADD_GROUP
     * 
     *  \brief Enumerator for type of localization method used in EFP parameters
     * 
     *  \details TODO
     * 
     *  \author Bryce M. Westheimer
     * 
     *  \date Mar 28, 2019
     * 
     */ 
    enum class LocalizationMethod {
      Edmiston_Ruedenberg,
      Boys,
      Pipek_Mezey
      // TODO: Add as necessary
    };

    // Typedefs for distributed multipole moments
    // Storage is only for symmetry-unique elements
    typedef std::array<double, 3> dipole_moment;
    typedef std::array<double, 6> quadrupole_moment;
    typedef std::array<double, 10> hexapole_moment;
    typedef std::array<double, > octopole_moment;

    // Typedef for polarizability tensor (3 x 3 Matrix)
    typedef std::array< std::array<double, 3>, 3> polarizability_tensor;

    /*! \class Efp
     *  \ingroup TODO:ADD_GROUP
     * 
     *  \brief The Effective Fragment Potential fragment class
     * 
     *  \details TODO
     * 
     *  \note Currently only Restriced Hartree-Fock is supported by EFP, but generic WavefnType is used
     *        for eventual addition of other wave functions (e.g. ROHF)
     * 
     *  \note All members are restricted to public/protected access because the EFMO class extends this
     *        class and needs access to all members.
     * 
     *  \author Bryce M. Westheimer
     * 
     *  \date Mar 28, 2019
     * 
     */ 
    template<class libaccqc::wavefunctions::WavefnType>
    class Efp : public libfrag::methods::Fragment<libaccqc::wavefunctions::WavefnType> {

      public:

      /***** Constructor(s) and Destructor(s) *****/

        // Default constructor. Assume localization method is Edmiston-Ruedenberg and all parameters turned on (dispersion, charge-transfer, etc.)
        Efp();

        // TODO: Etc.

        ~Efp();

      /***** Public Member Functions *****/

        // Generate the effective potential for this fragment
        void make_efp();

      /***** Public Member Variables *****/

        // TODO: Copy over when possible
        
      /***** Operator Overloads *****/

        // TODO: Copy over when possible

      protected:

      /***** Protected Member Functions *****/

        // Generate the distributed multipole moments for this fragment up to 'multipole_order'
        void generate_distributed_multipoles();

      /***** Protected Member Variables *****/

      /***** Effective Potential flags *****/

        // Multipole order for DMA
        // TODO

        // Electrostatic screening
        // TODO

        // Polarization screening 
        // TODO

        // Valence-Virtual-Orbital flag for Charge-Transfer
        // TODO

        // Use Bond-midpoints in DMA along with atom centers
        // TODO

        // Boolean flags for including each term
        // Default: All terms included
        bool include_electrostatics_      = true;
        bool include_polarization_        = true;
        bool include_exchange_repulsion_  = true;
        bool include_dispersion_          = true;
        bool include_charge_transfer_     = true;

      /***** Effective Potential parameters *****/
        
        // Multipole moments
        dtl::math::Vector<double>                   monopoles_;
        dtl::math::Vector<dipole_moment>            dipoles_;
        dtl::math::Vector<quadrupole_moment>        quadrupoles_;
        dtl::math::VectorVector<hexapole_moment>    hexapoles_;
        dtl::math::VectorVector<octopole_moment>    octopoles_;
        
        // Polarizability tensors
        dtl::math::Vector<polarizability_tensor> static_polarizability_tensors_;

        // Dynamic polarizability tensors for dispersion
        dtl::math::Vector<polarizability_tensor> dynamic_polarizability_tensors_;

        // Localized Molecular Orbital parameters
        LocalizationMethod localization_method_ = LocalizationMethod::Edmiston_Ruedenberg;
        dtl::math::Matrix<double> lmo_coefficients_;

        // TODO: Etc.

      /***** Friends *****/

        // Full EFP energy functions
        friend double energy(const std::vector< std::reference_wrapper<Efp> >& fragments);

        friend double libaccefp::drivers::many_body_pol_energy(const std::vector< std::reference_wrapper<Efp> >& efp_fragments);

        // EFP-EFP energy functions
        friend double libaccefp::drivers::energy(Efp& frag_1, Efp& frag_2);

        friend double libaccefp::drivers::electrostatic_energy(Efp& frag_1, Efp& frag_2);

        friend double libaccefp::drivers::exchange_repulsion_energy(Efp& frag_1, Efp& frag_2);

        friend double libaccefp::drivers::polarization_energy(Efp& frag_1, Efp& frag_2);

        friend double libaccefp::drivers::dispersion_energy(Efp& frag_1, Efp& frag_2);

        friend double libaccefp::drivers::charge_transfer_energy(Efp& frag_1, Efp& frag_2);

        // QM-EFP energy function templates
        template<class QmWavefnType>
        friend double energy(const libaccefp::methods::Efp& efp_frag, const QmWavefnType& qm_frag);

        template<class QmWavefnType>
        friend double electrostatic_energy(const libaccefp::methods::Efp& efp_frag, const QmWavefnType& qm_frag);

        template<class QmWavefnType>
        friend double exchange_repulsion_energy(const libaccefp::methods::Efp& efp_frag, const QmWavefnType& qm_frag);

        template<class QmWavefnType>
        friend double polarization_energy(const libaccefp::methods::Efp& efp_frag, const QmWavefnType& qm_frag);

        template<class QmWavefnType>
        friend double dispersion_energy(const libaccefp::methods::Efp& efp_frag, const QmWavefnType& qm_frag);

        template<class QmWavefnType>
        friend double charge_transfer_energy(const libaccefp::methods::Efp& efp_frag, const QmWavefnType& qm_frag);

    };

  } // namespace libaccefp::methods

} // namespace libaccefp

#endif /* LIBACCEFP_INCLUDE_METHODS_EFP_HPP_ */