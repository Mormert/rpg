-- Simple lua component example

---@class ClassOne
---@serialized number aNumber
---@serialized number anotherNumber
---@serialized array<number> aNumberArray
ClassOne = {}

function ClassOne.new(o)
end

---@class ClassTwo
---@serialized number classTwoNumber
ClassTwo = {}

function ClassTwo.new(o)
end

---@class TestComponent : LuaComponent
---@inherits ClassOne
---@serialized array<integer> anIntegerArray
---@serialized ClassTwo aClassTwo
TestComponent = TestComponent or {}

function TestComponent:new(o)
    o = o or {}
    setmetatable(o, self)
    self.__index = self
    o.timer = o.timer or 0
    return o
end

function TestComponent:updateTimer(dt)
    self.timer = self.timer + dt
    --print(self.timer)
end

function TestComponent:updateTransform(dt)

    local object = self.getObject()
    local transform = object:transform();
    local localPos = transform:getLocalPosition()

    local vec = vec3.new(localPos.x, math.sin(self.timer), localPos.z)


    transform:setLocalPosition(vec)
end

-- Method to update the component
function TestComponent:update(dt)
    self:updateTimer(dt)
    self:updateTransform(dt)
end


function TestComponent:start()

end

function TestComponent:destroy()

end
