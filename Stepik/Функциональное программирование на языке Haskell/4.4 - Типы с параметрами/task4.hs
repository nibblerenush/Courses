import Data.Char(isDigit)

findDigit :: [Char] -> Maybe Char
findDigit [] = Nothing
findDigit (x : xs) = if (isDigit x) then Just x else findDigit (xs)

findDigitOrX :: [Char] -> Char
findDigitOrX xs = case findDigit xs of
    Nothing -> 'X'
    (Just c) -> c 
