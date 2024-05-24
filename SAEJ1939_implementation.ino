//Libraries

#include <SPI.h>
#include <mcp_can.h>
#include "1939_lib.h"


// CAN shield pins and define object CAN_Shield on that pin
#define CAN_CS_PIN 10
MCP_CAN CAN_shield(CAN_CS_PIN);

void CAN_Send(uint8_t ID[], uint8_t DLC, uint8_t data[]) {
  uint32_t wholeID = ID[0] | ID[1] << 8 | ID[2] << 16 | ID[3] << 24;
  if(CAN_shield.sendMsgBuf(wholeID, 1, DLC, data) == CAN_OK) {
    Serial.println("Sent!");
  }
  else {
    Serial.println("Error transmitting...");
  }
}

void CAN_Read(uint32_t* ID, uint8_t *EXT, uint8_t data[],uint8_t* DLC, bool* is_new_data) { 
  uint8_t comp[8] = {data};
  if(CAN_shield.readMsgBuf(ID, EXT, DLC, data) == CAN_OK) { 
    Serial.println("Read!");
  }          
  else {
    Serial.println("Error reading buffer...");
    Serial.print("Error code:");
    Serial.println(CAN_shield.readMsgBuf(ID, EXT, DLC, data));
  }
  if (comp == data) {
    is_new_data = 0;
  }
  else  {
    is_new_data = 1;
  }
}
  
  ECU_data data_struct = {0};
  ECU_data *str_ptr = &data_struct;

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 14; ++i) {
    data_struct.Sensor_data[i] = i + 1;  // Assigning values 1 to 14
  }
  for(uint8_t i = 0; i < 14; i++) {
  Serial.print(data_struct.Sensor_data[i], HEX);
  Serial.print(" ");
}
 if(CAN_shield.begin(MCP_ANY, CAN_250KBPS, MCP_16MHZ) == CAN_OK) {
    Serial.println("MCP2515 Initialized Successfully!");
  }
  else {
    Serial.println("Error Initializing MCP2515...");
  }
  Set_Address(str_ptr, 0x01);
  for(uint8_t i = 0; i < 8; i++) {
  Serial.print(data_struct.data[i], HEX);
  Serial.print(" ");
}

}




void loop() {
  Create_Request(str_ptr, 0x01,0x00EB00U);
  CAN_Send(data_struct.myMessage.PGN_DA_SA, 8 ,data_struct.myMessage.Bytes_8);
  CAN_Listen(str_ptr);
  for(uint8_t j = 0; j < 8; j++) {
  Serial.print(data_struct.Recieved_data[j], HEX);
  Serial.print(" "); 
}
  delay(250);
}

void CAN_Listen(ECU_data *data)
{
    //Create temporary variables to hold read data
    uint32_t ID = 0;
    uint8_t temp_data[8] = {0};
    bool new_message = true;    //Ändrad till always true för bugtesting
    uint8_t EXT = 0;
    uint8_t len = 0;
    //Create pointers of temp variables to pass to Read_CAN function
    uint32_t *ID_ptr = &ID;
    uint8_t *EXT_ptr = &EXT;
    uint8_t *len_ptr = &len;
    uint8_t *data_ptr = &temp_data[8];
    bool *ptr = &new_message;
    /*Place Arduino Read CAN function here*/
    CAN_Read(ID_ptr, EXT_ptr, data_ptr,len_ptr, ptr);
    //Split ID to use in if statements
    uint8_t PGN1 =  ID;
    uint8_t PGN2 = ID >> 8;
    uint8_t ID1 = ID >> 16;
    uint8_t ID2 = ID >> 24;
    //Place new data into struct
    Serial.println(ID);
    for(uint8_t j = 0; j < 8; j++) {
      Serial.print(temp_data[j], HEX);
      Serial.print(" "); 
    }
    if(new_message == true)
    {
        data->ID = ID;
        data->data[8] = temp_data[8];
        data->new_data = new_message;
    }

    if(ID1 == data->myaddress || ID1 == 0xFF && PGN1 == 0x18 && PGN2 == 0xEA)
    {
        Read_Request(data, ID2);
        CAN_Send(data->myMessage.PGN_DA_SA, 8, data->myMessage.Bytes_8);
    }
    else if(ID1 == data->myaddress || ID1 == 0xFF && PGN1 == 0x18 && PGN2 == 0xE8)
    {
        Read_Ack(data, ID2);
        CAN_Send(data->myMessage.PGN_DA_SA, 8, data->myMessage.Bytes_8);
    }
    else if(ID1 == data->myaddress || ID1 == 0xFF && PGN1 == 0x1C && PGN2 == 0xEC && temp_data[0] == 0x11U)
    {
      Read_CTS(data, ID2);
      CAN_Send(data->myMessage.PGN_DA_SA, 8, data->myMessage.Bytes_8);
    }
    else if(ID1 == data->myaddress || ID1 == 0xFF && PGN1 == 0x1C && PGN2 == 0xEB)
    {
      Read_DT(data, ID2, temp_data);
      CAN_Send(data->myMessage.PGN_DA_SA, 8, data->myMessage.Bytes_8);
    }
    else if(ID1 == data->myaddress || ID1 == 0xFF && PGN1 == 0x1C && PGN2 == 0xEB && temp_data[0] == 0x13U)
    {
      Read_EOM(data, ID2);
      CAN_Send(data->myMessage.PGN_DA_SA, 8, data->myMessage.Bytes_8);
    }
    else if(ID1 == data->myaddress || ID1 == 0xFF && PGN1 == 0xFF && PGN2 == 0x10)
    {
      Read_Signal(data);
      digitalWrite(data_struct.Signal, HIGH);
    }
}  