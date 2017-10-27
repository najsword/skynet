local skynet = require "skynet"
local crypt = require "skynet.crypt"

local binary = string.char( 0x03, 0x00, 0x01, 0x29, 0x33, 0x34, 0x35 )


print(crypt.base64encode(binary))

skynet.start(skynet.exit)