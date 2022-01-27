#ifndef AISDECODER_H
#define AISDECODER_H
#include <string>
#include <functional>

#include "ais_bitstring.h"
namespace aislib {
	enum AIS_RESULT {
		AIS_PARSED,
		AIS_MULTIPART_PENDING,
		AIS_ERR_BAD_SIZE,
		AIS_ERR_BAD_CRC,
		AIS_ERR_MULTIPART_BAD_SEQUENCE,
		AIS_ERR_UNKNOWN_FORMAT
	};

	class AISDecoder
	{
	public:
		AISDecoder(bool check_crc = true, bool parse_meta = false) : m_check_crc(check_crc), m_parse_meta(parse_meta), m_talker(2, '\0') {}
		AIS_RESULT parseSentence(std::string_view sentence);
		// Process a data chunk from a file or socket stream. For each founded and decoded ais nmea 
		// sencence will be called a callback function if it exists. 
		void processChunk(const char* data, std::size_t size);
		inline void processChunk(std::string_view chunk) { processChunk(chunk.data(), chunk.size()); }
		inline bool decode(std::string_view ais_payload) { return m_bitstring.decode(ais_payload); }
		void setOptions(bool check_crc, bool parse_meta);
		inline void setCallback(const std::function<void(AISDecoder*)>& callback) { m_callback = callback; }
		inline const AISBitstring& get_bitstring() const { return m_bitstring; }
		// Returns true if last decoded message was "AIVDO" - i.e. ownship (if parse_meta enabled)
		inline bool isOwnship() const { return is_ownship; }
		// Returns talker id of last decoded message, for example "AI" (if parse_meta enabled)
		inline const std::string getTalker() const { return m_talker; }
		// Returns channel of last decoded message, for example "A" (if parse_meta enabled)
		inline char getChannel() const { return m_channel; }
	private:
		AISBitstring m_bitstring;
		uint8_t m_last_part;
		uint8_t m_last_seq_id;
		bool m_check_crc;
		bool m_parse_meta;
		std::string m_accumulator; // Multipart sentence accumulator
		// sentence meta info
		bool is_ownship;
		std::string m_talker;
		char m_channel;
		// stream parsing related fields
		std::string m_buffer;
		int state = 0;
		int crc_left;
		std::function<void(AISDecoder*)> m_callback = nullptr;
	};

}
#endif // AISDECODER_H
