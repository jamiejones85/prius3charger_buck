#pragma once

#include <EEPROM.h>

#define DEFAULT_BATTERY_CHARGE_VOLTAGE 300
#define DEFAULT_PRECHARGE_VOLTAGE 550  // European 3-phase rectifies to 600V
#define DEFAULT_PRECHARGE_BOOST_ENABLED true
#define DEFAULT_CANBUS_ENABLE true
#define DEFAULT_MG1_CURRENT_A_PER_BIT 1.0
#define DEFAULT_DCBUS1_OFFSET_BITS 74
#define DEFAULT_DCBUS1_V_PER_BIT 0.560
#define DEFAULT_DCBUS2_OFFSET_BITS 0
#define DEFAULT_DCBUS2_V_PER_BIT 1.234
#define DEFAULT_INPUT_MINIMUM_VOLTAGE 300  
#define CHECK_NUMBER 34 //check on loading EEPROM this value 


typedef struct ChargerConfig {
  int16_t battery_charge_voltage_V;
  int16_t precharge_voltage_V;
  int16_t min_input_voltage_V;
  bool precharge_boost_enabled;
  bool canbus_enabled;
  float mg1_current_amp_per_bit;
  float dc1_volt_per_bit;
  int8_t dc1_volt_offset;
  float dc2_volt_per_bit;
  int8_t dc2_volt_offset;
  int8_t check;
};


ChargerConfig loadConfig() {
 ChargerConfig c;
 
 EEPROM.get(0, c);

 if (c.check == CHECK_NUMBER) {
  return c;
 }

 return ChargerConfig {
  DEFAULT_BATTERY_CHARGE_VOLTAGE,
  DEFAULT_PRECHARGE_VOLTAGE,
  DEFAULT_INPUT_MINIMUM_VOLTAGE,
  DEFAULT_PRECHARGE_BOOST_ENABLED,
  DEFAULT_CANBUS_ENABLE,
  DEFAULT_MG1_CURRENT_A_PER_BIT,
  DEFAULT_DCBUS1_V_PER_BIT,
  DEFAULT_DCBUS1_OFFSET_BITS,
  DEFAULT_DCBUS2_V_PER_BIT,
  DEFAULT_DCBUS2_OFFSET_BITS,
  CHECK_NUMBER
 };
}


bool getBoolValue(char* value) {
  if (strcmp(value, "true") == 0 || strcmp(value, "1") == 0) {
    return true;
  }
  return false;
}


ChargerConfig saveChargerConfig(ChargerConfig conf) {
  
    return EEPROM.put(0, conf);
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
  } else if (strcmp(parameter, "mg1_current_amp_per_bit") == 0) {
    c->mg1_current_amp_per_bit = atof(value);
  } else if (strcmp(parameter, "dc1_volt_offset") == 0) {
    c->dc1_volt_offset = atoi(value);
  } else if (strcmp(parameter, "dc1_volt_per_bit") == 0) {
    c->dc1_volt_per_bit = atof(value);
  } else if (strcmp(parameter, "dc2_volt_offset") == 0) {
    c->dc2_volt_offset = atoi(value);
  } else if (strcmp(parameter, "dc2_volt_per_bit") == 0) {
    c->dc2_volt_per_bit = atof(value);
  } else if (strcmp(parameter, "min_input_voltage_V") == 0) {
    c->min_input_voltage_V = atoi(value);
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
  CONSOLE.print(F("\"min_input_voltage_V\": { \"type\": \"number\", \"value\":"));
  CONSOLE.print(c->min_input_voltage_V);
  CONSOLE.println(F("},"));

  CONSOLE.print(F("\"battery_charge_voltage_V\": { \"type\": \"number\", \"value\":"));
  CONSOLE.print(c->battery_charge_voltage_V);
  CONSOLE.println(F("},"));
  
  CONSOLE.print(F("\"precharge_voltage_V\": { \"type\": \"number\", \"value\":"));
  CONSOLE.print(c->precharge_voltage_V);
  CONSOLE.println(F("},"));
  
  CONSOLE.print(F("\"precharge_boost_enabled\": { \"type\": \"bool\", \"value\":"));
  CONSOLE.print(c->precharge_boost_enabled);
  CONSOLE.println(F("},"));
  
  CONSOLE.print(F("\"canbus_enabled\": { \"type\": \"bool\", \"value\":"));
  CONSOLE.print(c->canbus_enabled);
  CONSOLE.println(F("},"));
  
  CONSOLE.print(F("\"mg1_current_amp_per_bit\": { \"type\": \"number\", \"value\":"));
  CONSOLE.print((float)c->mg1_current_amp_per_bit, 2);
  CONSOLE.println(F("},"));
  
  CONSOLE.print(F("\"dc1_volt_per_bit\": { \"type\": \"number\", \"value\":"));
  CONSOLE.print((float)c->dc1_volt_per_bit, 2);
  CONSOLE.println(F("},"));
  
  CONSOLE.print(F("\"dc1_volt_offset\": { \"type\": \"number\", \"value\":"));
  CONSOLE.print(c->dc1_volt_offset);
  CONSOLE.println(F("},"));
  
  CONSOLE.print(F("\"dc2_volt_per_bit\": { \"type\": \"number\", \"value\":"));
  CONSOLE.print((float)c->dc2_volt_per_bit, 2);
  CONSOLE.println(F("},"));
  
  CONSOLE.print(F("\"dc2_volt_offset\": { \"type\": \"number\", \"value\":"));
  CONSOLE.print(c->dc2_volt_offset);
  CONSOLE.println(F("}}"));

}
