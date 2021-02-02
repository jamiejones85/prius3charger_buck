#pragma once

#include <EEPROM.h>

#define DEFAULT_BATTERY_CHARGE_VOLTAGE 300
#define DEFAULT_PRECHARGE_VOLTAGE 550  // European 3-phase rectifies to 600V
#define DEFAULT_PRECHARGE_BOOST_ENABLED true
#define DEFAULT_CANBUS_ENABLE true


typedef struct ChargerConfig {
  volatile int16_t battery_charge_voltage_V;
  volatile int16_t precharge_voltage_V;
  volatile bool precharge_boost_enabled;
  volatile bool canbus_enabled;
};


ChargerConfig loadConfig() {
 ChargerConfig c;
 
 EEPROM.get(0, c);

 if (c.battery_charge_voltage_V != 0) {
  return c;
 }

 return ChargerConfig {
  DEFAULT_BATTERY_CHARGE_VOLTAGE,
  DEFAULT_PRECHARGE_VOLTAGE,
  DEFAULT_PRECHARGE_BOOST_ENABLED,
  DEFAULT_CANBUS_ENABLE
 };
}


bool getBoolValue(char* value) {
  if (strcmp(value, "true") || strcmp(value, "1")) {
    return true;
  }
  return false;
}


ChargerConfig saveChargerConfig(ChargerConfig conf) {
    EEPROM.put(0, conf);
}

void updateParameter(char* parameter, char* value, struct ChargerConfig* c) {
  if (strcmp(parameter, "battery_charge_voltage_V") == 0) {
    c->battery_charge_voltage_V = atoi(value);
  } else if (strcmp(parameter, "precharge_voltage_V") == 0) {
    c->precharge_voltage_V = atoi(value);
  } else if (strcmp(parameter, "precharge_boost_enabled") == 0) {
    c->precharge_boost_enabled = getBoolValue(value);
  } else if (strcmp(parameter, "canbus_enabled") == 0) {
    c->canbus_enabled = getBoolValue(value);
  }
}

void parseCommand(char* command, struct ChargerConfig* c) {
  char* p = strchr(command, ' ');
  int size = p - command;
  char variable[size];
  memcpy(variable, command, size);
  variable[size] = '\0';
  updateParameter(variable, &command[size], c);
}



void printChargerConfig(struct ChargerConfig* c) {
  CONSOLE.println(F("{"));
  CONSOLE.print(F("\"battery_charge_voltage_V\": "));
  CONSOLE.print(c->battery_charge_voltage_V);
  CONSOLE.println(F(","));

  CONSOLE.print(F("\"precharge_voltage_V\": "));
  CONSOLE.print(c->precharge_voltage_V);
  CONSOLE.println(F(","));

  CONSOLE.print(F("\"precharge_boost_enabled\": "));
  CONSOLE.print(c->precharge_boost_enabled);
  CONSOLE.println(F(","));

  CONSOLE.print(F("\"canbus_enabled\": "));
  CONSOLE.print(c->canbus_enabled);

  CONSOLE.println(F("}"));

}
