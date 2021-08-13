data Color = Red | Green | Blue
    deriving (Show)

stringToColor :: String -> Color
stringToColor "Red" = Red
stringToColor "Green" = Green
stringToColor "Blue" = Blue
