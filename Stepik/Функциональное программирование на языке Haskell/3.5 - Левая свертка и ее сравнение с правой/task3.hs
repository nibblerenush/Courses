evenOnly :: [a] -> [a]

evenOnly = foldr (\(pos, x) xs -> if even pos then x : xs else xs) [] . zip [1..]
