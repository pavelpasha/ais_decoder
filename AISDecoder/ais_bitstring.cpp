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

#include "ais_bitstring.h"

#include <cassert>
#include <algorithm>

#define AIS_CHAR_BIT 6

#ifdef USE_SIMD
#include <immintrin.h>
__m128i a = _mm_set1_epi8('0');
__m128i b = _mm_set1_epi8(40);
__m128i c = _mm_set1_epi8(8);
#endif
namespace aislib {
	bool AISBitstring::decode(std::string_view ais_char_bytes) {

		assert(ais_char_bytes.size() <= AIS_MAX_PAYLOAD_BYTES);
		if (ais_char_bytes.size() > AIS_MAX_PAYLOAD_BYTES) return false;

		m_size = ais_char_bytes.size();
#ifdef USE_SIMD
		for (int i = 0; i < ais_char_bytes.size(); i += 16) {

			__m128i data = _mm_loadu_si128((__m128i*)(ais_char_bytes.data() + i));
			auto sub = _mm_subs_epi8(data, a); // substract '0'
			auto mask = _mm_min_epu8(c, _mm_cmpgt_epi8(sub, b)); // create a mask, consists of 0 and 8
			auto result = _mm_subs_epi8(sub, mask); // then substract this mask
			_mm_store_si128((__m128i*)(m_payload + i), result); // store result to buffer

		}
#else
		for (int i = 0; i < m_size; ++i) {

			m_payload[i] = ais_char_bytes[i] - '0';
			if (m_payload[i] > 40)
				m_payload[i] -= 8;

		}
#endif

		m_bit_length = m_size * 6;
		return true;
	}

	int32_t AISBitstring::get_int(unsigned start, unsigned bit_len, bool is_signed) const
	{
		assert(start + bit_len < m_bit_length);
		if (!(start + bit_len < m_bit_length)) return 0;
		int32_t result = 0;
		int byte, bit;
		for (int i = start; i < start + bit_len; ++i)
		{
			result = result << 1;
			byte = m_payload[i / AIS_CHAR_BIT];
			bit = (byte >> (5 - (i % AIS_CHAR_BIT))) & 1;
			if (i == start && is_signed && bit)
				result = ~result;
			result |= bit;
		}

		return result;
	}

	bool AISBitstring::get_flag(unsigned offset) const {
		assert(offset + 1 < m_bit_length);
		if (!(offset + 1 < m_bit_length)) return false;
		auto byte = m_payload[offset / AIS_CHAR_BIT];
		return (byte >> (5 - (offset % AIS_CHAR_BIT))) & 1;
	}

	std::string AISBitstring::get_string(unsigned start, unsigned bit_len) const {

		int nchars = bit_len / AIS_CHAR_BIT;
		static const char sixchr[] =
			"\0ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_ !\"#$%&'()*+,-./0123456789:;<=>?";

		std::string result(nchars, '\0');
		int nchar = 0;
		int bit_idx = start;
		int max_bit = std::min(bit_len + start, m_bit_length);
		while (bit_idx < max_bit && nchar < nchars)
		{
			int byte = get_int(bit_idx, AIS_CHAR_BIT);
			if (!byte) break;

			bit_idx += AIS_CHAR_BIT;
			result[nchar++] = sixchr[byte];
		}

		return result;
	}

	std::vector<uint8_t> AISBitstring::get_raw_bytes(unsigned start_bit, unsigned bit_len) const {
		std::vector<uint8_t> buffer(bit_len*AIS_CHAR_BIT);
		
		// TODO: realize how to implement it
		int start_byte_idx = start_bit / AIS_CHAR_BIT;

		printf("Get raw bytes not implemented yet\n");
		return buffer;
	}
}