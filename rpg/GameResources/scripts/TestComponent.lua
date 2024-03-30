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
    self.timer = self.timer + 1
end

function TestComponent:updateTransform(dt)
    local localPos = self.transform:getLocalPosition()
    local vec = vec3.new(localPos.x, math.sin(self.timer), localPos.z)
    self.transform:setLocalPosition(vec)
end

-- Method to update the component
function TestComponent:update(dt)
    self:updateTimer(dt)
    self:updateTransform(dt)
end


function TestComponent:start()
    local light = self.object:addComponent_cLight();
    local color = light.color;
    color.x = 1.25;
end

function TestComponent:destroy()

end
