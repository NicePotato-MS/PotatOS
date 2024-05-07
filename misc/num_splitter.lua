-- Function to split a decimal number into bytes
local function splitIntoBytes(decimal)
    while decimal > 0 do
        print(decimal % 256)
        decimal = math.floor(decimal / 256)
    end
end

splitIntoBytes(tonumber(arg[1]))
