#ifndef __1939_lib_h__
#define __1939_lib_h__

typedef unsigned long uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short int uint16;

//Data to be sent
struct Message {
        //ID
        uint8_t PGN_DA_SA[4];
        
        //Data
        uint8_t Bytes_8[8];
       };
       
struct Name {
	    uint32_t identity_number;						/* Specify the ECU serial ID - 0 to 2097151 */
	    uint16 manufacturer_code;						/* Specify the ECU manufacturer code - 0 to 2047 */
	    uint8_t function_instance;						/* Specify the ECU function number - 0 to 31 */
	    uint8_t ECU_instance;							/* Specify the ECU number - 0 to 7 */
	    uint8_t function;								/* Specify the ECU function - 0 to 255 */
	    uint8_t vehicle_system;							/* Specify the type of vehicle where ECU is located - 0 to 127 */
	    uint8_t arbitrary_address_capable;				/* Specify if the ECU have right to change address if addresses conflicts - 0 to 1 */
	    uint8_t industry_group;							/* Specify the group where this ECU is located - 0 to 7 */
	    uint8_t vehicle_system_instance;				/* Specify the vehicle system number - 0 to 15 */
	    uint8_t from_ecu_address;						/* From which ECU came this message */
};

typedef struct {
    
    //Last message read
    uint32_t ID;
    uint8_t data[8];
    bool new_data;
    
    //Store lastest Selector value
    uint8_t Signal;
    
    //Store lastest acknowledge
    uint8_t Source;
    uint8_t Control_Ack;
    uint8_t Group;
    uint32_t PGN_ACK;

    //Store information to Send
    struct Message myMessage;

    //Information about this ECU
    uint8_t myaddress;
    struct Name myName;

    //Information about other ECU's
    uint8_t Addresses[255]; //Can be max size of 255 different addresses in the network
    uint8_t Connected_Amount;  
    //Temporarily store read info
    struct Name yourName;

    //Variables for DT
    uint8_t Control_byte;
    uint16 Bytes_data; //Bytes to Send
    uint8_t Package_amount;
    uint32_t PGN;
    uint8_t Package_received;
    uint16 Bytes_expected; //Bytes to recieve
    uint16 Bytes_recieved;
    uint8_t PackageTS; //Which nr package to send

    //Data on this device from sensors
    uint8_t Sensor_data[70];
    //Place to recieve data from other ECU's
    uint8_t Recieved_data[70];

}ECU_data;

extern void Set_Address(ECU_data *data, uint8_t address);

extern void Create_Address_Claimed(ECU_data *data);

extern void Create_Address_Not_Claimed(ECU_data* data);

extern void Create_Request(ECU_data *data, uint8_t DA, uint32_t PGN);

extern void Create_Acknowledge(ECU_data *data, uint8_t DA, uint8_t Control_byte, uint8_t Group_function);

extern void Create_RTS(ECU_data *data, uint8_t DA, uint8_t Control_byte, uint32_t PGN, uint16 Size_Bytes);

extern void Create_CTS(ECU_data *data, uint8_t DA, uint8_t Control_byte);

extern void Create_EOM(ECU_data *data, uint8_t DA, uint8_t Control_byte);

extern void Create_DT(ECU_data *data, uint8_t DA);

extern void Create_DT_Abort(ECU_data * data, uint8_t DA, uint8_t Control_byte, uint8_t AbortMSG);

extern void Read_Request(ECU_data *data, uint8_t SA);

extern void Read_Ack(ECU_data *data, uint8_t SA);

extern void Read_CTS(ECU_data *data, uint8_t SA);

extern void Read_DT(ECU_data *data, uint8_t SA, uint8_t tempdata[]);

extern void Read_EOM(ECU_data *data, uint8_t SA);
#endif