
#include "1939_lib.h"

// Control describes status of requested PGN
/*Group function describes cause of the status
	GROUP_FUNCTION_VALUE_NORMAL = 0x0U,
	GROUP_FUNCTION_VALUE_CANNOT_MAINTAIN_ANOTHER_CONNECTION = 0x1U,
	GROUP_FUNCTION_VALUE_LACKING_NECESSARY_RESOURCES = 0x2U,
	GROUP_FUNCTION_VALUE_ABORT_TIME_OUT = 0x3U,
	GROUP_FUNCTION_VALUE_NO_CAUSE = 0xFFU */

void Set_Address(ECU_data *data, uint8_t address)
{
    data->myaddress = address;
}

void Create_Address_Claimed(ECU_data *data)
{
    data->myMessage.PGN_DA_SA[0] =  0x18;
    data->myMessage.PGN_DA_SA[1] =  0xEE;
    data->myMessage.PGN_DA_SA[2] =  0xFF;
    data->myMessage.PGN_DA_SA[3] =  data->myaddress;


    
    data->myMessage.Bytes_8[0] = data->myName.identity_number;
    data->myMessage.Bytes_8[1] = data->myName.identity_number >> 8;
    data->myMessage.Bytes_8[2] = (data->myName.identity_number >> 16) | (data->myName.manufacturer_code << 5); 
    data->myMessage.Bytes_8[3] = data->myName.manufacturer_code >> 3;
    data->myMessage.Bytes_8[4] = (data->myName.ECU_instance) | (data->myName.function_instance << 3);
    data->myMessage.Bytes_8[5] = data->myName.function;
    data->myMessage.Bytes_8[6] = (data->myName.vehicle_system << 1) | (0x1);
    data->myMessage.Bytes_8[7] = (data->myName.vehicle_system_instance) | (data->myName.industry_group << 4) | (data->myName.arbitrary_address_capable << 7);
}

void Create_Address_Not_Claimed(ECU_data* data)
{
    data->myMessage.PGN_DA_SA[0] =  0x18;
    data->myMessage.PGN_DA_SA[1] =  0xEE;
    data->myMessage.PGN_DA_SA[2] =  0xFF;
    data->myMessage.PGN_DA_SA[3] =  0xFE;


    
    data->myMessage.Bytes_8[0] = data->myName.identity_number;
    data->myMessage.Bytes_8[1] = data->myName.identity_number >> 8;
    data->myMessage.Bytes_8[2] = (data->myName.identity_number >> 16) | (data->myName.manufacturer_code << 5); 
    data->myMessage.Bytes_8[3] = data->myName.manufacturer_code >> 3;
    data->myMessage.Bytes_8[4] = (data->myName.ECU_instance) | (data->myName.function_instance << 3);
    data->myMessage.Bytes_8[5] = data->myName.function;
    data->myMessage.Bytes_8[6] = (data->myName.vehicle_system << 1) | (0x1);
    data->myMessage.Bytes_8[7] = (data->myName.vehicle_system_instance) | (data->myName.industry_group << 4) | (data->myName.arbitrary_address_capable << 7);
}

void Create_Request(ECU_data *data, uint8_t DA, uint32_t PGN)
{
    data->myMessage.PGN_DA_SA[0] =  0x18;
    data->myMessage.PGN_DA_SA[1] =  0xEA;
    data->myMessage.PGN_DA_SA[2] =  DA;
    data->myMessage.PGN_DA_SA[3] =  data->myaddress;


    
    data->myMessage.Bytes_8[0] = PGN;
    data->myMessage.Bytes_8[1] = PGN >> 8;
    data->myMessage.Bytes_8[2] = PGN >> 16;
    data->myMessage.Bytes_8[3] = 0;
    data->myMessage.Bytes_8[4] = 0;
    data->myMessage.Bytes_8[5] = 0;
    data->myMessage.Bytes_8[6] = 0;
    data->myMessage.Bytes_8[7] = 0;
}

void Create_Acknowledge(ECU_data *data, uint8_t DA, uint8_t Control_byte, uint8_t Group_function)
{
    data->myMessage.PGN_DA_SA[0] =  0x18;
    data->myMessage.PGN_DA_SA[1] =  0xE8;
    data->myMessage.PGN_DA_SA[2] =  DA;
    data->myMessage.PGN_DA_SA[3] =  data->myaddress;


    
    data->myMessage.Bytes_8[0] = Control_byte;
    data->myMessage.Bytes_8[1] = Group_function;
    data->myMessage.Bytes_8[2] = 0xFF;
    data->myMessage.Bytes_8[3] = 0xFF;
    data->myMessage.Bytes_8[4] = data->ID;
    data->myMessage.Bytes_8[5] = data->data[0];
    data->myMessage.Bytes_8[6] = data->data[1];
    data->myMessage.Bytes_8[7] = data->data[2];  
}

void Create_RTS(ECU_data *data, uint8_t DA, uint8_t Control_byte, uint32_t PGN, uint16 Size_Bytes)
{
    
    uint8_t Package_amount = Size_Bytes/7;
    if (Size_Bytes % 7 != 0)
    Package_amount = Package_amount + 1;    
    
    data->myMessage.PGN_DA_SA[0] =  0x1C;
    data->myMessage.PGN_DA_SA[1] =  0xEC;
    data->myMessage.PGN_DA_SA[2] =  DA;
    data->myMessage.PGN_DA_SA[3] =  data->myaddress;

    data->myMessage.Bytes_8[0] = Control_byte;
    data->myMessage.Bytes_8[1] = Size_Bytes;
    data->myMessage.Bytes_8[2] = Size_Bytes >> 8;
    data->myMessage.Bytes_8[3] = Package_amount;
    data->myMessage.Bytes_8[4] = 0xFF;
    data->myMessage.Bytes_8[5] = PGN;
    data->myMessage.Bytes_8[6] = PGN >> 8;
    data->myMessage.Bytes_8[7] = PGN >> 16;
}

void Create_CTS(ECU_data *data, uint8_t DA, uint8_t Control_byte)

{
    uint8_t next_package_TS = (data->Package_received++);

    data->myMessage.PGN_DA_SA[0] =  0x1C;
    data->myMessage.PGN_DA_SA[1] =  0xEC;
    data->myMessage.PGN_DA_SA[2] =  DA;
    data->myMessage.PGN_DA_SA[3] =  data->myaddress;

    data->myMessage.Bytes_8[0] = Control_byte;
    data->myMessage.Bytes_8[1] = data->Package_amount;
    data->myMessage.Bytes_8[2] = next_package_TS;
    data->myMessage.Bytes_8[3] = 0xFF;
    data->myMessage.Bytes_8[4] = 0xFF;
    data->myMessage.Bytes_8[5] = data->PGN;
    data->myMessage.Bytes_8[6] = data->PGN >> 8;
    data->myMessage.Bytes_8[7] = data->PGN >> 16;
}

void Create_EOM(ECU_data *data, uint8_t DA, uint8_t Control_byte)
{
    data->myMessage.PGN_DA_SA[0] =  0x1C;
    data->myMessage.PGN_DA_SA[1] =  0xEC;
    data->myMessage.PGN_DA_SA[2] =  DA;
    data->myMessage.PGN_DA_SA[3] =  data->myaddress;

    data->myMessage.Bytes_8[0] = Control_byte;
    data->myMessage.Bytes_8[1] = data->Bytes_recieved;
    data->myMessage.Bytes_8[2] = data->Bytes_recieved >> 8;
    data->myMessage.Bytes_8[3] = data->Package_received;
    data->myMessage.Bytes_8[4] = 0xFF;
    data->myMessage.Bytes_8[5] = data->PGN;
    data->myMessage.Bytes_8[6] = data->PGN >> 8;
    data->myMessage.Bytes_8[7] = data->PGN >> 16;
}

void Create_DT(ECU_data *data, uint8_t DA)
{
    uint8_t FirstByteTS = 7*(data->PackageTS) - 7;
    
    data->myMessage.PGN_DA_SA[0] =  0x1C;
    data->myMessage.PGN_DA_SA[1] =  0xEB;
    data->myMessage.PGN_DA_SA[2] =  DA;
    data->myMessage.PGN_DA_SA[3] =  data->myaddress;

    data->myMessage.Bytes_8[0] = data->PackageTS;
    data->myMessage.Bytes_8[1] = data->Sensor_data[FirstByteTS];
    data->myMessage.Bytes_8[2] = data->Sensor_data[FirstByteTS + 1];
    data->myMessage.Bytes_8[3] = data->Sensor_data[FirstByteTS + 2];
    data->myMessage.Bytes_8[4] = data->Sensor_data[FirstByteTS + 3];
    data->myMessage.Bytes_8[5] = data->Sensor_data[FirstByteTS + 4];
    data->myMessage.Bytes_8[6] = data->Sensor_data[FirstByteTS + 5];
    data->myMessage.Bytes_8[7] = data->Sensor_data[FirstByteTS + 6];
}

void Create_DT_Abort(ECU_data * data, uint8_t DA, uint8_t Control_byte, uint8_t AbortMSG)
{
    data->myMessage.PGN_DA_SA[0] =  0x1C;
    data->myMessage.PGN_DA_SA[1] =  0xEC;
    data->myMessage.PGN_DA_SA[2] =  DA;
    data->myMessage.PGN_DA_SA[3] =  data->myaddress;

    data->myMessage.Bytes_8[0] = Control_byte;
    data->myMessage.Bytes_8[1] = AbortMSG;
    data->myMessage.Bytes_8[2] = 0xFF;
    data->myMessage.Bytes_8[3] = 0xFF;
    data->myMessage.Bytes_8[4] = 0xFF;
    data->myMessage.Bytes_8[5] = data->PGN;
    data->myMessage.Bytes_8[6] = data->PGN >> 8;
    data->myMessage.Bytes_8[7] = data->PGN >> 16;
}

void Read_Request(ECU_data *data, uint8_t SA)
{
    uint32_t PGN = data->data[2] << 16 | data->data[1] << 8 | data->data[0];

    if (PGN == 0x00E800U) //If PGN is acknowledge
    {
        Create_Acknowledge(data, SA, 0, 0);
    }
    else if (PGN == 0x00EE00U) //PGN is Address Claimed
    {
        data->Connected_Amount++;
        data->Addresses[data->Connected_Amount - 1] == SA;
    }
    else if(PGN == 0x00EB00U) //Request Data transfer, Pretend that my sensor is reading engine temperature with 7 bytes of data
    {
        data->Package_received = 0;
        Create_RTS(data, SA, 0x10U, 0x00FEEEU, 14);
    }


    
}

void Read_Ack(ECU_data *data, uint8_t SA)
{
    data->Control_byte = data->data[0];
    data->PGN_ACK = data->data[7] << 16 | data->data[6] << 8 | data->data[5];
    data->Source = data->data[0];
    data->Group = data->data[1];
}
//After every read function a send msg should be placed
void Read_CTS(ECU_data *data, uint8_t SA)
{
    data->Control_byte = data->data[0];
    data->Package_amount = data->data[1];
    data->PackageTS = data->data[2];
    data->PGN = data->data[7] << 16 | data->data[6] << 8 | data->data[5];

    Create_DT(data, SA);    
}

void Read_DT(ECU_data *data, uint8_t SA, uint8_t tempdata[])
{
    data->PackageTS = tempdata[0];
    uint8_t Storage_index = (tempdata[0] - 1);
    for (uint8_t i = 1; i < 8; i++)
    data->Recieved_data[(7*Storage_index) +i - 1] = tempdata[i];
    data->Package_received++;

    if(tempdata[0] != data->Package_amount){
        Create_CTS(data, SA, 0x11U);
    }
    else
    {
        Create_EOM(data, SA, 0x13U);
    }
}

void Read_EOM(ECU_data *data, uint8_t SA)
{
    data->Control_byte = data->data[0];
    data->Bytes_recieved = data->data[2] << 8 | data->data[1];
    data->Package_received = data->data[3];
    data->PGN = data->data[7] << 16 | data->data[6] << 8 | data->data[5];

    if(data->Package_amount != data->Package_received)
    {
        Create_RTS(data,SA,0x10U,0x00EB00U, data->Bytes_expected);
    }
    else
    {
        Create_Acknowledge(data,SA,0x13U,0x00U);
    }
}

void Create_Signal(ECU_data *data, uint8_t DA, uint8_t Select)
{
    data->myMessage.PGN_DA_SA[0] =  0xFF; //Unused Prioity 3 PGN in SAEJ1939 
    data->myMessage.PGN_DA_SA[1] =  0x10;
    data->myMessage.PGN_DA_SA[2] =  DA;
    data->myMessage.PGN_DA_SA[3] =  data->myaddress;

    data->myMessage.Bytes_8[0] = Select; //Select which Digital output pin to use (1-8);
    data->myMessage.Bytes_8[1] = 0xFF; //Reserved positions for future improvements
    data->myMessage.Bytes_8[2] = 0xFF;
    data->myMessage.Bytes_8[3] = 0xFF;
    data->myMessage.Bytes_8[4] = 0xFF;
    data->myMessage.Bytes_8[5] = 0xFF;
    data->myMessage.Bytes_8[6] = 0xFF;
    data->myMessage.Bytes_8[7] = 0xFF;
}

void Read_Signal(ECU_data *data)
{
    data->Signal = data->data[0];
}