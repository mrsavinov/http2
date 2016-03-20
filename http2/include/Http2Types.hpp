#pragma once

/// Define WIN32 when build target is Win32 API (borrowed from libcurl) //
#if (defined(_WIN32) || defined(__WIN32__)) && !defined(WIN32)
#define WIN32
#endif

#include <stdlib.h>
#if defined(_MSC_VER) && (_MSC_VER < 1800)
#include <stdint.h>
#else /// !defined(_MSC_VER) || (_MSC_VER >= 1800) //
#include <inttypes.h>
#endif /// !defined(_MSC_VER) || (_MSC_VER >= 1800) //
#include <sys/types.h>
#include <cstddef>
#include <string>
#include <vector>
	
#define ETU_HTTP2_PROTO_VERSION_ID "h2"
	
#define ETU_HTTP2_PROTO_VERSION_ID_LEN 2

#define ETU_HTTP2_PROTO_ALPN "\x2h2"

#define ETU_HTTP2_PROTO_ALPN_LEN (sizeof(ETU_HTTP2_PROTO_ALPN) - 1)

#define ETU_HTTP2_CLEARTEXT_PROTO_VERSION_ID "h2c"

#define ETU_HTTP2_CLEARTEXT_PROTO_VERSION_ID_LEN 3

	struct ETU_HTTP2_session;

	typedef struct ETU_HTTP2_session ETU_HTTP2_session;
	typedef std::size_t size_t;

#define ETU_HTTP2_VERSION_AGE 1

//The default weight of stream dependency.
#define ETU_HTTP2_DEFAULT_WEIGHT 16

//The maximum weight of stream dependency.
#define ETU_HTTP2_MAX_WEIGHT 256

//The minimum weight of stream dependency.
#define ETU_HTTP2_MIN_WEIGHT 1

//The maximum window size
#define ETU_HTTP2_MAX_WINDOW_SIZE ((int32_t)((1U << 31) - 1))

//The initial window size for stream level flow control.
#define ETU_HTTP2_INITIAL_WINDOW_SIZE ((1 << 16) - 1)
	
//The initial window size for connection level flow control.
#define ETU_HTTP2_INITIAL_CONNECTION_WINDOW_SIZE ((1 << 16) - 1)

	//
	// @macro
	//
	// The default header table size.
	//
#define ETU_HTTP2_DEFAULT_HEADER_TABLE_SIZE (1 << 12)

	//
	// @macro
	//
	// The client magic string, which is the first 24 bytes byte string of
	// client connection preface.
	//
#define ETU_HTTP2_CLIENT_MAGIC "PRI // HTTP/2.0\r\n\r\nSM\r\n\r\n"

	//
	// @macro
	//
	// The length of :macro:`ETU_HTTP2_CLIENT_MAGIC`.
	//
#define ETU_HTTP2_CLIENT_MAGIC_LEN 24

namespace etu{
namespace http2 {
	//
	// @enum
	//
	// Error codes used in this library.  The code range is [-999, -500],
	// inclusive. The following values are defined:
	//
namespace Error {
	enum class Code {
		//
		// Invalid argument passed.
		//
		ETU_HTTP2_ERR_INVALID_ARGUMENT = -501,
			//
			// Out of buffer space.
			//
			ETU_HTTP2_ERR_BUFFER_ERROR = -502,
			//
			// The specified protocol version is not supported.
			//
			ETU_HTTP2_ERR_UNSUPPORTED_VERSION = -503,
			//
			// Used as a return value from :type:`ETU_HTTP2_send_callback`,
			// :type:`ETU_HTTP2_recv_callback` and
			// :type:`ETU_HTTP2_send_data_callback` to indicate that the operation
			// would block.
			//
			ETU_HTTP2_ERR_WOULDBLOCK = -504,
			//
			// General protocol error
			//
			ETU_HTTP2_ERR_PROTO = -505,
			//
			// The frame is invalid.
			//
			ETU_HTTP2_ERR_INVALID_FRAME = -506,
			//
			// The peer performed a shutdown on the connection.
			//
			ETU_HTTP2_ERR_EOF = -507,
			//
			// Used as a return value from
			// :func:`ETU_HTTP2_data_source_read_callback` to indicate that data
			// transfer is postponed.  See
			// :func:`ETU_HTTP2_data_source_read_callback` for details.
			//
			ETU_HTTP2_ERR_DEFERRED = -508,
			//
			// Stream ID has reached the maximum value.  Therefore no stream ID
			// is available.
			//
			ETU_HTTP2_ERR_STREAM_ID_NOT_AVAILABLE = -509,
			//
			// The stream is already closed; or the stream ID is invalid.
			//
			ETU_HTTP2_ERR_STREAM_CLOSED = -510,
			//
			// RST_STREAM has been added to the outbound queue.  The stream is
			// in closing state.
			//
			ETU_HTTP2_ERR_STREAM_CLOSING = -511,
			//
			// The transmission is not allowed for this stream (e.g., a frame
			// with END_STREAM flag set has already sent).
			//
			ETU_HTTP2_ERR_STREAM_SHUT_WR = -512,
			//
			// The stream ID is invalid.
			//
			ETU_HTTP2_ERR_INVALID_STREAM_ID = -513,
			//
			// The state of the stream is not valid (e.g., DATA cannot be sent
			// to the stream if response HEADERS has not been sent).
			//
			ETU_HTTP2_ERR_INVALID_STREAM_STATE = -514,
			//
			// Another DATA frame has already been deferred.
			//
			ETU_HTTP2_ERR_DEFERRED_DATA_EXIST = -515,
			//
			// Starting new stream is not allowed (e.g., GOAWAY has been sent
			// and/or received).
			//
			ETU_HTTP2_ERR_START_STREAM_NOT_ALLOWED = -516,
			//
			// GOAWAY has already been sent.
			//
			ETU_HTTP2_ERR_GOAWAY_ALREADY_SENT = -517,
			//
			// The received frame contains the invalid header block (e.g., There
			// are duplicate header names; or the header names are not encoded
			// in US-ASCII character set and not lower cased; or the header name
			// is zero-length string; or the header value contains multiple
			// in-sequence NUL bytes).
			//
			ETU_HTTP2_ERR_INVALID_HEADER_BLOCK = -518,
			//
			// Indicates that the context is not suitable to perform the
			// requested operation.
			//
			ETU_HTTP2_ERR_INVALID_STATE = -519,
			//
			// The user callback function failed due to the temporal error.
			//
			ETU_HTTP2_ERR_TEMPORAL_CALLBACK_FAILURE = -521,
			//
			// The length of the frame is invalid, either too large or too small.
			//
			ETU_HTTP2_ERR_FRAME_SIZE_ERROR = -522,
			//
			// Header block inflate/deflate error.
			//
			ETU_HTTP2_ERR_HEADER_COMP = -523,
			//
			// Flow control error
			//
			ETU_HTTP2_ERR_FLOW_CONTROL = -524,
			//
			// Insufficient buffer size given to function.
			//
			ETU_HTTP2_ERR_INSUFF_BUFSIZE = -525,
			//
			// Callback was paused by the application
			//
			ETU_HTTP2_ERR_PAUSE = -526,
			//
			// There are too many in-flight SETTING frame and no more
			// transmission of SETTINGS is allowed.
			//
			ETU_HTTP2_ERR_TOO_MANY_INFLIGHT_SETTINGS = -527,
			//
			// The server push is disabled.
			//
			ETU_HTTP2_ERR_PUSH_DISABLED = -528,
			//
			// DATA or HEADERS frame for a given stream has been already
			// submitted and has not been fully processed yet.  Application
			// should wait for the transmission of the previously submitted
			// frame before submitting another.
			//
			ETU_HTTP2_ERR_DATA_EXIST = -529,
			//
			// The current session is closing due to a connection error or
			// `ETU_HTTP2_session_terminate_session()` is called.
			//
			ETU_HTTP2_ERR_SESSION_CLOSING = -530,
			//
			// Invalid HTTP header field was received and stream is going to be
			// closed.
			//
			ETU_HTTP2_ERR_HTTP_HEADER = -531,
			//
			// Violation in HTTP messaging rule.
			//
			ETU_HTTP2_ERR_HTTP_MESSAGING = -532,
			//
			// Stream was refused.
			//
			ETU_HTTP2_ERR_REFUSED_STREAM = -533,
			//
			// Unexpected internal error, but recovered.
			//
			ETU_HTTP2_ERR_INTERNAL = -534,
			//
			// Indicates that a processing was canceled.
			//
			ETU_HTTP2_ERR_CANCEL = -535,
			//
			// The errors < :enum:`ETU_HTTP2_ERR_FATAL` mean that the library is
			// under unexpected condition and processing was terminated (e.g.,
			// out of memory).  If application receives this error code, it must
			// stop using that :type:`ETU_HTTP2_session` object and only allowed
			// operation for that object is deallocate it using
			// `ETU_HTTP2_session_del()`.
			//
			ETU_HTTP2_ERR_FATAL = -900,
			//
			// Out of memory.  This is a fatal error.
			//
			ETU_HTTP2_ERR_NOMEM = -901,
			//
			// The user callback function failed.  This is a fatal error.
			//
			ETU_HTTP2_ERR_CALLBACK_FAILURE = -902,
			//
			// Invalid client magic (see :macro:`ETU_HTTP2_CLIENT_MAGIC`) was
			// received and further processing is not possible.
			//
			ETU_HTTP2_ERR_BAD_CLIENT_MAGIC = -903,
			//
			// Possible flooding by peer was detected in this HTTP/2 session.
			// Flooding is measured by how many PING and SETTINGS frames with
			// ACK flag set are queued for transmission.  These frames are
			// response for the peer initiated frames, and peer can cause memory
			// exhaustion on server side to send these frames forever and does
			// not read network.
			//
			ETU_HTTP2_ERR_FLOODED = -904
	};

}
	//
	// @enum
	//
	// The flags for header field name/value pair.
	//
	enum KeyValueFlags {
		//
		// No flag set.
		//
		ETU_HTTP2_NV_FLAG_NONE = 0,
		//
		// Indicates that this name/value pair must not be indexed ("Literal
		// Header Field never Indexed" representation must be used in HPACK
		// encoding).  Other implementation calls this bit as "sensitive".
		//
		ETU_HTTP2_NV_FLAG_NO_INDEX = 0x01,
		//
		// This flag is set solely by application.  If this flag is set, the
		// library does not make a copy of header field name.  This could
		// improve performance.
		//
		ETU_HTTP2_NV_FLAG_NO_COPY_NAME = 0x02,
		//
		// This flag is set solely by application.  If this flag is set, the
		// library does not make a copy of header field value.  This could
		// improve performance.
		//
		ETU_HTTP2_NV_FLAG_NO_COPY_VALUE = 0x04
	} ;



	//
	// @enum
	//
	// The frame types in HTTP/2 specification.
	//
	enum class FrameType {
		//
		// The DATA frame.
		//
		ETU_HTTP2_DATA = 0,
		//
		// The HEADERS frame.
		//
		ETU_HTTP2_HEADERS = 0x01,
		//
		// The PRIORITY frame.
		//
		ETU_HTTP2_PRIORITY = 0x02,
		//
		// The RST_STREAM frame.
		//
		ETU_HTTP2_RST_STREAM = 0x03,
		//
		// The SETTINGS frame.
		//
		ETU_HTTP2_SETTINGS = 0x04,
		//
		// The PUSH_PROMISE frame.
		//
		ETU_HTTP2_PUSH_PROMISE = 0x05,
		//
		// The PING frame.
		//
		ETU_HTTP2_PING = 0x06,
		//
		//The GOAWAY frame.
		//
		ETU_HTTP2_GOAWAY = 0x07,
		//
		//The WINDOW_UPDATE frame.
		//
		ETU_HTTP2_WINDOW_UPDATE = 0x08,
		//
		//The CONTINUATION frame.  This frame type won't be passed to any
		//callbacks because the library processes this frame type and its
		//preceding HEADERS/PUSH_PROMISE as a single frame.
		//
		ETU_HTTP2_CONTINUATION = 0x09
	};

	//The flags for HTTP/2 frames.  This enum defines all flags for allframes.
	enum class FrameFlags {
		//
		// No flag set.
		//
		ETU_HTTP2_FLAG_NONE = 0,
		//
		// The END_STREAM flag.
		//
		ETU_HTTP2_FLAG_END_STREAM = 0x01,
		//
		// The END_HEADERS flag.
		//
		ETU_HTTP2_FLAG_END_HEADERS = 0x04,
		//
		// The ACK flag.
		//
		ETU_HTTP2_FLAG_ACK = 0x01,
		//
		// The PADDED flag.
		//
		ETU_HTTP2_FLAG_PADDED = 0x08,
		//
		// The PRIORITY flag.
		//
		ETU_HTTP2_FLAG_PRIORITY = 0x20
	};

	//
	// @enum
	// The SETTINGS ID.
	//
	enum class SettingsId {
		//
		// SETTINGS_HEADER_TABLE_SIZE
		//
		ETU_HTTP2_SETTINGS_HEADER_TABLE_SIZE = 0x01,
		//
		// SETTINGS_ENABLE_PUSH
		//
		ETU_HTTP2_SETTINGS_ENABLE_PUSH = 0x02,
		//
		// SETTINGS_MAX_CONCURRENT_STREAMS
		//
		ETU_HTTP2_SETTINGS_MAX_CONCURRENT_STREAMS = 0x03,
		//
		// SETTINGS_INITIAL_WINDOW_SIZE
		//
		ETU_HTTP2_SETTINGS_INITIAL_WINDOW_SIZE = 0x04,
		//
		// SETTINGS_MAX_FRAME_SIZE
		//
		ETU_HTTP2_SETTINGS_MAX_FRAME_SIZE = 0x05,
		//
		// SETTINGS_MAX_HEADER_LIST_SIZE
		//
		ETU_HTTP2_SETTINGS_MAX_HEADER_LIST_SIZE = 0x06
	} ;
	/// Note: If we add SETTINGS, update the capacity of ETU_HTTP2_INBOUND_NUM_IV as well //

	//
	// @macro
	//
	// Default maximum number of incoming concurrent streams.  Use
	// `ETU_HTTP2_submit_settings()` with
	// :enum:`ETU_HTTP2_SETTINGS_MAX_CONCURRENT_STREAMS` to change the
	// maximum number of incoming concurrent streams.
	//
	// .. note::
	//
	//   The maximum number of outgoing concurrent streams is 100 by
	//   default.
	//
#define ETU_HTTP2_INITIAL_MAX_CONCURRENT_STREAMS ((1U << 31) - 1)

	//
	// @enum
	// The status codes for the RST_STREAM and GOAWAY frames.
	//
	enum class ErrorCode {
		//
		// No errors.
		//
		ETU_HTTP2_NO_ERROR = 0x00,
		//
		// PROTOCOL_ERROR
		//
		ETU_HTTP2_PROTOCOL_ERROR = 0x01,
		//
		// INTERNAL_ERROR
		//
		ETU_HTTP2_INTERNAL_ERROR = 0x02,
		//
		// FLOW_CONTROL_ERROR
		//
		ETU_HTTP2_FLOW_CONTROL_ERROR = 0x03,
		//
		// SETTINGS_TIMEOUT
		//
		ETU_HTTP2_SETTINGS_TIMEOUT = 0x04,
		//
		// STREAM_CLOSED
		//
		ETU_HTTP2_STREAM_CLOSED = 0x05,
		//
		// FRAME_SIZE_ERROR
		//
		ETU_HTTP2_FRAME_SIZE_ERROR = 0x06,
		//
		// REFUSED_STREAM
		//
		ETU_HTTP2_REFUSED_STREAM = 0x07,
		//
		// CANCEL
		//
		ETU_HTTP2_CANCEL = 0x08,
		//
		// COMPRESSION_ERROR
		//
		ETU_HTTP2_COMPRESSION_ERROR = 0x09,
		//
		// CONNECT_ERROR
		//
		ETU_HTTP2_CONNECT_ERROR = 0x0a,
		//
		// ENHANCE_YOUR_CALM
		//
		ETU_HTTP2_ENHANCE_YOUR_CALM = 0x0b,
		//
		// INADEQUATE_SECURITY
		//
		ETU_HTTP2_INADEQUATE_SECURITY = 0x0c,
		//
		// HTTP_1_1_REQUIRED
		//
		ETU_HTTP2_HTTP_1_1_REQUIRED = 0x0d
	};

	//
	// @struct
	// The frame header.
	//
	struct FrameHeader {
		//
		// The length field of this frame, excluding frame header.
		//
		std::size_t length;
		//
		// The stream identifier (aka, stream ID)
		//
		int32_t stream_id;
		//
		// The type of this frame.  See `ETU_HTTP2_frame_type`.
		//
		uint8_t type;
		//
		// The flags.
		//
		uint8_t flags;
		//
		// Reserved bit in frame header.  Currently, this is always set to 0
		// and application should not expect something useful in here.
		//
		uint8_t reserved;
	};

	//
	// @enum
	//
	// The flags used to set in |data_flags| output parameter in
	// :type:`ETU_HTTP2_data_source_read_callback`.
	//
	enum class DataFlag  {
		//
		// No flag set.
		//
		ETU_HTTP2_DATA_FLAG_NONE = 0,
		//
		// Indicates EOF was sensed.
		//
		ETU_HTTP2_DATA_FLAG_EOF = 0x01,
		//
		// Indicates that END_STREAM flag must not be set even if
		// ETU_HTTP2_DATA_FLAG_EOF is set.  Usually this flag is used to send
		// trailer fields with `ETU_HTTP2_submit_request()` or
		// `ETU_HTTP2_submit_response()`.
		//
		ETU_HTTP2_DATA_FLAG_NO_END_STREAM = 0x02,
		//
		// Indicates that application will send complete DATA frame in
		// :type:`ETU_HTTP2_send_data_callback`.
		//
		ETU_HTTP2_DATA_FLAG_NO_COPY = 0x04
	} ;

	//
	// @enum
	//
	// The category of HEADERS, which indicates the role of the frame.  In
	// HTTP/2 spec, request, response, push response and other arbitrary
	// headers (e.g., trailer fields) are all called just HEADERS.  To
	// give the application the role of incoming HEADERS frame, we define
	// several categories.
	//
	enum class HeaderCategory {
		//
		// The HEADERS frame is opening new stream, which is analogous to
		// SYN_STREAM in SPDY.
		//
		ETU_HTTP2_HCAT_REQUEST = 0,
		//
		// The HEADERS frame is the first response headers, which is
		// analogous to SYN_REPLY in SPDY.
		//
		ETU_HTTP2_HCAT_RESPONSE = 1,
		//
		// The HEADERS frame is the first headers sent against reserved
		// stream.
		//
		ETU_HTTP2_HCAT_PUSH_RESPONSE = 2,
		//
		// The HEADERS frame which does not apply for the above categories,
		// which is analogous to HEADERS in SPDY.  If non-final response
		// (e.g., status 1xx) is used, final response HEADERS frame will be
		// categorized here.
		//
		ETU_HTTP2_HCAT_HEADERS = 3
	};

	typedef struct DataFrame {
		FrameHeader hd;
		/**
		* The length of the padding in this frame.  This includes PAD_HIGH
		* and PAD_LOW.
		*/
		size_t padlen;
	};
	//
	// @struct
	//
	// The structure to specify stream dependency.
	//
	struct PrioritySpec {
		//
		// The stream ID of the stream to depend on.  Specifying 0 makes
		// stream not depend any other stream.
		//
		int32_t stream_id;
		//
		// The weight of this dependency.
		//
		int32_t weight;
		//
		// nonzero means exclusive dependency
		//
		uint8_t exclusive;
	};

	struct KeyValue {
		std::string key;
		std::string value;
	};

	//
	// @struct
	//
	// The HEADERS frame.  It has the following members:
	//
	struct HeaderFrame {
		//
		// The frame header.
		//
		FrameHeader hd;
		//
		// The length of the padding in this frame.  This includes PAD_HIGH
		// and PAD_LOW.
		//
		std::size_t padlen;
		//
		// The priority specification
		//
		PrioritySpec pri_spec;
		//
		// The name/value pairs.
		//
		KeyValue nva;
		//
		// The number of name/value pairs in |nva|.
		//
		std::size_t nvlen;
		//
		// The category of this HEADERS frame.
		//
		HeaderCategory category;
	} ;

	//
	// @struct
	//
	// The PRIORITY frame.  It has the following members:
	//
	struct PriorityFrame {
		//
		// The frame header.
		//
		FrameHeader hd;
		//
		// The priority specification.
		//
		PrioritySpec pri_spec;
	};

	//
	// @struct
	//
	// The RST_STREAM frame.  It has the following members:
	//
	struct RstStreamFrame {
		//
		// The frame header.
		//
		FrameHeader hd;
		//
		// The error code.  See :type:`ETU_HTTP2_error_code`.
		//
		uint32_t error_code;
	};

	//
	// @struct
	//
	// The SETTINGS ID/Value pair.  It has the following members:
	//
	struct SettingsEntry {
		//
		// The SETTINGS ID.  See :type:`ETU_HTTP2_settings_id`.
		//
		int32_t settings_id;
		//
		// The value of this entry.
		//
		uint32_t value;
	};

	//
	// @struct
	//
	// The SETTINGS frame.  It has the following members:
	//
	struct SettingsFrame {
		typedef std::vector<SettingsEntry> SettingsEntryArray;
		//
		// The frame header.
		//
		FrameHeader hd;
		//
		// The pointer to the array of SETTINGS ID/Value pair.
		//
		SettingsEntryArray entries;
	};

	//
	// @struct
	//
	// The PUSH_PROMISE frame.  It has the following members:
	//
	struct PushPromiseFrame {
		typedef std::vector<KeyValue> KeyValueArray;
		//
		// The frame header.
		//
		FrameHeader hd;
		//
		// The length of the padding in this frame.  This includes PAD_HIGH
		// and PAD_LOW.
		//
		std::size_t padlen;

		KeyValueArray keyValue;
		//
		// The promised stream ID
		//
		int32_t promised_stream_id;
	};

	//
	// @struct
	//
	// The PING frame.  It has the following members:
	//
	struct PingFrame {
		//
		// The frame header.
		//
		FrameHeader hd;
		//
		// The opaque data
		//
		std::uint8_t opaque_data[8];
	};

	//
	// @struct
	//
	// The GOAWAY frame.  It has the following members:
	//
	struct GoAwayFrame {
		//
		// The frame header.
		//
		FrameHeader hd;
		//
		// The last stream stream ID.
		//
		int32_t last_stream_id;
		//
		// The error code.  See :type:`ETU_HTTP2_error_code`.
		//
		uint32_t error_code;
		//
		// The additional debug data
		//
		uint8_t* opaque_data;
		//
		// The length of |opaque_data| member.
		//
		std::size_t opaque_data_len;
		//
		// Reserved bit.  Currently this is always set to 0 and application
		// should not expect something useful in here.
		//
		uint8_t reserved;
	};

	//
	// @struct
	//
	// The WINDOW_UPDATE frame.  It has the following members:
	//
	typedef struct WindowUpdateFrame {
		//
		// The frame header.
		//
		FrameHeader hd;
		//
		// The window size increment.
		//
		int32_t window_size_increment;
		//
		// Reserved bit.  Currently this is always set to 0 and application
		// should not expect something useful in here.
		//
		uint8_t reserved;
	};

	//
	// @struct
	//
	// The extension frame.  It has following members:
	//
	typedef struct ExtensionFrame {
		//
		// The frame header.
		//
		FrameHeader hd;
		//
		// The pointer to extension payload.  The exact pointer type is
		// determined by hd.type.
		//
		// Currently, no extension is supported.  This is a place holder for
		// the future extensions.
		//
		void* payload;
	};

	//
	// @union
	//
	// This union includes all frames to pass them to various function
	// calls as ETU_HTTP2_frame type.  The CONTINUATION frame is omitted
	// from here because the library deals with it internally.
	//
	union Frame {
		//
		// The frame header, which is convenient to inspect frame header.
		//
		FrameHeader hd;
		//
		// The DATA frame.
		//
		DataFrame data;
		//
		// The HEADERS frame.
		//
		HeaderFrame headers;
		//
		// The PRIORITY frame.
		//
		PriorityFrame priority;
		//
		// The RST_STREAM frame.
		//
		RstStreamFrame rst_stream;
		//
		// The SETTINGS frame.
		//
		SettingsFrame settings;
		//
		// The PUSH_PROMISE frame.
		//
		PushPromiseFrame push_promise;
		//
		// The PING frame.
		//
		PingFrame ping;
		//
		// The GOAWAY frame.
		//
		GoAwayFrame goaway;
		//
		// The WINDOW_UPDATE frame.
		//
		WindowUpdateFrame window_update;
		//
		// The extension frame.
		//
		ExtensionFrame ext;
	};
}
}
