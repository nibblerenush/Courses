import Data.List

revRange :: (Char,Char) -> [Char]

revRange (a, b) = reverse $ unfoldr g a
  where g = (\x -> if x >= a && x <= b then Just (x, succ x) else Nothing)
