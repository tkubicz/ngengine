-- This is a set of library function used by NGE

-- Simple way of printing all (key, value) pairs in table.
function print_table(table)
    for key, value in pairs(table) do print(key, value) end
end

function class(className)
    local ret = {}
    ret.className = className
    ret.cpp_object = nil

    ret.__index = function(self, key)
        local obj = rawget(self, key)
        if not obj then
            local mt = rawget(getmetatable(self), key)
            if mt then
                return function(self, ...) return mt(self, ...) end
            else
                if self.cpp_object ~= nil then
                    obj = self.cpp_object[key]
                    if type(obj) == "function" then
                        return function(self, ...) return obj(self.cpp_object, ...) end
                    else
                        return obj
                    end
                end
            end
        else
            return obj
        end
    end

    -- Constructor of the class
    ret.new = function(o)
        local instance = o or {}
        instance.__index = instance

		if className ~= nil then
			if instance.shared == true then
				instance.cpp_object = className.shared()
			else
				instance.cpp_object = className.new()
			end
		else
			instance.cpp_object = nil
		end

        setmetatable(instance, ret)
        return instance
    end 

    setmetatable(ret, ret)
    return ret
end
