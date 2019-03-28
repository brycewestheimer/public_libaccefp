/*
 * generate_dma.hpp
 * 
 * Copied: Mar 28, 2019
 * Author: Bryce M. Westheimer
 * 
 * Description: Drivers for generating distributed multipoles based on Stone's 
 *              Distributed Multipole Analysis (DMA).
 * 
 */

#ifndef LIBACCEFP_SOURCE_DRIVERS_DMA_GENERATE_DMA_HPP_
#define LIBACCEFP_SOURCE_DRIVERS_DMA_GENERATE_DMA_HPP_

#include "efp.hpp"

#include <libaccqc.hpp>   // WavefnType classes

namespace libaccefp {

  namespace drivers {

    /*! \fn generate_distributed_multipoles
     *  \ingroup TODO:ADD_GROUP
     * 
     *  \brief Driver function for generating distributed multipoles
     * 
     *  \details TODO
     * 
     *  \author Bryce M. Westheimer
     * 
     *  \date Mar 28, 2019
     * 
     */ 
    template<class libaccqc::wavefunctions::WavefnType>
    void generate_distributed_multipoles(libaccefp::methods::Efp<WavefnType>& efp_fragment);

  } // namespace libaccefp::drivers

} // namespace libaccefp

#endif /* LIBACCEFP_SOURCE_DRIVERS_DMA_GENERATE_DMA_HPP_ */