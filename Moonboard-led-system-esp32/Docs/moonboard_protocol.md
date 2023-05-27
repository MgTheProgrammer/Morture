Moonboard protocol
------------------
These are the essentials to setup a GATT server that will communicate with the app. There might be a more detailed definition of the protocol but it isn't necessary to know it.
## Advertising
As it seems, the Moonboard dedicated app finds the Moonboard leds by scanning for service UUID.  
The custom 128 bit service UUID which the dedicated hardware advertise is: ``` 6e400001-b5a3-f393-e0a9-e50e24dcca9e ```.  

Conveniently enough this is the same UUID of [Nordic UART Service][Nordic UART], and it's not by chance that they landed on the same UUID out of the three hundred forty undecillion, two hundred eighty-two decillion... options; The Moonboard use this well known UART service.

## The data transmitted
After establishing a connection the data transfer between the app and the leds can begin. Whenever the app wants to lit a led it sends a write command (```0x52```) containing a value describing which leds should be lit.  
  
### Value structure
| Prefix       | holds                            | suffix     |
|--------------|----------------------------------|------------|
| ```0x6c23``` | one hold ```0x2c``` another hold | ```0x23``` |

The prefix and the suffix are constant, the holds are separated by ```2c``` . In case there are too many holds to send in one packet the remaining data will just be sent in a another packet until a ```0x23``` is sent. The data isn't split by hold or anything, just by the 20 Bytes size limit of the Ble GATT.
### one hold structure:
Hold has two fields:  
```Color | Led address```  
  
The color can be one of three values:
| Color | Value |
|-------|-------|
| Red   | 0x45  |
| Green | 0x53  |
| Blue  | 0x50  |

The leds are connected in a continuous strip of leds as seen in this picture from the moonboard installation guide:
![Moonboard led connection]   
Each led receives an address. The first led (A1) receive the address ```0x30``` then the next led gets ```0x31``` and so on... in a weird design choice A11 instead of ```0x40``` will receive **```0x3130```**. The ```3```'s are basically ignored and symbol that there is a digit after them.  
Any hold address is just the decimal number with a ```3``` before every digit.  

| hold number | Address        |
|-------------|----------------|
| 0           | ```0x30```     |
| 1           | ```0x31```     |
| ...         | ```0x3x```     |
| 10          | ```0x3130```   |
| 11          | ```0x3131```   |
| ...         | ```0x313x```   |
| 20          | ```0x3230```   |
| ...         | ```0x3x3x```   |
| 100         | ```0x313030``` |
| ...         | ```0x3x3x3x``` |





<!-- # How I've figured the protocol
## Gathering the data
Thankfully the Moonboard app has an option to make new problems, and better then that you can lit just a single hold.

So I've just sniffed sending every single hold in blue (I probably could have stopped half way but better safe than sorry 😎). 

To get a complete sense of how things work I also needed to get data on the different colors and how the data is structured when there are more than one hold. To gather this data I've just sent real routes (my bookmarks actually).

## looking at the data
After all the data was gathered I've looked through the wireshark recordings and put everything into google sheets. having everything in sheets I could see that if am sorting by value the values are sequential. -->

<!-- ## Advertising
This is the advertising packet coming out of the real moonboard (interpreted by Wireshark):
``` Bluetooth Low Energy Link Layer
    Access Address: 0x8e89bed6
    Packet Header: 0x1e40 (PDU Type: ADV_IND, ChSel: #1, TxAdd: Random)
        .... 0000 = PDU Type: 0x0 ADV_IND
        ...0 .... = Reserved: 0
        ..0. .... = Channel Selection Algorithm: #1
        .1.. .... = Tx Address: Random
        0... .... = Reserved: 0
        Length: 30
    Advertising Address: fc:17:a7:34:b1:d4 (fc:17:a7:34:b1:d4)
    Advertising Data
        Flags
            Length: 2
            Type: Flags (0x01)
            000. .... = Reserved: 0x0
            ...0 .... = Simultaneous LE and BR/EDR to Same Device Capable (Host): false (0x0)
            .... 0... = Simultaneous LE and BR/EDR to Same Device Capable (Controller): false (0x0)
            .... .1.. = BR/EDR Not Supported: true (0x1)
            .... ..1. = LE General Discoverable Mode: true (0x1)
            .... ...0 = LE Limited Discoverable Mode: false (0x0)
        Tx Power Level
            Length: 2
            Type: Tx Power Level (0x0a)
            Power Level (dBm): 0
        128-bit Service Class UUIDs (incomplete)
            Length: 17
            Type: 128-bit Service Class UUIDs (incomplete) (0x06)
            Custom UUID: 6e400001-b5a3-f393-e0a9-e50e24dcca9e (Nordic UART Service)
    CRC: 0x8849f0
```
And in raw hex:
```
0000   d6 be 89 8e 40 1e d4 b1 34 a7 17 fc 02 01 06 02
0010   0a 00 11 06 9e ca dc 24 0e e5 a9 e0 93 f3 a3 b5
0020   01 00 40 6e 11 92 0f
``` -->

[Nordic UART]: https://developer.nordicsemi.com/nRF_Connect_SDK/doc/1.5.1/nrf/include/bluetooth/services/nus.html
[Moonboard led connection]: https://www.moonboard.com/content/images/2020/build/Moonboard-Back-LED-min.jpg
[Moonboard Installation]: https://www.moonboard.com/how-to-build-your-moonboard#buildinfo