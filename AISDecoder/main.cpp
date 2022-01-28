// AISDecoder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <cassert>
#include <chrono>

#include "ais_message.h"
#include "aisdecoder.h"

using namespace aislib;
using namespace std;
void test() {

	AISDecoder dec;
	// Bad crc
	auto result = dec.parseSentence("!AIVDM,2,1,3,B,55P5TL01VIaAL@7WKO@mBplU@<PDhh000000001S;AJ::4A80?4i@E53,0*3F");
	assert(result == AIS_ERR_BAD_CRC);

	// Wrong multipart sequence
	result = dec.parseSentence("!AIVDM,2,1,0,B,539S:k40000000c3G04PPh63<00000000080000o1PVG2uGD:00000000000,0*34");
	assert(result == AIS_MULTIPART_PENDING);
	result = dec.parseSentence("!AIVDM,2,2,2,B,00000000000,2*25");
	assert(result == AIS_ERR_MULTIPART_BAD_SEQUENCE);

	// OK
	result = dec.parseSentence("!AIVDM,2,2,0,B,00000000000,2*27");
	assert(result == AIS_PARSED);

	const AISBitstring& message = dec.getBitstring();
	
	assert(msg_get_type(message) == 5);
	assert(msg_get_mmsi(message) == 211339980);
	assert(msg5_get_to_stern(message) == 38);
	assert(msg5_get_to_bow(message) == 12);
	assert(msg5_get_to_starboard(message) == 2);
	assert(msg5_get_to_port(message) == 23);
	assert(msg5_get_month(message) == 5);
	assert(msg5_get_day(message) == 14);
	assert(msg5_get_hour(message) == 20);
	assert(msg5_get_minute(message) == 10);
	assert(msg5_get_dte_flag(message) == false);
	assert(msg5_get_draught(message) == 0);

}



void bench() {

	using std::chrono::high_resolution_clock;
	using std::chrono::duration_cast;
	using std::chrono::duration;
	using std::chrono::milliseconds;

	auto t1 = high_resolution_clock::now();
	for (int i = 0; i < 10000000; ++i)
		test();

	auto t2 = high_resolution_clock::now();

	auto ms_int = duration_cast<milliseconds>(t2 - t1);

	std::cout << ms_int.count() << "ms\n";


}



static void example_simple() {

	std::string pos_report = "18UG;P0012G?Uq4EdHa=c;7@051@";
	std::string static_voyage = "539S:k40000000c3G04PPh63<00000000080000o1PVG2uGD:0000000000000000000000";
	AISBitstring bitstring;
	bitstring.decode(static_voyage);

	std::cout << "message type " << int(msg_get_type(bitstring)) << endl;
	std::cout << "message mmsi " << msg_get_mmsi(bitstring) << endl;

	AISDecoder dec;
	auto result = dec.parseSentence("!AIVDM,2,1,3,B,55P5TL01VIaAL@7WKO@mBplU@<PDhh000000001S;AJ::4A80?4i@E53,0*3E");

	switch (msg_get_type(bitstring))
	{
	case 1:
	case 2:
	case 3:
		std::cout << "message latitude " << msg123_get_latitude(bitstring) << endl;
		std::cout << "message longitude " << msg123_get_longitude(bitstring) << endl;
		std::cout << "message cog " << msg123_get_cog(bitstring) << endl;
		std::cout << "message sog " << msg123_get_sog(bitstring) << endl;
		std::cout << "message rot " << msg123_get_rot(bitstring) << endl;
		break;

	case 5:
		std::cout << "message callsign " << msg5_get_callsign(bitstring) << endl;
		std::cout << "message shipname " << msg5_get_shipname(bitstring) << endl;
		std::cout << "message to stern " << msg5_get_to_stern(bitstring) << endl;
		std::cout << "message to bow " << msg5_get_to_bow(bitstring) << endl;
		std::cout << "message to port " << msg5_get_to_port(bitstring) << endl;
		std::cout << "message to sb " << msg5_get_to_starboard(bitstring) << endl;
		std::cout << "message month " << msg5_get_month(bitstring) << endl;
		std::cout << "message day " << msg5_get_day(bitstring) << endl;
		std::cout << "message hour " << msg5_get_hour(bitstring) << endl;
		std::cout << "message minute " << msg5_get_minute(bitstring) << endl;
		std::cout << "message dte " << msg5_get_dte_flag(bitstring) << endl;
		break;
	default:
		std::cout << "unsupported message type \n";
		break;
	}

}


static void decode_file(const string& path) {

	AISDecoder dec;
	int counter = 0;
	dec.setCallback([&counter](AISDecoder* decoder) {
	
		auto& bitstring = decoder->getBitstring();
		std::cout << "message type " << int(msg_get_type(bitstring)) << endl;
		std::cout << "message mmsi " << msg_get_mmsi(bitstring) << endl;
		++counter;
	
	});

	std::ifstream fin(path);
	std::string buffer(1024, 0); //reads only the first 1024 bytes


	while (!fin.eof()) {
		fin.read(buffer.data(), buffer.size());
		dec.processChunk(buffer.data(), fin.gcount());
	}

	cout << "messages sucessfuly decoded " << counter << endl;

}

int main()
{
	
	test();

	example_simple();

	decode_file("C:\\Users\\virtualBox\\Desktop\\Work\\rpl_player\\experiment_11_13_2020.rpl");

	return 0;
	
}


