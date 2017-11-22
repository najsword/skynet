local skynet = require "skynet"
local osext = require "osext"

local map = {}
for i=1, 10000000 do
	local ret = osext.uuid_str8()
	if map[ret] then
		print("uuid重复了 ret=", ret)
	end
	map[ret] = true
end

-- print(osext.uuid_str())

-- print(osext.uuid_str8())

-- print(osext.uuid_hex())

-- local bin = osext.uuid_binary()

-- print(osext.to_basex(bin, 16))
-- print(osext.to_basex(bin, 62))
-- print(osext.to_basex(bin, 64))
-- print(osext.to_basex(bin, 90))

skynet.start(skynet.exit)