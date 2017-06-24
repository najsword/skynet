local skynet = require "skynet"
local redis = require "skynet.db.redis"

local conf = {
	host = "127.0.0.1" ,
	port = 6379 ,
	db = 0
}

local function watching()
	local w = redis.watch(conf)
	w:subscribe "foo"
	w:psubscribe "hello.*"
	while true do
		print("Watch", w:message())
		skynet.sleep(1*100)
	end
end

skynet.start(function()
	skynet.fork(watching)
	local db = nil
	local ok, msg = xpcall(function()
		db = redis.connect(conf)
	end, debug.traceback)
	if not ok then
		error(msg)
	end

	db:del "C"
	db:set("A", "hello")
	db:set("B", "world")
	db:sadd("C", "one")

	print(db:get("A"))
	print(db:get("B"))

	db:del "D"
	for i=1,10 do
		db:hset("D",i,i)
	end
	local r = db:hvals "D"
	for k,v in pairs(r) do
		print(k,v)
	end

	db:multi()
	db:get "A"
	db:get "B"
	local t = db:exec()
	for k,v in ipairs(t) do
		print("Exec", v)
	end

	print(db:exists "A")
	print(db:get "A")
	print(db:set("A","hello world"))
	print(db:get("A"))
	print(db:sismember("C","one"))
	print(db:sismember("C","two"))

	print("===========publish============")

	for i=1,10 do
		db:publish("foo", i)
		print("publish foo", i)
	end
	for i=11,20 do
		db:publish("hello.foo", i)
		print("publish hello.foo", i)
	end

	db:disconnect()
--	skynet.exit()
end)

