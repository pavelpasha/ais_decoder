#pragma once
#include <string>
#include <vector>

#define AIS_MAX_PAYLOAD_BYTES (5 * 82) // AIS Spec allows up to 5 sentences per message, 82 bytes each    
namespace aislib {
	class AISBitstring
	{
	public:
		AISBitstring() {}
		AISBitstring(const AISBitstring&) = delete;
		AISBitstring& operator= (const AISBitstring&) = delete;

		bool decode(std::string_view ais_char_bytes);
		inline unsigned size() const { return m_size; };
		inline unsigned bit_length() const { return m_bit_length; };
		inline const uint8_t* data() const { return m_payload; };

		int32_t get_int(unsigned start, unsigned bit_len, bool is_signed = false) const;
		bool get_flag(unsigned offset) const;
		std::string get_string(unsigned start, unsigned bit_len) const;
		std::vector<uint8_t> get_raw_bytes(unsigned start, unsigned bit_len) const;
		inline float get_coord(unsigned start, unsigned bit_len) const { return get_int(start, bit_len, true) * _coord_mult; }
	protected:
		unsigned m_bit_length = 0; // size of usefull payload in bits(not bytes)
		unsigned m_size = 0; // size of usefull payload in bytes(not bits)
		uint8_t m_payload[AIS_MAX_PAYLOAD_BYTES];

	private:
		static constexpr float _coord_mult = 1.f / 600000.f;
	};
}

