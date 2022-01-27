/*
 * Copyright (c) 2022 Pavel Saenko <pasha03.92@mail.ru>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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

