from time import sleep

import serial.tools.list_ports
from serial.serialutil import SerialException
from telethon.utils import chunks

from config import USB_VID, USB_BAUD_RATE
from connection.packet import UsbPacket
from connection.packet_type import PacketType
import crcmod

class UsbConnection:
    def __init__(self):
        ports = serial.tools.list_ports.comports()
        self.port = None
        self.crc = crcmod.mkCrcFun(0x11021, initCrc=0xFFFF, rev=False, xorOut=0x0000)

        found_count = 0
        for port in ports:
            if ("VID:PID=" + USB_VID) in port.hwid:
                found_count += 1
                self.port = port

        if found_count != 1:
            raise Exception("Multiple or none devices has been found")

        self.serial = serial.Serial()
        self.serial.port = self.port.name
        self.serial.baudrate = USB_BAUD_RATE
        self.serial.timeout = 5

        self.serial.open()
        if not self.serial.is_open:
            raise SerialException("Coulnd't open serial")

    def __del__(self):
        if self.serial.is_open:
            self.serial.close()

    def read_packet(self) -> UsbPacket:
        chunks_left = 1
        packet_type = PacketType.MissedPacket
        result_data = b''

        while chunks_left > 0:
            # Search for start
            buffer = "Initial"
            while buffer != b'\xc3':
                buffer = self.serial.read()

                # Timeout
                if buffer == b'':
                    return UsbPacket()

            data_length = int.from_bytes(self.serial.read(2), byteorder="little")

            # It overrides type and I don't really care
            packet_type =  PacketType(int.from_bytes(self.serial.read(1), byteorder="little"))
            chunks_left =  int.from_bytes(self.serial.read(1), byteorder="little")
            data = self.serial.read(data_length)
            while len(data) < data_length:
                chunk = self.serial.read(data_length - len(data))
                if chunk == b'':
                    return UsbPacket()  # timeout
                data += chunk
            received_crc = int.from_bytes(self.serial.read(2), byteorder="little")

            calculated_crc = self.crc(data)

            if calculated_crc  != received_crc:
                return UsbPacket()

            result_data += data

        return UsbPacket(packet_type, result_data)

