-- This is a set of library function used by NGE

-- Simple way of printing all (key, value) pairs in table.
function print_table(table)
    for key, value in pairs(table) do print(key, value) end
end

function class(className)
    local ret = {}
    local ret_metatable = {}
    ret.className = className

    ret_metatable.__index = function(self, key)
		local obj = rawget(self, key)
		if not obj then
			obj = self.cppObject[key]
			if type(obj) == "function" then
				return function(self, ...) return obj(self.cppObject, ...) end
			else
				return obj
			end
		else
			return obj
		end
	end

    ret.new = function(o)
        local instance = o or {}
        instance.cppObject = className.new()
        setmetatable(instance, ret_metatable)
        return instance
    end 

    setmetatable(ret, ret_metatable)

    return ret
end
