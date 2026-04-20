-- RU: Регистрация игрока и выбор пути
-- EN: Player registration and path choice

local name = input("Enter your name / Введите имя: ")
if name == "" then name = "Stranger" end

say("Guide", C_GREEN, "Welcome, " .. name .. "!", C_RESET, 50)

local path = menu("Where to go?", {
    "Forest / Лес",
    "Castle / Замок"
})

if path == 1 then
    dialogue("You entered the dark forest...", C_RED, 60)
else
    say("Guard", C_CYAN, "Halt! Who goes there?", C_RESET, 40)
end
