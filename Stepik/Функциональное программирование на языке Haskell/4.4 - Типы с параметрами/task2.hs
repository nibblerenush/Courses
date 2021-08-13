data Coord a = Coord a a
    deriving (Show)

getCenter :: Double -> Coord Int -> Coord Double
getCenter width (Coord x y) = Coord (fromIntegral (x) * width + width / 2) (fromIntegral (y) * width + width / 2)

getCell :: Double -> Coord Double -> Coord Int
getCell width (Coord x y) = Coord (floor $ x / width) (floor $ y / width)
