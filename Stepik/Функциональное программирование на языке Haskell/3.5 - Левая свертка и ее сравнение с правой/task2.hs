evenOnly :: [a] -> [a]

evenOnly xs =  (\(x, y) -> x) $ foldr (\x (evens, pos) -> if even pos then (x : evens, pos - 1) else (evens, pos - 1)) ([], length xs) xs
