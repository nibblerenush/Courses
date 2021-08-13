pythagoreanTriple :: Int -> [(Int, Int, Int)]
pythagoreanTriple x = if (x <= 0)
    then
        []
    else
        do
            a <- [1 .. x]
            b <- [1 .. x]
            c <- [1 .. x]
            if (a * a + b * b == c * c && a < b) then "Z" else []
            return (a, b, c)
