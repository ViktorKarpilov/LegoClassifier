from enum import Enum


class PacketType(int, Enum):
    MissedPacket = -1,

    Info = 0,
    Debug = 1,
    Error = 2,

    Image = 3,

    Ping = 4,

    ImageRequest = 5,

    SetExposure = 6,
