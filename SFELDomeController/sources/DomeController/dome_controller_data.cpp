

/** ********************************************************************************************************************
 * @file dome_controller_data.cpp
 * @brief This file contains the implementation of common elements for the DomeController module.
 * @author Degoras Project Team
 * @author AVS AMELAS Team
 * @copyright EUPL License
***********************************************************************************************************************/

// C++ INCLUDES
// =====================================================================================================================

// =====================================================================================================================

// PROJECT INCLUDES
// =====================================================================================================================
#include "LibSFELDomeController/DomeController/dome_controller_data.h"
// =====================================================================================================================

// AMELAS NAMESPACES
// =====================================================================================================================
namespace sfeldome{
namespace controller{
// =====================================================================================================================

AltAzPos::AltAzPos(double az, double el):
    az(az), el(el){}

AltAzPos::AltAzPos(): az(-1), el(-1){}

size_t AltAzPos::serialize(zmqutils::serializer::BinarySerializer &serializer) const
{
    return serializer.write(this->az, this->el);
}

void AltAzPos::deserialize(zmqutils::serializer::BinarySerializer &serializer)
{
    serializer.read(this->az, this->el);
}

size_t AltAzPos::serializedSize() const
{
    return Serializable::calcTotalSize(this->az, this->el);
}

AltAzPos::~AltAzPos(){}

// =====================================================================================================================


}} // END NAMESPACES.
// =====================================================================================================================
