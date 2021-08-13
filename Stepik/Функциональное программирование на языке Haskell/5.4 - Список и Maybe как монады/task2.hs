--Тип Board и функция nextPositions заданы, реализовывать их не нужно

data Board = JOPA deriving (Show)

nextPositions :: Board -> [Board]
nextPositions x = [x, x]

nextPositionsN :: Board -> Int -> (Board -> Bool) -> [Board]
nextPositionsN b n pred = if (n < 0)
    then
        []
    else
        let
            helper b 0 = return b
            helper b n = (nextPositions b) >>= (\x -> helper x (n - 1) )
        in
            filter pred $ helper b n
