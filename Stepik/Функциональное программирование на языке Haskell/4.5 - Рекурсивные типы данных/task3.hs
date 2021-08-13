data Tree a = Leaf a | Node (Tree a) (Tree a)
    deriving (Show)

height :: Tree a -> Int
height (Leaf a) = 0
height (Node a b) = 1 + max (height a) (height b)

size :: Tree a -> Int
size (Leaf a) = 1
size (Node a b) = 1 + size a + size b 
