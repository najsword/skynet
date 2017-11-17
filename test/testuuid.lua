local skynet = require "skynet"
local uuid = require "uuid"

for i=1, 100 do
	print("uuid_str=", uuid.uuid_str())
	print("uuid_str8=", uuid.uuid_str8())
	print("uuid_str20=", uuid.uuid_str20())
	print("uuid_hex=", uuid.uuid_hex())
	print("uuid_num=", uuid.uuid_num())
end

skynet.start(skynet.exit)