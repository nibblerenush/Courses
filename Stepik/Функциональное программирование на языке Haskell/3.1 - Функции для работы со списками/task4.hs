isPalindrome :: Eq a => [a] -> Bool

isPalindrome x =
    let
        helper [] [] = True
        helper (x:xs) (y:ys) = if x == y then helper xs ys else False
    in
        helper x (reverse x)
