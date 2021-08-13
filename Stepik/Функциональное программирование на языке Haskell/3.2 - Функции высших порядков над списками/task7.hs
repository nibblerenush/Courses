max3 :: Ord a => [a] -> [a] -> [a] -> [a]

max3 xs ys zs = zipWith3 (\x y z -> max x $ max y z) xs ys zs 
