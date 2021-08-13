qsort :: Ord a => [a] -> [a]

qsort [] = []
qsort (x:xs) = qsort (filter (< x) xs) ++ (x : qsort (filter (>= x) xs))
