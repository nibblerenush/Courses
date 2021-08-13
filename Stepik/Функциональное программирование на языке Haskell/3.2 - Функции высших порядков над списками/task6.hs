import Data.Char

delAllUpper :: String -> String
delAllUpper = unwords . filter (\x -> not $ all isUpper x) . words
