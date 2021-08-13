data Tree a = Leaf a | Node (Tree a) (Tree a)
    deriving (Show)

avg :: Tree Int -> Int
avg t =
    let (c, s) = go t
    in s `div` c
  where
    go :: Tree Int -> (Int,Int)
    go (Leaf a) = (1, a)
    go (Node a b) =
        let
            (c1, s1) = go a
            (c2, s2) = go b
        in
            (c1 + c2, s1 + s2)
