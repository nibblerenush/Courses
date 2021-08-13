squares'n'cubes :: Num a => [a] -> [a]

squares'n'cubes xs = concatMap (\x -> [x * x,  x * x * x]) xs
