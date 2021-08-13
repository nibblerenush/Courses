data Bit = Zero | One
data Sign = Minus | Plus
data Z = Z Sign [Bit]

instance Show Bit where
    show x = case x of
        Zero -> "Zero"
        One -> "One"

instance Show Sign where
    show x = case x of
        Minus -> "Minus"
        Plus -> "Plus"

instance Show Z where
    show (Z sign bits) = "Z " ++ show sign ++ " " ++ show bits

binToInt :: Z -> Int
binToInt (Z sign bits) =
    let
        helper [] _ = 0
        helper (x : xs) power = case x of
            Zero -> 0 + helper xs (2 * power)
            One -> power + helper xs (2 * power)
        num = helper bits 1
    in
        case sign of
            Minus -> (-1) * num
            Plus -> num

intToBin :: Int -> Z
intToBin x =
    let
        helper 0 = []
        helper 1 = [One]
        helper x = (if (x `mod` 2 == 1) then One else Zero) : helper (x `div` 2)
        bits = helper $ abs x
    in
        if (x < 0) then (Z Minus bits) else (Z Plus bits)

add :: Z -> Z -> Z
add x y = intToBin $ binToInt x + binToInt y

mul :: Z -> Z -> Z
mul x y = intToBin $ binToInt x * binToInt y
