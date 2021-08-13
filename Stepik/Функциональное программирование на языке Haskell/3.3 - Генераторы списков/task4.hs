coins :: (Num a) => [a]
coins = [2, 3, 7]

change :: (Ord a, Num a) => a -> [[a]]

helper cost temp =
    (filter (\x -> sum x == cost) temp) ++ ( if length temp == 0 then [] else helper cost ([ x:y | x <- coins, y <- temp, x + sum y <= cost ]) )

change cost = helper cost [[]]
