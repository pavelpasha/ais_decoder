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
#include "ais_bitstring.h"
namespace aislib {
	enum {
		AIS_MSG_1_POISITION_REPORT_CLASSA = 1,
		AIS_MSG_2_POISITION_REPORT_CLASSA,
		AIS_MSG_3_POISITION_REPORT_CLASSA,
		AIS_MSG_4_BASE_STATION_REPORT,
		AIS_MSG_5_STATIC_VOYAGE,
		AIS_MSG_6_BINARY_ADRESSED_MESSAGE,
		AIS_MSG_7_BINARY_ACKNOWLEDGE,
		AIS_MSG_8_BINARY_BROADCAST_MESSAGE,
		AIS_MSG_9_AIRCRAFT_POSITION_REPORT,
		AIS_MSG_10_UTC_DATE_INQUIRY,
		AIS_MSG_11_UTC_DATE_RESPONCE,
		AIS_MSG_12_ADRESSED_SAFETY_RELATED_MESSAGE,
		AIS_MSG_13_SAFETY_RELATED_ACKNOWLEDGE,
		AIS_MSG_14_SAFETY_RELATED_BROADCAST_MESSAGE,
		AIS_MSG_15_INTERROGATION,
		AIS_MSG_16_ASSIGNMENT_MODE_COMMAND,
		AIS_MSG_17_DGNSS_BINARY_BROADCAST_MESSAGE,
		AIS_MSG_18_STANDARD_POISITION_REPORT_CLASSB,
		AIS_MSG_19_EXTENDED_POISITION_REPORT_CLASSB,
		AIS_MSG_20_DATA_LINK_MANAGEMENT,
		AIS_MSG_21_AID_TO_NAVIGATION_REPORT,
		AIS_MSG_22_CHANNEL_MANAGEMENT,
		AIS_MSG_23_GROUP_ASSIGNMENT_COMMAND,
		AIS_MSG_24_STATIC_DATA_REPORT,
		AIS_MSG_25_SINGLE_SLOT_BINARY_MESSAGE,
		AIS_MSG_26_MULTIPLE_SLOT_BINARY_MESSAGE,
		AIS_MSG_27_POSITION_REPORT_LONG_RANGE
	};


	// Common for all message types
	static inline int msg_get_mmsi(const AISBitstring& bs) { return bs.get_int(8, 30); }
	static inline int msg_get_repeat(const AISBitstring& bs) { return bs.get_int(6, 2); }
	static inline char msg_get_type(const AISBitstring& bs) { return bs.data()[0]; }

	// TYPES 1, 2 and 3 Position Report Class A
	static inline int msg123_get_nav_status(const AISBitstring& bs) { return bs.get_int(38, 4); }
	static inline int msg123_get_rot(const AISBitstring& bs) { return bs.get_int(42, 8, true); }
	static inline float msg123_get_sog(const AISBitstring& bs) { return bs.get_int(50, 10)*0.1f; }
	static inline bool msg123_get_pos_accuracy(const AISBitstring& bs) { return bs.get_flag(60); }
	static inline float msg123_get_longitude(const AISBitstring& bs) { return bs.get_coord(61, 28); }
	static inline float msg123_get_latitude(const AISBitstring& bs) { return bs.get_coord(89, 27); }
	static inline int msg123_get_cog(const AISBitstring& bs) { return bs.get_int(116, 12)*0.1f; }
	static inline int msg123_get_hdg(const AISBitstring& bs) { return bs.get_int(128, 9); }
	static inline int msg123_get_timestamp(const AISBitstring& bs) { return bs.get_int(137, 6); }
	static inline int msg123_get_maneuver_indicator(const AISBitstring& bs) { return bs.get_int(143, 2); }
	static inline int msg123_get_raim_flag(const AISBitstring& bs) { return bs.get_flag(148); }
	static inline int msg123_get_radio(const AISBitstring& bs) { return bs.get_int(149, 19); }
	static inline int msg123_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 168; }

	// TYPE 4 Base Station Report
	static inline int msg4_get_year(const AISBitstring& bs) { return bs.get_int(38, 14); }
	static inline int msg4_get_month(const AISBitstring& bs) { return bs.get_int(52, 4); }
	static inline int msg4_get_day(const AISBitstring& bs) { return bs.get_int(56, 5); }
	static inline int msg4_get_hour(const AISBitstring& bs) { return bs.get_int(61, 5); }
	static inline int msg4_get_minute(const AISBitstring& bs) { return bs.get_int(66, 6); }
	static inline int msg4_get_second(const AISBitstring& bs) { return bs.get_int(72, 6); }
	static inline int msg4_get_fix_quality(const AISBitstring& bs) { return bs.get_flag(78); }
	static inline float msg4_get_longitude(const AISBitstring& bs) { return bs.get_coord(79, 28); }
	static inline float msg4_get_latitude(const AISBitstring& bs) { return bs.get_coord(107, 27); }
	static inline int msg4_get_epfd(const AISBitstring& bs) { return bs.get_int(134, 4); }
	static inline int msg4_get_raim_flag(const AISBitstring& bs) { return bs.get_flag(148); }
	static inline int msg4_get_radio(const AISBitstring& bs) { return bs.get_int(149, 19); }
	static inline int msg4_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 168; }

	// Type 5: Static and Voyage Related Data
	static inline int msg5_get_ais_verison(const AISBitstring& bs) { return bs.get_int(38, 2); }
	static inline int msg5_get_imo_number(const AISBitstring& bs) { return bs.get_int(40, 30); }
	static inline std::string msg5_get_callsign(const AISBitstring& bs) { return bs.get_string(70, 42); }
	static inline std::string msg5_get_shipname(const AISBitstring& bs) { return bs.get_string(112, 120); }
	static inline int msg5_get_ship_type(const AISBitstring& bs) { return bs.get_int(232, 8); }
	static inline int msg5_get_to_bow(const AISBitstring& bs) { return bs.get_int(240, 9); }
	static inline int msg5_get_to_stern(const AISBitstring& bs) { return bs.get_int(249, 9); }
	static inline int msg5_get_to_port(const AISBitstring& bs) { return bs.get_int(258, 6); }
	static inline int msg5_get_to_starboard(const AISBitstring& bs) { return bs.get_int(264, 6); }
	static inline int msg5_get_epfd(const AISBitstring& bs) { return bs.get_int(270, 4); }
	static inline int msg5_get_month(const AISBitstring& bs) { return bs.get_int(274, 4); }
	static inline int msg5_get_day(const AISBitstring& bs) { return bs.get_int(278, 5); }
	static inline int msg5_get_hour(const AISBitstring& bs) { return bs.get_int(283, 5); }
	static inline int msg5_get_minute(const AISBitstring& bs) { return bs.get_int(288, 6); }
	static inline int msg5_get_draught(const AISBitstring& bs) { return bs.get_int(294, 8); }
	static inline std::string msg5_get_destination(const AISBitstring& bs) { return bs.get_string(302, 120); }
	static inline bool msg5_get_dte_flag(const AISBitstring& bs) { return bs.get_flag(422); }
	static inline int msg5_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 424; }

	// Type 6: Binary Addressed Message
	static inline int msg6_get_seqno(const AISBitstring& bs) { return bs.get_int(38, 2); }
	static inline int msg6_get_dest_mmsi(const AISBitstring& bs) { return bs.get_int(40, 30); }
	static inline bool msg6_get_retransmit_flag(const AISBitstring& bs) { return bs.get_flag(70); }
	static inline int msg6_get_dac(const AISBitstring& bs) { return bs.get_int(72, 10); }
	static inline int msg6_get_fid(const AISBitstring& bs) { return bs.get_int(82, 6); }
	static inline int msg6_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 88; } // minimum size

	// Type 7: Binary Acknowledge 
	// NOTE! (Total length varies between 72 and 168 bits by 32-bit increments, depending on the number of destination MMSIs included.)
	// So test a bitstring size before retrieving values
	static inline int msg7_get_mmsi1(const AISBitstring& bs) { return bs.get_int(40, 30); }
	static inline int msg7_get_mmsiseq1(const AISBitstring& bs) { return bs.get_int(70, 2); }
	static inline int msg7_get_mmsi2(const AISBitstring& bs) { return bs.get_int(72, 30); }
	static inline int msg7_get_mmsiseq2(const AISBitstring& bs) { return bs.get_int(102, 2); }
	static inline int msg7_get_mmsi3(const AISBitstring& bs) { return bs.get_int(104, 30); }
	static inline int msg7_get_mmsiseq3(const AISBitstring& bs) { return bs.get_int(134, 2); }
	static inline int msg7_get_mmsi4(const AISBitstring& bs) { return bs.get_int(136, 30); }
	static inline int msg7_get_mmsiseq4(const AISBitstring& bs) { return bs.get_int(166, 2); }
	static inline int msg7_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 72; } // minimum size

	// Type 8: Binary Broadcast Message
	static inline int msg8_get_dac(const AISBitstring& bs) { return bs.get_int(40, 10); }
	static inline int msg8_get_fid(const AISBitstring& bs) { return bs.get_int(50, 6); }
	static inline int msg8_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 56; } // minimum size

	// TYPE 9 Standard SAR Aircraft Position Report
	static inline int msg9_get_altitude(const AISBitstring& bs) { return bs.get_int(38, 12); }
	static inline int msg9_get_sog(const AISBitstring& bs) { return bs.get_int(50, 10); }
	static inline int msg9_get_pos_accuracy(const AISBitstring& bs) { return bs.get_flag(60); }
	static inline float msg9_get_longitude(const AISBitstring& bs) { return bs.get_coord(61, 28); }
	static inline float msg9_get_latitude(const AISBitstring& bs) { return bs.get_coord(89, 27); }
	static inline int msg9_get_cog(const AISBitstring& bs) { return bs.get_int(116, 12)*0.1f; }
	static inline int msg9_get_timestamp(const AISBitstring& bs) { return bs.get_int(128, 6); }
	static inline int msg9_get_dte_flag(const AISBitstring& bs) { return bs.get_flag(142); }
	static inline bool msg9_get_assigned_flag(const AISBitstring& bs) { return bs.get_flag(146); }
	static inline bool msg9_get_raim_flag(const AISBitstring& bs) { return bs.get_flag(147); }
	static inline int msg9_get_radio(const AISBitstring& bs) { return bs.get_int(148, 20); }
	static inline int msg9_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 168; }

	// Type 10: UTC / Date Inquiry 
	static inline int msg10_get_dest_mmsi(const AISBitstring& bs) { return bs.get_int(40, 30); }
	static inline int msg10_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 72; }

	// Type 11: UTC / Date Response
	// Identical to message 4.

	// Type 12: Addressed Safety - Related Message
	static inline int msg12_get_seqno(const AISBitstring& bs) { return bs.get_int(38, 2); }
	static inline int msg12_get_dest_mmsi(const AISBitstring& bs) { return bs.get_int(40, 30); }
	static inline bool msg12_get_retransmit_flag(const AISBitstring& bs) { return bs.get_flag(70); }
	static inline std::string msg12_get_text(const AISBitstring& bs) { return bs.get_string(72, 936); }
	static inline int msg12_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 72; } // minimum size

	// Type 13: Safety-Related Acknowledgment
	// Identical to message 7

	// Type 14: Safety-Related Broadcast Message
	static inline std::string msg14_get_text(const AISBitstring& bs) { return bs.get_string(40, 968); }
	static inline int msg14_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 40; } // minimum size

	// Type 15: Interrogation
	static inline int msg15_get_mmsi1(const AISBitstring& bs) { return bs.get_int(40, 30); }
	static inline int msg15_get_type1_1(const AISBitstring& bs) { return bs.get_int(70, 6); }
	static inline int msg15_get_offset1_1(const AISBitstring& bs) { return bs.get_int(76, 12); }
	static inline int msg15_get_type1_2(const AISBitstring& bs) { return bs.get_int(90, 6); }
	static inline int msg15_get_offset1_2(const AISBitstring& bs) { return bs.get_int(96, 12); }
	static inline int msg15_get_mmsi2(const AISBitstring& bs) { return bs.get_int(110, 30); }
	static inline int msg15_get_type2_1(const AISBitstring& bs) { return bs.get_int(140, 6); }
	static inline int msg15_get_offset2_1(const AISBitstring& bs) { return bs.get_int(146, 12); }
	static inline int msg15_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 88; } // 88-160 bits depending on the number of queries.

	// Type 16: Assignment Mode Command
	// NOTE: If the message is 96 bits long, it should be interpreted as an assignment for a single station (92 bits) 
	// followed by 4 bits of padding reserved for future use
	static inline int msg16_get_mmsi1(const AISBitstring& bs) { return bs.get_int(40, 30); }
	static inline int msg16_get_offset1(const AISBitstring& bs) { return bs.get_int(70, 12); }
	static inline int msg16_get_increment1(const AISBitstring& bs) { return bs.get_int(82, 10); }
	static inline int msg16_get_mmsi2(const AISBitstring& bs) { return bs.get_int(92, 30); }
	static inline int msg16_get_offset2(const AISBitstring& bs) { return bs.get_int(122, 12); }
	static inline int msg16_get_increment2(const AISBitstring& bs) { return bs.get_int(134, 10); }
	static inline int msg16_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 96; } // 96-144  bits depending on the number of queries.

	// Type 17: DGNSS Broadcast Binary Message
	static inline float msg17_get_longitude(const AISBitstring& bs) { return bs.get_int(40, 18, true) / 600.f; } //   signed: minutes / 10
	static inline float msg17_get_latitude(const AISBitstring& bs) { return bs.get_int(58, 17, true) / 600.f; } // 	 signed: minutes / 10
	static inline std::vector<uint8_t> get_payload(const AISBitstring& bs) { return bs.get_raw_bytes(80, 736); }
	static inline int msg17_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 80; } // 80 to 816 bits

	// Type 18: Standard Class B CS Position Report
	static inline float msg18_get_sog(const AISBitstring& bs) { return bs.get_int(46, 10)*0.1f; }
	static inline bool msg18_get_pos_accuracy(const AISBitstring& bs) { return bs.get_flag(56); }
	static inline float msg18_get_longitude(const AISBitstring& bs) { return bs.get_coord(57, 28); }
	static inline float msg18_get_latitude(const AISBitstring& bs) { return bs.get_coord(85, 27); }
	static inline float msg18_get_cog(const AISBitstring& bs) { return bs.get_int(112, 12)*0.1f; }
	static inline int msg18_get_hdg(const AISBitstring& bs) { return bs.get_int(124, 9); }
	static inline int msg18_get_timestamp(const AISBitstring& bs) { return bs.get_int(133, 6); }
	static inline bool msg18_get_cs_unit_flag(const AISBitstring& bs) { return bs.get_flag(141); }
	static inline bool msg18_get_display_flag(const AISBitstring& bs) { return bs.get_flag(142); }
	static inline bool msg18_get_dsc_flag(const AISBitstring& bs) { return bs.get_flag(143); }
	static inline bool msg18_get_band_flag(const AISBitstring& bs) { return bs.get_flag(144); }
	static inline bool msg18_get_message22_flag(const AISBitstring& bs) { return bs.get_flag(145); }
	static inline bool msg18_get_assigned_flag(const AISBitstring& bs) { return bs.get_flag(146); }
	static inline bool msg18_get_raim_flag(const AISBitstring& bs) { return bs.get_flag(147); }
	static inline int msg18_get_radio(const AISBitstring& bs) { return bs.get_int(148, 20); }
	static inline int msg18_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 168; }

	// Type 19: Extended Class B Equipment Position Report
	static inline float msg19_get_sog(const AISBitstring& bs) { return bs.get_int(46, 10)*0.1f; }
	static inline bool msg19_get_pos_accuracy(const AISBitstring& bs) { return bs.get_flag(56); }
	static inline float msg19_get_longitude(const AISBitstring& bs) { return bs.get_coord(57, 28); }
	static inline float msg19_get_latitude(const AISBitstring& bs) { return bs.get_coord(85, 27); }
	static inline int msg19_get_cog(const AISBitstring& bs) { return bs.get_int(112, 12)*0.1f; }
	static inline int msg19_get_hdg(const AISBitstring& bs) { return bs.get_int(124, 9); }
	static inline int msg19_get_timestamp(const AISBitstring& bs) { return bs.get_int(133, 6); }
	static inline std::string msg19_get_shipname(const AISBitstring& bs) { return bs.get_string(143, 120); }
	static inline int msg19_get_ship_type(const AISBitstring& bs) { return bs.get_int(263, 8); }
	static inline int msg19_get_to_bow(const AISBitstring& bs) { return bs.get_int(271, 9); }
	static inline int msg19_get_to_stern(const AISBitstring& bs) { return bs.get_int(280, 9); }
	static inline int msg19_get_to_port(const AISBitstring& bs) { return bs.get_int(289, 6); }
	static inline int msg19_get_to_starboard(const AISBitstring& bs) { return bs.get_int(295, 6); }
	static inline int msg19_get_epfd(const AISBitstring& bs) { return bs.get_int(301, 4); }
	static inline bool msg19_get_raim_flag(const AISBitstring& bs) { return bs.get_flag(305); }
	static inline bool msg19_get_dte_flag(const AISBitstring& bs) { return bs.get_flag(306); }
	static inline bool msg19_get_assigned_flag(const AISBitstring& bs) { return bs.get_flag(307); }
	static inline int msg19_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 312; }

	// Type 20 Data Link Management Message
	// NOTE: Length varies from 72-160 depending on the number of slot reservations (1 to 4) in the message.
	static inline int msg20_get_offset1(const AISBitstring& bs) { return bs.get_int(40, 12); }
	static inline int msg20_get_number1(const AISBitstring& bs) { return bs.get_int(52, 4); }
	static inline int msg20_get_timeout1(const AISBitstring& bs) { return bs.get_int(56, 3); }
	static inline int msg20_get_increment1(const AISBitstring& bs) { return bs.get_int(59, 11); }
	static inline int msg20_get_offset2(const AISBitstring& bs) { return bs.get_int(70, 12); }
	static inline int msg20_get_number2(const AISBitstring& bs) { return bs.get_int(82, 4); }
	static inline int msg20_get_timeout2(const AISBitstring& bs) { return bs.get_int(86, 3); }
	static inline int msg20_get_increment2(const AISBitstring& bs) { return bs.get_int(89, 11); }
	static inline int msg20_get_offset3(const AISBitstring& bs) { return bs.get_int(100, 12); }
	static inline int msg20_get_number3(const AISBitstring& bs) { return bs.get_int(112, 4); }
	static inline int msg20_get_timeout3(const AISBitstring& bs) { return bs.get_int(116, 3); }
	static inline int msg20_get_increment3(const AISBitstring& bs) { return bs.get_int(119, 11); }
	static inline int msg20_get_offset4(const AISBitstring& bs) { return bs.get_int(130, 12); }
	static inline int msg20_get_number4(const AISBitstring& bs) { return bs.get_int(142, 4); }
	static inline int msg20_get_timeout4(const AISBitstring& bs) { return bs.get_int(146, 3); }
	static inline int msg20_get_increment4(const AISBitstring& bs) { return bs.get_int(149, 11); }
	static inline int msg20_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 72; }


	// Type 21: Aid-to-Navigation Report
	// NOTE: The message varies in length depending on the presence and size of the Name Extension field. May vary between 272 and 360 bits.
	// The name field is up to 20 characters of 6-bit ASCII. If this field is full (has no trailing @ characters) 
	// the decoder should interpret the Name Extension field later in the message (no more than 14 6-bit characters) 
	// and concatenate it to this one to obtain the full name.
	static inline int msg21_get_aid_type(const AISBitstring& bs) { return bs.get_int(38, 5); }
	static inline std::string msg21_get_name(const AISBitstring& bs) { return bs.get_string(43, 120); }
	static inline bool msg21_get_pos_accuracy(const AISBitstring& bs) { return bs.get_flag(163); }
	static inline float msg21_get_longitude(const AISBitstring& bs) { return bs.get_coord(164, 28); }
	static inline float msg21_get_latitude(const AISBitstring& bs) { return bs.get_coord(192, 27); }
	static inline int msg21_get_to_bow(const AISBitstring& bs) { return bs.get_int(219, 9); }
	static inline int msg21_get_to_stern(const AISBitstring& bs) { return bs.get_int(228, 9); }
	static inline int msg21_get_to_port(const AISBitstring& bs) { return bs.get_int(237, 6); }
	static inline int msg21_get_to_starboard(const AISBitstring& bs) { return bs.get_int(243, 6); }
	static inline int msg21_get_epfd(const AISBitstring& bs) { return bs.get_int(249, 4); }
	static inline int msg21_get_timestamp(const AISBitstring& bs) { return bs.get_int(253, 6); }
	static inline bool msg21_get_off_position_flag(const AISBitstring& bs) { return bs.get_flag(259); }
	static inline bool msg21_get_raim_flag(const AISBitstring& bs) { return bs.get_flag(268); }
	static inline bool msg21_get_virtual_aid_flag(const AISBitstring& bs) { return bs.get_flag(269); }
	static inline bool msg21_get_assigned_flag(const AISBitstring& bs) { return bs.get_flag(270); }
	static inline std::string msg21_get_name_extension(const AISBitstring& bs) { return bs.get_string(272, 88); }
	static inline int msg21_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 272; }


	// Type 22: Channel Management
	// NOTE: If the message is broadcast (addressed field is 0), the ne_lon, ne_lat, sw_lon, and sw_lat fields are the corners of a rectangular jurisdiction area
	// over which control parameters are to be set. If it is addressed (addressed field is 1),
	// the same span of data is interpreted as two 30-bit MMSIs beginning at at bit offsets 69 and 104 respectively.
	static inline int msg22_get_channel_a(const AISBitstring& bs) { return bs.get_int(40, 12); }
	static inline int msg22_get_channel_b(const AISBitstring& bs) { return bs.get_int(52, 12); }
	static inline int msg22_get_rxtx(const AISBitstring& bs) { return bs.get_int(64, 4); }
	static inline bool msg22_get_power_flag(const AISBitstring& bs) { return bs.get_flag(68); }
	static inline float msg22_get_ne_longitude(const AISBitstring& bs) { return bs.get_int(69, 18, true) / 600.f; }
	static inline float msg22_get_ne_latitude(const AISBitstring& bs) { return bs.get_int(87, 17, true) / 600.f; }
	static inline float msg22_get_sw_longitude(const AISBitstring& bs) { return bs.get_int(104, 18, true) / 600.f; }
	static inline float msg22_get_sw_latitude(const AISBitstring& bs) { return bs.get_int(122, 17, true) / 600.f; }
	static inline int msg22_get_mmsi1(const AISBitstring& bs) { return bs.get_int(69, 30); }
	static inline int msg22_get_mmsi2(const AISBitstring& bs) { return bs.get_int(104, 30); }
	static inline bool msg22_get_addresed_flag(const AISBitstring& bs) { return bs.get_flag(139); }
	static inline bool msg22_get_band_a_flag(const AISBitstring& bs) { return bs.get_flag(140); }
	static inline bool msg22_get_band_b_flag(const AISBitstring& bs) { return bs.get_flag(141); }
	static inline int msg22_get_zone_size(const AISBitstring& bs) { return bs.get_int(142, 3); }
	static inline int msg22_get_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 144; } // Actually 168, but last 30 bits are spare


	// Type 23: Group Assignment Command
	static inline float msg23_get_ne_longitude(const AISBitstring& bs) { return bs.get_int(40, 18, true) / 600.f; }
	static inline float msg23_get_ne_latitude(const AISBitstring& bs) { return bs.get_int(58, 17, true) / 600.f; }
	static inline float msg23_get_sw_longitude(const AISBitstring& bs) { return bs.get_int(75, 18, true) / 600.f; }
	static inline float msg23_get_sw_latitude(const AISBitstring& bs) { return bs.get_int(93, 17, true) / 600.f; }
	static inline int msg23_get_station_type(const AISBitstring& bs) { return bs.get_int(110, 4); }
	static inline int msg23_get_ship_type(const AISBitstring& bs) { return bs.get_int(114, 8); }
	static inline int msg23_get_rxtx(const AISBitstring& bs) { return bs.get_int(144, 2); }
	static inline int msg23_get_report_interval(const AISBitstring& bs) { return bs.get_int(146, 4); }
	static inline int msg23_get_quet_time(const AISBitstring& bs) { return bs.get_int(150, 4); }
	static inline int msg23_get_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 153; } // Actually 160, but last 6 bits are spare

	// Type 24: Static Data Report
	// NOTE: If the Part Number field is 0, the rest of the message is interpreted as a Part A; 
	// if it is 1, the rest of the message is interpreted as a Part B
	static inline int msg24_get_part_no(const AISBitstring& bs) { return bs.get_int(38, 2); }
	static inline int msg24_is_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 160; }
	// Type 24: Static Data Report part A
	static inline std::string msg24a_get_shipname(const AISBitstring& bs) { return bs.get_string(40, 120); }

	// Type 24: Static Data Report part B
	static inline int msg24b_get_ship_type(const AISBitstring& bs) { return bs.get_int(40, 8); }
	static inline std::string msg24b_get_vendor_id(const AISBitstring& bs) { return bs.get_string(48, 18); }
	static inline int msg24b_get_unit_model_code(const AISBitstring& bs) { return bs.get_int(66, 4); }
	static inline int msg24b_get_serial_number(const AISBitstring& bs) { return bs.get_int(70, 20); }
	static inline std::string msg24b_get_callsign(const AISBitstring& bs) { return bs.get_string(90, 42); }
	static inline int msg24b_get_to_bow(const AISBitstring& bs) { return bs.get_int(132, 9); }
	static inline int msg24b_get_to_stern(const AISBitstring& bs) { return bs.get_int(141, 9); }
	static inline int msg24b_get_to_port(const AISBitstring& bs) { return bs.get_int(150, 6); }
	static inline int msg24b_get_to_starboard(const AISBitstring& bs) { return bs.get_int(156, 6); }
	static inline int msg24b_get_mothership_mmsi(const AISBitstring& bs) { return bs.get_int(132, 30); }


	// Type 25: Single Slot Binary Message
	// NOTE: If the 'addressed' flag is on, 30 bits of data at offset 40 are interpreted as a destination MMSI.
	// Otherwise that field span becomes part of the message payload, with the first 16 bits used as an Application ID if the 'structured' flag is on.
	static inline bool msg25_get_destination_indicator(const AISBitstring& bs) { return bs.get_flag(38); }
	static inline bool msg25_get_binary_data_flag(const AISBitstring& bs) { return bs.get_flag(39); }
	static inline int msg25_get_dest_mmsi(const AISBitstring& bs) { return bs.get_int(40, 30); }
	static inline int msg25_get_application_id(const AISBitstring& bs) { return bs.get_int(40, 16); }
	static inline int msg25_get_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 40; }

	// Type 26: Multiple Slot Binary Message
	// Not fully implemented
	static inline bool msg26_get_destination_indicator(const AISBitstring& bs) { return bs.get_flag(38); }
	static inline bool msg26_get_binary_data_flag(const AISBitstring& bs) { return bs.get_flag(39); }
	static inline int msg26_get_dest_mmsi(const AISBitstring& bs) { return bs.get_int(40, 30); }
	static inline int msg26_get_application_id(const AISBitstring& bs) { return bs.get_int(40, 16); }
	//static inline int msg26_get_radio_status(const AISBitstring& bs)			{ return bs.get_int(40,20);}
	static inline int msg26_get_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 60; }

	// Type 27: Long Range AIS Broadcast message
	static inline bool msg27_get_pos_accuracy(const AISBitstring& bs) { return bs.get_flag(38); }
	static inline bool msg27_get_raim_flag(const AISBitstring& bs) { return bs.get_flag(39); }
	static inline int msg27_get_nav_status(const AISBitstring& bs) { return bs.get_int(40, 4); }
	static inline float msg27_get_longitude(const AISBitstring& bs) { return bs.get_int(44, 18, true) / 600.f; }		// 	minutes / 10 East positive, West negative 181000 = N / A(default)
	static inline float msg27_get_latitude(const AISBitstring& bs) { return bs.get_int(62, 17, true) / 600.f; }
	static inline float msg27_get_sog(const AISBitstring& bs) { return bs.get_int(79, 6); }					// Knots (0-62); 63 = N/A (default)
	static inline int msg27_get_cog(const AISBitstring& bs) { return bs.get_int(85, 9); }					// 0 to 359 degrees, 511 = not available.
	static inline int msg27_get_gnss_satatus(const AISBitstring& bs) { return bs.get_flag(94); }					// 0 = current GNSS position 1 = not GNSS position (default)
	static inline int msg27_get_valid_size(const AISBitstring& bs) { return bs.bit_length() >= 95; }				// Actually 96, but last bit is spare

}