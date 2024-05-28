void CAN_Listen(ECU_data *data)
  {
    //Create temporary variables to hold read data
    uint32_t ID = 0;
    uint8_t temp_data[8] = {0};
    //Create pointers of temp variables to pass to Read_CAN function
    uint32_t *ID_ptr = &ID;
    uint8_t *data_ptr = &temp_data[8];
    /*Place Arduino Read CAN function here I.E readMsgBuf from mcp_can*/
    
    //Split ID to use in if statements
    uint8_t PGN1 =  ID;
    uint8_t PGN2 = ID >> 8;
    uint8_t ID1 = ID >> 16;
    uint8_t ID2 = ID >> 24;

    //Place new data into struct

    data->ID = ID;
    data->data[8] = temp_data[8];

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
