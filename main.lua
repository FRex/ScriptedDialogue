local yield = coroutine.yield
local unpack = unpack or table.unpack

function test()
    local choices = {"a", "b", "c"}
    local ans = yield(unpack(choices))
    yield(("You picked %s"):format(choices[ans]))
    return "Bye!"
end
