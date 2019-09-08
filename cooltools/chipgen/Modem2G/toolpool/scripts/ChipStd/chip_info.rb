module ChipInfo
    CHIPID_TABLE = {
        0x809c8000 => "8955U01",
        0x809c8001 => "8955U02",
        0x8b340001 => "8909U01",
    }
    CHIPID_TABLE.default = "Unknown chip"

    def getChipID()
        id = R(0x01a24000)
        return CHIPID_TABLE[id]
    end

    addHelpEntry("chip", "chipID", "", "", "Show chip id")
    def chipID()
        puts getChipID
    end
end # ChipInfo

include ChipInfo
