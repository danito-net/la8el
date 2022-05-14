// la8el - The Phygital (Physical + Digital) Labeling System
// (c) 2022 - danito | Danny Ismarianto Ruhiyat
// www.la8el.com

// Astra DB's Build-A-Thon by DataStax and AngelHack

// Note: 
// Phygital = Physical + Digital

#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiMulti.h>
#include <WiFiUdp.h>
#include <WiFiScan.h>
#include <WiFiClient.h>
#include <WiFiSTA.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiGeneric.h>

#include <esp_crt_bundle.h>
#include <WiFiClientSecure.h>
#include <ssl_client.h>

#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

// Astra DB (Apache Cassandra database) library for ESP8266 / ESP32 microcontroller
// https://github.com/NathanBak/astra_esp8266
// Thanks a lot Nathan Bak ... greetings from Indonesia!
#include <Astra.h>

#include <SPI.h>
#include "DW1000Ranging.h"

const String long_id = "A5:71:2A:DB:DA:F0:01";
// a bit note about this unique Tag ID:
// A5:71:2A:DB = ASTRADB
// DA = DataStax + AngleHack
// A0:01 = Tag-001 
// The "F" prefix is for the tags devices and "A" for the anchors one

// I live in Cimahi, a beautiful city at western of Java island; the device has been placed there
// The western Indonesia time (WIB) is +0700 so the time offset become 7 * 60 * 60 = 25200
const String wib_tz = "+0700";
const int wib_to = 7 * 60 * 60;

// Local wireless network information
const char* ssid = "DANITO";
const char* password = "...";

// Astra DB account information
const char* astra_db_id = "...";
const char* astra_db_region = "asia-south1";
const char* astra_db_client_id = "...";
const char* astra_db_client_secret = "...";
const char* astra_db_keyspace = "la8el";
const char* tags_table = "tags";
const char* tags_log_table = "tags_log";

WiFiUDP ntp_udp;
NTPClient time_client(ntp_udp, "id.pool.ntp.org"); // Public NTP server for Indonesia

AstraClient client = AstraClient();

#define SPI_SCK 18
#define SPI_MISO 19
#define SPI_MOSI 23
#define DW_CS 4

const uint8_t PIN_RST = 27;
const uint8_t PIN_IRQ = 34;
const uint8_t PIN_SS = 4;

float calibration_range = 0.78;


void setup() {

     Serial.begin(115200);
     delay(1000);

     Serial.print(" The tag device will connect to the '");
     Serial.println(String(ssid) + "' wireless access point ... ");

     WiFi.mode(WIFI_STA);
     WiFi.begin(ssid, password);

     while (WiFi.status() != WL_CONNECTED) {
           Serial.print(" Can not connect to the '");
           Serial.println(String(ssid) + "' wireless access point! Will trying to connecting again after 1 second ... ");
           delay(1000);
     }

     String str_mac_address = WiFi.macAddress();
     Serial.print(" The ESP32 microcontroller's MAC Address: ");
     Serial.println(str_mac_address);

     String str_ip_address = WiFi.localIP().toString();
     Serial.print(" The ESP32 microcontroller's IP Address: ");
     Serial.println(str_ip_address);

     char chr_mac_address[str_mac_address.length() + 1];
     str_mac_address.toCharArray(chr_mac_address, str_mac_address.length() + 1);

     // SQUID-GEN = simple & quick UUID generator!
     // f071d8ee-f051-83c8-1c33-6abdda41488f
     // 12345678-1234-1234-1234-123456789012
     String str_uuid = String(random(0xffff), HEX) + String(random(0xffff), HEX) + "-" + 
                       String(random(0xffff), HEX) + "-" + 
                       String(random(0xffff), HEX) + "-" + 
                       String(random(0xffff), HEX) + "-" + 
                       String(random(0xffff), HEX) + String(random(0xffff), HEX) + String(random(0xffff), HEX) ;

     char chr_uuid[str_uuid.length() + 1];
     str_uuid.toCharArray(chr_uuid, str_uuid.length() + 1);

     client.logger->setLevel(LEVEL_INFO);
     client.connect(astra_db_id, astra_db_region, astra_db_client_id, astra_db_client_secret);

     time_client.begin();
     time_client.setTimeOffset(wib_to);

     SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI);
     DW1000Ranging.initCommunication(PIN_RST, PIN_SS, PIN_IRQ);

     DW1000Ranging.attachNewRange(newRange);
     DW1000Ranging.attachNewDevice(newDevice);
     DW1000Ranging.attachInactiveDevice(inactiveDevice);
     // DW1000Ranging.useRangeFilter(true); // Enable the filter to smooth the distance

     DW1000.enableDebounceClock();
     DW1000.enableLedBlinking();
     DW1000.setGPIOMode(MSGP0, LED_MODE);

     char chr_long_id[long_id.length() + 1];
     long_id.toCharArray(chr_long_id, long_id.length() + 1);

     // DW1000Ranging.startAsTag(chr_long_id, DW1000.MODE_LONGDATA_RANGE_ACCURACY);
     DW1000Ranging.startAsTag(chr_long_id, DW1000.MODE_LONGDATA_RANGE_LOWPOWER);

     String tag_short_id = String(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);

     char chr_short_id[tag_short_id.length() + 1];
     tag_short_id.toCharArray(chr_short_id, tag_short_id.length() + 1);

     String str_location = "Cimahi, West Java, Indonesia";
     char chr_location[str_location.length() + 1];
     str_location.toCharArray(chr_location, str_location.length() + 1);

     time_client.update();
     time_t epoch_time = time_client.getEpochTime();
     setTime(epoch_time);

     struct tm *ptm = gmtime ((time_t *)&epoch_time);

     int current_year = ptm->tm_year+1900;
     String str_current_year = String(current_year);
     char chr_current_year[str_current_year.length() + 1];
     str_current_year.toCharArray(chr_current_year, str_current_year.length() + 1);

     uint8_t current_month = ptm->tm_mon+1;
     String str_current_month = String(current_month);
     char chr_current_month[str_current_month.length() + 1];
     str_current_month.toCharArray(chr_current_month, str_current_month.length() + 1);
     if (current_month < 10) { str_current_month = "0" + str_current_month; }

     uint8_t current_day = ptm->tm_mday;
     String str_current_date = String(current_day);
     char chr_current_date[str_current_date.length() + 1];
     str_current_date.toCharArray(chr_current_date, str_current_date.length() + 1);
     if (current_day < 10) { str_current_date = "0" + str_current_date; } // add a leading zero for 1 digit value

     uint8_t current_hour = ptm->tm_hour;
     String str_current_hour = String(current_hour);
     char chr_current_hour[str_current_hour.length() + 1];
     str_current_hour.toCharArray(chr_current_hour, str_current_hour.length() + 1);
     if (current_hour < 10) { str_current_hour = "0" + str_current_hour; } // add a leading zero for 1 digit value

     uint8_t current_minute = ptm->tm_min;
     String str_current_minute = String(current_minute);
     char chr_current_minute[str_current_minute.length() + 1];
     str_current_minute.toCharArray(chr_current_minute, str_current_minute.length() + 1);
     if (current_minute < 10) { str_current_minute = "0" + str_current_minute; } // add a leading zero for 1 digit value

     uint8_t current_second = ptm->tm_sec;
     String str_current_second = String(current_second);
     char chr_current_second[str_current_second.length() + 1];
     str_current_second.toCharArray(chr_current_second, str_current_second.length() + 1);
     if (current_second < 10) { str_current_second = "0" + str_current_second; } // add a leading zero for 1 digit value

     String str_current_timestamp = str_current_year + "-" + 
                                    str_current_month + "-" + 
                                    str_current_date + " " + 
                                    str_current_hour + ":" + 
                                    str_current_minute + ":" + 
                                    str_current_second + 
                                    wib_tz;

     // 1970-01-01 07:00:08+0700 --> can not access NTP server

     char chr_current_timestamp[str_current_timestamp.length() + 1];
     str_current_timestamp.toCharArray(chr_current_timestamp, str_current_timestamp.length() + 1);

     String str_calibration_range = String(calibration_range);
     char chr_tag_calibration_range[str_calibration_range.length() + 1] = {' '};
     int val_int_calibration_range = (int) calibration_range;
     float val_float_calibration_range = (abs(calibration_range) - abs(val_int_calibration_range)) * 100000;
     int val_fra_calibration_range = (int)val_float_calibration_range;
     sprintf (chr_tag_calibration_range, "%d.%d", val_int_calibration_range, val_fra_calibration_range);

     float tag_latitude = -1.111111;
     String str_tag_latitude = String(tag_latitude);
     char chr_tag_latitude[str_tag_latitude.length() + 1] = {' '};
     int val_int_tag_latitude = (int) tag_latitude;
     float val_float_tag_latitude = (abs(tag_latitude) - abs(val_int_tag_latitude)) * 100000;
     int val_fra_tag_latitude = (int)val_float_tag_latitude;
     sprintf (chr_tag_latitude, "%d.%d", val_int_tag_latitude, val_fra_tag_latitude);

     float tag_longitude = 1.111111;
     String str_tag_longitude = String(tag_longitude);
     char chr_tag_longitude[str_tag_longitude.length() + 1] = {' '};
     int val_int_tag_longitude = (int) tag_longitude;
     float val_float_tag_longitude = (abs(tag_longitude) - abs(val_int_tag_longitude)) * 100000;
     int val_fra_tag_longitude = (int)val_float_tag_longitude;
     sprintf (chr_tag_longitude, "%d.%d", val_int_tag_longitude, val_fra_tag_longitude);

     float tag_altitude = 1000;
     String str_tag_altitude = String(tag_altitude);
     char chr_tag_altitude[str_tag_altitude.length() + 1] = {' '};
     int val_int_tag_altitude = (int) tag_altitude;
     float val_float_tag_altitude = (abs(tag_altitude) - abs(val_int_tag_altitude)) * 100000;
     int val_fra_tag_altitude = (int)val_float_tag_altitude;
     sprintf (chr_tag_altitude, "%d.%d", val_int_tag_altitude, val_fra_tag_altitude);


     int tag_relative_position_x = 0;
     String str_tag_relative_position_x = String(tag_relative_position_x);
     char chr_tag_relative_position_x[str_tag_relative_position_x.length() + 1];
     str_tag_relative_position_x.toCharArray(chr_tag_relative_position_x, str_tag_relative_position_x.length() + 1);

     int tag_relative_position_y = 0;
     String str_tag_relative_position_y = String(tag_relative_position_y);
     char chr_tag_relative_position_y[str_tag_relative_position_y.length() + 1];
     str_tag_relative_position_y.toCharArray(chr_tag_relative_position_y, str_tag_relative_position_y.length() + 1);

     int tag_relative_position_z = 0;
     String str_tag_relative_position_z = String(tag_relative_position_z);
     char chr_tag_relative_position_z[str_tag_relative_position_z.length() + 1];
     str_tag_relative_position_z.toCharArray(chr_tag_relative_position_z, str_tag_relative_position_z.length() + 1);


     // Tag azimuth is device facing from the magnetic north 0° (range: 0-359°)
     int16_t tag_azimuth = 180;  // int16_t datatype is suitable for Astra DB's smallint datatype
     String str_tag_azimuth = String(tag_azimuth);
     char chr_tag_azimuth[str_tag_azimuth.length() + 1];
     str_tag_azimuth.toCharArray(chr_tag_azimuth, str_tag_azimuth.length() + 1);


     // Lets make a note of the tag
     String str_note = " ";
     // Error message can also noted here
     if (current_year < 2022) { str_note = "NTP Error"; }
     //  if (...) { str_note = str_note + "; XXX Error"; }
     char chr_note[str_note.length()];
     str_note.toCharArray(chr_note, str_note.length() + 1);


     // Lets sets status of the tag
     int8_t tag_status = 0; // int8_t datatype is suitable for Astra DB's tinyint datatype
     String str_tag_status = String(tag_status);
     char chr_tag_status[str_tag_status.length() + 1];
     str_tag_status.toCharArray(chr_tag_status, str_tag_status.length() + 1);

     struct AstraClient::KeyVal tags_log_columns[22] = { 
            { "id", chr_uuid }, 
            { "long_id", chr_long_id }, 
            { "mac_address", chr_mac_address }, 
            { "year", chr_current_year }, 
            { "month", chr_current_month }, 
            { "date", chr_current_date }, 
            { "hour", chr_current_hour }, 
            { "minute", chr_current_minute }, 
            { "second", chr_current_second }, 
            { "short_id", chr_short_id }, 
            { "location", chr_location }, 
            { "latitude", chr_tag_latitude }, 
            { "longitude", chr_tag_longitude }, 
            { "altitude", chr_tag_altitude }, 
            { "position_x", chr_tag_relative_position_x }, 
            { "position_y", chr_tag_relative_position_y }, 
            { "position_z", chr_tag_relative_position_z }, 
            { "azimuth", chr_tag_azimuth }, 
            { "calibration", chr_tag_calibration_range }, 
            { "timestamp", chr_current_timestamp },
            { "note", chr_note }, 
            { "status", chr_tag_status }
     };

     client.addRow(astra_db_keyspace, tags_log_table, 22, tags_log_columns);

     struct AstraClient::KeyVal tags_columns[15] = { 
            { "long_id", chr_long_id }, 
            { "mac_address", chr_mac_address }, 
            { "short_id", chr_short_id }, 
            { "location", chr_location }, 
            { "latitude", chr_tag_latitude }, 
            { "longitude", chr_tag_longitude }, 
            { "altitude", chr_tag_altitude }, 
            { "position_x", chr_tag_relative_position_x }, 
            { "position_y", chr_tag_relative_position_y }, 
            { "position_z", chr_tag_relative_position_z }, 
            { "azimuth", chr_tag_azimuth }, 
            { "calibration", chr_tag_calibration_range }, 
            { "timestamp", chr_current_timestamp },
            { "note", chr_note }, 
            { "status", chr_tag_status }
     };

     client.addRow(astra_db_keyspace, tags_table, 15, tags_columns);

}


void loop() {
  
     DW1000Ranging.loop();
  
}


void newRange() {

     String str_short_id = String(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);
     String str_range = String(DW1000Ranging.getDistantDevice()->getRange() * calibration_range);
     String str_power = String(DW1000Ranging.getDistantDevice()->getRXPower());

     String str_message = str_short_id + " " + str_range + " " + str_power;
     char chr_message[str_message.length() + 1];
     str_message.toCharArray(chr_message, str_message.length() + 1);

     Serial.println(str_message);

     String str_mac_address = WiFi.macAddress();

     char chr_mac_address[str_mac_address.length() + 1];
     str_mac_address.toCharArray(chr_mac_address, str_mac_address.length() + 1);

     // SQUID-GEN = simple & quick UUID generator!
     // f071d8ee-f051-83c8-1c33-6abdda41488f
     // 12345678-1234-1234-1234-123456789012
     String str_uuid = String(random(0xffff), HEX) + String(random(0xffff), HEX) + "-" + 
                       String(random(0xffff), HEX) + "-" + 
                       String(random(0xffff), HEX) + "-" + 
                       String(random(0xffff), HEX) + "-" + 
                       String(random(0xffff), HEX) + String(random(0xffff), HEX) + String(random(0xffff), HEX) ;

     char chr_uuid[str_uuid.length() + 1];
     str_uuid.toCharArray(chr_uuid, str_uuid.length() + 1);

//     client.logger->setLevel(LEVEL_INFO);
     client.logger->setLevel(LEVEL_ERROR);
     client.connect(astra_db_id, astra_db_region, astra_db_client_id, astra_db_client_secret);

     time_client.begin();
     time_client.setTimeOffset(wib_to);

     char chr_long_id[long_id.length() + 1];
     long_id.toCharArray(chr_long_id, long_id.length() + 1);

     String tag_short_id = String(DW1000Ranging.getDistantDevice()->getShortAddress(), HEX);

     char chr_short_id[tag_short_id.length() + 1];
     tag_short_id.toCharArray(chr_short_id, tag_short_id.length() + 1);

     String str_location = "Cimahi, West Java, Indonesia";
     char chr_location[str_location.length() + 1];
     str_location.toCharArray(chr_location, str_location.length() + 1);

     time_client.update();
     time_t epoch_time = time_client.getEpochTime();
     setTime(epoch_time);

     struct tm *ptm = gmtime ((time_t *)&epoch_time);

     int current_year = ptm->tm_year+1900;
     String str_current_year = String(current_year);
     char chr_current_year[str_current_year.length() + 1];
     str_current_year.toCharArray(chr_current_year, str_current_year.length() + 1);

     uint8_t current_month = ptm->tm_mon+1;
     String str_current_month = String(current_month);
     char chr_current_month[str_current_month.length() + 1];
     str_current_month.toCharArray(chr_current_month, str_current_month.length() + 1);
     if (current_month < 10) { str_current_month = "0" + str_current_month; }

     uint8_t current_day = ptm->tm_mday;
     String str_current_date = String(current_day);
     char chr_current_date[str_current_date.length() + 1];
     str_current_date.toCharArray(chr_current_date, str_current_date.length() + 1);
     if (current_day < 10) { str_current_date = "0" + str_current_date; } // add a leading zero for 1 digit value

     uint8_t current_hour = ptm->tm_hour;
     String str_current_hour = String(current_hour);
     char chr_current_hour[str_current_hour.length() + 1];
     str_current_hour.toCharArray(chr_current_hour, str_current_hour.length() + 1);
     if (current_hour < 10) { str_current_hour = "0" + str_current_hour; } // add a leading zero for 1 digit value

     uint8_t current_minute = ptm->tm_min;
     String str_current_minute = String(current_minute);
     char chr_current_minute[str_current_minute.length() + 1];
     str_current_minute.toCharArray(chr_current_minute, str_current_minute.length() + 1);
     if (current_minute < 10) { str_current_minute = "0" + str_current_minute; } // add a leading zero for 1 digit value

     uint8_t current_second = ptm->tm_sec;
     String str_current_second = String(current_second);
     char chr_current_second[str_current_second.length() + 1];
     str_current_second.toCharArray(chr_current_second, str_current_second.length() + 1);
     if (current_second < 10) { str_current_second = "0" + str_current_second; } // add a leading zero for 1 digit value

     String str_current_timestamp = str_current_year + "-" + 
                                    str_current_month + "-" + 
                                    str_current_date + " " + 
                                    str_current_hour + ":" + 
                                    str_current_minute + ":" + 
                                    str_current_second + 
                                    wib_tz;

     // 1970-01-01 07:00:08+0700 --> can not access NTP server

     char chr_current_timestamp[str_current_timestamp.length() + 1];
     str_current_timestamp.toCharArray(chr_current_timestamp, str_current_timestamp.length() + 1);

     String str_calibration_range = String(calibration_range);
     char chr_tag_calibration_range[str_calibration_range.length() + 1] = {' '};
     int val_int_calibration_range = (int) calibration_range;
     float val_float_calibration_range = (abs(calibration_range) - abs(val_int_calibration_range)) * 100000;
     int val_fra_calibration_range = (int)val_float_calibration_range;
     sprintf (chr_tag_calibration_range, "%d.%d", val_int_calibration_range, val_fra_calibration_range);

     float tag_latitude = -1.111111; 
     String str_tag_latitude = String(tag_latitude);
     char chr_tag_latitude[str_tag_latitude.length() + 1] = {' '};
     int val_int_tag_latitude = (int) tag_latitude;
     float val_float_tag_latitude = (abs(tag_latitude) - abs(val_int_tag_latitude)) * 100000;
     int val_fra_tag_latitude = (int)val_float_tag_latitude;
     sprintf (chr_tag_latitude, "%d.%d", val_int_tag_latitude, val_fra_tag_latitude);

     float tag_longitude = 1.111111;
     String str_tag_longitude = String(tag_longitude);
     char chr_tag_longitude[str_tag_longitude.length() + 1] = {' '};
     int val_int_tag_longitude = (int) tag_longitude;
     float val_float_tag_longitude = (abs(tag_longitude) - abs(val_int_tag_longitude)) * 100000;
     int val_fra_tag_longitude = (int)val_float_tag_longitude;
     sprintf (chr_tag_longitude, "%d.%d", val_int_tag_longitude, val_fra_tag_longitude);

     float tag_altitude = 1000;
     String str_tag_altitude = String(tag_altitude);
     char chr_tag_altitude[str_tag_altitude.length() + 1] = {' '};
     int val_int_tag_altitude = (int) tag_altitude;
     float val_float_tag_altitude = (abs(tag_altitude) - abs(val_int_tag_altitude)) * 100000;
     int val_fra_tag_altitude = (int)val_float_tag_altitude;
     sprintf (chr_tag_altitude, "%d.%d", val_int_tag_altitude, val_fra_tag_altitude);


     int tag_relative_position_x = 0;
     String str_tag_relative_position_x = String(tag_relative_position_x);
     char chr_tag_relative_position_x[str_tag_relative_position_x.length() + 1];
     str_tag_relative_position_x.toCharArray(chr_tag_relative_position_x, str_tag_relative_position_x.length() + 1);

     int tag_relative_position_y = 0;
     String str_tag_relative_position_y = String(tag_relative_position_y);
     char chr_tag_relative_position_y[str_tag_relative_position_y.length() + 1];
     str_tag_relative_position_y.toCharArray(chr_tag_relative_position_y, str_tag_relative_position_y.length() + 1);

     int tag_relative_position_z = 0;
     String str_tag_relative_position_z = String(tag_relative_position_z);
     char chr_tag_relative_position_z[str_tag_relative_position_z.length() + 1];
     str_tag_relative_position_z.toCharArray(chr_tag_relative_position_z, str_tag_relative_position_z.length() + 1);


     // Tag azimuth is device facing from the magnetic north 0° (range: 0-359°)
     int16_t tag_azimuth = 180;  // int16_t datatype is suitable for Astra DB's smallint datatype
     String str_tag_azimuth = String(tag_azimuth);
     char chr_tag_azimuth[str_tag_azimuth.length() + 1];
     str_tag_azimuth.toCharArray(chr_tag_azimuth, str_tag_azimuth.length() + 1);


     char chr_note[str_message.length() + 1];
     str_message.toCharArray(chr_note, str_message.length() + 1);


     // Lets sets status of the tag
     int8_t tag_status = 0; // int8_t datatype is suitable for Astra DB's tinyint datatype
     String str_tag_status = String(tag_status);
     char chr_tag_status[str_tag_status.length() + 1];
     str_tag_status.toCharArray(chr_tag_status, str_tag_status.length() + 1);

     struct AstraClient::KeyVal tags_log_columns[22] = { 
            { "id", chr_uuid }, 
            { "long_id", chr_long_id }, 
            { "mac_address", chr_mac_address }, 
            { "year", chr_current_year }, 
            { "month", chr_current_month }, 
            { "date", chr_current_date }, 
            { "hour", chr_current_hour }, 
            { "minute", chr_current_minute }, 
            { "second", chr_current_second }, 
            { "short_id", chr_short_id }, 
            { "location", chr_location }, 
            { "latitude", chr_tag_latitude }, 
            { "longitude", chr_tag_longitude }, 
            { "altitude", chr_tag_altitude }, 
            { "position_x", chr_tag_relative_position_x }, 
            { "position_y", chr_tag_relative_position_y }, 
            { "position_z", chr_tag_relative_position_z }, 
            { "azimuth", chr_tag_azimuth }, 
            { "calibration", chr_tag_calibration_range }, 
            { "timestamp", chr_current_timestamp },
            { "note", chr_message }, 
            { "status", chr_tag_status }
     };

     client.addRow(astra_db_keyspace, tags_log_table, 22, tags_log_columns);
     
}

void newDevice(DW1000Device* device) {
  Serial.print(" ranging init; 1 device added ! -> ");
  Serial.print(" short:");
  Serial.println(device->getShortAddress(), HEX);
}

void inactiveDevice(DW1000Device* device) {
     Serial.print(" Inactive device: ");
     Serial.println(device->getShortAddress(), HEX);
}
