integration :: (Double -> Double) -> Double -> Double -> Double

integration f a b = 
    let
        n = 1000
        h = (b - a) / n
        helper f a h sum 1000 = sum
        helper f a h sum i = helper (f) (a) (h) (sum + f (a + i * h)) (i + 1)
    in
        h * ((f a + f b) / 2 + helper (f) (a) (h) (0) (1))
