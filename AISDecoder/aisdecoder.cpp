#include "aisdecoder.h"

#include <iostream>
#include <cassert>

#include "ais_message.h"

using namespace std;

enum RECEIVE_STATE {
	INITIAL,
	RECEIVING_DATA,
	RECEIVING_CRC
};

// <Utills
static const int8_t hex_values[256] = {
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
	-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};


static inline bool extract_crc16(string_view str, size_t offset, unsigned* result)
// There is no length check because we already did it in the check_NMEA_CRC function
{
	int hi = hex_values[str[offset]];
	int lo = hex_values[str[offset + 1]];

	if (hi == -1 || lo == -1)
		return false;
	*result = hi << 4 | lo;
	return  true;
}

static bool check_NMEA_CRC(string_view sentence)
{
	size_t crc_start = sentence.find_last_of('*');
	if (crc_start == std::string::npos || crc_start > sentence.size() - 3)
		return false; // * not found, or it didn't have 2 characters following it.

	unsigned extracted_crc;
	if (!extract_crc16(sentence, crc_start + 1, &extracted_crc))
		return false;

	unsigned calculated_checksum = 0;
	int i = 1; // exclude the first NMEA symbol from crc calculation
	while (i < crc_start)
		calculated_checksum ^= sentence[i++];

	return calculated_checksum == extracted_crc;
}

// Returns a substring between two symbol enterance. For example for string "122,456,want_this,101112"
// if we call substr_between_nth("122,456,want_this,101112",',',1,2) it will return us a "want_this" string
static string_view substr_between_nth(string_view str, char symbol, int nth_from, int nth_to)
{
	size_t  pos_from = 0;
	int     cnt = 0;

	while (cnt != nth_from)
	{
		pos_from = str.find(symbol, ++pos_from);
		if (pos_from == std::string::npos)
			return "";
		++cnt;
	}
	size_t  pos_to = pos_from;
	++pos_from;
	while (cnt != nth_to)
	{
		pos_to = str.find(symbol, ++pos_to);
		if (pos_to == std::string::npos)
			return "";
		++cnt;
	}

	return str.substr(pos_from, pos_to - pos_from);
}
// Utills/>

namespace aislib {
	void AISDecoder::processChunk(const char* data, std::size_t size) {
		if (!size) return;
		const char* byte = data;
		for (std::size_t i = 0; i < size; ++i, ++byte) {
			switch (state)
			{
			case INITIAL:
				if (*byte == '!') {
					state = RECEIVING_DATA;
					m_buffer.clear();
				}
				break;
			case RECEIVING_DATA:
				if (*byte == '*') {
					if (state == RECEIVING_DATA) {
						state = RECEIVING_CRC;
						crc_left = 3;
					}
				}
				break;
			case RECEIVING_CRC:
				--crc_left;
				if (crc_left == 0) {

					if (parseSentence(m_buffer) == AIS_RESULT::AIS_PARSED) {

						if (m_callback) {
							m_callback(this);
						}

					}

					state = INITIAL;
				}
				break;
			default:
				break;
			}

			if (state != INITIAL) {
				m_buffer.push_back(*byte);
			}
		}

	}


	void AISDecoder::setOptions(bool check_crc, bool parse_meta) {
		m_check_crc = check_crc;
		m_parse_meta = parse_meta;
	}

	AIS_RESULT AISDecoder::parseSentence(string_view sentence) {

		// Example of AIVDx sentence.
		// Note: certain fields might be empty
		//!AIVDM,2,1,3,B,55P5TL01VIaAL@7WKO@mBplU@<PDhh000000001S;AJ::4A80?4i@E53,0*3E
		if (sentence.size() > 100 || sentence.size() < 20)
			return AIS_ERR_BAD_SIZE;

		if (m_check_crc) {
			if (!check_NMEA_CRC(sentence))
				return AIS_ERR_BAD_CRC;
		}

		if (sentence.find("VD", 3, 2) != std::string::npos) {

			string_view payload = substr_between_nth(sentence, ',', 5, 6);

			if (m_parse_meta) {
				m_talker = sentence.substr(1, 2);
				is_ownship = sentence[5] == 'O';
				string_view channel = substr_between_nth(sentence, ',', 4, 5);
				if (!channel.empty())
					m_channel = channel[0];
			}

			uint8_t total_parts = sentence[7];
			if (total_parts == '1') {
				decode(payload);
				return AIS_PARSED;
			}
			else { //multipart message
				uint8_t current_part = sentence[9];
				uint8_t seq_id = sentence[11];
				if (current_part == '1') {
					m_accumulator = payload;
					m_last_seq_id = seq_id;
					m_last_part = current_part;
					return AIS_MULTIPART_PENDING;
				}
				else {
					if (current_part - m_last_part != 1 || m_last_seq_id != seq_id)
						return AIS_ERR_MULTIPART_BAD_SEQUENCE;

					m_accumulator += payload;
					// last sentence
					if (current_part == total_parts) {
						decode(m_accumulator);
						return AIS_PARSED;
					}
					m_last_part = current_part;
					return AIS_MULTIPART_PENDING;
				}

			}

		}

		return AIS_ERR_UNKNOWN_FORMAT;
	}
}


