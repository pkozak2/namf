//
// Created by viciu on 28.01.2020.
//
#include "variables.h"
#include "helpers.h"

/*****************************************************************
 * convert float to string with a                                *
 * precision of two (or a given number of) decimal places        *
 *****************************************************************/
String Float2String(const double value) {
    return Float2String(value, 2);
}

String Float2String(const double value, uint8_t digits) {
    // Convert a float to String with two decimals.
    char temp[15];

    dtostrf(value, 13, digits, temp);
    String s = temp;
    s.trim();
    return s;
}

/*****************************************************************
 * convert value to json string                                  *
 *****************************************************************/
String Value2Json(const String& type, const String& value) {
    String s = F("{\"value_type\":\"{t}\",\"value\":\"{v}\"},");
    s.replace("{t}", type);
    s.replace("{v}", value);
    return s;
}

/*****************************************************************
 * convert string value to json string                           *
 *****************************************************************/
String Var2Json(const String& name, const String& value) {
    String s = F("\"{n}\":\"{v}\",");
    String tmp = value;
    tmp.replace("\\", "\\\\"); tmp.replace("\"", "\\\"");
    s.replace("{n}", name);
    s.replace("{v}", tmp);
    return s;
}

/*****************************************************************
 * convert boolean value to json string                          *
 *****************************************************************/
String Var2Json(const String& name, const bool value) {
    String s = F("\"{n}\":\"{v}\",");
    s.replace("{n}", name);
    s.replace("{v}", (value ? "true" : "false"));
    return s;
}

/*****************************************************************
 * convert boolean value to json string                          *
 *****************************************************************/
String Var2Json(const String& name, const int value) {
    String s = F("\"{n}\":\"{v}\",");
    s.replace("{n}", name);
    s.replace("{v}", String(value));
    return s;
}



/*****************************************************************
 * Debug output                                                  *
 *****************************************************************/
void debug_out(const String& text, const int level, const bool linebreak) {
    if (level <= cfg::debug) {
        if (linebreak) {
            Serial.println(text);
        } else {
            Serial.print(text);
        }
    }
}




//Create string with config as JSON
String getConfigString(boolean maskPwd = false) {
    using namespace cfg;
    String json_string = "{";
    debug_out(F("saving config..."), DEBUG_MIN_INFO, 1);

#define copyToJSON_Bool(varname) json_string += Var2Json(#varname,varname);
#define copyToJSON_Int(varname) json_string += Var2Json(#varname,varname);
#define copyToJSON_String(varname) json_string += Var2Json(#varname,String(varname));
    copyToJSON_String(current_lang);
    copyToJSON_String(SOFTWARE_VERSION);
    copyToJSON_String(wlanssid);
    //mask WiFi password?
    if (maskPwd) {
        json_string += Var2Json("wlanpwd",String("***"));
    } else {
        copyToJSON_String(wlanpwd);
    }
    copyToJSON_String(www_username);
    copyToJSON_String(www_password);
    copyToJSON_String(fs_ssid);
    copyToJSON_String(fs_pwd);
    copyToJSON_Bool(www_basicauth_enabled);
    copyToJSON_Bool(dht_read);
    copyToJSON_Bool(sds_read);
    copyToJSON_Bool(pms_read);
    copyToJSON_Bool(bmp280_read);
    copyToJSON_Bool(bme280_read);
    copyToJSON_Bool(heca_read);
    copyToJSON_Bool(ds18b20_read);
    copyToJSON_Bool(gps_read);
    copyToJSON_Bool(send2dusti);
    copyToJSON_Bool(ssl_dusti);
    copyToJSON_Bool(send2madavi);
    copyToJSON_Bool(ssl_madavi);
    copyToJSON_Bool(send2sensemap);
    copyToJSON_Bool(send2fsapp);
    copyToJSON_Bool(send2lora);
    copyToJSON_Bool(send2csv);
    copyToJSON_Bool(auto_update);
    copyToJSON_Bool(use_beta);
    copyToJSON_Bool(has_display);
    copyToJSON_Bool(has_lcd1602);
    copyToJSON_Bool(has_lcd1602_27);
    copyToJSON_Bool(has_lcd2004_27);
    copyToJSON_Bool(has_lcd2004_3f);
    copyToJSON_String(debug);
    copyToJSON_String(sending_intervall_ms);
    copyToJSON_String(time_for_wifi_config);
    copyToJSON_String(senseboxid);
    copyToJSON_Bool(send2custom);
    copyToJSON_String(host_custom);
    copyToJSON_String(url_custom);
    copyToJSON_Int(port_custom);
    copyToJSON_String(user_custom);
    copyToJSON_String(pwd_custom);

    copyToJSON_Bool(send2influx);
    copyToJSON_String(host_influx);
    copyToJSON_String(url_influx);
    copyToJSON_Int(port_influx);
    copyToJSON_String(user_influx);
    copyToJSON_String(pwd_influx);
#undef copyToJSON_Bool
#undef copyToJSON_Int
#undef copyToJSON_String

    json_string.remove(json_string.length() - 1);
    json_string += "}";

    return json_string;

}

//Create string with config as JSON
String getMaskedConfigString() {
    return getConfigString(true);
}

/*****************************************************************
 * write config to spiffs                                        *
 *****************************************************************/
void writeConfigRaw(String json_string) {
    debug_out(json_string, DEBUG_MIN_INFO, 1);
    File configFile = SPIFFS.open("/config.json", "w");
    if (configFile) {
        configFile.print(json_string);
        debug_out(F("Config written: "), DEBUG_MIN_INFO, 0);
        debug_out(json_string, DEBUG_MIN_INFO, 1);
        configFile.close();
    } else {
        debug_out(F("failed to open config file for writing"), DEBUG_ERROR, 1);
    }
}

/*
 * get current config and save it
 */

void writeConfig(){
    String json_string  = getConfigString();
    writeConfigRaw(json_string);
}
