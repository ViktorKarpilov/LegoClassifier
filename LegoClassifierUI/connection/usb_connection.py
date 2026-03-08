import serial.tools.list_ports
from serial.serialutil import SerialException

from config import USB_VID, USB_BAUD_RATE


class UsbConnection:
    def __init__(self):
        ports = serial.tools.list_ports.comports()
        self.port = None

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

        self.serial.open()
        if not self.serial.is_open:
            raise SerialException("Coulnd't open serial")

    def __del__(self):
        if self.serial.is_open:
            self.serial.close()


    def read(self):
        test = self.serial.read(1000)
        print(test)

