avg :: Int -> Int -> Int -> Double

avg x y z = 
    let
        x1 = (fromIntegral x) / 3
        x2 = (fromIntegral y) / 3
        x3 = (fromIntegral z) / 3
    in x1 + x2 + x3
