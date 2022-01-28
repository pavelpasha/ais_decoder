# Another AIS sentence decoder implementation with C++ 17.

I tried to make it simple, stupid and perfomance as possible. The main idea - not pay for what you don't use.

There are no data classes, representing each message type. But only bitstring object and set of static functions,
wich gives you acces for certain field of certain message type.
You get only fields you need, and not even wasting CPU time and memory to parse the others.

# Usage example
NMEA sentence parsing
```
AISDecoder decoder;
auto result = decoder.parseSentence("AIVDM,1,1,,A,15MgK45P3@G?fl0E`JbR0OwT0@MS,0*4E");
if(result != AIS_PARSED) return;
auto&  bitstring = decoder->getBitstring();
std::cout << "message type " << int(msg_get_type(bitstring)) << endl;
std::cout << "message mmsi " << msg_get_mmsi(bitstring) << endl;
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
```
Parsing from stream-like source (file or socket) chunk by chunk
```
AISDecoder decoder;
int counter = 0;
decoder.setCallback([&counter](AISDecoder* dec) {

  auto& bitstring = dec->getBitstring();
  std::cout << "message type " << int(msg_get_type(bitstring)) << endl;
  std::cout << "message mmsi " << msg_get_mmsi(bitstring) << endl;
  ++counter;

});

std::ifstream fin(path);
std::string buffer(1024, 0); 

while (!fin.eof()) {
  fin.read(buffer.data(), buffer.size());
  dec.processChunk(buffer.data(), fin.gcount());
}

cout << "messages sucessfuly decoded " << counter << endl;
```

