print("Loading script")

function ScriptProcess:OnInit()
	print("ScriptProcess OnInit()")
end

function ScriptProcess:OnSuccess()
	print("ScriptProcess OnSuccess()")
end

function inheritsFrom( baseClass )

    local new_class = {}
    local class_mt = { __index = new_class }

    function new_class:new()
        local newinst = {}
        setmetatable( newinst, class_mt )
        return newinst
    end

    if nil ~= baseClass then
        setmetatable( new_class, { __index = baseClass } )
    end

    -- Implementation of additional OO properties starts here --

    -- Return the class object of the instance
    function new_class:class()
        return new_class
    end

    -- Return the super class object of the instance
    function new_class:superClass()
        return baseClass
    end

    -- Return true if the caller is an instance of theClass
    function new_class:isa( theClass )
        local b_isa = false

        local cur_class = new_class

        while ( nil ~= cur_class ) and ( false == b_isa ) do
            if cur_class == theClass then
                b_isa = true
            else
                cur_class = cur_class:superClass()
            end
        end

        return b_isa
    end

    return new_class
end

SubClass = inheritsFrom(ScriptProcess)

function SubClass:SetValue(value) 
	self.value = value
end

function SubClass:GetValue()
	if self.value ~= nil then
		return self.value
	else
		return 0
	end
end

function SubClass:OnInit()
	print("SubClass OnInit()")
end

function SubClass:OnSuccess()
	print("SubClass OnSuccess()")
end

function SubClass:OnUpdate(delta)
	print(delta)
end

local i = ScriptProcess.new()
local j = SubClass.new()

i:OnInit()
i.OnSuccess()

j:OnInit()
j.OnSuccess()

j:SetValue(10)
print(j:GetValue())

CreateProcess("SubClass", "p")

p:OnInit()
p:OnSuccess()
p:OnInit()

print("Script loaded")