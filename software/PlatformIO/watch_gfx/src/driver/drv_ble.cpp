/*** 
 * @Author: MakerM0
 * @Date: 2024-09-05 16:16:21
 * @LastEditTime: 2024-10-03 19:50:56
 * @FilePath: \jdi_watch_gfx\src\driver\drv_ble.cpp
 * @Description: 
 * @https://github.com/MakerM0/
 * @Copyright (c) 2024 by MakerM0, All Rights Reserved. 
 */
#include "drv_ble.h"

// BLE Service
BLEDfu bledfu;   // OTA DFU service
BLEDis bledis;   // device information
BLEUart bleuart; // uart over ble
BLEBas blebas;   // battery

// callback invoked when central connects
void connect_callback(uint16_t conn_handle)
{
    // Get the reference to current connection
    BLEConnection *connection = Bluefruit.Connection(conn_handle);

    char central_name[32] = {0};
    connection->getPeerName(central_name, sizeof(central_name));

    //Serial.print("Connected to ");
    //Serial.println(central_name);
}

/**
 * Callback invoked when a connection is dropped
 * @param conn_handle connection where this event happens
 * @param reason is a BLE_HCI_STATUS_CODE which can be found in ble_hci.h
 */
void disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
    (void)conn_handle;
    (void)reason;

    //Serial.println();
    //Serial.print("Disconnected, reason = 0x");
    //Serial.println(reason, HEX);
}

void startAdv(void)
{
    // Advertising packet
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();

    // Include bleuart 128-bit uuid
    Bluefruit.Advertising.addService(bleuart);

    // Secondary Scan Response packet (optional)
    // Since there is no room for 'Name' in Advertising packet
    Bluefruit.ScanResponse.addName();

    /* Start Advertising
     * - Enable auto advertising if disconnected
     * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
     * - Timeout for fast mode is 30 seconds
     * - Start(timeout) with timeout = 0 will advertise forever (until connected)
     *
     * For recommended advertising interval
     * https://developer.apple.com/library/content/qa/qa1931/_index.html
     */
    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(244, 1600); // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);   // number of seconds in fast mode
    Bluefruit.Advertising.start(0);             // 0 = Don't stop advertising after n seconds
}

void ble_setup()
{
    // Config the peripheral connection with maximum bandwidth
    // more SRAM required by SoftDevice
    // Note: All config***() function must be called before begin()
    Bluefruit.configPrphBandwidth(BANDWIDTH_MAX);
    Bluefruit.begin();       // Sleep functions need the softdevice to be active.
    // Bluefruit.setTxPower(4); // Check bluefruit.h for supported values
    // // Bluefruit.setName(getMcuUniqueID()); // useful testing with multiple central connections
    // Bluefruit.setName("FreshWatch"); // useful testing with multiple central connections

    // Bluefruit.Periph.setConnectCallback(connect_callback);
    // Bluefruit.Periph.setDisconnectCallback(disconnect_callback);
    // // Bluefruit.Periph.setConnInterval(9, 16); // min = 9*1.25=11.25 ms, max = 16*1.25=20ms

    // // To be consistent OTA DFU should be added first if it exists
    // bledfu.begin();

    // // Configure and Start Device Information Service
    // bledis.setManufacturer("MakerM0");
    // bledis.setModel("FreshWatch");
    // bledis.setSoftwareRev("0.1.0");
    // bledis.begin();

    // // Configure and Start BLE Uart Service
    // bleuart.begin();

    // // Start BLE Battery Service
    // blebas.begin();
    // blebas.write(100);
    // // Set up and start advertising
    // startAdv();
}