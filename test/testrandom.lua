local skynet = require "skynet"
local random = require "random" 
local osext = require "osext"

local seed = os.time()
print("seed=", seed)
random.randomseed(seed)
-- math.randomseed(osext.timestamp())
-- math.randomseed(osext.timestamp())
-- print("osext.timestamp()=", osext.timestamp())

local seed = osext.timestamp()
local map = {}
for i=1, 100 do
	local ret = random.random(10)
	-- math.randomseed(seed)
	-- local ret = math.random(10)
	print("i=", ret)
	map[ret] = map[ret] or 0
	map[ret] = map[ret] + 1
end

for k, v in pairs(map) do
	print("k=", k, "v=", v)
end

skynet.start(skynet.exit)