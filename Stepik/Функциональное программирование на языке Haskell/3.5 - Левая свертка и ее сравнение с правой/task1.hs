meanList :: [Double] -> Double

meanList = (\(s, l) -> s / l) . foldr (\x (s, l) -> (s + x, l + 1)) (0, 0)
