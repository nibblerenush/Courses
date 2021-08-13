import Data.Char(isDigit)

findDigit :: [Char] -> Maybe Char
findDigit [] = Nothing
findDigit (x : xs) = if (isDigit x) then Just x else findDigit (xs)
