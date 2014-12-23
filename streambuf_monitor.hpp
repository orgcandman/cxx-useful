#include <map>
#include <streambuf>

#include "interlocked_traits.hpp"

namespace cxx_utils
{
    namespace io
    {
        class streambuf_callback
        {
        public:
            enum streambuf_cb_result
            {
                CB_NONE,
                CB_DROP_STREAM
            };

            enum streambuf_cb_status
            {
                CB_READ_OK,
                CB_WRITE_OK,
                CB_OPENED,
                CB_CLOSED
            };
            
            streambuf_callback(){}
            virtual ~streambuf_callback(){}

            virtual streambuf_cb_result callback(
                streambuf_cb_status eStatus, std::streambuf *pStreamBuf)=0;
        };

        template <class interlocked_trait = cxx_utils::null_interlocked_trait>
        class streambuf_monitor
        {
            std::map<std::streambuf*,streambuf_callback&> m_cCallbacks;
            interlocked_trait :: locked_exchange_type m_eLock;

            static void check_buffer
            (std::map<std::streambuf*,streambuf_callback&>::value_type t)
            {
                std::streambuf *pStream = t.first;
                std::streamsize nBytes = pStream->in_avail();
                streambuf_callback::streambuf_cb_result result =
                    streambuf_callback::CB_NONE;
                if( nBytes == -1 )
                {
                    result = (t.second).callback
                        (streambuf_callback::CB_CLOSED, pStream);
                } else if ( nBytes > 0 )
                {
                    result = (t.second).callback
                        (streambuf_callback::CB_READ_OK, pStream);
                }

                if( result == streambuf_callback::CB_DROP_STREAM )
                { /* -- to do -- */ }

            }

        public:
            streambuf_monitor() {}
            ~streambuf_monitor() {}

            bool push(std::streambuf *pBuffer, streambuf_callback &rCB,
                      bool bCallOpen)
            {
                interlocked_trait::locked_exchange e(m_eLock);
                if ( bCallOpen )
                {
                    streambuf_callback::streambuf_cb_result result =
                        rCB.callback(streambuf_callback::CB_OPENED,
                                     pBuffer);
                    if( result == streambuf_callback::CB_DROP_STREAM )
                        return;
                }
                m_cCallbacks[pBuffer] = rCB;
            }

            bool pop(std::streambuf *pBuffer)
            {
                interlocked_trait::locked_exchange e(m_eLock);
                m_cCallbacks.erase(pBuffer);
            }
            
            void operator()
            {
                interlocked_trait::locked_exchange e(m_eLock);
                for_each( m_cCallbacks.begin(), m_cCallbacks.end(),
                          check_buffer );
            }
        };
    }
}
