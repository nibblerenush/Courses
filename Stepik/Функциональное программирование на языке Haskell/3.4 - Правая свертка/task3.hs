sumOdd :: [Integer] -> Integer

sumOdd = foldr (\x s -> if odd x then x + s else s) 0
