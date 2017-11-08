local skynet = require "skynet"
local codec = require "codec"

--test ok
local data = "hello world"
local key = "1234567890"
print("1=", codec.md5_encode(data))
print("2=", codec.hmac_sha1_encode(data, key))
print("3=", codec.aes_encrypt(data, key))
print("4=", codec.aes_decrypt(codec.aes_encrypt(data, key), key))

local private_file = assert(io.open("./test/private.pem"), "private_file open failed")
local private_pem = private_file:read "a"
private_file:close()
local public_file = assert(io.open("./test/public.pem"), "public_file open failed")
local public_pem = public_file:read "a"
public_file:close()

print("private_pem=", private_pem)
print("public_pem=", public_pem)

--创建签名
local bs = codec.rsa_private_sign("something", private_pem)
local dst = codec.base64_encode(bs)
print("dst=", dst)

--效验签名
local ret = codec.rsa_public_verify("something", bs, public_pem, 1)
print("ret=", ret)


--公钥加密, 私钥解密
local en_bs = codec.rsa_public_encrypt("something", public_pem, 1)
local en_dest = codec.base64_encode(en_bs)
print("en_dest=", en_dest)
print(codec.rsa_private_decrypt(en_bs, private_pem))

--私钥加密, 公钥解密(禁止这种用法)
-- local en_bs1 = codec.rsa_public_encrypt("something", private_pem, 1)
-- local en_dest1 = codec.base64_encode(en_bs1)
-- print("en_dest1=", en_dest1)
-- print(codec.rsa_private_decrypt(en_bs1, public_pem))

skynet.start(skynet.exit)