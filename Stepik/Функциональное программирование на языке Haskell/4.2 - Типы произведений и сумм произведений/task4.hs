data Shape = Circle Double | Rectangle Double Double

square :: Double -> Shape
square a = Rectangle a a

isSquare :: Shape -> Bool
isSquare (Circle _) = False
isSquare (Rectangle a b) = if a == b then True else False
