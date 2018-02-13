
// standard headers
#include <limits>

// project headers
#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Util.h"
#include "RingBuffer.h"

#include "CombFilterIf.h"
#include "CombFilter.h"


CCombFilterBase::CCombFilterBase( int iMaxDelayInFrames, int iNumChannels ) :
    m_ppCRingBuffer(0),
    m_iNumChannels(iNumChannels)
{
    assert(iMaxDelayInFrames > 0);
    assert(iNumChannels > 0);
    
    // Parameter range set for FIR filters
    CCombFilterBase::m_aafParamRange[0][0] = std::numeric_limits<float>::min();
    CCombFilterBase::m_aafParamRange[0][1] = std::numeric_limits<float>::max();
    CCombFilterBase::m_aafParamRange[1][0] = 1.0f;
    CCombFilterBase::m_aafParamRange[1][1] = iMaxDelayInFrames;
    
    // Allocate memory for ring buffer
    m_ppCRingBuffer = new CRingBuffer<float>* [iNumChannels];
    for(int i = 0; i < iNumChannels; i++)
    {
        m_ppCRingBuffer[i] = new CRingBuffer<float>(iMaxDelayInFrames);
    }
    
}

CCombFilterBase::~CCombFilterBase()
{
    // Deallocating memory to avoid memory leak
    for(int i = 0; i < m_iNumChannels; i++)
    {
        delete [] m_ppCRingBuffer[i];
    }
    delete [] m_ppCRingBuffer;
    m_ppCRingBuffer = 0;
}

Error_t CCombFilterBase::resetInstance()
{
    for(int i = 0; i < m_iNumChannels; i++)
    {
        m_ppCRingBuffer[i]->reset();
    }
    return kNoError;
}

Error_t CCombFilterBase::setParam( CCombFilterIf::FilterParam_t eParam, float fParamValue )
{
    if (!isInParamRange(eParam, fParamValue))
        return kFunctionInvalidArgsError;
    
    // Sets value for given filter parameter
    m_afParam[eParam] = fParamValue;
    if (eParam == CCombFilterIf::FilterParam_t::kParamDelay)
    {
        for(int i = 0; i < m_iNumChannels; i++)
        {
            m_ppCRingBuffer[i]->setReadIdx(int(m_ppCRingBuffer[i]->getLength()-fParamValue));
            m_ppCRingBuffer[i]->setWriteIdx(0);
        }
    }

    return kNoError;
}

float CCombFilterBase::getParam( CCombFilterIf::FilterParam_t eParam ) const
{
    // Returns value of desired filter parameter
    return m_afParam[eParam];
}

bool CCombFilterBase::isInParamRange( CCombFilterIf::FilterParam_t eParam, float fValue )
{
    if (fValue < m_aafParamRange[eParam][0] || fValue > m_aafParamRange[eParam][1])
    {
        return false;
    }
    else
    {
        return true;
    }
}

Error_t CCombFilterFir::process( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames )
{
    for(int i = 0; i < CCombFilterBase::m_iNumChannels; i++)
    {
        for(int j = 0; j < iNumberOfFrames; j++)
        {
            ppfOutputBuffer[i][j] = ppfInputBuffer[i][j] + CCombFilterBase::m_afParam[0] * CCombFilterBase::m_ppCRingBuffer[i]->getPostInc();
            CCombFilterBase::m_ppCRingBuffer[i]->putPostInc(ppfInputBuffer[i][j]);
        }
    
    }
    
    return kNoError;
}


CCombFilterIir::CCombFilterIir (int iMaxDelayInFrames, int iNumChannels) : CCombFilterBase(iMaxDelayInFrames, iNumChannels)
{
    CCombFilterBase::m_aafParamRange[0][0] = -1.0f;
    CCombFilterBase::m_aafParamRange[0][1] = 1.0f;
}

Error_t CCombFilterIir::process( float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames )
{
    for(int i = 0; i < CCombFilterBase::m_iNumChannels; i++)
    {
        for(int j = 0; j < iNumberOfFrames; j++)
        {
            ppfOutputBuffer[i][j] = ppfInputBuffer[i][j] + CCombFilterBase::m_afParam[0] * CCombFilterBase::m_ppCRingBuffer[i]->getPostInc();
            CCombFilterBase::m_ppCRingBuffer[i]->putPostInc(ppfOutputBuffer[i][j]);
        }
        
    }
    
    return kNoError;
}
