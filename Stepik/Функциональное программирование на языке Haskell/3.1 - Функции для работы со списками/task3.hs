oddsOnly :: Integral a => [a] -> [a]

oddsOnly [] = []
oddsOnly (x:xs) = if odd x then x : oddsOnly xs else oddsOnly xs
