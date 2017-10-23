local skynet = require "skynet"
local cipher = require "cipher"

local data = "hello world"
local key = "1234567890"

--md5是32位的
print("1=", cipher.md5(data))
print("2=", cipher.sha1(data))
print("3=", cipher.sha224(data))
print("4=", cipher.sha256(data))
print("5=", cipher.sha512(data))

print("6=", cipher.hmac_md5(data, key))
print("7=", cipher.hmac_sha1(data, key))
print("8=", cipher.hmac_sha224(data, key))
print("9=", cipher.hmac_sha256(data, key))

-- Segmentation fault 段错误 hmac_sha384不能用
-- print("10=", cipher.hmac_sha384(data, key))
print("11=", cipher.hmac_sha512(data, key))

print("12=", cipher.crc16(data))
print("13=", cipher.crc32(data))
print("14=", cipher.crc64(data))


skynet.start(skynet.exit)