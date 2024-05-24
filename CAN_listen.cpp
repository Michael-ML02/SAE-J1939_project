void CAN_Listen(ECU_data *data)
{
    //Create temporary variables to hold read data
    uint32_t ID = 0;
    uint8_t temp_data[8] = {0};
    bool new_message = false;
    //Create pointers of temp variables to pass to Read_CAN function
    uint32_t *ID_ptr = &ID;
    uint8_t *data_ptr = &temp_data[8];
    bool *ptr = &new_message;
    /*Place Arduino Read CAN function here*/
    
    //Split ID to use in if statements
    uint8_t PGN1 =  ID;
    uint8_t PGN2 = ID >> 8;
    uint8_t ID1 = ID >> 16;
    uint8_t ID2 = ID >> 24;

    //Place new data into struct
    if(new_message == true)
    {
        data->ID = ID;
        data->data[8] = temp_data[8];
        data->new_data = new_message;
    }

    if(ID1 == data->myaddress || 0xFF && PGN1 == 0x18 && PGN2 == 0xEA)
    {
        Read_Request(data, ID2);
    }
    else if(ID1 == data->myaddress || 0xFF && PGN1 == 0x18 && PGN2 == 0xE8)
    {
        Read_Ack(data, ID2);
    }
}   