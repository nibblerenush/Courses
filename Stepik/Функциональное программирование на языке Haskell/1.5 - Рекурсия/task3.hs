fibonacci :: Integer -> Integer
fibonacci 0 = 0
fibonacci 1 = 1
fibonacci (-1) = 1
fibonacci n | n > 0 = helper' 1 0 n
            | n < 0 = helper'' 1 0 n

helper' prev1 prev2 1 = prev1
helper' prev1 prev2 n = helper' (prev1 + prev2) (prev1) (n - 1)

helper'' prev1 prev2 (-1) = prev1
helper'' prev1 prev2 n = helper'' (prev2 - prev1) (prev1) (n + 1)
