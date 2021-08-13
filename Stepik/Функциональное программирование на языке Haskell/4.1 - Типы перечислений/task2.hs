import Data.Char

charToInt :: Char -> Int

charToInt x = if x >= '0' && x <= '9' then ord x - ord '0' else undefined
