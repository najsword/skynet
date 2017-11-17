local skynet = require "skynet"
local snowflake = require "snowflake" 

snowflake.init(1)

for i=1, 100 do
	print("id=", snowflake.next_id())
end

skynet.start(skynet.exit)