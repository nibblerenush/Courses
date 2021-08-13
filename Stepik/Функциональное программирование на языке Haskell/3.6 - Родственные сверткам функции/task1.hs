lastElem :: [a] -> a

lastElem = foldl1 (\x y -> y)
