local skynet = require "skynet"
local codec = require "codec"

--test ok
local data = "hello world"
local key = "1234567890"
print("1=", codec.md5_encode(data))
print("2=", codec.hmac_sha1_encode(data, key))
print("3=", codec.aes_encrypt(data, key))
print("4=", codec.aes_decrypt(codec.aes_encrypt(data, key), key))

skynet.start(skynet.exit)