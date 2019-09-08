module SpiFlash
    OPCODE_WRITE_STATUS = 0x01
    OPCODE_PAGE_PROGRAM = 0x02
    OPCODE_WRITE_DISABLE = 0x04
    OPCODE_READ_STATUS = 0x05
    OPCODE_WRITE_ENABLE = 0x06
    OPCODE_SECTOR_ERASE = 0x20
    OPCODE_READ_STATUS_2 = 0x35
    OPCODE_BLOCK32K_ERASE = 0x52
    OPCODE_BLOCK_ERASE = 0xd8
    OPCODE_CHIP_ERASE = 0xc7 # or 0x60

    FLASH_SECTOR_SIZE = 0x1000
    FLASH_BLOCK_SIZE = 0x10000
    FLASH_BLOCK32K_SIZE = 0x8000

    # The followings should work for 8955 and 8909

    def spiTxCmd(cmd)
        return ((cmd & 0xff) << 8)
    end

    def setWritable(en)
        $SPI_FLASH.spi_cmd_addr.w(en ? OPCODE_WRITE_ENABLE : OPCODE_WRITE_DISABLE)
    end

    def alignCheck(address, aligned)
        begin
            if (address & (aligned - 1)) == 0
                return true
            end
        rescue Exception
        end
        puts "Flash address must be 0x%x aligned hex or dec" % aligned
        return false
    end

    def txFifoClear()
        $SPI_FLASH.spi_fifo_control = (1 << 1)
    end

    def rxFifoClear()
        $SPI_FLASH.spi_fifo_control = (1 << 0)
    end

    def waitWipDone()
        begin
            sleep(0.1)
            $SPI_FLASH.spi_cmd_addr.w(OPCODE_READ_STATUS)
        end until ($SPI_FLASH.spi_read_back.R & 1) == 0
    end

    def readStatus()
        $SPI_FLASH.spi_cmd_addr.w(OPCODE_READ_STATUS)
        lo = $SPI_FLASH.spi_read_back.R & 0xff
        $SPI_FLASH.spi_cmd_addr.w(OPCODE_READ_STATUS_2)
        hi = $SPI_FLASH.spi_read_back.R & 0xff
        return [lo, hi]
    end

    def writeStatus(lo, hi)
        setWritable(true)
        txFifoClear()
        $SPI_FLASH.spi_data_fifo.w(lo & 0xff)
        $SPI_FLASH.spi_data_fifo.w(hi & 0xff)
        $SPI_FLASH.spi_cmd_addr.w(spiTxCmd(OPCODE_WRITE_STATUS))
        waitWipDone()
        setWritable(false)
    end

    def eraseWithOpCode(address, opcode)
        setWritable(true)
        $SPI_FLASH.spi_cmd_addr.w(opcode | (address << 8))
        waitWipDone()
        setWritable(false)
    end

    def sectorErase(address)
        if alignCheck(address, FLASH_SECTOR_SIZE)
            eraseWithOpCode(address, OPCODE_SECTOR_ERASE)
        end
    end

    def blockErase(address)
        if alignCheck(address, FLASH_BLOCK_SIZE)
            eraseWithOpCode(address, OPCODE_BLOCK_ERASE)
        end
    end

    def block32kErase(address)
        if alignCheck(address, FLASH_BLOCK32K_SIZE)
            eraseWithOpCode(address, OPCODE_BLOCK32K_ERASE)
        end
    end

    def chipErase()
        setWritable(true)
        $SPI_FLASH.spi_cmd_addr.w(OPCODE_CHIP_ERASE)
        waitWipDone
        setWritable(false)
    end

end

include SpiFlash

addHelpEntry("flash", "flashReadStatus", "", "",
             "Read flash status register.")
def flashReadStatus()
    xfbp()
    status = SpiFlash.readStatus()
    xrbp()

    puts "Flash status register: [lo: 0x%02x hi: 0x%02x]" % status
end

addHelpEntry("flash", "flashWriteStatus", "low, high", "",
             "Write flash status register.")
def flashWriteStatus(lo, hi)
    xfbp()
    SpiFlash.writeStatus(lo, hi)
    xrbp()

    puts "Flash write status OK"
end

addHelpEntry("flash", "flashSectorErase", "address", "",
             "Erase flash sector with starting address")
def flashSectorErase(address)
    xfbp()
    SpiFlash.sectorErase(address)
    xrbp()

    puts "Flash sector erase done."
end

addHelpEntry("flash", "flashBlockErase", "address", "",
             "Erase flash block with starting address.")
def flashBlockErase(address)
    xfbp()
    SpiFlash.blockErase(address)
    xrbp()

    puts "Flash block erase done."
end

addHelpEntry("flash", "flashBlock32kErase", "address", "",
             "Erase flash 32K block with starting address.")
def flashBlock32kErase(address)
    xfbp()
    SpiFlash.block32kErase(address)
    xrbp()

    puts "Flash block 32k erase done"
end

addHelpEntry("flash", "flashChipErase", "", "", "Erase the entire flash.")
def flashChipErase()
    xfbp()
    SpiFlash.chipErase()
    xrbp()

    puts "Flash chip erase done"
end

def flashListCmd()
    puts "flashReadStatus"
    puts "flashWriteStatus"
    puts "flashSectorErase"
    puts "flashBlockErase"
    puts "flashBlock32kErase"
    puts "flashChipErase"
end
