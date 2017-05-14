local lpeg = require "lpeg"
local C, Cs, Ct, P, S = lpeg.C, lpeg.Cs, lpeg.Ct, lpeg.P, lpeg.S

local eol = P'\r\n' + P'\n'
local quoted_field = '"' * Cs(((P(1) - '"') + P'""' / '"')^0) * '"'
local unquoted_field = C((1 - S',\r\n"')^0)
local field = quoted_field + unquoted_field
local record = Ct(field * (',' * field)^0)
local one_line_record = record * (eol + -1)

local function parse_line(line)
    assert(type(line) == 'string', 'bad argument #1 (expected string)')
    return lpeg.match(one_line_record, line)
end

local M = {}

function M.fromfile(file)
    local f = io.open(file, "r")
    if not f then return nil end
    local headers = {}
    local types = {}
    local rows = {}
    local n = 1
    while(true) do
        local line = f:read("*line")
        if not line then break end
        local fields = parse_line(line)
        if fields then
            if n == 1 then
                -- headers
                for _, v in ipairs(fields) do table.insert(headers, v) end
            elseif n == 2 then
                -- types
                for _, v in ipairs(fields) do table.insert(types, v) end
            else
                -- rows
                local r = {}
                for i, v in ipairs(fields) do
                    if types[i] == 'number' and v ~= '' then
                        r[headers[i]] = assert(tonumber(v))
                    elseif types[i] == 'string' then
                        r[headers[i]] = assert(tostring(v))
                    end
                end
                table.insert(rows, r)
            end
        end
        n = n + 1
    end
    f:close()
    return rows
end

return M