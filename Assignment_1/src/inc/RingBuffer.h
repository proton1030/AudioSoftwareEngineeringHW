#if !defined(__RingBuffer_hdr__)
#define __RingBuffer_hdr__

#include <cassert>
#include <algorithm>
#include <cmath>

/*! \brief implement a circular buffer of type T
*/
template <class T> 
class CRingBuffer
{
public:
    explicit CRingBuffer (int iBufferLengthInSamples) :
        m_iBuffLength(iBufferLengthInSamples),
        m_iReadIdx(0),                          
        m_iWriteIdx(0),
        m_ptBuff(0)
    {
        assert(iBufferLengthInSamples > 0);

        m_ptBuff        = new T [m_iBuffLength];
        reset();
    }

    virtual ~CRingBuffer ()
    {
        delete [] m_ptBuff;
        m_ptBuff    = 0;
    }

    /*! add a new value of type T to write index
    \param tNewValue the new value
    \return void
    */
    void put(T tNewValue)
    {
        m_ptBuff[m_iWriteIdx] = tNewValue;
    }
    
    /*! return the value at the current read index
    \return type T the value from the read index
    */
    T get () const
    {
        return m_ptBuff[m_iReadIdx];
    }

    /*! return the current index for writing/put
    \return int
    */
    int getWriteIdx () const
    {
        return m_iWriteIdx;
    }

    /*! move the write index to a new position
    \param iNewWriteIdx: new position
    \return void
    */
    void setWriteIdx (int iNewWriteIdx)
    {
        m_iWriteIdx = mod(iNewWriteIdx , m_iBuffLength);
    }

    /*! return the current index for reading/get
    \return int
    */
    int getReadIdx () const
    {
        return m_iReadIdx;
    }

    /*! move the read index to a new position
    \param iNewReadIdx: new position
    \return void
    */
    void setReadIdx (int iNewReadIdx)
    {
        m_iReadIdx = mod(iNewReadIdx , m_iBuffLength);
    }

    /*! add a new value of type T to write index and increment write index
    \param tNewValue the new value
    \return void
    */
    void putPostInc (T tNewValue)
    {
        put(tNewValue);
        m_iWriteIdx = m_iWriteIdx + 1 >= m_iBuffLength ? 0 : m_iWriteIdx + 1;
    }

    /*! return the value at the current read index and increment the read pointer
    \return type T the value from the read index
    */
    T getPostInc ()
    {
        int m_tempRead = get();
        m_iReadIdx = m_iReadIdx + 1 >= m_iBuffLength ? 0 : m_iReadIdx + 1;
        return m_tempRead;
    }

    /*! return the value at the index with an arbitrary offset
    \param iOffset: read at offset from read index
    \return type T the value from the read index
    */
    T get (int iOffset) const
    {
        return m_ptBuff[mod(m_iReadIdx + iOffset , m_iBuffLength)];
    }
    
    /*! set buffer content and indices to 0
    \return void
    */
    void reset ()
    {
        memset (m_ptBuff, 0, sizeof(T)*m_iBuffLength);
        m_iReadIdx  = 0;
        m_iWriteIdx = 0;
    }

    /*! returns the number of values currently buffered (note: 0 could also mean the buffer is full!)
    \return int
    */
    int getNumValuesInBuffer () const
    {
        return (m_iWriteIdx - m_iReadIdx + m_iBuffLength)%m_iBuffLength;
    }

    /*! returns the length of the internal buffer
    \return int
    */
    int getLength () const
    {
        return m_iBuffLength;
    }
private:
    CRingBuffer ();
    CRingBuffer(const CRingBuffer& that);
    
    int mod (int a, int b) const
    {
        int ret = a % b;
        if (ret < 0) ret += b;
        return ret;
    }

    int m_iBuffLength,              //!< length of the internal buffer
        m_iReadIdx,                 //!< current read index
        m_iWriteIdx;                //!< current write index

    T   *m_ptBuff;                  //!< data buffer
};
#endif // __RingBuffer_hdr__
