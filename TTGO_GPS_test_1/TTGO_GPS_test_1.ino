//More information at: https://www.aeq-web.com/

#define SERIAL1_RX 34 // GPS_TX -> 34
#define SERIAL1_TX 12 // 12 -> GPS_RX
String read_sentence;

void setup() {
  Serial.begin(115200);
  Serial.println("TTGO GPS TEST");
  delay(2000);
  Serial1.begin(9600, SERIAL_8N1, SERIAL1_RX, SERIAL1_TX);
}

void loop() {
  read_sentence = Serial1.readStringUntil(13); //13 = return (ASCII)
  read_sentence.trim();

  if (read_sentence.startsWith("$GPGGA")) {
    String gps_lat = sentence_sep(read_sentence, 2); //Latitude in degrees & minutes
    String gps_lon = sentence_sep(read_sentence, 4); //Longitude in degrees & minutes
    String gps_sat = sentence_sep(read_sentence, 7);
    String gps_hgt = sentence_sep(read_sentence, 9);
    String gps_lat_o = sentence_sep(read_sentence, 3);  //Orientation (N or S)
    String gps_lon_o = sentence_sep(read_sentence, 5); //Orientation (E or W)

    Serial.print("LAT: ");
    Serial.print(gps_lat);
    Serial.print(" LON: ");
    Serial.print(gps_lon);
    Serial.print(" HEIGHT: ");
    Serial.print(gps_hgt);
    Serial.print(" SAT: ");
    Serial.println(gps_sat);

    float latitude = convert_gps_coord(gps_lat.toFloat(), gps_lat_o);
    float longitude = convert_gps_coord(gps_lon.toFloat(), gps_lon_o);

    Serial.print(latitude, 6);
    Serial.print(",");
    Serial.println(longitude, 6);
  }
}

String sentence_sep(String input, int index) {
  int finder = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = input.length() - 1;

  for (int i = 0; i <= maxIndex && finder <= index; i++) {
    if (input.charAt(i) == ',' || i == maxIndex) {  //',' = separator
      finder++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return finder > index ? input.substring(strIndex[0], strIndex[1]) : "";
}

float convert_gps_coord(float deg_min, String orientation) {
  double gps_min = fmod((double)deg_min, 100.0);
  int gps_deg = deg_min / 100;
  double dec_deg = gps_deg + ( gps_min / 60 );
  if (orientation == "W" || orientation == "S") {
    dec_deg = 0 - dec_deg;
  }
  return dec_deg;
}
