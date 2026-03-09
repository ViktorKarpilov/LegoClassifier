from connection.packet_type import PacketType


# For protocol implementation see Docs folder
class UsbPacket:
    def __init__(self, packet_type: PacketType = PacketType.MissedPacket, data: bytes = 0x00):
        self.type = packet_type
        self.byte_data = data


